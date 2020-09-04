#ifndef MODEL_H
#define MODEL_H

#include "server.h"
#include "logline.h"

#include <QAbstractTableModel>
#include <QVector>
#include <QSet>
#include <QPair>

class Model: public QAbstractTableModel 
{
    Q_OBJECT
private:
    QVector<LogLine*> lines;
public: 
    Model(QObject* parent = nullptr);
    ~Model();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,  int role=Qt::DisplayRole) const override;

    QMap<LogLine::Fields, QSet<QString>> possibleValues;
    QMap<LogLine::Fields, QPair<QDateTime, QDateTime>> timestampRanges;

public slots:
    void onNewLine(LogLine* line);

signals:
    void newFilterValue(LogLine::Fields field, const QString& value);
};

#endif
