#pragma once

#include <DGM/dgm.hpp>
#include <TGUI/TGUI.hpp>

#include "include/Dialogs/FormValidatorToken.hpp"

class ToolInterface;

class PropertyTag
{
private:
	uint32_t value = 0;

	PropertyTag() {}
	PropertyTag(const PropertyTag& other) = delete;
	PropertyTag(PropertyTag&& other) = delete;
	PropertyTag& operator=(const PropertyTag& other) = delete;

public:
	static PropertyTag& get()
	{
		static PropertyTag instance;
		return instance;
	}

	void updateTag(uint32_t val)
	{
		if (value < val) value = val;
	}

	uint32_t getNewTag()
	{
		return ++value;
	}

	~PropertyTag() {}
};

class ToolPropertyInterface
{
public:
	~ToolPropertyInterface() = default;

public:
	virtual void fillEditDialog(
		tgui::Panel::Ptr& panel,
		FormValidatorToken& formValidatorToken) = 0;
};

class NullToolProperty2 final : public ToolPropertyInterface
{
public:
	void fillEditDialog(
		tgui::Panel::Ptr&,
		FormValidatorToken&) override
	{}
};
