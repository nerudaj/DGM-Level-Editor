#include <DGM/dgm.hpp>

const sf::Vector2i FRAME_SIZE = { 16, 16 };
const dgm::Clip MAP_CLIP(FRAME_SIZE, {0, 0, 128, 48});

void loadLevel(const std::string &path, dgm::Tileset &tiles) {
	LevelD level;
	level.loadFromFile(path);

	std::vector<int> data(level.mesh.tiles.begin(), level.mesh.tiles.end());
	tiles.build(MAP_CLIP, sf::Vector2u(FRAME_SIZE), data, { level.mesh.width, level.mesh.height });
}



int main(int argc, char *argv[]) {
	std::string path = "../pokus.lvd";
	if (argc == 2) path = argv[1];

	dgm::ResourceManager resmgr;
	resmgr.loadResourceDir<sf::Texture>("graphics/textures");

	dgm::Tileset tiles;
	loadLevel(path, tiles);
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