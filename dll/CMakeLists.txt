cmake_minimum_required(VERSION 3.20)
project(GroundedTracker LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)

# Static CRT for self-contained DLL
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# --- Vendor: ImGui ---
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_dx11.cpp
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
)

# --- Vendor: MinHook ---
set(MINHOOK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/minhook)
file(GLOB MINHOOK_SOURCES ${MINHOOK_DIR}/src/*.c ${MINHOOK_DIR}/src/hde/*.c)

# --- DLL target ---
add_library(grounded_tracker SHARED
    src/dllmain.cpp
    src/core/engine.cpp
    src/core/memory.cpp
    src/core/ue4_types.cpp
    src/game/game_state.cpp
    src/game/achievements.cpp
    src/game/creature_cards.cpp
    src/game/landmarks.cpp
    src/game/scab_schemes.cpp
    src/game/milk_molars.cpp
    src/game/mixr.cpp
    src/game/table_items.cpp
    src/game/upgrades.cpp
    src/ipc/pipe_server.cpp
    src/overlay/dx11_hook.cpp
    src/overlay/imgui_renderer.cpp
    src/overlay/overlay_ui.cpp
    src/util/logger.cpp
    ${IMGUI_SOURCES}
    ${MINHOOK_SOURCES}
)

target_include_directories(grounded_tracker PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${MINHOOK_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)

target_link_libraries(grounded_tracker PRIVATE
    d3d11
    dxgi
    dxguid
)

set_target_properties(grounded_tracker PROPERTIES
    OUTPUT_NAME "grounded_tracker"
    PREFIX ""
)

# --- Injector executable ---
add_executable(injector tools/injector.cpp)

target_link_libraries(injector PRIVATE
    advapi32
)

# --- CMake presets ---
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
