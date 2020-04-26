#pragma once

#include <QString>

enum class TestStatus
{
	NOT_STARTED,
	RUNNING,
	SUCCEED,
	FAILED,
	CRASHED,
	TIMEOUT
};
