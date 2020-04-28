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

	void setTestFamilyFilter(const QString& familyName);
	const QString& getTestFamilyFilter() const;

protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourcePrent) const override;

private:
	bool filterAcceptsTestStatus(int sourceRow, const QModelIndex &sourceParent) const;
	bool filterAcceptsTestFamily(int sourceRow, const QModelIndex &sourceParent) const;

	TestStatus::Enum mTestStatusFilter = TestStatus::UNDEFINED;
	QString mTestFamilyFilter;
};

