// MoonRPGPch.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
//

#pragma once


#include "UtilsMacro.h"


/*Windows*/
#define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#undef WIN32_LEAN_AND_MEAN


#include "MoonRPGHelperPch.h"
#include "MoonRPGElephantLoggerPch.h"
#include "targetver.h"


/*Direct X*/
#include <DirectXMath.h>
#include <D3D11.h>


/*Standard*/
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <assert.h>
#include <stdexcept>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>


/*Lib include*/

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#ifdef _DEBUG
#   pragma comment(lib, "MoonRPGHelper_d.lib")
#   pragma comment(lib, "MoonRPGElephantLogger_d.lib")
#else
#   pragma comment(lib, "MoonRPGHelper.lib")
#   pragma comment(lib, "MoonRPGElephantLogger.lib")
#endif

