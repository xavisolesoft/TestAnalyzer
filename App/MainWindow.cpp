#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFile>
#include <QSettings>
#include <QFileDialog>

#include "GTest/GTestParser.hpp"
#include "TestModel/TestModel.hpp"
#include "TestModel/TestStatus.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	createFileMenu();
	createSettingsMenu();

	initImportButton();
	initCleanButton();
}

MainWindow::~MainWindow()
{
	delete ui;
}

QMenu *MainWindow::createFileMenu()
{
	QMenu* fileMenu = ui->menubar->addMenu(tr("File"));
	fileMenu->addAction(tr("Import"),
						[this](){
							import();
						},
						QKeySequence("Ctrl+I"));

	return fileMenu;
}

QMenu *MainWindow::createSettingsMenu()
{
	QMenu* settingsMenu = ui->menubar->addMenu(tr("Settings"));
	settingsMenu->addAction(tr("Notepad++"),
							[this](){
								QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TestRunner", "TestRunner");
								QString notepadppPath = settings.value("notepadpp/path", "").toString();
								notepadppPath = QFileDialog::getOpenFileName(this, tr("Select Notepad++ Executable"), notepadppPath, tr("Executable (*.exe)"));
								if(!notepadppPath.isNull()){
									settings.setValue("notepadpp/path", notepadppPath);
								}
							});

	return settingsMenu;
}

void MainWindow::initImportButton()
{
	connect(ui->importButton,
			&QAbstractButton::clicked,
			[this](){
				import();
	});
}

void MainWindow::initCleanButton()
{
	connect(ui->cleanButton,
			&QAbstractButton::clicked,
			[this](){
				setTestModel(std::make_shared<TestModel>());
	});
}

void MainWindow::setTestModel(std::shared_ptr<TestModel> testModel)
{
	ui->testTableView->setTestModel(testModel);

	ui->runTestsLabel->setText(tr("Run: %1")
							   .arg(testModel->getNumTests()));
	ui->okTestsLabel->setText(tr("OK: %1")
							   .arg(testModel->getNumTests(TestStatus::SUCCEED)));
	ui->failedTestsLabel->setText(tr("FAILED: %1")
							   .arg(testModel->getNumTests(TestStatus::FAILED)));
	ui->timeoutTestsLabel->setText(tr("TIMEOUT: %1")
								   .arg(testModel->getNumTests(TestStatus::TIMEOUT)));
	ui->crashedTestsLabel->setText(tr("CRASHED: %1")
							   .arg(testModel->getNumTests(TestStatus::CRASHED)));
}

void MainWindow::import()
{
	if(QStringList filePaths = QFileDialog::getOpenFileNames(this); !filePaths.empty()){
		auto allTestModels = std::make_shared<TestModel>();
		foreach(const QString& filePath, filePaths){
			QFile qTestOutputFile(filePath);
			GTestParser gTestParser;
			auto testModel = gTestParser.parseTestModel(qTestOutputFile);
			allTestModels->merge(testModel);
		}

		allTestModels->merge(mTestModel);
		mTestModel = allTestModels;
		setTestModel(allTestModels);
	}
}
