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
			lastTestEntry->setStatus(TestStatus::RUNNING);
			lastTag = GTestTag::RUN;
		}
		else if(line.startsWith("[       OK ]"))
		{
			if(lastTestEntry){
				lastTestEntry->setExecutionTimeMiliSecs(extractExecutionTimeMiliSecs(line));
				lastTestEntry->setStatus(TestStatus::SUCCEED);
				lastTag = GTestTag::OK;
			} else {
				//TODO: Report bad parsing.
			}
		}
		else if(line.startsWith("[  FAILED  ]") && lastTestEntry->getStatus() != TestStatus::FALIED)
		{
			if(lastTestEntry){
				lastTestEntry->setExecutionTimeMiliSecs(extractExecutionTimeMiliSecs(line));
				lastTestEntry->setStatus(TestStatus::FALIED);
				lastTag = GTestTag::FAILED;
			} else {
				//TODO: Report bad parsing.
			}
		}
		else if(line.startsWith("[  TIMEOUT ]"))
		{
			if(lastTestEntry){
				lastTestEntry->setExecutionTimeMiliSecs(extractExecutionTimeMiliSecs(line));
				lastTestEntry->setStatus(TestStatus::TIMEOUT);
				lastTag = GTestTag::TIMEOUT;
			} else {
				//TODO: Report bad parsing.
			}
		}
		lineNumber++;
	}

	if(lastTag == GTestTag::RUN){
		lastTestEntry->setStatus(TestStatus::CRASHED);
	}

	return testModel;
}

long long GTestParser::extractExecutionTimeMiliSecs(const QString &line)
{
	QStringList openSplit = line.split("(");
	if(openSplit.size() != 2){
		//TODO: Report bad parsing.
		return -1;
	}

	QStringList closeSplit = openSplit[1].split(")");
	if(closeSplit.size() != 2){
		//TODO: Report bad parsing.
		return -1;
	}

	QString executionTimeStr = openSplit[1].remove(")").remove("ms").trimmed();
	bool isNumber;
	long long executionTime = executionTimeStr.toLongLong(&isNumber);
	if(!isNumber){
		//TODO: Report bad parsing.
		return -1;
	}

	return executionTime;
}
