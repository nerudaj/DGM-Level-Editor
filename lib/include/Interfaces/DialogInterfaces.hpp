#pragma once

#include <string>
#include <functional>

enum class UserChoice
{
	Confirmed = 0,
	Denied,
	Cancelled
};

class YesNoCancelDialogInterface
{
public:
	virtual void open(
		const std::string title,
		const std::string& text,
		std::function<void(UserChoice)> completedCallback) = 0;

	virtual ~YesNoCancelDialogInterface() = default;
};

class ErrorInfoDialogInterface
{
public:
	virtual ~ErrorInfoDialogInterface() = default;

public:
	virtual void open(const std::string& text) = 0;
};
