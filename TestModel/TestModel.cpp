#include "TestModel.hpp"

#include <iostream>
#include <algorithm>

TestModel::TestModel()
{

}

TestEntry& TestModel::addTestEntry()
{
	testEntries.emplace_back();
	return testEntries.back();
}

int TestModel::getNumTests() const
{
	return testEntries.size();
}

const TestEntry& TestModel::getTest(int index) const
{
	return testEntries[index];
}

int TestModel::getNumTests(TestStatus testStatus) const
{
	return
		std::count_if(testEntries.begin(),
						testEntries.end(),
						[testStatus](const TestEntry& testEntry){
							return testEntry.getStatus() == testStatus;
						});
}

void TestModel::merge(std::shared_ptr<TestModel> theirs)
{
	for(int i = 0; i < theirs->getNumTests(); ++i){
		testEntries.push_back(theirs->getTest(i));
	}
}
