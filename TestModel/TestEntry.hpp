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

	TestStatus getStatus() const;
	void setStatus(TestStatus value);

	const QString& getOutputFilePath() const;
	void setOutputFilePath(QString value);

	int getLine() const;
	void setLineNumber(int value);

private:
	QString mName;
	QString mFamily;
	QString mDump;
	TestStatus mStatus;
	QString mOutputFilePath;
	int mLineNumber;
};
