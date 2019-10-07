#include <DGM/dgm.hpp>

const sf::Vector2i FRAME_SIZE = { 16, 16 };
const dgm::Clip MAP_CLIP(FRAME_SIZE, {0, 0, 128, 48});

void loadLevel(const std::string &path, dgm::Tileset &tiles) {
	LevelD level;
	level.loadFromFile(path);

	std::vector<int> data(level.mesh.tiles.begin(), level.mesh.tiles.end());
	tiles.build(MAP_CLIP, sf::Vector2u(FRAME_SIZE), data, { level.mesh.width, level.mesh.height });
}

#include <fstream>

int main(int argc, char *argv[]) {
	cfg::Args args("r:l:");

	try {
		args.parse(argc, argv);
	}
	catch (std::exception &e) {
		std::ofstream save("log.txt");
		save << e.what() << std::endl;
		save.close();
		save.clear();

		return 1;
	}

	std::string levelPath = "../pokus.lvd";
	if (args.isSet('l')) levelPath = args.getArgumentValue('l').asString();

	std::string resourceRoot = "../sample-project/";
	if (args.isSet('r')) resourceRoot = args.getArgumentValue('r').asString();

	dgm::ResourceManager resmgr;
	resmgr.loadResourceDir<sf::Texture>(resourceRoot + "graphics/textures");

	dgm::Tileset tiles;
	loadLevel(levelPath, tiles);
	tiles.setTexture(resmgr.get<sf::Texture>("tiles.png"));

	dgm::Window window;
	window.open({ 1024, 768 }, "Sample Binary", false);
	
	sf::Event event;
	while (window.isOpen()) {
		while(window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.beginDraw();
		
		window.draw(tiles);
		
		window.endDraw();
	}
}