#pragma once

#include "include/Commands/CommandInterface.hpp"
#include <include/LevelMesh/DrawableLeveldMesh.hpp>

class ResizeMeshCommand final : public CommandInterface
{
protected:
	DrawableLeveldMesh& target;
	sf::Vector2u newSize;

public:
	ResizeMeshCommand(
		DrawableLeveldMesh& target,
		sf::Vector2u newSize)
		: target(target)
		, newSize(newSize)
	{}

public:
	void exec() override;

	[[nodiscard]]
	std::unique_ptr<CommandInterface> getInverse() const override;
};

class ResizeCommand final : public CommandInterface
{
protected:
	std::unique_ptr<ResizeMeshCommand> meshCommand;

public:
	ResizeCommand(std::unique_ptr<ResizeMeshCommand> meshCommand)
		: meshCommand(std::move(meshCommand))
	{}

public:
	void exec() override;

	[[nodiscard]]
	std::unique_ptr<CommandInterface> getInverse() const override;
};