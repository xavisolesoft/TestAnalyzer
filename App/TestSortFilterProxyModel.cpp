#include "TestSortFilterProxyModel.hpp"

#include "TestTableModel.hpp"

TestSortFilterProxyModel::TestSortFilterProxyModel(QObject* parent)
	: QSortFilterProxyModel(parent)
{

}

void TestSortFilterProxyModel::setTestStatusFilter(TestStatus::Enum value)
{
	mTestStatusFilter = value;
	invalidateFilter();
}

TestStatus::Enum TestSortFilterProxyModel::getTestStatusFilter() const
{
	return mTestStatusFilter;
}

void TestSortFilterProxyModel::setTestFamilyFilter(const QString &familyName)
{
	mTestFamilyFilter = familyName;
	invalidateFilter();
}

const QString& TestSortFilterProxyModel::getTestFamilyFilter() const
{
	return mTestFamilyFilter;
}

void TestSortFilterProxyModel::setTestFileFilter(const QString &fileName)
{
	mTestFileFilter = fileName;
	invalidateFilter();
}

const QString &TestSortFilterProxyModel::getTestFileFilter() const
{
	return mTestFileFilter;
}

bool TestSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	bool ret = true;

	ret &= filterAcceptsTestStatus(sourceRow, sourceParent);
	ret &= filterAcceptsTestFamily(sourceRow, sourceParent);
	ret &= filterAcceptsTestFile(sourceRow, sourceParent);

	return ret;
}

bool TestSortFilterProxyModel::filterAcceptsTestStatus(int sourceRow, const QModelIndex &sourceParent) const
{
	QModelIndex testStatusIndex = sourceModel()->index(sourceRow,
													   TestTableModel::TEST_STATUS_SECTION,
													   sourceParent);
	TestStatus::Enum testStatus = TestStatus::fromString(sourceModel()->data(testStatusIndex).toString());

	return mTestStatusFilter == TestStatus::UNDEFINED || mTestStatusFilter == testStatus;
}

bool TestSortFilterProxyModel::filterAcceptsTestFamily(int sourceRow, const QModelIndex &sourceParent) const
{
	QModelIndex testFamilyIndex = sourceModel()->index(sourceRow,
													   TestTableModel::TEST_FAMILY_SECTION,
													   sourceParent);
	QString testFamilyName = sourceModel()->data(testFamilyIndex).toString();

	return mTestFamilyFilter.isEmpty() || mTestFamilyFilter == testFamilyName;
}

bool TestSortFilterProxyModel::filterAcceptsTestFile(int sourceRow, const QModelIndex &sourceParent) const
{
	QModelIndex testFileIndex = sourceModel()->index(sourceRow,
													   TestTableModel::OUTPUT_FILE_SECTION,
													   sourceParent);
	QString testFileName = sourceModel()->data(testFileIndex).toString();

	return mTestFileFilter.isEmpty() || mTestFileFilter == testFileName;
}
