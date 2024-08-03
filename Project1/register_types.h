#ifndef RTS_REGISTER_TYPES_H
#define RTS_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

namespace godot{

void initialize_RTS_module(ModuleInitializationLevel p_level);
void uninitialize_RTS_module(ModuleInitializationLevel p_level);
}
#endif // RTS_REGISTER_TYPES_H