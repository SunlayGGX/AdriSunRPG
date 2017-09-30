// AdriSunRPGPch.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once


#include "UtilsMacro.h"


#define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#undef WIN32_LEAN_AND_MEAN



#include "targetver.h"

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <assert.h>
#include <stdexcept>



#ifdef _DEBUG
#   pragma comment(lib, "AdriSunRPGHelper_d.lib")
#else
#   pragma comment(lib, "AdriSunRPGHelper.lib")
#endif
