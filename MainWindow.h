#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define COLUMN_AMNT 16
#define MAX_RESULT 100

#include <QMainWindow>
#include <QFile>
#include <QModelIndex>
#include <QMessageBox>
#include <QErrorMessage>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QStandardItem>

//new
#include <ConvertInformation.h>
#include <ViewWork.h>
#include <FileWork.h>
#include <AnalyzeInformation.h>
#include <QFileDialog>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void getValue(QModelIndex, QModelIndex, QVector<int>);
    //mein
    void updateWindow(QString);
    void updateAnalyzeBar(int);

    void on_loadFile_clicked();
    void on_clearWindows_clicked();
    void on_convertBar_valueChanged(int);
    void on_saveDefault_clicked();
    void on_saveAnother_clicked();
    void on_editHex_clicked();

    void openFileToRead(QString);
    void initOutFileField(int);
    void convertInfo(int);
    void showAnalyzerResult(QStringList);
    void analyzeFileInfo();
    void writeToNewFile();
    void writeToFile();

private:
    QString decodePosition(QString);
    //Threads
    QThread* ptrViewWorkThread;
    QThread* ptrConvertThread;
    QThread* ptrFileWorkThread;
    QThread* ptrAnalyzerThread;

    QThread* chooseFileThread;
    ///////////////////////////
    ViewWork* ptrViewWork;
    ConvertInformation* ptrConvertInfo;
    FileWork* ptrFileWork;
    AnalyzeInformation* ptrAnalyzeInfo;

    //////////////////////////

    //file's fields:
    QByteArray byteArray;
    QString outFileInfo = "";
    QString convertedInfo = "";
    QString fileName = "";
    int rowAmount;

    Ui::MainWindow *ui;
    QStandardItemModel* fileModel;
    QStandardItemModel* hexModel;
    QHeaderView *header;
};

#endif // MAINWINDOW_H
