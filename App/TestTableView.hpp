#pragma once

#include <QTableView>

#include <memory>

#include "TestModel/TestStatus.hpp"

class TestModel;
class TestTableModel;

class TestTableView : public QTableView
{
public:
	TestTableView(QWidget* parent = nullptr);

	void setTestModel(std::shared_ptr<TestModel> testModel);

	void setTestStatusFilter(TestStatus::Enum testStatus);
	void setTestFamilyFilter(const QString& familyName);

protected:
	void initOpenGTestOutputOnFileNameClick(const TestTableModel& testTableModel);
	void keyPressEvent(QKeyEvent *event) override;

private:
	void clearTableModels();
};
