#include "dx11_hook.h"
#include "imgui_renderer.h"
#include "overlay_ui.h"
#include "../util/logger.h"

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <MinHook.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <atomic>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Globals
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
static HWND g_hWnd = nullptr;
static WNDPROC g_origWndProc = nullptr;
static std::atomic<bool> g_overlayVisible{false};
static bool g_imguiInitialized = false;

using PFN_Present = HRESULT(WINAPI*)(IDXGISwapChain*, UINT, UINT);
static PFN_Present g_origPresent = nullptr;

static LRESULT WINAPI HookedWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // F9 toggles overlay
    if (msg == WM_KEYDOWN && wParam == VK_F9) {
        g_overlayVisible.store(!g_overlayVisible.load());
        return 0;
    }

    if (g_overlayVisible.load()) {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
            return 0;
        }

        // Block game input when overlay is active and ImGui wants capture
        if (ImGui::GetCurrentContext()) {
            auto& io = ImGui::GetIO();
            if (io.WantCaptureMouse && (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)) {
                return 0;
            }
            if (io.WantCaptureKeyboard && (msg >= WM_KEYFIRST && msg <= WM_KEYLAST)) {
                return 0;
            }
        }
    }

    return CallWindowProcW(g_origWndProc, hWnd, msg, wParam, lParam);
}

static HRESULT WINAPI HookedPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    if (!g_imguiInitialized) {
        // Get device and context from swap chain
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice))) {
            g_pd3dDevice->GetImmediateContext(&g_pd3dContext);

            DXGI_SWAP_CHAIN_DESC desc;
            pSwapChain->GetDesc(&desc);
            g_hWnd = desc.OutputWindow;

            // Create render target view
            ID3D11Texture2D* pBackBuffer = nullptr;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
            if (pBackBuffer) {
                g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
                pBackBuffer->Release();
            }

            // Initialize ImGui
            ImGuiRenderer::Init(g_hWnd, g_pd3dDevice, g_pd3dContext);

            // Subclass window for input
            g_origWndProc = reinterpret_cast<WNDPROC>(
                SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HookedWndProc))
            );

            g_imguiInitialized = true;
            LOG_INFO("ImGui initialized in Present hook");
        }
    }

    if (g_imguiInitialized && g_overlayVisible.load()) {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        OverlayUI::Render();

        ImGui::Render();

        g_pd3dContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    return g_origPresent(pSwapChain, SyncInterval, Flags);
}

// ============================================================================
// Present vtable extraction — avoids creating a hardware device that conflicts
// with the game's existing device. Uses WARP (software renderer) or NULL driver.
// ============================================================================

// SEH-protected wrapper for device creation (must be free of C++ destructors)
static HRESULT CreateDummyDeviceSEH(HWND hWnd, D3D_DRIVER_TYPE driverType,
                                     IDXGISwapChain** ppSwapChain,
                                     ID3D11Device** ppDevice,
                                     ID3D11DeviceContext** ppContext) {
    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferCount = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.Width = 2;
    sd.BufferDesc.Height = 2;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = E_FAIL;

    __try {
        hr = D3D11CreateDeviceAndSwapChain(
            nullptr, driverType, nullptr, 0, nullptr, 0,
            D3D11_SDK_VERSION, &sd, ppSwapChain, ppDevice, &featureLevel, ppContext
        );
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        hr = E_FAIL;
    }

    return hr;
}

static PFN_Present ExtractPresentFromVtable() {
    // Create a tiny hidden window for the dummy swap chain
    WNDCLASSEXW wc{sizeof(WNDCLASSEXW), CS_CLASSDC, DefWindowProcW, 0, 0,
                   GetModuleHandleW(nullptr), nullptr, nullptr, nullptr, nullptr,
                   L"DummyDX11Vtable", nullptr};
    RegisterClassExW(&wc);

    HWND hDummyWnd = CreateWindowExW(0, wc.lpszClassName, L"", WS_OVERLAPPEDWINDOW,
                                      0, 0, 2, 2, nullptr, nullptr, wc.hInstance, nullptr);
    if (!hDummyWnd) {
        LOG_ERROR("Failed to create dummy window for vtable extraction");
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return nullptr;
    }

    IDXGISwapChain* pSwapChain = nullptr;
    ID3D11Device* pDevice = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
    PFN_Present pPresent = nullptr;

    // Try WARP first — software renderer, doesn't conflict with game's hardware device
    HRESULT hr = CreateDummyDeviceSEH(hDummyWnd, D3D_DRIVER_TYPE_WARP,
                                       &pSwapChain, &pDevice, &pContext);
    if (SUCCEEDED(hr)) {
        LOG_INFO("Dummy device created with WARP driver");
    } else {
        // Try REFERENCE driver
        hr = CreateDummyDeviceSEH(hDummyWnd, D3D_DRIVER_TYPE_REFERENCE,
                                   &pSwapChain, &pDevice, &pContext);
        if (SUCCEEDED(hr)) {
            LOG_INFO("Dummy device created with REFERENCE driver");
        } else {
            // Last resort: hardware (may crash, but SEH-protected)
            hr = CreateDummyDeviceSEH(hDummyWnd, D3D_DRIVER_TYPE_HARDWARE,
                                       &pSwapChain, &pDevice, &pContext);
            if (SUCCEEDED(hr)) {
                LOG_INFO("Dummy device created with HARDWARE driver");
            }
        }
    }

    if (SUCCEEDED(hr) && pSwapChain) {
        // Extract Present from vtable index 8
        void** vtable = *reinterpret_cast<void***>(pSwapChain);
        pPresent = reinterpret_cast<PFN_Present>(vtable[8]);
        LOG_INFO("IDXGISwapChain::Present at 0x%llx", reinterpret_cast<uintptr_t>(pPresent));
    } else {
        LOG_ERROR("All dummy device creation methods failed (HRESULT 0x%08x)",
                  static_cast<uint32_t>(hr));
    }

    // Cleanup
    if (pSwapChain) pSwapChain->Release();
    if (pDevice) pDevice->Release();
    if (pContext) pContext->Release();
    DestroyWindow(hDummyWnd);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return pPresent;
}

namespace DX11Hook {

bool Install() {
    auto pPresent = ExtractPresentFromVtable();
    if (!pPresent) {
        LOG_ERROR("Could not extract Present address — DX11 hook disabled");
        return false;
    }

    // Install MinHook
    if (MH_Initialize() != MH_OK) {
        LOG_ERROR("MinHook initialize failed");
        return false;
    }

    if (MH_CreateHook(reinterpret_cast<void*>(pPresent),
                       reinterpret_cast<void*>(&HookedPresent),
                       reinterpret_cast<void**>(&g_origPresent)) != MH_OK) {
        LOG_ERROR("MinHook CreateHook(Present) failed");
        return false;
    }

    if (MH_EnableHook(reinterpret_cast<void*>(pPresent)) != MH_OK) {
        LOG_ERROR("MinHook EnableHook(Present) failed");
        return false;
    }

    LOG_INFO("DX11 Present hook installed successfully");
    return true;
}

void Remove() {
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    if (g_origWndProc && g_hWnd) {
        SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_origWndProc));
        g_origWndProc = nullptr;
    }

    ImGuiRenderer::Shutdown();

    if (g_pRenderTargetView) { g_pRenderTargetView->Release(); g_pRenderTargetView = nullptr; }
    if (g_pd3dContext) { g_pd3dContext->Release(); g_pd3dContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }

    LOG_INFO("DX11 hook removed");
}

bool IsOverlayVisible() {
    return g_overlayVisible.load();
}

void ToggleOverlay() {
    g_overlayVisible.store(!g_overlayVisible.load());
}

} // namespace DX11Hook
