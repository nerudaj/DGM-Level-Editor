#include "Layer.hpp"

void LayerTile::draw() {
	data.draw();
}

void LayerTile::click(int x, int y) {
	data.set(x, y, tool.getSelectedProperty());
}

void LayerTile::init(nlohmann::json& config) {
	try {
		textureHandler.init(config);
		data.init();
		tool.init(config);
	}
	catch (std::exception & e) {
		std::cerr << "ERROR: LayerTile:init(): " << e.what() << std::endl;
		throw;
	}
}

void LayerTile::resize(unsigned width, unsigned height) {
	data.resize(width, height);
}
