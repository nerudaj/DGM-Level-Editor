#include "EditorBrush.hpp"
#include "../json.hpp"
#include <fstream>

nlohmann::json loadJsonFromFile(const std::string &filename) {
	std::ifstream load(filename);
	nlohmann::json file;
	load >> file;
	load.close();
	load.clear();

	return file;
}

void EditorBrushTile::init(const std::string& cfgfile, const dgm::ResourceManager& resmgr) {
	//auto json = loadJsonFromFile(cfgfile);

	texture = resmgr.get<sf::Texture>("basictiles.png");
	clip = dgm::Clip({ 16, 16 }, { 0, 0, 128, 48 });

	meshMap = {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1};
}

void EditorBrushItem::init(const std::string& cfgfile, const dgm::ResourceManager& resmgr) {
	//auto json = loadJsonFromFile(cfgfile);

	struct ConfigLine {
		std::string animationDataName;
		std::string animationState;
		std::string textureName;
	};

	std::vector<ConfigLine> lines = {
		{ "item.json", "idle", "pot.png" },
		{ "item.json", "idle", "torch.png" }
	};

	for (auto& line : lines) {
		auto& anim = resmgr.get<std::shared_ptr<dgm::AnimationStates>>(line.animationDataName);
		auto rect = (*anim)[line.animationState].getFrame(0);
		sf::Texture& txt = resmgr.get<sf::Texture>(line.textureName);

		items.push_back({ rect, txt });
	}
}


