#include "include/Editor/EditorStateManager.hpp"

void EditorStateManager::forallStates(std::function<void(Tool&)> callback) {
	for (auto&& stateId : insertionOrder)
	{
		callback(*statesToTools.at(stateId));
	}
}

void EditorStateManager::forallStates(std::function<void(Tool&, bool)> callback) {
	for (auto&& stateId : insertionOrder)
	{
		callback(*statesToTools.at(stateId), stateId == currentState);
	}
}

void EditorStateManager::forallInactiveStates(std::function<void(Tool&)> callback)
{
	for (auto&& stateId : insertionOrder)
	{
		if (stateId == currentState)
			continue;
		callback(*statesToTools.at(stateId));
	}
}

void EditorStateManager::changeState(EditorState state) {
	assert(statesToTools.contains(state));
	currentState = state;
}
