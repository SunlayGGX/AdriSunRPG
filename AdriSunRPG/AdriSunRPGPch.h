// AdriSunRPGPch.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
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
