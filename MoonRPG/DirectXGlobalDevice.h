#pragma once

namespace MoonRPG
{
    class DirectXGlobalDevice
    {
    public:
        const float BACKGROUND_DEFAULT_COLOR[4] = { 0.f, 0.5f, 0.f, 1.f };
        const float BLEND_FACTOR[4] = { 0.f, 0.f, 0.f, 0.f };


    private:
        ID3D11Device* m_D3DDevice;
        ID3D11DeviceContext* m_immediateContext;
        IDXGISwapChain* m_swapChain;
        ID3D11RenderTargetView* m_renderTargetView;

        ID3D11Texture2D* m_depthTexture;
        ID3D11DepthStencilView* m_depthStencilView;

        ID3D11RasterizerState* m_solidCullBackRS;
        ID3D11RasterizerState* m_solidCullNoneRS;
        ID3D11RasterizerState* m_wireframeCullNoneRS;

        ID3D11DepthStencilState* m_zBufferEnable;
        ID3D11DepthStencilState* m_zBufferDisable;

        ID3D11BlendState* m_alphaBlendEnable;
        ID3D11BlendState* m_alphaBlendDisable;

        float m_screenWidth;
        float m_screenHeight;

        ID3D11Debug* m_DXDebugDevice;


    public:
        DirectXGlobalDevice();
        ~DirectXGlobalDevice();


    public:
        void initialize(const HWND hWnd);

        void setWireFrameState();
        void setSolidCullNoneState();
        void setSolidCullBackState();

        void setEnableZBuffer(bool enable);
        void setEnableBlendAlpha(bool enable);


    public:
        void clearView();

        void initView();

        void presentToDisplay();

        void unbindTargetView();

        //Debug
    public:
        void reportLiveObject();


    private:
        void internalCreateRenderView();
        void internalInitializeDepthBuffer();

        void internalConfigureStates();

        void internalInitializeDebugDevice();

        void internalConfigureImmediateContextToDefault();

        void internalInitializeViewPort();


    public:
        FORCEINLINE ID3D11Device* getD3DDevice() const
        {
            return m_D3DDevice;
        }

        FORCEINLINE ID3D11DeviceContext* getImmediateContext() const
        {
            return m_immediateContext;
        }

        FORCEINLINE IDXGISwapChain* getSwapChain() const
        {
            return m_swapChain;
        }

        FORCEINLINE ID3D11RenderTargetView* getRenderTargetView() const
        {
            return m_renderTargetView;
        }

        FORCEINLINE ID3D11Texture2D* getDepthTexture() const
        {
            return m_depthTexture;
        }

        FORCEINLINE ID3D11DepthStencilView* getDepthStencilView() const
        {
            return m_depthStencilView;
        }
        
        FORCEINLINE float getScreenWidth() const
        {
            return m_screenWidth;
        }

        FORCEINLINE float getScreenHeight() const
        {
            return m_screenHeight;
        }
    };
}
