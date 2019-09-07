#pragma once

#include <vector>
#include <algorithm>

/**
 *  Class for holding editor history - vector of numbers with pointer to active slot
 */
class EditorHistory {
private:
	std::vector<unsigned> history;
	std::size_t active;
	std::size_t limit;

public:
	/**
	 *  Get index of active history item
	 */
	std::size_t getActive() const {
		return active;
	}

	/**
	 *  Set index of active history item
	 */
	void setActive(std::size_t index) {
		active = index;
	}

	/**
	 *  Access history item
	 */
	unsigned operator[](std::size_t index) const {
		return history[index];
	}

	/**
	 *  @brief Add item to history
	 *
	 *  @param[in]  value  Item to add
	 *
	 *  This function will insert @p value to start of history
	 *  and set the active pointer to it
	 *  If item already exists in history, nothing is inserted
	 *  and active pointer will adjust accordingly.
	 *  If old items overflow history, they will be deleted
	 */
	void addItem(unsigned value) {
		auto itr = std::find(history.begin(), history.end(), value);
		if (itr != history.end()) {
			active = itr - history.begin();
		}
		else {
			history.insert(history.begin(), value);
			active = 0;
		}

		if (history.size() > limit) {
			history.resize(limit);
		}
	}

	void clear() {
		history.clear();
	}

	void setLimit(std::size_t limit) {
		EditorHistory::limit = limit;
	}

	/**
	 *  Get number of items in history
	 */
	std::size_t getSize() const {
		return history.size();
	}

	EditorHistory() {
		active = 0;
		limit = 6;
	}
};