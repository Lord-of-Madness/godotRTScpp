#include "Obstacle.hpp"
#include "UtilityMacros.hpp"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include<godot_cpp/variant/utility_functions.hpp>
#include "ExtensionMethods.hpp"
using namespace godot;


Obstacle::Obstacle()
{
	rect.instantiate();
	collider.set_shape(rect);
}
Obstacle::~Obstacle()
{
}
void Obstacle::_ready() {
	add_child(&collider);
	set_pos();
	set_map_from_path(get_parent()->get_path());
	if (!Engine::get_singleton()->is_editor_hint()) {
		set_replacing(true);
	}
}
void godot::Obstacle::_exit_tree()
{
	UnlockGround();
}
inline void Obstacle::set_pos() {
	Vector2 size(width * TILESIZE, height * TILESIZE);
	rect.ptr()->set_size(size);
	Vector2 rectmid = (size / 2) % 16;
	set_position(((get_position() / 16).floor()) * 16 + rectmid);
	if (cm.atlascoords.size() != 0)UnlockGround();
	LockGround();
}
inline void Obstacle::set_height(const int h) {
	height = h;
	set_pos();
}
inline int Obstacle::get_height() const { return height; }
inline void Obstacle::set_width(const int w) {
	width = w;
	set_pos();
}
inline int Obstacle::get_width() const { return width; }
inline TileMap* Obstacle::get_map() const { return map; }
inline void Obstacle::set_map(TileMap* m) {
	map = m;
}
inline void Obstacle::set_map_from_path(const NodePath& n) {
	Node* ptr = get_node_or_null(n);
	if (ptr == nullptr) {
		map = nullptr;
	}
	else if (ptr->is_class(NAMEOF(TileMap))) {
		set_map((TileMap*)ptr);
		TILESIZE = get_tileSize();
	}
	else {
		map = nullptr;
	}
}
inline void Obstacle::LockGround() {
	if (replacing && map != nullptr) {
		Vector2i pos((get_position() / 16));
		int offx = 0;
		int offy = 0;
		if (get_position().x < 0 && (width % 2 == 1 || pos.x == 0))offx = -1;
		if (get_position().y < 0 && (height % 2 == 1 || pos.y == 0))offy = -1;
		cm = CellMatrix(pos + Vector2i(offx, offy), Vector2i(width, height)) ;
		for (int h = 0; h < height; h++)
			for (int w = 0; w < width; w++) {
				Vector2i coords(pos.x + w + offx - width / 2, pos.y + h + offy - height / 2);
				cm.atlascoords[h][w] = map->get_cell_atlas_coords(0, coords);
				map->set_cell(0, coords, 33, Vector2i(14, 6));
			}
	}
}
inline void Obstacle::UnlockGround() const {
	for (int h = 0; h < cm.height; h++)
		for (int w = 0; w < cm.width; w++) {
			map->set_cell(0, cm.coords + Vector2i(w - cm.width / 2, h - cm.height / 2), 33, cm.atlascoords[h][w]);
		}
}
inline void Obstacle::_process(const double delta) {
	if (oldPosition != get_position() && map != nullptr) {
		set_pos();
		oldPosition = get_position();
	}
}
inline bool Obstacle::get_replacing() const { return replacing; }
inline void Obstacle::set_replacing(const bool b) {
	replacing = b;
	if (b)LockGround();
	else UnlockGround();
}
int Obstacle::get_tileSize()
{
	return map->get_tileset()->get_tile_size().x;//Assuming tiles are squares
}
void Obstacle::_bind_methods()
{
	PROPERTYADD(PropertyInfo(Variant::INT, NAMEOF(height), PROPERTY_HINT_RANGE, "0,999,1"), get_height, set_height);
	PROPERTYADD(PropertyInfo(Variant::INT, NAMEOF(width), PROPERTY_HINT_RANGE, "0,999,1"), get_width, set_width);
	PROPERTYADD(PropertyInfo(Variant::BOOL, NAMEOF(replacing)), get_replacing, set_replacing);
	PROPERTYADD(PropertyInfo(Variant::NODE_PATH, NAMEOF(map), PROPERTY_HINT_NODE_TYPE, "Map onto which we put obstacle"), get_map, set_map_from_path);
}
