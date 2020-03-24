#include "TestEntry.hpp"

TestEntry::TestEntry()
{

}

const QString& TestEntry::getName() const
{
	return name;
}

void TestEntry::setName(QString value)
{
	name = std::move(value);
}

const QString& TestEntry::getFamily() const
{
	return family;
}

void TestEntry::setFamily(QString value)
{
	family = std::move(value);
}

const QString& TestEntry::getDump() const
{
	return dump;
}

void TestEntry::setDump(QString value)
{
	dump = std::move(value);
}

TestStatus TestEntry::getStatus() const
{
	return status;
}

void TestEntry::setStatus(TestStatus value)
{
	status = value;
}

const QString& TestEntry::getOutputFilePath() const
{
	return outputFilePath;
}

void TestEntry::setOutputFilePath(QString value)
{
	outputFilePath = value;
}

int TestEntry::getLine() const
{
    return lineNumber;
}

void TestEntry::setLineNumber(int value)
{
    lineNumber = value;
}
