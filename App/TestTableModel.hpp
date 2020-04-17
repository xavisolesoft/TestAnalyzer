#pragma once

#include <QAbstractTableModel>

#include <memory>

#include "TestModel/TestStatus.hpp"

class TestModel;
class TestEntry;

class TestTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit TestTableModel(QObject *parent, std::shared_ptr<TestModel> mTestModel);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	const TestEntry* getRowTestEntry(int row) const;

private:
	//TODO: Move this function to TestStatus.hpp if finaly is not removed.
	static QString TestStatusToString(TestStatus testStatus);

public:
	static constexpr int TEST_STATUS_SECTION = 0;
	static constexpr int TEST_FAMILY_SECTION = 1;
	static constexpr int TEST_NAME_SECTION = 2;
	static constexpr int TEST_EXECUTION_TIME_SECTION = 3;
	static constexpr int OUTPUT_FILE_SECTION = 4;

private:
	std::shared_ptr<TestModel> mTestModel;
};
