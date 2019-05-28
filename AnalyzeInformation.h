#ifndef ANALYZEINFORMATION_H
#define ANALYZEINFORMATION_H

#include <QObject>
#include <QTableView>
#include <DBWork.h>
#include <QDebug>
#include <QVector>
#include <QProgressBar>

class AnalyzeInformation : public QObject
{
    Q_OBJECT
public:
    //~AnalyzeInformation();
    AnalyzeInformation(QTableView*, QTableView*, int);
    AnalyzeInformation(QTableView*, int);
    void setBarStatus(QProgressBar*);

public slots:
    void readFromFileView();
    void readFromHexView();

    void runVirusAnalyzer();

signals:
    void emitByteArray(QByteArray);
    void emitAnalyzerResult(QStringList);
    void emitAnalyzeStatus(int);

private:
    bool isGotVirusSignature(QString, QString);
    int virusAmount = 0;
    QVector<int> virusPosition;
    QList<QStringList> virusInfo;

    QTableView* fileView;
    QTableView* hexView;
    int rowAmount;

    DBWork* ptrDB;
    QProgressBar* analyzeBar;
};

#endif // ANALYZEINFORMATION_H
