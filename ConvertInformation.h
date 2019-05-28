#ifndef CONVERTINFORMATION_H
#define CONVERTINFORMATION_H

#include <QObject>
#include <AnalyzeInformation.h>
#include <QThread>

class ConvertInformation : public QObject
{
    Q_OBJECT
public:
    ConvertInformation();
    ConvertInformation(QTableView*, QTableView*, QString, int);

public slots:
    void convert();

signals:
    void emitString(QString);

private:
    QTableView* hexView;
    QTableView* fileView;

    QString outFileInfo;
    AnalyzeInformation* ptrAI;
    QThread* ptrAIThread;

    int rowAmount;
};

#endif // CONVERTINFORMATION_H
