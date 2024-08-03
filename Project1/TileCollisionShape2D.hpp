#ifndef RTS_TILECOLLISIONSHAPE2D
#define RTS_TILECOLLISIONSHAPE2D

#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/classes/tile_map.hpp>

#include "CellMatrix.hpp"

namespace godot {

	/// <summary>
	/// Collider used for CharacterBodies placed on Tilemaps
	/// Snaps to the Tilemap grid (in editor and in game)
	/// Looks for default TileMap named "Ground" under scene root
	/// </summary>
	class TileCollisionShape2D :public CollisionShape2D
	{
		GDCLASS(TileCollisionShape2D, CollisionShape2D)
	public:
		TileCollisionShape2D();
		~TileCollisionShape2D();
		void set_size(const Vector2i& size);
		Vector2i get_size() const;
		TileMap* get_map() const;
		void set_map(TileMap* tm);
		void set_map_from_path(const NodePath& n);
		void updateParentPosition();
		void LockGround();
		void UnlockGround() const;
		virtual void _ready()override;
		virtual void _process(const double delta) override;
		bool isTileMap(const Dictionary& x)const;
		/// <summary>
		/// Gets list of collisions at the target location with the area of rect exludes self
		/// </summary>
		/// <param name="place"> centerpoint of the checked area</param>
		/// <returns>intersected shapes</returns>
		TypedArray<Dictionary> get_collisions_at(Vector2 place,int32_t max_results = 50) const;
		/// <summary>
		/// gets parent node position if possible
		/// </summary>
		/// <returns>position of the parent node, Vector.ZERO if unable</returns>
		Vector2 get_parent_position() const;
		/// <summary>
		/// gets parents position snapped to the grid
		/// </summary>
		/// <returns></returns>
		Vector2 get_parent_position_grided();
		/// <summary>
		/// Looks at the expected locations for map (root/Ground or root/Map/Ground)
		/// </summary>
		void try_set_map();
		void set_replacing(bool r);
		bool get_replacing() const;
		void set_tile_layer(int l);
		int get_tile_layer()const;
		void set_tile_coords(Vector2i c);
		Vector2i get_tile_coords();
		int get_tile_source_id()const;
		void set_tile_source_id(int s);
	protected:
		static void _bind_methods();
		Ref<RectangleShape2D> rect;
		TileMap* map = nullptr;
		Vector2i size = Vector2i(0, 0);
		CellMatrix cellMatrix = CellMatrix();
		bool replacing = false;
		bool ready = false;
	private:
		Vector2 TILESIZE{ 16,16 };//defaulting to 16/16 when map not present
		Vector2 oldPosition = Vector2(0, 0);
		int layer = 0;
		Vector2i tile_coords = Vector2i(14, 6);
		int tile_source_id = 33;
	};
	
}
#endif //RTS_TILESCOLLISIONSHAPE2D