#include "AnalyzeInformation.h"

AnalyzeInformation::AnalyzeInformation(QTableView* _fileView, QTableView* _hexView,
                                       int _rowAmount) {
    this->fileView = _fileView;
    this->hexView = _hexView;
    this->rowAmount = _rowAmount;
}

AnalyzeInformation::AnalyzeInformation(QTableView* _fileView, int _rowAmount) {
    this->fileView = _fileView;
    this->rowAmount = _rowAmount;
}

void AnalyzeInformation::setBarStatus(QProgressBar* _analyzeBar) {
    this->analyzeBar = _analyzeBar;
}

void AnalyzeInformation::readFromFileView() {
    qDebug() << " 10 - readFromFileView()" << endl;

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

    qDebug() << byteArray << "(readFromFileView)" << endl;
    emit emitByteArray(byteArray);
}

void AnalyzeInformation::readFromHexView() {
    QVariant data;
    QModelIndex modelIndex;
    QByteArray byteArray;

    for(int i = 0; i < this->rowAmount; ++i) {
        for(int j = 0; j < 16; ++j) {
            modelIndex = hexView->model()->index(i, j, QModelIndex());
            data = hexView->model()->data(modelIndex, Qt::DisplayRole);
            byteArray += data.toString().toUtf8();
        }
    }

    qDebug() << byteArray << "(readFromHexView)" << endl;
    emit emitByteArray(byteArray);
}

void AnalyzeInformation::runVirusAnalyzer() {
    qDebug() << " 9.1 - runVirusAnalyzer()" << endl;

    QVariant data;
    QModelIndex modelIndex;
    QString fileInfo;
    QStringList currentList;
    QString virusSignature;

    bool runFlag = true;
    ptrDB = new DBWork();


    for(int i = 0; i < this->rowAmount; ++i) {
        for(int j = 0; j < 16; ++j) {
            modelIndex = fileView->model()->index(i, j, QModelIndex());
            data = fileView->model()->data(modelIndex, Qt::DisplayRole);
            fileInfo += data.toString();
        }
    }

    qDebug() << fileInfo << "(runVirusAnalyzer)" << endl;
    int counter = 0;
    bool isGotVirus = false;


//    while(runFlag) {
        currentList = ptrDB->getSignature(counter);

            qDebug() << currentList.at(0);
            qDebug() << currentList.at(1);
            qDebug() << currentList.at(2) << endl;

        if(!currentList.isEmpty()) {
            virusSignature = currentList.at(2);

            //analyze is here
            if(isGotVirusSignature(fileInfo, virusSignature)) {
                QStringList infoList;
                infoList.push_back(currentList.at(1));
                infoList.push_back(currentList.at(2));

                for(int i = 0; i < virusPosition.size();i++) {
                    infoList.push_back(QString::number(virusPosition.at(i)));
                }

                virusInfo.push_back(infoList);
                isGotVirus = true;
            }
            //here we need to show window about the Virus & his position
        } else {
            runFlag = false;
        }

//        counter++;
//    }

    if(isGotVirus) {
        //qDebug() << "isGotVirusSignature : We've got a virus" << "(" << virusAmount << ")" << endl;
        emit emitAnalyzerResult(this->virusInfo.at(0));
    } else {
        //qDebug() << "isGotVirusSignature : Sector clear" << endl;
        //emit emitAnalyzerResult("Sector clear");
    }
}

bool AnalyzeInformation::isGotVirusSignature(QString fileInfo,
                                             QString signature) {
    bool isGotVirus = false;
    int counter, pos, checker = 0    ;
    QByteArray virusToUtf8 = signature.toUtf8();
    QByteArray fileToUtf8 = fileInfo.toUtf8();

    virusToUtf8 = virusToUtf8.toHex();
    //qDebug() << fileToUtf8 << " (fileInfo)" << endl;
    //qDebug() << virusToUtf8 << " (signature)" << endl;
    int divider = fileToUtf8.size() / 100;
    //qDebug() << divider << " - divider" << endl;

    for (int i = 0; i < fileToUtf8.size(); ++i) {
        //qDebug() << virusToUtf8.at(0) << " " << fileToUtf8.at(i) << " " << i << endl;
        if(virusToUtf8.at(0) == fileToUtf8.at(i)) {

            //qDebug() << "Start" << endl;
            counter = 1;
            pos = ++i;

            for (int j = 1; j < virusToUtf8.size(); ++j) {
                //qDebug() << fileToUtf8.at(pos) << " == " << virusToUtf8.at(j) << endl;
                fileToUtf8.at(pos) == virusToUtf8.at(j) ? counter++ : j = virusToUtf8.size() - 1;
                //qDebug() << counter << " (counter)" << virusToUtf8.size() << endl;
                pos++;
            }

            if(counter == virusToUtf8.size()) {
                virusPosition.push_back(i/2 + 1);
                isGotVirus = true;
                virusAmount++;
            }
        }

        if(fileToUtf8.size() % divider == 0) {
            checker++;
            //qDebug() << checker << " - checker" << endl;
            emit emitAnalyzeStatus(checker);
//            this->analyzeBar->setValue(checker);
        }
    }

    if(checker <= 100) {
        emit emitAnalyzeStatus(100);
    }

    return isGotVirus;
}






