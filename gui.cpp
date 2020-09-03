#include "gui.h"

#include <QTableView>
#include <QHeaderView>

Gui::Gui(QAbstractTableModel* pModel, QWidget *parent) : QMainWindow(parent), pTableView(nullptr), pModel(pModel)
{
    pTableView = new QTableView(this);
    setMinimumSize(1200, 800);
    pTableView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    pTableView->setModel(pModel);
    pTableView->horizontalHeader()->setStretchLastSection(true);
    pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    setCentralWidget(pTableView);
}
