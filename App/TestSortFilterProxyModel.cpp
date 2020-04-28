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

bool TestSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	bool ret = true;

	ret &= filterAcceptsTestStatus(sourceRow, sourceParent);
	ret &= filterAcceptsTestFamily(sourceRow, sourceParent);

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
