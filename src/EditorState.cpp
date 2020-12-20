#include "EditorState.hpp"

void EditorStateManager::addState(const std::string& id, Tool* layer) {
	states[id] = EditorState(layer);
}

void EditorStateManager::forallStates(std::function<void(Tool &)> callback) {
	for (auto &state : states) {
		callback(state.second.getTool());
	}
}

void EditorStateManager::changeState(const std::string& id) {
	currentState = states.find(id);
	if (currentState == states.end()) {
		throw std::runtime_error("State with id: " + id + " does not exist!");
	}
}
