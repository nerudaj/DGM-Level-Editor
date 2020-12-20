#include "LayerProperty.hpp"

void LayerPropertyTile::set(const std::string& key, uint32_t value) {
	if (key == "id") tileId = uint16_t(value);
	else if (key == "solid") solid = value;
}

uint32_t LayerPropertyTile::get(const std::string& key) {
	if (key == "id") return tileId;
	else if (key == "solid") return solid;
	return 0;
}
