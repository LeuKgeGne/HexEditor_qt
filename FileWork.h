#ifndef FILEWORK_H
#define FILEWORK_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <ViewWork.h>

#define SUCCESS 0
#define EMPTY_FILE_NAME -1
#define NOT_OPEN -2


class FileWork : public QObject
{
    Q_OBJECT
public:
    FileWork(QString _fileName, QString* _outFileInfo);

public slots:
    void readFile();
    void writeFile();

signals:
    void emitReadResult(int);
    void emitFileName(QString);

private:
    void getFileViewInfo();
    QString fileName;
    QString* outFileInfo;
};

#endif // FILEWORK_H
