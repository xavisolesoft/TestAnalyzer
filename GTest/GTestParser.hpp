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
	static QString cleanedLine(const QString& line, const QString& gtestTagString);
	static long long extractExecutionTimeMiliSecs(const QString& line);

	static constexpr auto RUN_HEADER = "[ RUN      ]";
	static constexpr auto OK_HEADER = "[       OK ]";
	static constexpr auto FAILED_HEADER = "[  FAILED  ]";
	static constexpr auto TIMEOUT_HEADER = "[  TIMEOUT ]";
};

