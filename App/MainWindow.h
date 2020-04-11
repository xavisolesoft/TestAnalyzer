#pragma once

#include <QMenu>
#include <QMainWindow>

class TestModel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
	QMenu* createFileMenu();
	QMenu* createSettingsMenu();

	void initImportButton();
	void initCleanButton();

	void setTestModel(std::shared_ptr<TestModel> testModel);

	void import();

    Ui::MainWindow *ui;
	std::shared_ptr<TestModel> mTestModel;
};
