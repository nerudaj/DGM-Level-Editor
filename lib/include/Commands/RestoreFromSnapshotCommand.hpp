#pragma once

#include "include/Interfaces/CommandInterface.hpp"
#include "include/Editor/Editor.hpp"

#include <LevelD.hpp>

class RestoreFromSnapshotCommand final : public CommandInterface
{
public:
	[[nodiscard]]
	RestoreFromSnapshotCommand(
		Editor& editor,
		LevelD snapshot) noexcept
		: editor(editor)
		, snapshot(snapshot)
	{}

public:
	void exec() override
	{
		editor.loadFrom(snapshot, true);
	}

protected:
	Editor& editor;
	LevelD snapshot;
};
