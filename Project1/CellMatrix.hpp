#ifndef CELLMATRIX
#define CELLMATRIX
#include<vector>
#include<godot_cpp/variant/variant.hpp>
namespace godot {
	class CellMatrix {
	public:
		std::vector<std::vector<Vector2i>> atlascoords;
		Vector2i coords;
		int32_t height = 0;
		int32_t width = 0;
		CellMatrix(Vector2i root, Vector2i size) {
			initialize(root, size);
		}
		CellMatrix() {
			coords = Vector2i(0, 0);
			atlascoords = std::vector<std::vector<Vector2i>>();
		}
		void initialize(Vector2i root, Vector2i size) {
			height = size.y;
			width = size.x;
			coords = root;
			atlascoords.reserve(height);
			for (int i = 0; i < height; i++) {
				atlascoords.push_back(std::vector<Vector2i>(width));
				for (int j = 0; j < width; j++)
					atlascoords[i].push_back(Vector2i());
			}
		}
	};
}
#endif // !CELLMATRIX