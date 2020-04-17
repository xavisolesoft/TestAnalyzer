#include "TestTableView.hpp"

#include <QKeyEvent>
#include <QClipboard>
#include <QApplication>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QProcess>
#include <QStringList>
#include <QSettings>
#include <QString>

#include <algorithm>

#include "TestModel/TestModel.hpp"
#include "TestModel/TestEntry.hpp"
#include "TestTableModel.hpp"

TestTableView::TestTableView(QWidget* parent) :
	QTableView(parent)
{

}

void TestTableView::setTestModel(std::shared_ptr<TestModel> testModel)
{
	auto testTableModel = new TestTableModel(this, testModel);
	auto sortFilterProxyModel = new QSortFilterProxyModel(this);
	sortFilterProxyModel->setDynamicSortFilter(true);
	sortFilterProxyModel->setSourceModel(testTableModel);

	setModel(sortFilterProxyModel);
	setSortingEnabled(true);
	horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	sortByColumn(0, Qt::AscendingOrder);

	initOpenGTestOutputOnFileNameClick(*testTableModel);
}

void TestTableView::initOpenGTestOutputOnFileNameClick(const TestTableModel& testTableModel)
{
	connect(this,
			&QTableView::clicked,
			[this, &testTableModel](const QModelIndex& modelIndex){
				QModelIndex mappedIndex = static_cast<QSortFilterProxyModel*>(model())->mapToSource(modelIndex);
				if(mappedIndex.column() == TestTableModel::OUTPUT_FILE_SECTION){
					if(const TestEntry* testEntry = testTableModel.getRowTestEntry(mappedIndex.row()); testEntry){
						QSettings settings(QSettings::IniFormat, QSettings::UserScope, "TestRunner", "TestRunner");
						QProcess process;
						process.execute(QString("\"%1\" \"%2\" -n%3")
										.arg(settings.value("notepadpp/path", "notepad++.exe").toString())
										.arg(testEntry->getOutputFilePath())
										.arg(testEntry->getLine()));
					}
				}
	});
}

void TestTableView::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_C && (event->modifiers() & Qt::ControlModifier))
		{
			QModelIndexList cells = selectedIndexes();
			std::sort(cells.begin(), cells.end()); // Necessary, otherwise they are in column order

			QString text;
			int currentRow = 0; // To determine when to insert newlines
			foreach (const QModelIndex& cell, cells) {
				if (text.length() == 0) {
					// First item
				} else if (cell.row() != currentRow) {
					// New row
					text += '\n';
				} else {
					// Next cell
					text += '\t';
				}
				currentRow = cell.row();
				text += cell.data().toString();
			}

			QApplication::clipboard()->setText(text);
	}
}
