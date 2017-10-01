#include "DirectXHardwareInformer.h"

#include "DirectXUtilitary.h"


using namespace MoonRPG;


DirectXHardwareInformer::DirectXHardwareInformer(const DXGI_MODE_DESC& wantedDesc) :
    m_valid{ false }
{
    IDXGIFactory* pFactory = nullptr;
    IDXGIAdapter* pAdapter = nullptr;
    IDXGIOutput* pOutput = nullptr;

    CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pFactory));

    if(FAILED(pFactory->EnumAdapters(0, &pAdapter)))
    {
        return;
    }

    if(FAILED(pAdapter->EnumOutputs(0, &pOutput)))
    {
        return;
    }

    { //current state info
        DXGI_OUTPUT_DESC outDesc;
        pOutput->GetDesc(&outDesc);

        m_width = outDesc.DesktopCoordinates.right - outDesc.DesktopCoordinates.left;
        m_height = outDesc.DesktopCoordinates.bottom - outDesc.DesktopCoordinates.top;

        m_valid = true;

        pOutput->FindClosestMatchingMode(&wantedDesc, &m_mode, nullptr);
    }

    DXGI_ADAPTER_DESC desc;
    pAdapter->GetDesc(&desc);

    m_memory = static_cast<uint64_t>(desc.DedicatedVideoMemory);

    wcscpy_s(m_cardName, DirectXHardwareInformer::CARD_NAME_MAX_LENGHT, desc.Description);

    DXRelease(
        pOutput,
        pAdapter,
        pFactory
    );
}
