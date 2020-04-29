#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFile>
#include <QFileInfo>
#include <QMimeData>
#include <QSettings>
#include <QClipboard>
#include <QFileDialog>
#include <QApplication>

#include <algorithm>

#include "GTest/GTestParser.hpp"
#include "TestModel/TestModel.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	createFileMenu();
	createSettingsMenu();

	initTestStatusFilterCombobox();
	initTestFamilyFilterCombobox();
	initTestFileFilterCombobox();
	initImportButton();
	initCleanButton();
	initNotPassingFilterButton();

	setAcceptDrops(true);
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

void MainWindow::initTestStatusFilterCombobox()
{
	ui->testStatusFilterComboBox->addItem(tr(ALL_TEXT));
	ui->testStatusFilterComboBox->addItem(tr(NOT_PASSING_TEXT));
	ui->testStatusFilterComboBox->addItem(TestStatus::toString(TestStatus::SUCCEED), TestStatus::SUCCEED);
	ui->testStatusFilterComboBox->addItem(TestStatus::toString(TestStatus::FAILED), TestStatus::FAILED);
	ui->testStatusFilterComboBox->addItem(TestStatus::toString(TestStatus::CRASHED), TestStatus::CRASHED);
	ui->testStatusFilterComboBox->addItem(TestStatus::toString(TestStatus::TIMEOUT), TestStatus::TIMEOUT);

	ui->testStatusFilterComboBox->setCurrentText(tr(ALL_TEXT));

	connect(ui->testStatusFilterComboBox,
			&QComboBox::currentTextChanged,
			[this](){
				ui->testTableView->setTestStatusFilter(getTestStatusFilter());
	});
}

void MainWindow::initTestFamilyFilterCombobox()
{
	if(mTestModel){
		ui->testFamilyFilterComboBox->setDisabled(false);
		ui->testFamilyFilterLabel->setDisabled(false);
		ui->testFamilyFilterComboBox->clear();

		ui->testFamilyFilterComboBox->addItem(tr(ALL_TEXT));
		foreach(const QString& testFamilyName, toOrderedList(mTestModel->getTestFamilyNames())){
			ui->testFamilyFilterComboBox->addItem(testFamilyName);
		}

		ui->testFamilyFilterComboBox->setCurrentText(tr(ALL_TEXT));

		connect(ui->testFamilyFilterComboBox,
				&QComboBox::currentTextChanged,
				[this](const QString& currentText){
					QString testFamilyFilter = currentText;
					if(currentText == "All"){
						testFamilyFilter = "";
					}
					ui->testTableView->setTestFamilyFilter(testFamilyFilter);
		});
	} else{
		ui->testFamilyFilterComboBox->setDisabled(true);
		ui->testFamilyFilterLabel->setDisabled(true);
	}
}

void MainWindow::initTestFileFilterCombobox()
{
	if(mTestModel){
		ui->testFileFilterComboBox->setDisabled(false);
		ui->testFileFilterLabel->setDisabled(false);
		ui->testFileFilterComboBox->clear();

		ui->testFileFilterComboBox->addItem(tr(ALL_TEXT));
		foreach(const QString& testFileName, toOrderedList(mTestModel->getOutputFilePaths())){
			ui->testFileFilterComboBox->addItem(QFileInfo(testFileName).baseName());
		}

		ui->testFileFilterComboBox->setCurrentText(tr(ALL_TEXT));

		connect(ui->testFileFilterComboBox,
				&QComboBox::currentTextChanged,
				[this](const QString& currentText){
					QString testFileFilter = currentText;
					if(currentText == "All"){
						testFileFilter = "";
					}
					ui->testTableView->setTestFileFilter(testFileFilter);
		});
	} else{
		ui->testFileFilterComboBox->setDisabled(true);
		ui->testFileFilterLabel->setDisabled(true);
	}
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

void MainWindow::initNotPassingFilterButton()
{
	connect(ui->notPassingFilterButton,
			&QAbstractButton::clicked,
			[this](){
				QString gtestFitler;
				for(int i = 0; i < mTestModel->getNumTests(); ++i){
					const TestEntry& testEntry = mTestModel->getTest(i);
					if(testEntry.getStatus() == TestStatus::FAILED ||
							testEntry.getStatus() == TestStatus::CRASHED ||
							testEntry.getStatus() == TestStatus::TIMEOUT){
						gtestFitler.append(testEntry.getFamily() + "." + testEntry.getName() + ":");
					}
				}

				if(!gtestFitler.isEmpty()){
					gtestFitler.chop(1);
				}

				QClipboard *clipboard = QApplication::clipboard();
				clipboard->setText(gtestFitler);
	});
}

QSet<TestStatus::Enum> MainWindow::getTestStatusFilter() const
{
	QSet<TestStatus::Enum> testStatuses;
	const QString currentText = ui->testStatusFilterComboBox->currentText();

	if(currentText == tr(NOT_PASSING_TEXT)){
		testStatuses.insert(TestStatus::UNDEFINED);
		testStatuses.insert(TestStatus::NOT_STARTED);
		testStatuses.insert(TestStatus::RUNNING);
		testStatuses.insert(TestStatus::FAILED);
		testStatuses.insert(TestStatus::CRASHED);
		testStatuses.insert(TestStatus::TIMEOUT);
	} else if(currentText != tr(ALL_TEXT)){
		testStatuses.insert(ui->testStatusFilterComboBox->currentData().value<TestStatus::Enum>());
	}

	return testStatuses;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
	if (const QMimeData* mimeData = event->mimeData(); mimeData){
		bool areAcceptedFileTypes = true;
		foreach(const QUrl& fileUrl, mimeData->urls()){
			QString fileExtension = QFileInfo(fileUrl.toLocalFile()).completeSuffix();
			if(fileExtension != "txt"){
				areAcceptedFileTypes = false;
				break;
			}
		}
		if(areAcceptedFileTypes){
			event->acceptProposedAction();
		}
	}

}

void MainWindow::dropEvent(QDropEvent* event)
 {
	if (const QMimeData* mimeData = event->mimeData(); mimeData){
		auto allTestModels = std::make_shared<TestModel>();
		foreach(const QUrl& fileUrl, mimeData->urls()){
			QFile qTestOutputFile(fileUrl.toLocalFile());
			GTestParser gTestParser;
			auto testModel = gTestParser.parseTestModel(qTestOutputFile);
			allTestModels->merge(testModel);
		}
		allTestModels->merge(mTestModel);
		setTestModel(allTestModels);
	}
}

void MainWindow::setTestModel(std::shared_ptr<TestModel> testModel)
{
	ui->testTableView->setTestModel(testModel);
	ui->testTableView->setTestStatusFilter(getTestStatusFilter());

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

	mTestModel = testModel;
	initTestFamilyFilterCombobox();
	initTestFileFilterCombobox();
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
		setTestModel(allTestModels);
	}
}

QList<QString> MainWindow::toOrderedList(const QSet<QString>& set)
{
	QList<QString> list = set.toList();
	std::sort(list.begin(), list.end());
	return list;
}
