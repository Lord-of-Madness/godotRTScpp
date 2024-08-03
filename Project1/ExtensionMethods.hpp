#ifndef ExtensionMethods
#define ExtensionMethods
#include <godot_cpp/variant/vector2.hpp>
namespace godot{
static Vector2 operator%(Vector2 v, int m) {
	return Vector2(
		v.x - ((int)(v.x / m)) * m,
		v.y - ((int)(v.y / m)) * m
	);
}
}
#endif // !ExtensionMethods

