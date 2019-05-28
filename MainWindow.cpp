#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>

//#define ROW_AMNT 3

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //THREADS FOR WORK
    this->ptrViewWorkThread = new QThread;
    this->ptrConvertThread = new QThread;
    this->ptrFileWorkThread = new QThread;
    this->ptrAnalyzerThread = new QThread;

    this->chooseFileThread = new QThread;
    ///////////////////////////////////

    this->fileModel = new QStandardItemModel;
    this->hexModel = new QStandardItemModel;

    ui->fileView->setModel(fileModel);
    ui->hexView->setModel(hexModel);

    ui->fileView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->hexView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->fileView->verticalHeader();

    ui->hexView->horizontalHeader();
    ui->hexView->verticalHeader()->resizeSection(0, 200);

    ui->analyzeBar->setRange(0, 100);
    ui->convertBar->setRange(0, 100);
    QFont font = ui->sizeLabel->font();
    //font.setPixelSize(12);
    ui->sizeLabel->setFont(font);
    ui->fileNameLabel->setFont(font);

    ui->fileView->setColumnWidth(1, 20);
    ui->fileView->setColumnWidth(2, 15);
    ui->fileView->setColumnWidth(3, 30);
}

MainWindow::~MainWindow() {
    delete ui;

    this->ptrViewWorkThread->quit();
    this->ptrViewWorkThread->wait();
    this->ptrConvertThread->quit();
    this->ptrConvertThread->wait();
    this->ptrFileWorkThread->quit();
    this->ptrFileWorkThread->wait();
    this->ptrAnalyzerThread->quit();
    this->ptrAnalyzerThread->wait();

    delete ptrConvertThread;
    delete ptrAnalyzerThread;
    delete ptrFileWorkThread;
    delete ptrViewWorkThread;
}

void MainWindow::getValue(QModelIndex x, QModelIndex y, QVector<int> data) {
    qDebug() << x.row() << " " << y.column() << " data: " << x.data().toString();
    //fileModel->setData()
    fileModel->item(x.row(), y.column())->setText(x.data().toString());
}
//***************************************************************//
void MainWindow::on_editHex_clicked() {
    ui->hexView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    connect(ui->hexView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
            this, SLOT(getValue(QModelIndex,QModelIndex,QVector<int>)), Qt::DirectConnection);
}

void MainWindow::on_loadFile_clicked() {
    qDebug() << "1 - on_loadFile_clicked()" << endl;
    fileName = ui->fileNameLine->text();
    if(fileName != "") {
        openFileToRead(fileName);
    } else {
        openFileToRead(QFileDialog::getOpenFileName(this));
    }
}

//FIND HOW TO DELETE TABLE
void MainWindow::on_clearWindows_clicked() {
    this->ptrViewWorkThread->quit();
    this->ptrViewWorkThread->wait();
    this->ptrConvertThread->quit();
    this->ptrConvertThread->wait();
    this->ptrFileWorkThread->quit();
    this->ptrFileWorkThread->wait();
    this->ptrAnalyzerThread->quit();
    this->ptrAnalyzerThread->wait();

    this->fileModel->clear();
    this->hexModel->clear();
    ui->analyzeBar->setValue(0);
    ui->convertBar->setValue(0);
    ui->sizeLine->clear();

    fileName.clear();
    fileName = "";
    outFileInfo.clear();
    outFileInfo = "";
    rowAmount = 0;
}

void MainWindow::on_convertBar_valueChanged(int value) {
    ui->convertBar->setValue(value);
}


void MainWindow::on_saveDefault_clicked() {
    ui->hexView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    if(fileName == "") {
        (new QErrorMessage(this))->showMessage("Not able to save by this button ;(");
        return;
    }

    QFile filPtr;

    this->ptrFileWork = new FileWork(fileName, &outFileInfo);
    this->ptrFileWork->moveToThread(this->ptrFileWorkThread);

    connect(ptrFileWorkThread, SIGNAL(started()), ptrFileWork, SLOT(writeFile()));
    ptrFileWorkThread->start();
}

void MainWindow::on_saveAnother_clicked() {
    ui->hexView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    writeToNewFile();
}


//______________________________________________________________//
//Mein functions:

void MainWindow::openFileToRead(QString _fileName) {
    qDebug() << "File name(this): " << fileName << endl;
    qDebug() << "File name: " << _fileName << endl;

    qDebug() << "2 - openFileToRead(QString _fileName)" << endl;

    this->fileName = _fileName;
    if(fileName.size() == 0) {
        (new QErrorMessage(this))->showMessage("Enter something!");
    }
    else {
        this->ptrFileWork = new FileWork(fileName, &outFileInfo);
        this->ptrFileWork->moveToThread(ptrFileWorkThread);

        connect(ptrFileWork, SIGNAL(emitReadResult(int)), this, SLOT(initOutFileField(int)));
        connect(ptrFileWorkThread, SIGNAL(started()), ptrFileWork, SLOT(readFile()));

        this->ptrFileWorkThread->start();
    }
}



void MainWindow::initOutFileField(int readResult) {

    qDebug() << this->outFileInfo.size() << "- size of initOutFileField" << endl;

    rowAmount = (this->outFileInfo.size() / COLUMN_AMNT) + 1;

    switch(readResult) {
        case 0: {
            this->ptrViewWork = new ViewWork(hexModel, fileModel, &outFileInfo, rowAmount);

            this->ptrViewWork->moveToThread(ptrViewWorkThread);

            connect(ptrViewWork, SIGNAL(emitInit(int)), this, SLOT(convertInfo(int)));
            connect(ptrViewWorkThread, SIGNAL(started()), ptrViewWork, SLOT(addRowsToFileView()));

            ptrViewWorkThread->start();

        } break;

        case -1: {
            qDebug() << "case -1 in the SWITCH" << endl;
        } break;

        case -2: {
            qDebug() << "case -2 in the SWITCH" << endl;
        } break;

        default: {
            qDebug() << "Default in the SWITCH" << endl;
        } break;
    }
    ui->sizeLine->setText(QString::number(outFileInfo.size()));
}

void MainWindow::convertInfo(int isReady) {
    qDebug() << " 8 - convertInfo(int isReady)" << endl;

    if(isReady == 0) {
        this->ptrConvertInfo = new ConvertInformation(ui->hexView, ui->fileView,
                                                      outFileInfo, rowAmount);
        this->ptrConvertInfo->moveToThread(ptrConvertThread);

        connect(ptrConvertThread, SIGNAL(started()), ptrConvertInfo, SLOT(convert()));
        connect(ptrConvertInfo, SIGNAL(emitString(QString)), this, SLOT(updateWindow(QString)));
        connect(ptrConvertThread, SIGNAL(started()), this, SLOT(analyzeFileInfo()));
        ptrConvertThread->start();
    }
}

void MainWindow::analyzeFileInfo() {
    qDebug() << " 8.1 - analyzeFileInfo(int result)" << endl;

    for(int i = 0; i < COLUMN_AMNT; i++) {
        ui->hexView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
        ui->fileView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    ui->fileView->horizontalHeader()->stretchLastSection();
    ui->hexView->horizontalHeader()->stretchLastSection();

    this->ptrAnalyzeInfo = new AnalyzeInformation(ui->hexView, rowAmount);
    ptrAnalyzeInfo->setBarStatus(ui->analyzeBar);
    this->ptrAnalyzeInfo->moveToThread(ptrAnalyzerThread);


    connect(ptrAnalyzeInfo, SIGNAL(emitAnalyzeStatus(int)), this, SLOT(updateAnalyzeBar(int)));
    connect(ptrAnalyzeInfo, SIGNAL(emitAnalyzerResult(QStringList)),
            this, SLOT(showAnalyzerResult(QStringList)));

    connect(ptrAnalyzerThread, SIGNAL(started()), ptrAnalyzeInfo, SLOT(runVirusAnalyzer()));
    ptrAnalyzerThread->start();

}

QString MainWindow::decodePosition(QString position) {
    int rowNumber = ((position.toInt()) / 16 + 1);
    int rowPosition = (position.toInt()) % 16;

    return "Row: " + QString::number(rowNumber) + " Position: " + QString::number(rowPosition);
}

void MainWindow::writeToNewFile() {
    if(fileName == "") {
        (new QErrorMessage(this))->showMessage("Not able to save by this button ;(");
        return;
    }

    QString str = QFileDialog::getExistingDirectory(this);
    QFile filPtr;

    this->ptrFileWork = new FileWork(str, &outFileInfo);
    this->ptrFileWork->moveToThread(this->ptrFileWorkThread);


}

void MainWindow::writeToFile() {

}

void MainWindow::updateWindow(QString result) {
    if(result == "") {
        (new QErrorMessage(this))->showMessage("Incorrect result of Convertation");
        return;
    }
    qDebug() << " 12 - updateWindow(QString result)" << endl;


    QString currentStr;
    int checker = 0;
    int byteAmount = rowAmount * COLUMN_AMNT;
    int divider = byteAmount / MAX_RESULT;

    if(divider == 0) divider = 1;

    int counter = 0;
    for(int i = 0; i < this->rowAmount; ++i) {
        for(int j = 0; j < COLUMN_AMNT; ++j) {
            currentStr = result.at(counter);
            currentStr += result.at(counter+1);

            //qDebug() << currentStr << "currentStr " << i << " " << j << endl;
            hexModel->item(i, j)->setText(currentStr);
            counter += 2;

            if(byteAmount % divider == 0) {
                checker++;
                ui->convertBar->setValue(checker);
            }
        }
    }

    if(checker <= MAX_RESULT) {
        ui->convertBar->setValue(MAX_RESULT);
    }

}

void MainWindow::updateAnalyzeBar(int status) {
    ui->analyzeBar->setValue(status);
}


void MainWindow::showAnalyzerResult(QStringList resultList) {
    for(int i = 0; i < resultList.size(); ++i) {
        if(i == 0) {
            qDebug() << "level: " << resultList.at(i) << endl;
        }
        if(i == 1) {
            qDebug() << "signature: " << resultList.at(i) << endl;
        }
        if(i > 1) {
            qDebug() << "before: " << resultList.at(i) << endl;
            qDebug() << "after: " << decodePosition(resultList.at(i)) << endl;
        }
    }
}


