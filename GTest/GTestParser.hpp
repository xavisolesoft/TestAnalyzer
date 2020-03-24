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
		FAILED
	};

public:
	GTestParser();

	std::shared_ptr<TestModel> parseTestModel(QIODevice& gTestOutput) const;
};

