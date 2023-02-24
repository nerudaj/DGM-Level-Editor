# Writing your own config

In order to use the level editor in your own project, you first need to write a config that tells the editor where it can find all the sprites, tilesets, which tiles should be blocking, what trigger actions do you have, etc.

When you look at the [sample_config](../sample_project/sample-config.json), you can see that the config has couple basic sections

```json
{
	"toolMesh": {}
	"toolItem": {}
	"toolTrigger": {}
}
```

Each of these top level keys matches one of the editor modes of operation:

 * Mesh mode - drawing level tiles
 * Item mode - placing objects - player spawn, items, monster spawns
 * Triggers mode - placing invisible areas that trigger scripted actions when activated

Each of these modes of operation needs its own configuration, so let's look at each mode's config.

# Mesh mode

Uses the `toolMesh` key. Tool meash needs to know a path to tileset texture. This texture needs to hold all of the tiles, you are going to use, but it doesn't have to be the production one (if you are employing some autotexturing, then you can use the editor just to do some blockmesh and you can design quite small tileset for such purpose).

Path to texture can be relative or absolute. For the texture to load properly, editor needs to know pixel dimensions of each tile (`tileDimensions`), spacing between tiles (`tileSpacing`) and the area of the texture from where the tiles will be loaded (maybe your texture has some margins). The area is specified by the `boundaries` array and it contains coordinates of top-left corner where the first tile starts and then width/height of that area.

You also need to specify how many tiles are there within the texture (`defaultProperties/count`) and which tiles are impassable by default. Impassability is specified by `solids` array where `1` means impassable. Nth flag in the solids array corresponds with the nth tile in the texture, indexed from 0.

Final config section can look like this:

```json
"toolMesh": {
	"texture": {
		"path": "./textures/tileset.png",
		"tileDimensions": [32, 32],
		"tileSpacing": [0, 0],
		"boundaries": [0, 0, 96, 64]
	},
	"defaultProperties": {
		"count": 6,
		"solids": [1, 0, 0, 1, 0, 1]
	}
}
```
# Item mode

Item mode (`toolItem`) also needs to know path to sprite texture. Unlike tilesets, each object can have its own texture from which you need to select area that will be displayed in the editor (in case you are using spritesheets).

Items are stored in the array and the position in the array determines which `id` will be stored for the item in the LevelD file format.

Once again, each texture path can be relative to absolute. The `clip` property works the same as `boundaries` for `toolMesh`.

Final config section can look like this:

```json
"toolItem" : {
	"items": [
		{
			"texture": {
				"path": "./textures/actor.png",
				"clip": [0, 0, 64, 64]
			}
		}, {
			"texture": {
				"path": "./textures/items.png",
				"clip": [0, 0, 32, 32]
			}
		}, {
			"texture": {
				"path": "./textures/items.png",
				"clip": [32, 0, 32, 32]
			}
		}
	]
},
```

# Trigger mode

Identified by `toolTrigger`, allows you to specify what actions are available. If you don't specify this, the editor will still work, but you will be unable to create usable triggers.

Each action is stored in a `actionDefinitions` array, but rather than be indexed by position in the array, it maintains its own id. Each action has a name that will display in a dropdown in editor and list of parameter names.

Due to limitations of LevelD file format, you can only have up to five parameters and all of them are unsigned integers.

Final config section can look like this:

```json
"toolTrigger": {
	"actionDefinitions": [
		{
			"id": 0,
			"name": "No action",
			"params": []
		},
		{
			"id": 1,
			"name": "Open door",
			"params": [ "Speed", "Close delay" ]
		}
	]
}
```
