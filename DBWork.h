#ifndef DBWORK_H
#define DBWORK_H

#include <QObject>
#include <QVariant>
#include <QDir>
#include <QDebug>

//Database
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>

class DBWork : public QObject
{
    Q_OBJECT
public:
    DBWork();

public slots:
    QStringList getSignature(int);
    //QList<QString> getSignature();

signals:

private:
    QList<QStringList> dbInfo;
    QSqlDatabase db;
    QSqlQuery query;
    //QStringList signatureInfo;

    QString key_t;
    QString level;
    QString signature;
};

#endif // DBWORK_H
