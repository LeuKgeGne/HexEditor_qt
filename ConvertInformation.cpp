#include "ConvertInformation.h"
#include <QErrorMessage>
#include <QDebug>

ConvertInformation::ConvertInformation() {
}

ConvertInformation::ConvertInformation(QTableView* hexView, QTableView* fileView,
                                       QString _outFileInfo, int rowAmount) {
    this->outFileInfo = _outFileInfo;
    this->hexView = hexView;
    this->fileView = fileView;

    this->rowAmount = rowAmount;

    this->ptrAIThread = new QThread;
    this->ptrAI = new AnalyzeInformation(fileView, rowAmount);
}

void ConvertInformation::convert() {
    QString result;
    QVariant data;
    QModelIndex modelIndex;
    QByteArray byteArray;

    for(int i = 0; i < this->rowAmount; ++i) {
        for(int j = 0; j < 16; ++j) {
            modelIndex = fileView->model()->index(i, j, QModelIndex());
            data = fileView->model()->data(modelIndex, Qt::DisplayRole);
            byteArray += data.toString().toUtf8();
        }
    }

    result = byteArray.toHex();

    emit emitString(result);
}


