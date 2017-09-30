#include "DirectXGlobalDevice.h"

#include "DirectXHardwareInformer.h"

#include "GameConfig.h"
#include "DirectXUtilitary.h"

using namespace AdriSunRPG;


DirectXGlobalDevice::DirectXGlobalDevice() :
    m_depthTexture{ nullptr },
    m_depthStencilView{ nullptr },
    m_solidCullBackRS{ nullptr },
    m_solidCullNoneRS{ nullptr },
    m_wireframeCullNoneRS{ nullptr },
    m_zBufferEnable{ nullptr },
    m_zBufferDisable{ nullptr },
    m_alphaBlendEnable{ nullptr },
    m_alphaBlendDisable{ nullptr },
    m_renderTargetView{ nullptr },
    m_swapChain{ nullptr },
    m_D3DDevice{ nullptr },
    m_immediateContext{ nullptr }
{

}

DirectXGlobalDevice::~DirectXGlobalDevice()
{
    DXRelease(
        m_depthTexture,
        m_solidCullBackRS,
        m_solidCullNoneRS,
        m_wireframeCullNoneRS,
        m_zBufferEnable,
        m_zBufferDisable,
        m_alphaBlendEnable,
        m_alphaBlendDisable
    );

    this->unbindTargetView();

    if(m_immediateContext != nullptr)
    {
        m_immediateContext->ClearState();
        m_immediateContext->Release();
        m_immediateContext = nullptr;
    }

    if(m_swapChain != nullptr)
    {
        //swap chain must be windowed if we want to release it.
        m_swapChain->SetFullscreenState(FALSE, nullptr);
        m_swapChain->Release();
    }

    DXRelease(
        m_renderTargetView,
        m_depthStencilView,
        m_D3DDevice
    );

    this->reportLiveObject();
    DXRelease(m_DXDebugDevice);
}

void DirectXGlobalDevice::initialize(const HWND hWnd)
{
    { //Create the device, the immediate context and the swap chain altogether

      //Swap chain mode initialization
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        DXZeroMemory(swapChainDesc);

        {
            DXGI_MODE_DESC descModeWanted;
            descModeWanted.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            descModeWanted.Height = AdriSunRPG::WINDOWED_HEIGTH_RESOLUTION;
            descModeWanted.Width = AdriSunRPG::WINDOWED_WIDTH_RESOLUTION;
            descModeWanted.RefreshRate.Numerator = AdriSunRPG::ENGINE_FPS;
            descModeWanted.RefreshRate.Denominator = 1;
            descModeWanted.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            descModeWanted.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

            DirectXHardwareInformer infos{ descModeWanted };

            swapChainDesc.BufferDesc.Format = infos.m_mode.Format;

            swapChainDesc.BufferDesc.Width = infos.m_mode.Width;
            swapChainDesc.BufferDesc.Height = infos.m_mode.Height;

            m_screenWidth = static_cast<float>(swapChainDesc.BufferDesc.Width);
            m_screenHeight = static_cast<float>(swapChainDesc.BufferDesc.Height);
        }

        swapChainDesc.BufferDesc.RefreshRate.Numerator = AdriSunRPG::ENGINE_FPS;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;

        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 1;

        swapChainDesc.OutputWindow = hWnd;

        swapChainDesc.Windowed = AdriSunRPG::WINDOW_SIZE_CONFIGURATION;

        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        {
            RECT rcClient;
            RECT rcWindow;
            GetClientRect(hWnd, &rcClient);
            GetWindowRect(hWnd, &rcWindow);

            POINT ptDiff{ (rcWindow.right - rcWindow.left) - rcClient.right, (rcWindow.bottom - rcWindow.top) - rcClient.bottom };

            MoveWindow(
                hWnd,
                rcWindow.left,
                rcWindow.top,
                swapChainDesc.BufferDesc.Width + ptDiff.x,
                swapChainDesc.BufferDesc.Height + ptDiff.y,
                TRUE
            );
        }

        UINT deviceCreationFlags = 0;

#ifdef _DEBUG
        deviceCreationFlags |= D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif

        constexpr const D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
        };

        DXTry(D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            deviceCreationFlags,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            &swapChainDesc,
            &m_swapChain,
            &m_D3DDevice,
            nullptr,
            &m_immediateContext
        ));
    }

    this->internalCreateRenderView();
    this->internalInitializeDepthBuffer();

    this->internalConfigureStates();

    this->internalInitializeDebugDevice();

    this->internalConfigureImmediateContextToDefault();

    this->internalInitializeViewPort();
}

void DirectXGlobalDevice::internalCreateRenderView()
{
    ID3D11Texture2D* backBuffer;

    DXTry(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBuffer)));
    DXTry(m_D3DDevice->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView));

    backBuffer->Release();
}

void DirectXGlobalDevice::internalInitializeDepthBuffer()
{
    D3D11_TEXTURE2D_DESC depthTextureDesc;
    DXZeroMemory(depthTextureDesc);

    depthTextureDesc.Width = static_cast<UINT>(m_screenWidth);
    depthTextureDesc.Height = static_cast<UINT>(m_screenHeight);
    depthTextureDesc.MipLevels = 1;
    depthTextureDesc.ArraySize = 1;
    depthTextureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthTextureDesc.SampleDesc.Count = 1;
    depthTextureDesc.SampleDesc.Quality = 0;
    depthTextureDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
    depthTextureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
    depthTextureDesc.CPUAccessFlags = 0;

    DXTry(m_D3DDevice->CreateTexture2D(&depthTextureDesc, nullptr, &m_depthTexture));

    D3D11_DEPTH_STENCIL_VIEW_DESC descStencilView;
    DXZeroMemory(descStencilView);

    descStencilView.Format = depthTextureDesc.Format;
    descStencilView.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
    descStencilView.Texture2D.MipSlice = 0;

    DXTry(m_D3DDevice->CreateDepthStencilView(m_depthTexture, &descStencilView, &m_depthStencilView));
}

void DirectXGlobalDevice::internalConfigureStates()
{
    { //Blend State creation
        D3D11_BLEND_DESC blendDesc;
        DXZeroMemory(blendDesc);

        blendDesc.IndependentBlendEnable = FALSE;

        blendDesc.RenderTarget[0].BlendEnable = TRUE;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        DXTry(m_D3DDevice->CreateBlendState(&blendDesc, &m_alphaBlendEnable));

        blendDesc.RenderTarget[0].BlendEnable = FALSE;

        DXTry(m_D3DDevice->CreateBlendState(&blendDesc, &m_alphaBlendDisable));
    }

    this->setEnableBlendAlpha(true);

    { //Rasterizer state creation
        D3D11_RASTERIZER_DESC rsDesc;
        DXZeroMemory(rsDesc);

        rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        rsDesc.FrontCounterClockwise = FALSE;
        m_D3DDevice->CreateRasterizerState(&rsDesc, &m_solidCullBackRS);

        rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

        DXTry(m_D3DDevice->CreateRasterizerState(&rsDesc, &m_solidCullNoneRS));

        rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

        DXTry(m_D3DDevice->CreateRasterizerState(&rsDesc, &m_wireframeCullNoneRS));
    }

    this->setSolidCullBackState();

    // Z-Buffer State creation
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    DXZeroMemory(depthStencilDesc);

    depthStencilDesc.DepthEnable = FALSE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;

    DXTry(m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_zBufferDisable));

    depthStencilDesc.DepthEnable = TRUE;

    DXTry(m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_zBufferEnable));

    this->setEnableZBuffer(true);
}

void DirectXGlobalDevice::internalInitializeDebugDevice()
{
    try
    {
        DXTry(m_D3DDevice->QueryInterface(&m_DXDebugDevice));
        //DXTry(m_Device->QueryInterface(IID_PPV_ARGS(&m_DXDebugDevice)));
    }
    catch(HRESULT resultException)
    {
        //TODO : Log something

        //LOG_DEBUG(m_Logger, "Error : Failed to initialize dispositif debug\nError : " << resultException);
    }
}

void DirectXGlobalDevice::internalConfigureImmediateContextToDefault()
{
    m_immediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DirectXGlobalDevice::internalInitializeViewPort()
{
    D3D11_VIEWPORT viewPort;

    viewPort.Width = m_screenWidth;
    viewPort.Height = m_screenHeight;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;

    m_immediateContext->RSSetViewports(1, &viewPort);
}

void DirectXGlobalDevice::setWireFrameState()
{
    m_immediateContext->RSSetState(m_wireframeCullNoneRS);
}

void DirectXGlobalDevice::setSolidCullNoneState()
{
    m_immediateContext->RSSetState(m_solidCullNoneRS);
}

void DirectXGlobalDevice::setSolidCullBackState()
{
    m_immediateContext->RSSetState(m_solidCullBackRS);
}

void DirectXGlobalDevice::setEnableZBuffer(bool enable)
{
    m_immediateContext->OMSetDepthStencilState(enable ? m_zBufferEnable : m_zBufferDisable, 0);
}

void DirectXGlobalDevice::setEnableBlendAlpha(bool enable)
{
    m_immediateContext->OMSetBlendState(enable ? m_alphaBlendEnable : m_alphaBlendDisable, BLEND_FACTOR, 0xFFFFFFFF);
}

void DirectXGlobalDevice::clearView()
{
    m_immediateContext->ClearRenderTargetView(m_renderTargetView, BACKGROUND_DEFAULT_COLOR);
    m_immediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.f, 0);
}

void DirectXGlobalDevice::unbindTargetView()
{
    ID3D11RenderTargetView* voidTargetView = nullptr;

    m_immediateContext->OMSetRenderTargets(1, &voidTargetView, nullptr);

    if(voidTargetView != nullptr)
    {
        voidTargetView->Release();
    }
}

void DirectXGlobalDevice::initView()
{
    m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}

void DirectXGlobalDevice::presentToDisplay()
{
    m_swapChain->Present(0, 0);
}

void DirectXGlobalDevice::reportLiveObject()
{
    if(m_DXDebugDevice != nullptr)
    {
        DXTry(m_DXDebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_DETAIL));
    }
}
