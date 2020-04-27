#include "TestSortFilterProxyModel.hpp"

#include "TestTableModel.hpp"

TestSortFilterProxyModel::TestSortFilterProxyModel(QObject* parent)
	: QSortFilterProxyModel(parent)
{

}

void TestSortFilterProxyModel::setTestStatusFilter(TestStatus::Enum value)
{
	testStatusFilter = value;
	invalidateFilter();
}

TestStatus::Enum TestSortFilterProxyModel::getTestStatusFilter() const
{
	return testStatusFilter;
}

bool TestSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	QModelIndex testStatusIndex = sourceModel()->index(sourceRow,
													   TestTableModel::TEST_STATUS_SECTION,
													   sourceParent);
	TestStatus::Enum testStatus = TestStatus::fromString(sourceModel()->data(testStatusIndex).toString());

	if(testStatusFilter == TestStatus::UNDEFINED){
		return true;
	}

	if(testStatusFilter == testStatus){
		return true;
	}

	return false;
}
