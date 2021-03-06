#include "TestModel.hpp"

#include <iostream>
#include <algorithm>

TestModel::TestModel()
{

}

TestEntry& TestModel::addTestEntry()
{
	mTestEntries.emplace_back();
	return mTestEntries.back();
}

int TestModel::getNumTests() const
{
	return static_cast<int>(mTestEntries.size());
}

const TestEntry& TestModel::getTest(int index) const
{
	return mTestEntries[index];
}

int TestModel::getNumTests(TestStatus::Enum testStatus) const
{
	return
	    std::count_if(mTestEntries.begin(),
	                    mTestEntries.end(),
						[testStatus](const TestEntry& testEntry){
							return testEntry.getStatus() == testStatus;
	});
}

QSet<QString> TestModel::getTestFamilyNames() const
{
	QSet<QString> familyNames;

	foreach(const TestEntry& testEntry, mTestEntries){
		familyNames.insert(testEntry.getFamily());
	}

	return familyNames;
}

QSet<QString> TestModel::getOutputFilePaths() const
{
	QSet<QString> fileNames;

	foreach(const TestEntry& testEntry, mTestEntries){
		fileNames.insert(testEntry.getOutputFilePath());
	}

	return fileNames;
}

void TestModel::merge(std::shared_ptr<TestModel> theirs)
{
	if(theirs){
		for(int i = 0; i < theirs->getNumTests(); ++i){
			mTestEntries.push_back(theirs->getTest(i));
		}
	}
}
