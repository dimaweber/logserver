#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
class QAbstractTableModel;
class QTableView;

class Gui : public QMainWindow
{
    Q_OBJECT
    QTableView* pTableView;
    QAbstractTableModel* pModel;
public:
    explicit Gui(QAbstractTableModel* pModel, QWidget *parent = nullptr);

signals:

};

#endif // GUI_H
