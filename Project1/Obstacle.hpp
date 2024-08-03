#ifndef RTSObstacle
#define RTSObstacle
#include "CellMatrix.hpp"
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/classes/static_body2d.hpp>
#include <godot_cpp/classes/tile_map.hpp>

namespace godot {
	class Obstacle :public StaticBody2D
	{
		GDCLASS(Obstacle, StaticBody2D)
	public:
		Obstacle();
		~Obstacle();
		virtual void _ready() override;
		virtual void _exit_tree() override;
		void set_pos();
		void set_height(const int h);
		int get_height() const;
		void set_width(const int w);
		int get_width() const;
		TileMap* get_map() const;
		void set_map(TileMap* m);
		void set_map_from_path(const NodePath& n);
		void LockGround();
		void UnlockGround() const;
		virtual void _process(const double delta) override;
		bool replacing = false;
		bool get_replacing()const;
		void set_replacing(const bool b);
		int get_tileSize();
	protected:
		static void _bind_methods();
	private:
		CellMatrix cm;
		Vector2 oldPosition = Vector2(0, 0);
		int TILESIZE;
		int height = 1;
		int width = 1;
		CollisionShape2D collider;
		TileMap* map = nullptr;
		Ref<RectangleShape2D> rect;

	};

	
}
#endif // !RTSObstacle
