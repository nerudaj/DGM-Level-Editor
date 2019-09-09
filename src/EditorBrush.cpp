#include "EditorBrush.hpp"

void EditorBrushTile::init(const nlohmann::json& config, const dgm::ResourceManager& resmgr) {
	texture = resmgr.get<sf::Texture>(config["tileBrush"]["texture"]);

	sf::Vector2i tileSize;
	tileSize.x = config["editor"]["tileSize"][0].get<int>();
	tileSize.y = config["editor"]["tileSize"][1].get<int>();

	sf::IntRect bounds;
	bounds.left = config["tileBrush"]["clip"]["boundaries"][0].get<int>();
	bounds.top = config["tileBrush"]["clip"]["boundaries"][1].get<int>();
	bounds.width = config["tileBrush"]["clip"]["boundaries"][2].get<int>();
	bounds.height = config["tileBrush"]["clip"]["boundaries"][3].get<int>();

	clip = dgm::Clip(tileSize, bounds);

	std::vector<int> map(config["tileBrush"]["meshMap"].begin(), config["tileBrush"]["meshMap"].end());
	meshMap = std::vector<bool>(map.begin(), map.end());
}

void EditorBrushItem::init(const nlohmann::json& config, const dgm::ResourceManager& resmgr) {
	for (auto& item : config["itemBrush"]) {
		auto& anim = resmgr.get<std::shared_ptr<dgm::AnimationStates>>(item["config"]);
		auto rect = (*anim)[item["state"]].getFrame(0);
		sf::Texture& txt = resmgr.get<sf::Texture>(item["texture"]);

		items.push_back({ rect, txt });
	}
}
