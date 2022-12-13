#pragma once

#include "include/Tools/Tool.hpp"
#include "include/Editor/EditorState.hpp"

#include <map>
#include <functional>
#include <vector>

class EditorStateManager
{
private:
	std::map<EditorState, std::unique_ptr<Tool>> statesToTools;
	EditorState currentState = EditorState();
	std::vector<EditorState> insertionOrder;

public:
	template<DerivedFromTool T, class ... Args>
		requires std::constructible_from<T, Args...>
	void addState(EditorState state, Args&&... args)
	{
		assert(!statesToTools.contains(state));
		statesToTools[state] = std::make_unique<T>(std::forward<Args>(args)...);
		insertionOrder.push_back(state);
	}

	void changeState(EditorState state);

	/**
	 *  \brief Loop over all states in order in which they were added
	 */
	void forallStates(std::function<void(Tool&)> callback);

	void forallStates(std::function<void(const Tool&)> callback) const;

	/**
	 *  \brief Loop over all states in order in which they were added
	 *
	 *  You need to provide callback that handles a tool reference
	 *  and a boolean flag indicating whether the particular tool
	 *  is active.
	 */
	void forallStates(std::function<void(Tool&, bool)> callback);

	void forallInactiveStates(std::function<void(Tool&)> callback);

	[[nodiscard]]
	Tool& getActiveTool() noexcept
	{
		return *statesToTools.at(currentState);
	}

	[[nodiscard]]
	const Tool& getActiveTool() const noexcept
	{
		return *statesToTools.at(currentState);
	}
};
