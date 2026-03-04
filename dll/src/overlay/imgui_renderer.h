#pragma once

#include <Windows.h>
#include <d3d11.h>

namespace ImGuiRenderer {

void Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* context);
void Shutdown();

} // namespace ImGuiRenderer
