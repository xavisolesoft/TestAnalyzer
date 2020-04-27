#include "TestStatus.hpp"

TestStatus::TestStatus()
{

}

QString TestStatus::toString(TestStatus::Enum testStatus)
{
	switch(testStatus)
	{
	case TestStatus::UNDEFINED:
		return QObject::tr("UDEFINED");
	case TestStatus::NOT_STARTED:
		return QObject::tr("NOT_STARTED");
	case TestStatus::RUNNING:
		return QObject::tr("RUNNING");
	case TestStatus::SUCCEED:
		return QObject::tr("SUCCEED");
	case TestStatus::FAILED:
		return QObject::tr("FAILED");
	case TestStatus::CRASHED:
		return QObject::tr("CRASHED");
	case TestStatus::TIMEOUT:
		return QObject::tr("TIMEOUT");
	}

	return QObject::tr("UNDEFINED");
}

TestStatus::Enum TestStatus::fromString(const QString &testStatusText)
{
	if(testStatusText == "UDEFINED"){
		return TestStatus::UNDEFINED;
	} else if(testStatusText == "NOT_STARTED"){
		return TestStatus::NOT_STARTED;
	} else if(testStatusText == "RUNNING"){
		return TestStatus::RUNNING;
	} else if(testStatusText == "SUCCEED"){
		return TestStatus::SUCCEED;
	} else if(testStatusText == "FAILED"){
		return TestStatus::FAILED;
	} else if(testStatusText == "CRASHED"){
		return TestStatus::CRASHED;
	} else if(testStatusText == "TIMEOUT"){
		return TestStatus::TIMEOUT;
	}

	return TestStatus::UNDEFINED;
}
