#include "GTestParser.hpp"

#include <QFile>

#include "TestModel/TestModel.hpp"
#include "TestModel/TestEntry.hpp"
#include "TestModel/TestStatus.hpp"

GTestParser::GTestParser()
{

}

std::shared_ptr<TestModel> GTestParser::parseTestModel(QIODevice& gTestOutput) const
{
	auto testModel = std::make_shared<TestModel>();

	if(!gTestOutput.open(QIODevice::ReadOnly)){
		return testModel;
	}

	auto gTestOutputFile = dynamic_cast<QFile*>(&gTestOutput);
	GTestTag lastTag = GTestTag::UNDEFINED;
	TestEntry* lastTestEntry = nullptr;
	int lineNumber = 1;
	while(!gTestOutput.atEnd()){
		QString line = gTestOutput.readLine();
		if(auto RUN = "[ RUN      ]"; line.startsWith(RUN))
		{
			if(lastTag == GTestTag::RUN){
				lastTestEntry->setStatus(TestStatus::CRASHED);
			}

			lastTestEntry = &testModel->addTestEntry();
			QStringList fullNameSplit = line.remove(RUN).trimmed().split('.');
			if(fullNameSplit.size() == 1){
				lastTestEntry->setName(fullNameSplit[0]);
			} else {
				lastTestEntry->setFamily(fullNameSplit[0]);
				lastTestEntry->setName(fullNameSplit[1]);
			}
			if(gTestOutputFile){
				lastTestEntry->setOutputFilePath(gTestOutputFile->fileName());
			}
			lastTestEntry->setLineNumber(lineNumber);
			lastTag = GTestTag::RUN;
		}
		else if(line.startsWith("[       OK ]"))
		{
			if(lastTestEntry){
				lastTestEntry->setStatus(TestStatus::SUCCEED);
				lastTag = GTestTag::OK;
			} else {
				//TODO: Report bad parsing.
			}
		}
		else if(line.startsWith("[  FAILED  ]"))
		{
			if(lastTestEntry){
				lastTestEntry->setStatus(TestStatus::FALIED);
				lastTag = GTestTag::FAILED;
			} else {
				//TODO: Report bad parsing.
			}
		}
		else if(line.startsWith("[  TIMEOUT ]"))
		{
			if(lastTestEntry){
				lastTestEntry->setStatus(TestStatus::TIMEOUT);
				lastTag = GTestTag::TIMEOUT;
			} else {
				//TODO: Report bad parsing.
			}
		}
		lineNumber++;
	}

	return testModel;
}
