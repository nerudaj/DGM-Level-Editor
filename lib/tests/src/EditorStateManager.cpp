#include <catch.hpp>
#include "../include/NullCallback.hpp"
#include <include/Editor/EditorStateManager.hpp>
#include <include/Tools/Tool.hpp>
#include <include/Shortcuts/ShortcutEngine.hpp>

class ToolPropertyMock : public ToolProperty
{
public:
	virtual void buildModalSpecifics(tgui::Gui& gui, tgui::Panel::Ptr& panel) override {}
};

class ToolMock final : public Tool
{
protected:
	//ToolPropertyMock propertyMock;
	std::string id;
	std::vector<std::string>& invocationsTracker;

public:
	virtual void buildSidebar(tgui::Gui& gui, tgui::Group::Ptr& sidebar, tgui::Theme& theme) override
	{}

	virtual void penClicked(const sf::Vector2i& position) override
	{}

	virtual void penDragEnded(const sf::Vector2i& start, const sf::Vector2i& end) override
	{}

	virtual void penDragCancel(const sf::Vector2i& origin) override
	{}

	virtual void configure(nlohmann::json& config) override
	{}

	virtual void resize(unsigned width, unsigned height) override
	{}

	virtual void saveTo(LevelD& lvd) const override
	{}

	virtual void loadFrom(const LevelD& lvd) override
	{}

	virtual void drawTo(tgui::Canvas::Ptr& canvas, uint8_t opacity) override
	{}

	virtual void penDelete() override
	{
		invocationsTracker.push_back(id + ":penDelete");
	}

	virtual std::unique_ptr<ToolProperty> getProperty() const override { return nullptr; }

	virtual void setProperty(const ToolProperty& prop) override
	{}

	virtual std::optional<GenericObject> getHighlightedObject() const override
	{
		return {};
	}

	virtual std::vector<sf::Vector2u> getPositionsOfObjectsWithTag(unsigned) const override
	{
		return {};
	}

	ToolMock(
		const std::string& id,
		tgui::Gui& gui,
		std::function<void(void)> onStateChanged,
		CommandQueue& commandQueue,
		ShortcutEngineInterface& shortcutEngine,
		std::vector<std::string>& invocationsTracker)
		: Tool(gui, onStateChanged, commandQueue, shortcutEngine)
		//, propertyMock(gui, this)
		, id(id)
		, invocationsTracker(invocationsTracker)
	{}
};

TEST_CASE("[EditorStateManager]")
{
	tgui::Gui gui;
	CommandHistory history;
	CommandQueue queue(history);
	ShortcutEngine engine;

	std::vector<std::string> invocations;

	EditorStateManager manager;

	manager.addState<ToolMock>(
		EditorState::Mesh,
		"tool1",
		gui,
		Null::Callback,
		queue,
		engine,
		invocations);
	manager.addState<ToolMock>(
		EditorState::Item,
		"tool2",
		gui,
		Null::Callback,
		queue,
		engine,
		invocations);

	SECTION("Loops over all states in insertion order")
	{
		manager.forallStates([] (Tool& t)
 {
	 t.penDelete();
		});

		REQUIRE(invocations.size() == 2u);
		REQUIRE(invocations[0] == "tool1:penDelete");
		REQUIRE(invocations[1] == "tool2:penDelete");
	}
}
