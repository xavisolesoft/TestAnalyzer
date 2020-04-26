#include "TestTableModel.hpp"

#include <QFileInfo>

#include "TestModel/TestModel.hpp"
#include "TestModel/TestEntry.hpp"

TestTableModel::TestTableModel(QObject *parent, std::shared_ptr<TestModel> testModel)
	: QAbstractTableModel(parent),
	  mTestModel(testModel)
{
}

int TestTableModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)

	return mTestModel->getNumTests();
}

int TestTableModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)

	return 5;
}

QVariant TestTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if(role == Qt::DisplayRole && orientation == Qt::Horizontal){
		if(section == TEST_STATUS_SECTION){
			return tr("Status");
		} else if(section == TEST_FAMILY_SECTION){
			return tr("Family");
		} else if(section == TEST_NAME_SECTION) {
			return tr("Name");
		} else if(section == TEST_EXECUTION_TIME_SECTION) {
			return tr("Execution Time (secs)");
		} else if(section == OUTPUT_FILE_SECTION) {
			return tr("Output File");
		}
	}

	return QVariant();
}

QVariant TestTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()){
		return QVariant();
	}

	if(role == Qt::DisplayRole){
		if(index.column() == TEST_STATUS_SECTION){
			return TestStatusToString(mTestModel->getTest(index.row()).getStatus());
		} else if(index.column() == TEST_FAMILY_SECTION){
			return mTestModel->getTest(index.row()).getFamily();
		} else if(index.column() == TEST_NAME_SECTION) {
			return mTestModel->getTest(index.row()).getName();
		} else if(index.column() == TEST_EXECUTION_TIME_SECTION) {
			return mTestModel->getTest(index.row()).getExecutionTimeMiliSecs()/1000.0;
		} else if(index.column() == OUTPUT_FILE_SECTION) {
			return QFileInfo(mTestModel->getTest(index.row()).getOutputFilePath()).completeBaseName();
		}
	}

	return QVariant();
}

const TestEntry* TestTableModel::getRowTestEntry(int row) const
{
	if(mTestModel->getNumTests() < mTestModel->getNumTests()){
		return nullptr;
	}

	return &mTestModel->getTest(row);
}

QString TestTableModel::TestStatusToString(TestStatus testStatus)
{
	switch(testStatus)
	{
	case TestStatus::NOT_STARTED:
		return tr("NOT_STARTED");
	case TestStatus::RUNNING:
		return tr("RUNNING");
	case TestStatus::SUCCEED:
		return tr("SUCCEED");
	case TestStatus::FAILED:
		return tr("FAILED");
	case TestStatus::CRASHED:
		return tr("CRASHED");
	case TestStatus::TIMEOUT:
		return tr("TIMEOUT");
	}

	return "UNDEFINED";
}
