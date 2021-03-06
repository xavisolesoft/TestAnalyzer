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

		if(QString runGtestLine = cleanedLine(line, RUN_HEADER); !runGtestLine.isEmpty())
		{
			if(lastTag == GTestTag::RUN){
				lastTestEntry->setStatus(TestStatus::CRASHED);
			}

			lastTestEntry = &testModel->addTestEntry();
			QStringList fullNameSplit = getTestFamilyAndNameFromRunLine(line).split('.');
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
		else if(const QString okGtestLine = cleanedLine(line, OK_HEADER); !okGtestLine.isEmpty())
		{
			if(lastTestEntry){
				lastTestEntry->setExecutionTimeMiliSecs(extractExecutionTimeMiliSecs(okGtestLine));
				lastTestEntry->setStatus(TestStatus::SUCCEED);
				lastTag = GTestTag::OK;
			} else {
				//TODO: Report bad parsing.
			}
		}
		else if(const QString failedGtestLine = cleanedLine(line, FAILED_HEADER);
				!failedGtestLine.isEmpty()
				&& lastTestEntry->getStatus() != TestStatus::FAILED)
		{
			if(lastTestEntry){
				lastTestEntry->setExecutionTimeMiliSecs(extractExecutionTimeMiliSecs(failedGtestLine));
				lastTestEntry->setStatus(TestStatus::FAILED);
				lastTag = GTestTag::FAILED;
			} else {
				//TODO: Report bad parsing.
			}
		}
		else if(const QString timeoutGtestLine = cleanedLine(line, TIMEOUT_HEADER); !timeoutGtestLine.isEmpty())
		{
			if(lastTestEntry){
				lastTestEntry->setExecutionTimeMiliSecs(extractExecutionTimeMiliSecs(timeoutGtestLine));
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

QString GTestParser::getTestFamilyAndNameFromRunLine(const QString &runGtestLine)
{
	QString cleanedLine = runGtestLine;
	cleanedLine.remove(RUN_HEADER);

	QStringList message_NameAndFamilySplit = cleanedLine.split("=");
	QString nameAndFamily = message_NameAndFamilySplit.last();
	nameAndFamily = nameAndFamily.trimmed();

	if(nameAndFamily.isEmpty()){
		//TODO: Report error test have no name and family
	}

	return nameAndFamily;
}

QString GTestParser::cleanedLine(const QString &line, const QString& gtestTagString)
{
	int index = line.indexOf(gtestTagString);
	if(index < 0){
		return "";
	}

	return line.right(line.size() - index);
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
