#pragma once

#include <QList>
#include <QMenu>
#include <QObject>
#include <QDropEvent>
#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDropEvent>

#include "TestModel/TestStatus.hpp"

class TestModel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
	~MainWindow() override ;

private:
	QMenu* createFileMenu();
	QMenu* createSettingsMenu();

	void initTestStatusFilterCombobox();
	void initTestFamilyFilterCombobox();
	void initTestFileFilterCombobox();
	void initImportButton();
	void initCleanButton();
	void initNotPassingFilterButton();

	QSet<TestStatus::Enum> getTestStatusFilter() const;

	void dragEnterEvent(QDragEnterEvent *event) override;
	void dropEvent(QDropEvent* event) override;

	void setTestModel(std::shared_ptr<TestModel> testModel);

	void import();

	//TODO: Move this function in a common place.
	static QList<QString> toOrderedList(const QSet<QString>& set);

    Ui::MainWindow *ui;
	std::shared_ptr<TestModel> mTestModel;

	static constexpr auto ALL_TEXT = "All";
	static constexpr auto NOT_PASSING_TEXT = "Not Passing";
};
