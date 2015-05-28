#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

QMap<QString,QString> makeFileMap(const QString& fileName){

    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QDir dir("../../shortcut_qt_2");
    QString actualFileName = dir.absoluteFilePath(fileName);

    QMap<QString, QString> stringMap;
    QFile inputFile(actualFileName);
    if (inputFile.open(QIODevice::ReadOnly)){
       QTextStream in(&inputFile);
       QString allLines = in.readAll();
       QStringList lineList = allLines.split("\r\n");
       QStringList stringPair;
       for(const auto& line : lineList ){
           stringPair = line.split(',');
           stringMap[stringPair[0]] = stringPair[1];
//           qDebug() << "line: " << line;
//           qDebug() << "stringPair[1]: " << stringPair[1];
//           qDebug() << "mapValue: " << stringMap.value(stringPair[0]);
       }
       inputFile.close();
    }
    return stringMap;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString fileName = "shortcut_list.txt";
    QMap<QString,QString> stringMap = makeFileMap(fileName);
    auto begin = stringMap.constBegin();
    auto end = stringMap.constEnd();
    for(auto mapIt = begin; mapIt != end; ++mapIt){
        new QListWidgetItem(mapIt.key(), ui->listWidget);
    }
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushButton_2_clicked(){
    QApplication::quit();
}

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index){
    QString fileName = "shortcut_list.txt";
    QMap<QString,QString> stringMap = makeFileMap(fileName);
    QString selectedItemKey = ui->listWidget->currentItem()->text();
    QString selectedPath = stringMap.value(selectedItemKey);
    const char* cStrPath = selectedPath.toStdString().c_str();
//    qDebug() << "selectedItemKey: " << selectedItemKey;
//    qDebug() << "selectedPath: " << selectedPath;
    ShellExecuteA(NULL, NULL, cStrPath, NULL, NULL, SW_MINIMIZE);
}
