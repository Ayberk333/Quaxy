#include <mjLib/mjLib.hpp>
#include <DirectX/D3DX11.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_internal.h>
#include <MenuGlobal.hpp>

bool keepRunning = true;
bool isMenuVisible = true;
bool screenSizeChanged = false;
bool sizeChanged = false;
bool mainReady = false;
int maxPlayerCount = 500;

#include "Images.hpp"
#include "Fonts.hpp"
#include "MenuItems.hpp"
#include "AimViewer.hpp"
#include "EspViewer.hpp"
#include "GameSDK.hpp"
#include "Core.hpp"
#include "Menu.hpp"
#include "Overlay.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    mjLib::Console::Enable();
    SetConsoleTitle(mjLib::String::Random(30).c_str());

    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    ImGui_ImplWin32_EnableDpiAwareness();

    mjLib::Logger::WriteLog(XorString("Waiting for game window..."), mjLib::Logger::LogLevel::LOG_INFO);
    while (!FindWindow(Game.lpClassName, NULL)) {
        Sleep(250);
    }

    if (!AttachToGameProcess()) {
        mjLib::Logger::WriteLog(XorString("Failed to attach to the game process."), mjLib::Logger::LogLevel::LOG_ERROR);
        Sleep(5000);
        return 1;
    }

    mjLib::Logger::WriteLog(XorString("Game window found."), mjLib::Logger::LogLevel::LOG_SUCCESSFUL);
    mjLib::Logger::WriteLog(XorString("Fetching game offsets and version..."), mjLib::Logger::LogLevel::LOG_INFO);
    if (!FetchOffsetsAndVersion()) {
        mjLib::Logger::WriteLog(XorString("Failed to fetch game offsets or version."), mjLib::Logger::LogLevel::LOG_ERROR);
        Sleep(5000);
        return 1;
    }

    mjLib::Logger::WriteLog(XorString("Game information retrieved successfully."), mjLib::Logger::LogLevel::LOG_SUCCESSFUL);
    mjLib::Logger::WriteLog(XorString("Starting overlay system..."), mjLib::Logger::LogLevel::LOG_INFO);

    std::thread([&]() { UpdatePeds(); }).detach();
    std::thread([&]() { UpdateVehicles(); }).detach();
    std::thread([&]() { AimAssist(); }).detach();
    std::thread([&]() { Exploits(); }).detach();
  
    int errorId = 0;
    auto exitCode = OverlayMain(errorId);
    switch (errorId) {
    case 1:
        mjLib::Logger::WriteLog(XorString("Overlay window creation failed."), mjLib::Logger::LogLevel::LOG_ERROR);
        Sleep(5000);
        break;
    case 2:
        mjLib::Logger::WriteLog(XorString("D3D11 device creation failed."), mjLib::Logger::LogLevel::LOG_ERROR);
        Sleep(5000);
        break;
    default:
        mjLib::Logger::WriteLog(XorString("Overlay exited normally."), mjLib::Logger::LogLevel::LOG_INFO);
        Sleep(5000);
        break;
    }
    return exitCode;
}

DWORD WINAPI Thread(LPVOID lpParam) {
    return WinMain(NULL, NULL, NULL, NULL);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        HANDLE hThread = CreateThread(NULL, 0, Thread, NULL, 0, NULL);
        if (hThread) {
            CloseHandle(hThread);
        }
        break;
    }
    return TRUE;
}
