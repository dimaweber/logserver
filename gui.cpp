#include "gui.h"
#include "filtermodel.h"

#include <QTableView>
#include <QHeaderView>

Gui::Gui(QAbstractTableModel* pModel, QWidget *parent)
    : QMainWindow(parent), pTableView(nullptr), pModel(pModel), pProxyModel(new FilterModel(this))
{
    pTableView = new QTableView(this);
    setMinimumSize(1200, 800);
    pTableView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    pProxyModel->setSourceModel(pModel);
    pTableView->setModel(pProxyModel);
    pTableView->horizontalHeader()->setStretchLastSection(true);
    pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setCentralWidget(pTableView);

    pProxyModel->addFilter(LogLine::ProcessName, "oamd");
    pProxyModel->addFilter(LogLine::Priority, 15);
}
