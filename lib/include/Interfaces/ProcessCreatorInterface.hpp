#pragma once

#include <expected>
#include "include/Utilities/Error.hpp"

class ProcessCreatorInterface
{
public:
	virtual ~ProcessCreatorInterface() = default;

public:
	[[nodiscard]]
	virtual std::expected<ReturnFlag, ErrorMessage> Exec(
		std::string pathToBinary,
		std::string params) const noexcept = 0;
};
