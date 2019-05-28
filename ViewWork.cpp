#include "ViewWork.h"

ViewWork::ViewWork(QTableView* _hexView, QTableView* _fileView,
                   QString* _outFileInfo, int rowAmount) {

    this->hexView = _hexView;
    this->fileView = _fileView;
    this->outFileInfo = _outFileInfo;
    this->rowAmount = rowAmount;
}

ViewWork::ViewWork(QStandardItemModel* hexModel, QStandardItemModel* fileModel,
                   QString* _outFileInfo, int rowAmount) {
    this->hexModel = hexModel;
    this->fileModel = fileModel;
    this->outFileInfo = _outFileInfo;
    this->rowAmount = rowAmount;
}

void ViewWork::addRowsToFileView() {
    qDebug() << " 5 - addRowsToFileView()" << endl;
    initRows();

    qDebug() << " 7 - addRowsToFileView()" << endl;
    bool isRead = false;

    int filePosition = 0;
    int fileSize = outFileInfo->size();
    qDebug() << "String SIZE: " << fileSize << endl;
    for(int i = 0; i < rowAmount; i++) {
        //qDebug() << "i = " << i << " (rowAmount)" << endl;
        for(int j = 0; j < COLUMN_AMNT; j++) {

            if(!(filePosition < fileSize)) {
                isRead = true;
            } else {
                //qDebug() << "j = " << j << " (COLUMN_AMNT)" << endl;
                fileModel->item(i, j)->setText(outFileInfo->at(filePosition));
                filePosition++;
            }
        }
    }

    qDebug() << "DONE IT!!!" << endl;
    if(isRead) {
        emit emitInit(SUCCESS);
    }
    else {
        emit emitInit(UNSUCCESS);
    }
}

void ViewWork::initRows() {
    qDebug() << " 6 - initRows()" << endl;
    for(int i = 0; i < rowAmount; ++i) {
        this->hexModel->insertRow(i, getFilledRow());
        this->fileModel->insertRow(i, getFilledRow());
    }
}

QList<QStandardItem*> ViewWork::getFilledRow() {
    QList<QStandardItem*> itemList;
    QStandardItem* item;
    for(int i = 0; i < COLUMN_AMNT; ++i) {
        item = new QStandardItem(QString("00"));
        itemList.push_back(item);
    }

    return itemList;
}
