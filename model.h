#ifndef MODEL_H
#define MODEL_H

#include "server.h"
#include "logline.h"

#include <QAbstractTableModel>
#include <QList>

class Model: public QAbstractTableModel 
{
    Q_OBJECT
private:
    QList<LogLine*> lines;
public: 
    Model(QObject* parent = nullptr);
    ~Model();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,  int role=Qt::DisplayRole) const override;
public slots:
    void onNewLine(LogLine* line);
};

#endif
