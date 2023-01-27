#pragma once

#include <cstdint>
#include <string>
#include <TGUI/TGUI.hpp>

namespace DialogBuilderHelper
{
	void addOption(
		tgui::ScrollablePanel::Ptr& target,
		const std::string& label,
		const std::string& tooltip,
		bool& data,
		unsigned ypos,
		bool enabled = true);

	void addOption(
		tgui::ScrollablePanel::Ptr& target,
		const std::string& label,
		const std::string& tooltip,
		uint32_t& data,
		unsigned ypos,
		bool enabled = true,
		bool tag = false);

	void addOption(
		tgui::ScrollablePanel::Ptr& target,
		const std::string& label,
		const std::string& tooltip,
		uint16_t& data,
		unsigned ypos,
		bool enabled = true);

	void addOption(
		tgui::ScrollablePanel::Ptr& target,
		const std::string& label,
		const std::string& tooltip,
		std::string& data,
		unsigned ypos,
		bool enabled = true);
};
