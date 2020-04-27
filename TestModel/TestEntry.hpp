#pragma once

#include <QString>

#include "TestStatus.hpp"

class TestEntry
{
public:
	TestEntry();

	const QString& getName() const;
	void setName(QString value);

	const QString& getFamily() const;
	void setFamily(QString value);

	const QString& getDump() const;
	void setDump(QString value);

	TestStatus::Enum getStatus() const;
	void setStatus(TestStatus::Enum value);

	const QString& getOutputFilePath() const;
	void setOutputFilePath(QString value);

	int getLine() const;
	void setLineNumber(int value);

	long long getExecutionTimeMiliSecs() const;
	void setExecutionTimeMiliSecs(long long value);

private:
	QString mName;
	QString mFamily;
	QString mDump;
	TestStatus::Enum mStatus = TestStatus::NOT_STARTED;
	QString mOutputFilePath;
	int mLineNumber = -1;
	long long executionTimeMiliSecs = -1;
};
