#pragma once

#include <QMenu>
#include <QDropEvent>
#include <QMainWindow>
#include <QDragEnterEvent>
#include <QDropEvent>

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

	void dragEnterEvent(QDragEnterEvent *event) override;
	void dropEvent(QDropEvent* event) override;

	void setTestModel(std::shared_ptr<TestModel> testModel);

	void import();

    Ui::MainWindow *ui;
	std::shared_ptr<TestModel> mTestModel;
};
