#pragma once

#define CONCAT(x,y) x##y
#define CONCATS(x, y, z) CONCAT(x, y)##z

#define STRINGIFY(x) #x


#define MOON_DECLARE_SLG_SINGLETON_ALLOCATOR_HELPER template<class Type> class MoonSingletonAllocatorHelper;

/*
Macro for generating :
- prototype
- usings for weak, shared and unique pointer
*/
#define MOON_GENERATE_GLOBAL_DATA_FOR(ClassOrStruct, ClassName) \
ClassOrStruct ClassName; \
using ClassName##Ref = std::shared_ptr<ClassName>; \
using ClassName##UniqueRef = std::unique_ptr<ClassName>; \
using ClassName##WeakRef = std::weak_ptr<ClassName>; \
using ClassName##ShArray = std::vector<ClassName##Ref>; \
using ClassName##UArray = std::vector<ClassName##UniqueRef>; \
using ClassName##WArray = std::vector<ClassName##WeakRef>; \


/*
Macro for generating :
- class prototype
- usings for weak, shared and unique pointer
*/
#define MOON_GENERATE_GLOBAL_DATA_FOR_CLASS(ClassName) MOON_GENERATE_GLOBAL_DATA_FOR(class, ClassName)

/*
Macro for generating :
- struct prototype
- usings for weak, shared and unique pointer
*/
#define MOON_GENERATE_GLOBAL_DATA_FOR_STRUCT(StructName) MOON_GENERATE_GLOBAL_DATA_FOR(struct, ClassName)



#define MOON_ERROR_CODE_LOCATION_INFO_WITH_LINE(line)                    "FILE : " __FILE__ " , LINE : " STRINGIFY(line)
#define MOON_ERROR_CODE_LOCATION_INFO_WITH_LINE_AND_FUNCTION_NAME(line)  "FUNCTION : " __FUNCTION__ " , " MOON_ERROR_CODE_LOCATION_INFO_WITH_LINE(line) 


#define MOON_ERROR_CODE_LOCATION_INFO                        MOON_ERROR_CODE_LOCATION_INFO_WITH_LINE(__LINE__)
#define MOON_ERROR_CODE_LOCATION_WITH_FUNCTION_NAME_INFO     MOON_ERROR_CODE_LOCATION_INFO_WITH_LINE_AND_FUNCTION_NAME(__LINE__)

#define MOON_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION(Message)   "STATIC ASSERT ERROR :  " MOON_ERROR_CODE_LOCATION_INFO " , ERROR INFO : " Message
#define MOON_NORMALIZE_STATIC_ASSERT_MESSAGE_INSIDE_FUNCTION(Message)    "STATIC ASSERT ERROR :  " MOON_ERROR_CODE_LOCATION_WITH_FUNCTION_NAME_INFO " , ERROR INFO : " Message

#define FORCEINLINE __forceinline
