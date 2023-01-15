#include <catch.hpp>
#include "include/Tools/ToolMesh.hpp"
#include "include/Shortcuts/ShortcutEngine.hpp"

#include "../include/NullCallback.hpp"
#include "../include/MeshAssets.hpp"

struct TestTile
{
	unsigned x;
	unsigned y;
	unsigned value;
	bool solid;
};

static LevelD createMesh(
	unsigned width,
	unsigned height,
	std::vector<TestTile> tiles)
{
	LevelD result;

	result.mesh.layerWidth = width;
	result.mesh.layerHeight = height;
	result.mesh.tileWidth = 32;
	result.mesh.tileHeight = 32;
	result.mesh.layers.resize(1);
	result.mesh.layers[0].tiles.resize(width * height, 0);
	result.mesh.layers[0].blocks.resize(width * height, 0);

	for (auto&& tile : tiles)
	{
		unsigned i = tile.y * width + tile.x;
		result.mesh.layers[0].tiles[i] = tile.value;
		result.mesh.layers[0].blocks[i] = tile.solid;
	}

	return result;
}

TEST_CASE("[ToolMesh]")
{
	GC<ShortcutEngine> shortcutEngine;
	GC<CommandHistory> commandHistory;
	GC<CommandQueue> commandQueue(commandHistory);
	tgui::Gui gui;
	tgui::Theme theme;

	ToolMesh mesh(
		Null::Callback,
		shortcutEngine,
		gui,
		theme,
		commandQueue);

	mesh.configure(
		MESH_TEXTURE_PATH,
		FRAME_SIZE,
		FRAME_SPACING,
		TEXTURE_BOUNDS,
		{ false, true, false, true });

	SECTION("getBoundingBox")
	{
		SECTION("returns empty box, when all tiles are empty")
		{
			LevelD level = createMesh(20, 20, {});
			mesh.loadFrom(level);
			REQUIRE(!mesh.getBoundingBox().has_value());
		}

		SECTION("returns valid box, when some tiles are set")
		{
			LevelD level = createMesh(
				20, 20,
				{ { 5, 5, 1, 1 },
				{ 10, 10, 2, 0 } });

			mesh.loadFrom(level);

			auto box = mesh.getBoundingBox();

			REQUIRE(box.has_value());
			REQUIRE(box.value().left == 5u);
			REQUIRE(box.value().top == 5u);
			REQUIRE(box.value().right == 10u);
			REQUIRE(box.value().bottom == 10u);
		}
	}

	SECTION("shrinkTo")
	{
		SECTION("correctly downsizes map into selected rect")
		{
			LevelD level = createMesh(
				20, 20,
				{ { 5, 5, 1, 1 },
				{ 10, 10, 2, 0 } });

			mesh.loadFrom(level);

			auto box = TileRect(5u, 5u, 10u, 10u);
			const unsigned width = 10u - 5u + 1u;
			const unsigned height = 10u - 5u + 1u;

			mesh.shrinkTo(box);

			LevelD exported;
			mesh.saveTo(exported);

			auto&& mesh = exported.mesh;
			REQUIRE(mesh.layers.size() == 1u);

			auto&& layer = mesh.layers[0];
			REQUIRE(layer.tiles.size() == width * height);
			REQUIRE(layer.tiles.front() == 1u);
			REQUIRE(layer.blocks.front() == true);

			REQUIRE(layer.tiles.back() == 2u);
			REQUIRE(layer.blocks.back() == false);
		}
	}
}