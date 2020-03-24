#pragma once

#include <QTableView>

#include <memory>

class TestModel;
class TestTableModel;

class TestTableView : public QTableView
{
public:
	TestTableView(QWidget* parent = nullptr);

	void setTestModel(std::shared_ptr<TestModel> testModel);

protected:
	void keyPressEvent(QKeyEvent *event) override;

	TestTableModel* testTableModel;
};
