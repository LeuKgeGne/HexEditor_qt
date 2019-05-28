#include "DBWork.h"

DBWork::DBWork() {
    db = QSqlDatabase::addDatabase("QSQLITE");

    QString path = QDir::currentPath();
    path += QString("/VirusSignatureDatabase.db");

    qDebug() << path << endl;

    db.setDatabaseName(path);
    db.open();

    QSqlQuery query("SELECT * FROM VirusSignature");

    while(query.next()) {
        QStringList list;
        list.push_back(query.value(0).toString());
        list.push_back(query.value(1).toString());
        list.push_back(query.value(2).toString());

        this->dbInfo.push_back(list);
    }
}


QStringList DBWork::getSignature(int index) {
    QStringList current;

    if(index < dbInfo.size()) {
        current = dbInfo.at(index);
        return current;
    }

    return current;
}
