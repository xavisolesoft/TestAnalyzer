#pragma once

#include <QString>
#include <QIODevice>

#include <memory>

class TestModel;

class GTestParser
{
private:
	enum class GTestTag
	{
		UNDEFINED,
		RUN,
		OK,
		FAILED,
		TIMEOUT
	};

public:
	GTestParser();

	std::shared_ptr<TestModel> parseTestModel(QIODevice& gTestOutput) const;

private:
	static long long extractExecutionTimeMiliSecs(const QString& line);
};

