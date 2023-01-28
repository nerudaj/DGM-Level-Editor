#include <catch.hpp>
#include "../include/NullCallback.hpp"
#include <include/Editor/EditorStateManager.hpp>
#include <include/Shortcuts/ShortcutEngine.hpp>
#include "../include/fakeit.hpp"
#include "include/Tools/ToolInterface.hpp"

class ToolMock : public ToolInterface
{
public:
	ToolMock(
		std::function<void(void)> onStateChanged,
		GC<ShortcutEngineInterface> shortcutEngine,
		const std::string& name,
		std::vector<std::string>& invocations)
		: ToolInterface(onStateChanged, shortcutEngine)
		, name(name)
		, invocations(invocations)
	{}

public:
	virtual void penClicked(const sf::Vector2i& position) override
	{}
	virtual void penDragStarted(const sf::Vector2i& start) override
	{}
	virtual void penDragUpdate(const sf::Vector2i& start, const sf::Vector2i& end) override
	{}
	virtual void penDragEnded(const sf::Vector2i& start, const sf::Vector2i& end) override
	{}
	virtual void penDragCancel(const sf::Vector2i& origin) override
	{}
	virtual void penDelete() override
	{}
	virtual void buildSidebar() override
	{
		invocations.push_back(name + ":buildSidebar");
	}
	virtual void configure(nlohmann::json& config) override
	{}
	virtual void resize(unsigned width, unsigned height) override
	{}
	virtual void shrinkTo(TileRect const& r) override {}
	virtual void saveTo(LevelD& lvd) const override
	{}
	virtual void loadFrom(const LevelD& lvd) override
	{}
	virtual void drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity) override
	{}
	virtual ExpectedPropertyPtr getProperty(const sf::Vector2i& penPos) const override
	{
		return std::unexpected(BaseError());
	}
	virtual void setProperty(const ToolPropertyInterface& prop) override
	{}
	virtual std::optional<GenericObject> getHighlightedObject(const sf::Vector2i& penPos) const override
	{
		return {};
	}
	virtual std::vector<sf::Vector2u> getPositionsOfObjectsWithTag(unsigned tag) const override
	{
		return {};
	}
	virtual void buildCtxMenuInternal(tgui::MenuBar::Ptr& menu) override
	{}

	std::optional<TileRect> getBoundingBox() const noexcept override
	{
		return TileRect();
	}

private:
	std::string name;
	std::vector<std::string>& invocations;
};

TEST_CASE("[EditorStateManager]")
{
	GC<ShortcutEngine> engine;
	std::vector<std::string> invocations;
	EditorStateManager manager;

	manager.addState<ToolMock>(
		EditorState::Mesh,
		Null::Callback,
		engine,
		"tool1",
		invocations);
	manager.addState<ToolMock>(
		EditorState::Item,
		Null::Callback,
		engine,
		"tool2",
		invocations);

	SECTION("Loops over all states in insertion order")
	{
		manager.forallStates([] (ToolInterface& t)
		{
			t.buildSidebar();
		});

		REQUIRE(invocations.size() == 2u);
		REQUIRE(invocations[0] == "tool1:buildSidebar");
		REQUIRE(invocations[1] == "tool2:buildSidebar");
	}
}
