#pragma once

#include <QString>
#include <QObject>
#include <QMetaType>

class TestStatus
{
private:
	TestStatus();

public:
	enum Enum
	{
		UNDEFINED,
		NOT_STARTED,
		RUNNING,
		SUCCEED,
		FAILED,
		CRASHED,
		TIMEOUT
	};

	static QString toString(TestStatus::Enum testStatus);
	static TestStatus::Enum fromString(const QString& testStatusText);
};

Q_DECLARE_METATYPE(TestStatus::Enum)
