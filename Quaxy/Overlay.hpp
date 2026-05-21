static struct OverlayStruct {
    HWND hWnd;
    WNDCLASSEX wndClassEx;
    LPCSTR lpClassName = "FiveM";
    LPCSTR lpWindowName = "FiveM";
    ID3D11Device* pDevice;
    ID3D11DeviceContext* pDeviceContext;
    IDXGISwapChain* pSwapChain;
    bool swapChainOccluded;
    UINT resizeWidth;
    UINT resizeHeight;
    ID3D11RenderTargetView* pMainRenderTargetView;
    POINT WindowSize;
    POINT ScreenSize;
}Overlay;

bool CreateOverlayWindow();
bool CreateDeviceD3D();
void CreateRenderTarget();
void UpdateOverlay();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void CleanupRenderTarget();
void CleanupOverlayWindow();
void CleanupDeviceD3D();
void CleanupMenuRenderer();

void InitMenuRenderer() {
    float mainScale = ImGui_ImplWin32_GetDpiScaleForMonitor(MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    SetupMenuFonts(Overlay.pDevice);
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(mainScale);
    //style.FontScaleDpi = mainScale;

    ImGui_ImplWin32_Init(Overlay.hWnd);
    ImGui_ImplDX11_Init(Overlay.pDevice, Overlay.pDeviceContext);
}

int OverlayMain(int& errorId) {
    Debug(XorString("Starting overlay initialization..."), LOG_INFO);
    if (!CreateOverlayWindow()) {
        CleanupOverlayWindow();
        errorId = 1;
        return 1;
    }

    if (!CreateDeviceD3D()) {
        Debug(XorString("D3D11 device creation failed."), LOG_ERROR);
        CleanupDeviceD3D();
        CleanupOverlayWindow();
        errorId = 2;
        return 1;
    }

    std::thread([]() {
        Debug(XorString("Starting overlay update thread."), LOG_INFO);
        UpdateOverlay();
    }).detach();

    InitMenuRenderer();
    Debug(XorString("Renderer initialized."), LOG_INFO);
    if (!debugMode) {
        mjLib::Console::Close();
    }

    while (keepRunning) {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) {
                keepRunning = false;
            }
        }

        if (!keepRunning) {
            break;
        }

        if (Overlay.swapChainOccluded && Overlay.pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED) {
            Debug(XorString("Swap chain is occluded. Skipping frame rendering."), LOG_INFO);
            Sleep(50);
            continue;
        }

        if (screenSizeChanged || sizeChanged) {
            Debug(XorString("Detected screen or window size change. Reinitializing overlay..."), LOG_WARNING);
            CleanupMenuRenderer();
            CleanupDeviceD3D();
            CleanupOverlayWindow();

            if (!CreateOverlayWindow()) {
                Debug(XorString("Re-creation of overlay window failed during resize handling."), LOG_ERROR);
                CleanupOverlayWindow();
                break;
            }

            Sleep(1000);

            if (!CreateDeviceD3D()) {
                Debug(XorString("Re-creation of D3D11 device failed during resize handling."), LOG_ERROR);
                CleanupDeviceD3D();
                CleanupOverlayWindow();
                break;
            }

            InitMenuRenderer();
            Debug(XorString("Reinitialization complete after resize."), LOG_SUCCESSFUL);
            screenSizeChanged = false;
            sizeChanged = false;
            continue;
        }

        Overlay.swapChainOccluded = false;
        if (Overlay.resizeWidth != 0 && Overlay.resizeHeight != 0) {
            Debug(XorString("Resizing swap chain buffers..."), LOG_WARNING);
            CleanupRenderTarget();
            Overlay.pSwapChain->ResizeBuffers(0, Overlay.resizeWidth, Overlay.resizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            Overlay.resizeHeight = Overlay.resizeWidth = 0;
            CreateRenderTarget();
        }

        if (GetAsyncKeyState(Features::Settings::MenuKey) & 1) {
            isMenuVisible = !isMenuVisible;
            ImGui::GetIO().MouseDrawCursor = isMenuVisible;
            SetWindowLong(Overlay.hWnd, GWL_EXSTYLE, isMenuVisible ? WS_EX_TOOLWINDOW : (WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW));
            UpdateWindow(Overlay.hWnd);
        }

        RenderFrame(Overlay.hWnd, isMenuVisible);

        const float clear_color_with_alpha[4] = { 0.f, 0.f, 0.f, 0.f };
        Overlay.pDeviceContext->OMSetRenderTargets(1, &Overlay.pMainRenderTargetView, NULL);
        Overlay.pDeviceContext->ClearRenderTargetView(Overlay.pMainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        HRESULT hr = Overlay.pSwapChain->Present(1, 0);
        Overlay.swapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
        if (FAILED(hr) && hr != DXGI_STATUS_OCCLUDED) {
            Debug(XorString("Present operation failed!"), LOG_ERROR);
        }
        mainReady = true;
    }

    Debug(XorString("Shutting down overlay..."), LOG_INFO);
    CleanupMenuRenderer();
    CleanupDeviceD3D();
    CleanupOverlayWindow();
    return 0;
}

bool CreateOverlayWindow() {
    Debug(XorString("Initializing overlay window class..."), LOG_INFO);
    Overlay.ScreenSize.x = GetSystemMetrics(SM_CXSCREEN);
    Overlay.ScreenSize.y = GetSystemMetrics(SM_CYSCREEN);

    ZeroMemory(&Overlay.wndClassEx, sizeof(Overlay.wndClassEx));
    Overlay.wndClassEx.cbSize = sizeof(WNDCLASSEX);
    Overlay.wndClassEx.lpfnWndProc = WndProc;
    Overlay.wndClassEx.hInstance = GetModuleHandle(NULL);
    Overlay.wndClassEx.lpszClassName = Overlay.lpClassName;
    Overlay.wndClassEx.lpszMenuName = Overlay.lpWindowName;
    Overlay.wndClassEx.hCursor = NULL;
    Overlay.wndClassEx.hbrBackground = NULL;
    Overlay.wndClassEx.hIcon = NULL;
    Overlay.wndClassEx.hIconSm = NULL;
    Overlay.wndClassEx.cbClsExtra = NULL;
    Overlay.wndClassEx.cbWndExtra = NULL;
    Overlay.wndClassEx.style = NULL;

    if (!RegisterClassEx(&Overlay.wndClassEx)) {
        Debug(XorString("Failed to register window class!"), LOG_ERROR);
        return false;
    }

    Debug(XorString("Window class registered successfully."), LOG_SUCCESSFUL);

    if (!GetClientRect(Game.hWnd, &Game.lpRect)) {
        Debug(XorString("Failed to get game window client rect!"), LOG_ERROR);
        return false;
    }

    if (!ClientToScreen(Game.hWnd, &Game.lpPoint)) {
        Debug(XorString("Failed to convert client coordinates to screen!"), LOG_ERROR);
        return false;
    }

    Overlay.WindowSize.x = Game.lpRect.right;
    Overlay.WindowSize.y = Game.lpRect.bottom;
    Debug(XorString("Creating overlay window..."), LOG_INFO);

    Overlay.hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST, Overlay.lpClassName, Overlay.lpWindowName, WS_POPUP | WS_VISIBLE, Game.lpPoint.x, Game.lpPoint.y, Game.lpRect.right - Game.lpRect.left, Game.lpRect.bottom - Game.lpRect.top, NULL, NULL, Overlay.wndClassEx.hInstance, NULL);
    if (!Overlay.hWnd) {
        Debug(XorString("Overlay window creation failed!"), LOG_ERROR);
        return false;
    }

    Debug(XorString("Overlay window created successfully."), LOG_SUCCESSFUL);

    if (!SetLayeredWindowAttributes(Overlay.hWnd, RGB(0, 0, 0), 255, LWA_ALPHA)) {
        Debug(XorString("Failed to set layered window attributes!"), LOG_ERROR);
        return false;
    }

    Debug(XorString("Layered window attributes set successfully."), LOG_SUCCESSFUL);

    MARGINS margin = { -1 };
    if (DwmExtendFrameIntoClientArea(Overlay.hWnd, &margin) != S_OK) {
        Debug(XorString("Failed to extend frame into client area!"), LOG_ERROR);
        return false;
    }
    Debug(XorString("Frame extended into client area successfully."), LOG_SUCCESSFUL);
    return true;
}

bool CreateDeviceD3D() {
    Debug(XorString("Creating D3D11 device and swap chain using hardware driver..."), LOG_INFO);
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = 0;
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = Overlay.hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &swapChainDesc, &Overlay.pSwapChain, &Overlay.pDevice, &featureLevel, &Overlay.pDeviceContext);

    if (res == DXGI_ERROR_UNSUPPORTED) {
        Debug(XorString("Hardware device unsupported. Trying WARP device..."), LOG_INFO);
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &swapChainDesc, &Overlay.pSwapChain, &Overlay.pDevice, &featureLevel, &Overlay.pDeviceContext);
    }

    if (res != S_OK) {
        Debug(XorString("Failed to create D3D11 device and swap chain!"), LOG_ERROR);
        return false;
    }

    Debug(XorString("D3D11 device and swap chain created successfully."), LOG_SUCCESSFUL);
    CreateRenderTarget();
    return true;
}

void CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    Overlay.pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    Overlay.pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &Overlay.pMainRenderTargetView);
    pBackBuffer->Release();
}

void UpdateOverlay() {
    static bool initOverlay = false;
    if (!initOverlay) {
        SetLayeredWindowAttributes(Overlay.hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);
        SetWindowLong(Overlay.hWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
        ShowWindow(Overlay.hWnd, SW_SHOWDEFAULT);
        UpdateWindow(Overlay.hWnd);
        initOverlay = true;
    }

    while (keepRunning) {
        Sleep(500);
        if (!mjLib::Process::Check(Game.pID) || !FindWindow(Game.lpClassName, NULL)) {
            keepRunning = false;
            continue;
        }

        if (screenSizeChanged || sizeChanged) {
            continue;
        }

        RECT currentRect{};
        POINT currentPoint{};
        POINT currentScreenSize = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
        GetClientRect(Game.hWnd, &currentRect);
        ClientToScreen(Game.hWnd, &currentPoint);

        if (currentScreenSize.x != Overlay.ScreenSize.x || currentScreenSize.y != Overlay.ScreenSize.y) {
            screenSizeChanged = true;
        }

        if (Overlay.WindowSize.x != Game.lpRect.right || Overlay.WindowSize.y != Game.lpRect.bottom) {
            Overlay.WindowSize.x = Game.lpRect.right;
            Overlay.WindowSize.y = Game.lpRect.bottom;
            sizeChanged = true;
        }

        if (currentRect.left != Game.lpRect.left || currentRect.right != Game.lpRect.right || currentRect.top != Game.lpRect.top || currentRect.bottom != Game.lpRect.bottom || currentPoint.x != Game.lpPoint.x || currentPoint.y != Game.lpPoint.y) {
            Game.lpRect = currentRect;
            Game.lpPoint = currentPoint;
            SetWindowPos(Overlay.hWnd, nullptr, currentPoint.x, currentPoint.y, currentRect.right - currentRect.left, currentRect.bottom - currentRect.top, SWP_NOREDRAW);
        }

        if (Features::Settings::StreamProof) {
            SetWindowDisplayAffinity(Overlay.hWnd, WDA_EXCLUDEFROMCAPTURE);
        }
        else {
            SetWindowDisplayAffinity(Overlay.hWnd, WDA_NONE);
        }
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }

    switch (msg) {
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) {
            return 0;
        }

        if ((wParam & 0xfff0) == SC_CLOSE) {
            return 0;
        }
        break;
    case WM_DESTROY:
        if (!sizeChanged && !screenSizeChanged) {
            PostQuitMessage(0);
        }
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void CleanupRenderTarget() {
    if (Overlay.pMainRenderTargetView) {
        Overlay.pMainRenderTargetView->Release();
        Overlay.pMainRenderTargetView = nullptr;
    }
}

void CleanupOverlayWindow() {
    if (Overlay.hWnd && IsWindow(Overlay.hWnd)) {
        DestroyWindow(Overlay.hWnd);
        Overlay.hWnd = nullptr;
    }

    if (Overlay.wndClassEx.lpszClassName && Overlay.wndClassEx.hInstance) {
        UnregisterClass(Overlay.wndClassEx.lpszClassName, Overlay.wndClassEx.hInstance);
        Overlay.wndClassEx.lpszClassName = nullptr;
        Overlay.wndClassEx.hInstance = nullptr;
    }
}

void CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (Overlay.pSwapChain) {
        Overlay.pSwapChain->Release();
        Overlay.pSwapChain = nullptr;
    }

    if (Overlay.pDeviceContext) {
        Overlay.pDeviceContext->Release();
        Overlay.pDeviceContext = nullptr;
    }

    if (Overlay.pDevice) {
        Overlay.pDevice->Release();
        Overlay.pDevice = nullptr;
    }
}

void CleanupMenuRenderer() {
    ReleaseTextures();
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    //ImGui::DestroyContext();
}
