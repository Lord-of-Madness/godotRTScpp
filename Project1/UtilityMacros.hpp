#ifndef RTS_UTILITYMACROS
#define RTS_UTILITYMACROS
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/class_db.hpp>

#define NAMEOF(name) #name

#define METHODADD(funcName)\
ClassDB::bind_method(D_METHOD(NAMEOF(funcName)),&funcName)

#define PRINT(what)UtilityFunctions::print(what)

#define PROPERTYADD(propertyInfo,getFunc,setFunc) \
METHODADD(getFunc);\
METHODADD(setFunc);\
ADD_PROPERTY(propertyInfo,NAMEOF(setFunc), NAMEOF(getFunc))
#endif // !RTS_UTILITYMACROS