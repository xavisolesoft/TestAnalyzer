#pragma once

#include <QSet>
#include <QString>

#include <vector>
#include <memory>

#include "TestEntry.hpp"

class TestModel
{
public:
	TestModel();

	TestEntry& addTestEntry();

	int getNumTests() const;
	const TestEntry& getTest(int index) const;

	int getNumTests(TestStatus::Enum testStatus) const;

	QSet<QString> getTestFamilyNames() const;

	void merge(std::shared_ptr<TestModel> theirs);

private:
	std::vector<TestEntry> mTestEntries;
};

