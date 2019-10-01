#include <DGM/dgm.hpp>

const sf::Vector2u FRAME_SIZE = { 16.f, 16.f};

void loadLevel(const std::string &path, dgm::Tileset &tiles) {
	lvd::LevelD level;
	level.loadFromFile(path);

	std::vector<int> data(level.mesh.tiles.begin(), level.mesh.tiles.end());
	tiles.build({level.mesh.width, level.mesh.height}, FRAME_SIZE, data, {});
}



int main(int argc, char *argv[]) {
	if (argc != 2) throw std::runtime_error("This program requires path to *.lvd file!");

	dgm::ResourceManager resmgr;
	resmgr.loadDirectory<sf::Texture>("graphics/textures");

	dgm::Tileset tiles;
	loadLevel(argv[1], tiles);

	dgm::Window window({1024, 578}, "Sample Binary");
	
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