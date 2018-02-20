// MoonRPGPch.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once


#include "UtilsMacro.h"


/*Windows*/
#define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#undef WIN32_LEAN_AND_MEAN


#include "MoonRPGHelperPch.h"
#include "MoonRPGElephantLoggerPch.h"
#include "MoonRPGInputPch.h"
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

#ifdef _DEBUG
#   pragma comment(lib, "MoonRPGHelper_d.lib")
#   pragma comment(lib, "MoonRPGInput_d.lib")
#   pragma comment(lib, "MoonRPGElephantLogger_d.lib")
#else
#   pragma comment(lib, "MoonRPGHelper.lib")
#   pragma comment(lib, "MoonRPGInput.lib")
#   pragma comment(lib, "MoonRPGElephantLogger.lib")
#endif

