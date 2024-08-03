# TileCollisionShape2D
This GDExtension adds an extra ```CollisionShape2D``` derived node ```TileCollisionShape2D```
and extra type of ```StaticBody2D``` - ```Obstacle``` - that behaves similarly

It is primarly made for the godot-rts toolkit but can be used in any project.

## TileCollisionShape2D
Derives from ```CollisionShape2D``` and has fixed ```RectangleShape2D``` as its ```Shape```. Once added to a parent ```Node2D``` as a child and given ```TileMap``` in the Inspector it will snap the parents position to the ```TileMap```'s grid.
- Works in game and in editor.
- Will attempt automatically attempt to find the ```TileMap``` in "rootnode/Ground" or "rootnode/Map/Ground" (which is the default location in godot-rts)
- In game it will also replace the tiles beneath it with specified tile (specified in editor). Used primarly for pathfinding purposes. Pathfinding in Godot uses NavigationLayer on a Tilemap and ignores collision. If an object has this CollisionShape the tiles beneath it will be replaced by unpathable ones without the need to replace them manually every time the object is moved either in editor or in game.

## Obstacle
Derives from ```StaticBody2D``` and is meant to be used as a collision object that snaps to a Tilemap and renders tiles beneath it unpathable
##### Notes
Has a lot of copypasted code from TileCollisionShape2D because adding custom child underneath a different custom node (Obstacle.add_child(TileCollisionShape2D)) will strip the child of the custom code leaving it CollisionShape2D -> therefore the custom functionality breaks.


## Notes
Godot 4.2 doesn't allow for in-editor documentation for GDExtension.
currently merged with Godot-master branch and expected in 4.3