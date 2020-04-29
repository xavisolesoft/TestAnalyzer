#pragma once

#include <QSet>
#include <QSortFilterProxyModel>

#include "TestModel/TestStatus.hpp"

class TestSortFilterProxyModel :
		public QSortFilterProxyModel
{
	Q_OBJECT
public:
	TestSortFilterProxyModel(QObject* parent = nullptr);

	void setTestStatusFilter(QSet<TestStatus::Enum> values);
	const QSet<TestStatus::Enum>& getTestStatusFilter() const;

	void setTestFamilyFilter(const QString& familyName);
	const QString& getTestFamilyFilter() const;

	void setTestFileFilter(const QString& familyName);
	const QString& getTestFileFilter() const;

protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourcePrent) const override;

private:
	bool filterAcceptsTestStatus(int sourceRow, const QModelIndex &sourceParent) const;
	bool filterAcceptsTestFamily(int sourceRow, const QModelIndex &sourceParent) const;
	bool filterAcceptsTestFile(int sourceRow, const QModelIndex &sourceParent) const;

	QSet<TestStatus::Enum> mTestStatusFilters;
	QString mTestFamilyFilter;
	QString mTestFileFilter;
};

