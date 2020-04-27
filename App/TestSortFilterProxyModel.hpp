#pragma once

#include <QSortFilterProxyModel>

#include "TestModel/TestStatus.hpp"

class TestSortFilterProxyModel :
		public QSortFilterProxyModel
{
	Q_OBJECT
public:
	TestSortFilterProxyModel(QObject* parent = nullptr);

	void setTestStatusFilter(TestStatus::Enum value);
	TestStatus::Enum getTestStatusFilter() const;

protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourcePrent) const override;

private:
	TestStatus::Enum testStatusFilter = TestStatus::UNDEFINED;
};

