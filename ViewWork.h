#ifndef VIEWWORK_H
#define VIEWWORK_H

#include <QObject>
#include <QList>
#include <QThread>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <QErrorMessage>
#include <AnalyzeInformation.h>

#define COLUMN_AMNT 16
#define SUCCESS 0
#define UNSUCCESS 1

class ViewWork : public QObject
{
    Q_OBJECT
public:
    ViewWork(QTableView*, QTableView*, QString*, int);
    ViewWork(QStandardItemModel*, QStandardItemModel*, QString*, int);

public slots:
    void addRowsToFileView();

signals:
    void emitInit(int);

private:
    void initRows();
    QList<QStandardItem*> getFilledRow();

    QTableView* hexView;
    QTableView* fileView;

    QStandardItemModel* hexModel;
    QStandardItemModel* fileModel;
    QString* outFileInfo;
    int rowAmount;
};

#endif // VIEWWORK_H
