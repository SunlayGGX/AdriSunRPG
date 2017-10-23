#pragma once

#include "targetver.h"



/*Standard*/
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>



/*Direct X*/
#include <DirectXMath.h>
#include <D3D11.h>
#include <Xinput.h>
#include <dinput.h>



#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "XInput.lib")

#ifdef _DEBUG
#   pragma comment(lib, "d3dx11d.lib")
#else
#   pragma comment(lib, "d3dx11.lib")
#endif



/*MoonRPG*/
#include "MoonRPGElephantLoggerPch.h"

#ifdef _DEBUG
#   pragma comment(lib, "MoonRPGHelper_d.lib")
#   pragma comment(lib, "MoonRPGElephantLogger_d.lib")
#else
#   pragma comment(lib, "MoonRPGHelper.lib")
#   pragma comment(lib, "MoonRPGElephantLogger.lib")
#endif

