#pragma once

#include "Tool.hpp"
#include "include/LevelMesh/DrawableLeveldMesh.hpp"

class MeshToolProperty : public ImageToolProperty
{
	virtual void buildModalSpecifics(tgui::Gui& gui, tgui::ScrollablePanel::Ptr& panel) override;

public:
	uint32_t tileX = 0;
	uint32_t tileY = 0;
	uint16_t tileValue = 0;
	bool blocking = false;
	bool defaultBlocking = false;
};

class ToolMesh : public ToolWithSprites
{
public:
	enum class DrawMode
	{
		Pencil, RectFill, RectEdge, Line
	};

private:
	DrawMode mode = DrawMode::Pencil;
	sf::RectangleShape rectShape;

	sf::Texture texture; // tileset texture
	dgm::Clip clip; // how tileset is sliced
	DrawableLeveldMesh map;

	std::vector<bool> defaultBlocks;

	bool drawing = false;
	bool enableOverlay = false;

	void changeDrawingMode(DrawMode newMode);

	[[nodiscard]]
	sf::Vector2u worldToTilePos(const sf::Vector2i& position) const noexcept
	{
		unsigned tileX = position.x / map.getTileSize().x;
		unsigned tileY = position.y / map.getTileSize().y;
		return { tileX, tileY };
	}

	[[nodiscard]]
	bool isPositionValid(const sf::Vector2u& tilePos) const
	{
		return !(tilePos.x < 0 || tilePos.y < 0
			|| tilePos.x >= map.getMapDimensions().x
			|| tilePos.y >= map.getMapDimensions().y);
	}

	virtual tgui::Texture getSpriteAsTexture(unsigned spriteId) const override
	{
		return tgui::Texture(texture, clip.getFrame(spriteId));
	}

	virtual std::size_t getSpriteCount() const override
	{
		return clip.getFrameCount();
	}

	virtual void penClicked(const sf::Vector2i& position) override;
	virtual void penDragStarted(const sf::Vector2i& start) override;
	virtual void penDragUpdate(const sf::Vector2i& start, const sf::Vector2i& end) override;
	virtual void penDragEnded(const sf::Vector2i& start, const sf::Vector2i& end) override;
	virtual void penDragCancel(const sf::Vector2i& origin) override;

public:
	virtual void configure(nlohmann::json& config);

	virtual void resize(unsigned width, unsigned height);

	virtual void saveTo(LevelD& lvd) override;

	virtual void loadFrom(const LevelD& lvd);

	virtual void drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity) override;

	virtual void penDelete() override {}

	virtual std::unique_ptr<ToolProperty> getProperty() const override;

	virtual void setProperty(const ToolProperty& prop) override;

	virtual void buildCtxMenu(tgui::MenuBar::Ptr& menu) override;

	// No highlight here
	virtual std::optional<unsigned> getTagOfHighlightedObject() override { return {}; }
	virtual std::vector<sf::Vector2u> getPositionsOfObjectsWithTag(unsigned tag) const override { return {}; }

	ToolMesh(
		tgui::Gui& gui,
		std::function<void(void)> onStateChanged,
		CommandQueue& commandQueue,
		ShortcutEngineInterface& shortcutEngine)
		: ToolWithSprites(gui, onStateChanged, commandQueue, shortcutEngine)
	{}
};

namespace std
{
	std::string to_string(ToolMesh::DrawMode mode);
}