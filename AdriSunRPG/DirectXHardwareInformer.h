#pragma once

#include "UtilsMacro.h"

namespace AdriSunRPG
{
    class DirectXHardwareInformer
    {
    public:
        enum
        {
            CARD_NAME_MAX_LENGHT = 128
        };


    public:
        bool m_valid;
        int m_width;
        int m_height;
        uint64_t m_memory;
        wchar_t m_cardName[DirectXHardwareInformer::CARD_NAME_MAX_LENGHT];
        DXGI_MODE_DESC m_mode;


    public:
        DirectXHardwareInformer(const DXGI_MODE_DESC& wantedDesc);
        ~DirectXHardwareInformer() = default;
    };
}

