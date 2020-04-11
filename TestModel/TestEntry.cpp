#include "TestEntry.hpp"

TestEntry::TestEntry()
{

}

const QString& TestEntry::getName() const
{
	return mName;
}

void TestEntry::setName(QString value)
{
	mName = std::move(value);
}

const QString& TestEntry::getFamily() const
{
	return mFamily;
}

void TestEntry::setFamily(QString value)
{
	mFamily = std::move(value);
}

const QString& TestEntry::getDump() const
{
	return mDump;
}

void TestEntry::setDump(QString value)
{
	mDump = std::move(value);
}

TestStatus TestEntry::getStatus() const
{
	return mStatus;
}

void TestEntry::setStatus(TestStatus value)
{
	mStatus = value;
}

const QString& TestEntry::getOutputFilePath() const
{
	return mOutputFilePath;
}

void TestEntry::setOutputFilePath(QString value)
{
	mOutputFilePath = value;
}

int TestEntry::getLine() const
{
    return mLineNumber;
}

void TestEntry::setLineNumber(int value)
{
    mLineNumber = value;
}
