#pragma once

#include "include/LevelMesh/DrawableLeveldMesh.hpp"
#include "include/Tools/ToolInterface.hpp"
#include "include/Tools/PenUserInterface.hpp"
#include "include/Tools/SidebarUserMesh.hpp"

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

class ToolMesh final : public ToolInterface
{
public:
	enum class DrawMode
	{
		Pencil, RectFill, RectEdge, Line
	};

public:
	[[nodiscard]]
	ToolMesh(
		std::function<void(void)> onStateChanged,
		ShortcutEngineInterface& shortcutEngine,
		tgui::Gui& gui,
		tgui::Theme& theme,
		CommandQueue& commandQueue) noexcept
		: ToolInterface(onStateChanged, shortcutEngine)
		, sidebarUser(gui, theme)
		, commandQueue(commandQueue)
	{}

public: // PenUserInterface
	void penClicked(const sf::Vector2i& position) override;
	void penDragStarted(const sf::Vector2i& start) override;
	void penDragUpdate(const sf::Vector2i& start, const sf::Vector2i& end) override;
	void penDragEnded(const sf::Vector2i& start, const sf::Vector2i& end) override;
	void penDragCancel(const sf::Vector2i& origin) override {}
	void penDelete() override {}

public: // ToolInterface
	void buildSidebar() override { sidebarUser.buildSidebar(); }

	void configure(nlohmann::json& config) override;

	void resize(unsigned width, unsigned height) override;

	void saveTo(LevelD& lvd) const override;

	void loadFrom(const LevelD& lvd) override;

	void drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity) override;

	std::unique_ptr<ToolProperty> getProperty(const sf::Vector2i& penPos) const override;

	void setProperty(const ToolProperty& prop) override;

	void buildCtxMenuInternal(tgui::MenuBar::Ptr& menu) override;

	// No highlight here
	virtual std::optional<GenericObject> getHighlightedObject(const sf::Vector2i& penPos) const override { return {}; }
	virtual std::vector<sf::Vector2u> getPositionsOfObjectsWithTag(unsigned tag) const override { return {}; }

private:
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

private:
	DrawMode mode = DrawMode::Pencil;
	sf::RectangleShape rectShape;
	DrawableLeveldMesh map;
	SidebarUserMesh sidebarUser;
	CommandQueue& commandQueue;

	std::vector<bool> defaultBlocks;

	bool drawing = false;
	bool enableOverlay = false;
	bool dragging = false;
};

namespace std
{
	std::string to_string(ToolMesh::DrawMode mode);
}