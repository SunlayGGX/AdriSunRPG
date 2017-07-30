#pragma once

#define CONCAT(x,y) x##y
#define CONCATS(x, y, z) CONCAT(x, y)##z

#define STRINGIFY(x) #x


#define ADRISUN_DECLARE_SLG_SINGLETON_ALLOCATOR_HELPER_FOR(Type) template<class Type> class AdriSunSingletonAllocatorHelper;

/*
Macro for generating :
- prototype
- usings for weak, shared and unique pointer
*/
#define ADRISUN_GENERATE_GLOBAL_DATA_FOR(ClassOrStruct, ClassName) \
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
#define ADRISUN_GENERATE_GLOBAL_DATA_FOR_CLASS(ClassName) ADRISUN_GENERATE_GLOBAL_DATA_FOR(class, ClassName)

/*
Macro for generating :
- struct prototype
- usings for weak, shared and unique pointer
*/
#define ADRISUN_GENERATE_GLOBAL_DATA_FOR_STRUCT(StructName) ADRISUN_GENERATE_GLOBAL_DATA_FOR(struct, ClassName)



#define ADRISUN_ERROR_CODE_LOCATION_INFO_WITH_LINE(line)                    "FILE : " __FILE__ " , LINE : " STRINGIFY(line)
#define ADRISUN_ERROR_CODE_LOCATION_INFO_WITH_LINE_AND_FUNCTION_NAME(line)  "FUNCTION : " __FUNCTION__ " , " ADRISUN_ERROR_CODE_LOCATION_INFO_WITH_LINE(line) 


#define ADRISUN_ERROR_CODE_LOCATION_INFO                        ADRISUN_ERROR_CODE_LOCATION_INFO_WITH_LINE(__LINE__)
#define ADRISUN_ERROR_CODE_LOCATION_WITH_FUNCTION_NAME_INFO     ADRISUN_ERROR_CODE_LOCATION_INFO_WITH_LINE_AND_FUNCTION_NAME(__LINE__)

#define ADRISUN_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION(Message)   "STATIC ASSERT ERROR :  " ADRISUN_ERROR_CODE_LOCATION_INFO " , ERROR INFO : " Message
#define ADRISUN_NORMALIZE_STATIC_ASSERT_MESSAGE_INSIDE_FUNCTION(Message)    "STATIC ASSERT ERROR :  " ADRISUN_ERROR_CODE_LOCATION_WITH_FUNCTION_NAME_INFO " , ERROR INFO : " Message
