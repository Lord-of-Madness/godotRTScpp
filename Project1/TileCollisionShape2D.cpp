#include "TileCollisionShape2D.hpp"
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include "UtilityMacros.hpp"
#include "ExtensionMethods.hpp"
#include <godot_cpp/classes/world2d.hpp>
#include <godot_cpp/classes/physics_direct_space_state2d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters2d.hpp>
namespace godot {
	inline bool TileCollisionShape2D::get_replacing() const {
		return replacing;
	}
	void TileCollisionShape2D::set_tile_layer(int l)
	{
		layer = l;
	}
	int TileCollisionShape2D::get_tile_layer() const
	{
		return layer;
	}
	void TileCollisionShape2D::set_tile_coords(Vector2i c)
	{
		tile_coords = c;
	}
	Vector2i TileCollisionShape2D::get_tile_coords()
	{
		return tile_coords;
	}
	int TileCollisionShape2D::get_tile_source_id() const
	{
		return tile_source_id;
	}
	void TileCollisionShape2D::set_tile_source_id(int s)
	{
		tile_source_id = s;
	}
	void TileCollisionShape2D::_bind_methods()
	{
		PROPERTYADD(PropertyInfo(Variant::VECTOR2I, "SizeInTiles"), get_size, set_size);
		PROPERTYADD(PropertyInfo(Variant::NODE_PATH, "TileMap", PROPERTY_HINT_NODE_TYPE, NAMEOF(TileMap)), get_map, set_map_from_path);
		ADD_GROUP("Replacement tile", "tile");
		PROPERTYADD(PropertyInfo(Variant::INT, "tile_Layer"), get_tile_layer, set_tile_layer);
		PROPERTYADD(PropertyInfo(Variant::INT, "tile_Source ID"), get_tile_source_id, set_tile_source_id);
		PROPERTYADD(PropertyInfo(Variant::VECTOR2I, "tile_Atlas coords"), get_tile_coords, set_tile_coords);
	}
	TileCollisionShape2D::TileCollisionShape2D()
	{
		rect.instantiate();
	}
	TileCollisionShape2D::~TileCollisionShape2D()
	{
		if (cellMatrix.atlascoords.size() != 0)UnlockGround();
	}
	void TileCollisionShape2D::updateParentPosition() {
		Node2D* parent = (Node2D*)get_parent();
		Vector2 new_position = get_parent_position_grided();
		if (get_collisions_at(new_position).size() == 0) {
			parent->set_position(new_position);

			if (cellMatrix.atlascoords.size() != 0)UnlockGround();
			LockGround();
		}
		else parent->set_position(oldPosition);
	}

	inline void TileCollisionShape2D::set_size(const Vector2i& size)
	{
		//PRINT("Setting size");
		//PRINT(size);
		//PRINT("SIZE SET");
		this->size = size;
		rect->set_size(size * TILESIZE);
		if (ready)updateParentPosition();
	}
	inline Vector2i TileCollisionShape2D::get_size() const
	{
		return size;
	}
	TileMap* TileCollisionShape2D::get_map() const {return map; }
	inline void TileCollisionShape2D::set_map(TileMap* tm)
	{
		map = tm;
		TILESIZE = map->get_tileset()->get_tile_size();
		updateParentPosition();
	}
	inline void TileCollisionShape2D::set_map_from_path(const NodePath& n)
	{
		Node* ptr = get_node_or_null(n);
		if (ptr != nullptr && ptr->is_class(NAMEOF(TileMap))) {
			set_map((TileMap*)ptr);
		}
		else {
			map = nullptr;
		}
	}
	inline void TileCollisionShape2D::LockGround() {
		if (replacing && map != nullptr) {
			Vector2i pos((get_parent_position() / 16));
			int offx = 0;
			int offy = 0;
			if (get_parent_position().x < 0 && (size.x % 2 == 1 || pos.x == 0))offx = -1;
			if (get_parent_position().y < 0 && (size.y % 2 == 1 || pos.y == 0))offy = -1;
			cellMatrix.initialize(pos + Vector2i(offx, offy), size);
			for (int h = 0; h < size.y; h++)
				for (int w = 0; w < size.x; w++) {
					Vector2i coords(pos.x + w + offx - size.x / 2, pos.y + h + offy - size.y / 2);
					cellMatrix.atlascoords[h][w] = map->get_cell_atlas_coords(layer, coords);
					map->set_cell(layer, coords, tile_source_id, tile_coords);
				}
		}
	}
	inline void TileCollisionShape2D::UnlockGround() const {
		for (int h = 0; h < cellMatrix.height; h++)
			for (int w = 0; w < cellMatrix.width; w++) {
				map->set_cell(0, cellMatrix.coords + Vector2i(w - cellMatrix.width / 2, h - cellMatrix.height / 2), 33, cellMatrix.atlascoords[h][w]);
			}
	}

	inline bool godot::TileCollisionShape2D::isTileMap(const Dictionary& x) const
	{
		return ((Object*)x["collider"])->is_class(NAMEOF(TileMap));
	}

	/// <summary>
	/// Gets list of collisions at the target location with the area of rect exludes self
	/// </summary>
	/// <param name="place"> centerpoint of the checked area</param>
	/// <returns>intersected shapes</returns>

	inline TypedArray<Dictionary> TileCollisionShape2D::get_collisions_at(Vector2 place, int32_t max_results) const {
		Ref<PhysicsShapeQueryParameters2D> query;
		query.instantiate();
		query->set_shape(rect);
		query->set_transform(Transform2D(0, place));
		TypedArray<Dictionary> found_collisions = get_world_2d()->get_direct_space_state()->intersect_shape(query, max_results);
		for (int64_t i = 0; i < found_collisions.size(); i++)//cleaning self from collisions list
		{
			if ((uint64_t)(found_collisions[i].get("collider_id")) == get_parent()->get_instance_id()) { found_collisions.remove_at(i);  break; }
		}
		return found_collisions;

	}

	/// <summary>
	/// gets parent node position if possible
	/// </summary>
	/// <returns>position of the parent node, Vector.ZERO if unable</returns>

	inline Vector2 TileCollisionShape2D::get_parent_position() const {
		if (get_parent() != nullptr && get_parent()->is_class(NAMEOF(Node2D))) {
			return ((Node2D*)get_parent())->get_position();
		}
		return Vector2(0, 0);
	}

	/// <summary>
	/// gets parents position snapped to the grid
	/// </summary>
	/// <returns></returns>

	inline Vector2 TileCollisionShape2D::get_parent_position_grided() {
		Vector2 size = rect->get_size();
		Vector2 rectmid = (size / 2) % 16;
		return ((get_parent_position() / 16).floor()) * 16 + rectmid;
	}

	inline void TileCollisionShape2D::try_set_map() {
		Node* node = nullptr;
		//scene root is accessed differently in game and in editor
		Node* scene_root = (Engine::get_singleton()->is_editor_hint()) ? get_tree()->get_edited_scene_root() : get_tree()->get_current_scene();
		if (!scene_root->is_class(NAMEOF(PhysicsBody2D))) {
			node = scene_root->get_node_or_null("Ground");
			if (node == nullptr)node = scene_root->get_node_or_null("Map/Ground");
		}
		if (node != nullptr && node->is_class(NAMEOF(TileMap))) {
			set_map((TileMap*)node);
		}
	}

	inline void TileCollisionShape2D::set_replacing(bool r) {
		replacing = r;
		if (r)LockGround();
		else UnlockGround();
	}

	inline void TileCollisionShape2D::_ready()
	{
		//PRINT("Ready");
		ready = true;
		set_shape(rect);


		if (!Engine::get_singleton()->is_editor_hint()) {
			oldPosition = get_parent_position_grided();
			set_size(size);
			replacing = true;
			if (get_map() == nullptr)try_set_map();
			if (cellMatrix.atlascoords.size() == 0)LockGround();
		}
		else {
			set_size(size);
		}
	}
	inline void TileCollisionShape2D::_process(const double delta) {
		if (get_map() == nullptr)try_set_map();
		if (get_parent()->is_class(NAMEOF(Node2D))) {
			Node2D* parent = (Node2D*)get_parent();
			if (oldPosition != parent->get_position() && map != nullptr) {
				updateParentPosition();
				oldPosition = parent->get_position();
			}
		}
	}
}