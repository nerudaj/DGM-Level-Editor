#pragma once

#include <memory>
#include <map>
#include "Tool.hpp"

class EditorState {
private:
	std::unique_ptr<Tool> layer;

public:
	Tool & getTool() {
		return *layer;
	}

	const Tool & getTool() const {
		return *layer;
	}

	EditorState() {}
	EditorState(Tool *layer) : layer(layer) {}
};

class EditorStateManager {
private:
	std::map<std::string, EditorState> states;
	std::map<std::string, EditorState>::iterator currentState;

public:
	void addState(const std::string& id, Tool * layer);

	void changeState(const std::string& id);

	void forallStates(std::function<void(Tool &)> callback);

	Tool & getTool() {
		return currentState->second.getTool();
	}

	const Tool & getTool() const {
		return currentState->second.getTool();
	}
};