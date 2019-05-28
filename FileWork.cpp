#include "FileWork.h"

FileWork::FileWork(QString _fileName, QString* _outFileInfo) {
    this->fileName = _fileName;
    this->outFileInfo = _outFileInfo;
}

void FileWork::readFile() {
    if(fileName == "") {
        qDebug() << "FILE NAME IS EMPTY" << endl;
        emit emitReadResult(EMPTY_FILE_NAME);
    }

    qDebug() << "3 - readFile()" << endl;

    QFile hFile(fileName);

    if(!hFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Not able to open this file" << endl;
        emit emitReadResult(NOT_OPEN);
    }

    *outFileInfo = hFile.readAll();

    hFile.close();

    emit emitReadResult(SUCCESS);
}

void FileWork::writeFile() {

    if(fileName == "") {
        qDebug() << "FILE NAME IS EMPTY" << endl;
        emit emitReadResult(EMPTY_FILE_NAME);
    }

    QFile hFile(fileName);
    QByteArray byteArray = outFileInfo->toUtf8();

    if(!hFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Not able to open this file" << endl;
        //emit emitReadResult(NOT_OPEN);
    }

    hFile.write(byteArray);

    hFile.close();
}

void FileWork::getFileViewInfo() {
    //outFileInfo->clear();
}
