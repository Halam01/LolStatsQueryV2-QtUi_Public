#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTranslator>
#include <fstream>
#include <iostream>
#include <QtAlgorithms>
#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QGraphicsEffect>
#include <QMap>
#include <QList>
#include <QStringBuilder>
#include <QString>
#include <QChar>
#include <QDir>
using namespace std;


//inserts trieNode into the trie
void trieInsert(trieNode *root, QString s){
    for(int i = 0; i < s.size(); i++){
        if(root->node.count(s[i]) == 0){
            root->node[s[i]] = new trieNode;
        }
        root = root->node[s[i]];
    }
    root->isLeaf = true;
}

//DFS traversal of trie, looks forward for autocompletion
void trieDFS(trieNode *root, QString seen, QList<QString> *found){
    if(root->node.count() == 0){
        //cout << ("Match: " + seen.toStdString() + "\n");
        found->append(seen);
        return;
    }
    if(root->isLeaf == true){
        //cout << ("Match: " + seen.toStdString() + "\n");
        found->append(seen);
    }
    for(auto n : root->node.keys()){
        trieDFS(root->node[n], seen+n, found);
    }
}

//search through trie for strings matching s
void trieSearch(trieNode *root, QString s, QString seen, QList<QString> *found){
    if(s.length() > 0){
        QChar k = s[0];
        s = s.remove(0,1);
        if(root->node.contains(k)){
            seen+=k;
            trieSearch(root->node[k], s, seen, found);
            return;
        }
        else{
            //cout<<("No matches.\n");
            return;
        }
    }
    else{
        if(root->isLeaf == true){
            //cout <<("Match: " + seen.toStdString() + "\n");
            found->append(seen);
            return;
        }
        for(auto n : root->node.keys()){
            trieDFS(root->node[n], seen+n, found);
        }
    }
    return;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1139,674);
    //sets overlay opacity to produce that League of Legends background style
    QGraphicsOpacityEffect *overlayOpac = new QGraphicsOpacityEffect(ui->overlayBackground);
    overlayOpac->setOpacity(0.45);
    ui->overlayBackground->setGraphicsEffect(overlayOpac);

    /*
     * code for retrieving all Champ Icons and splash images, runs webscraping python script to download art assets from Riot Games
     *
    QProcess process;
    QString scriptFile =  "C:/Users/Hanna/Documents/GitHub/LoLStatsQueryV2/queryChampImg.py";
    QStringList pythonCommandArguments = QStringList() << scriptFile;

    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start("python", pythonCommandArguments);
    process.waitForFinished(-1);

    cout << "END" << endl;

    */

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //reset UI and system state to default
    ui->dataDisplay->clear();
    ui->champImgIcon->setStyleSheet("background-color: rgba(1, 10, 19, 100); border-color: rgb(120, 90, 40); border: 1px solid rgb(120, 90, 40)");
    ui->champNAME->setStyleSheet("font: 48pt 'Narkisim'; background-color: rgba(1, 10, 19, 100); color: rgb(120, 90, 40); border-color: rgb(120, 90, 40); border: 1px solid rgb(120, 90, 40);");
    ui->champList->clear();
    champDataHash.clear();
    champTrie = new trieNode();
    ofstream clearFile (QDir::currentPath().toStdString() + "/data_parsed.txt"); //C:/Users/Hanna/Documents/GitHub/LoLStatsQueryV2/
    clearFile.close();


    //handle search, grabs input from UI and runs python script to query Riot API for player's statistics
    QString summonerName = ui->SUMMONERNAME->toPlainText();
    QString season = ui->SEASONS->currentText();

    QProcess process;
    QString scriptFile =  QDir::currentPath() + "/QueryStats.py"; //C:/Users/Hanna/Documents/GitHub/LoLStatsQueryV2/

    QStringList pythonCommandArguments = QStringList() << scriptFile
         << summonerName  <<  season;

    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start("python", pythonCommandArguments);
    process.waitForFinished(-1);

    QString output(process.readAllStandardOutput());
    cout << output.toStdString() << endl;


    //handle data parsing
    ifstream file (QDir::currentPath().toStdString() + "/data_parsed.txt"); //C:/Users/Hanna/Documents/GitHub/LoLStatsQueryV2/
    champDataHash;
    string champName, champStats;
    string line;
    if(file.is_open()){
        while(getline(file, line)){
            champName = line.substr(0, line.find(" :"));
            champName.erase(remove_if(champName.begin(), champName.end(), isspace), champName.end());
            champStats = line.substr(line.find(": ") + 1);
            champDataHash[QString::fromStdString(champName)] = QString::fromStdString(champStats);
            trieInsert(champTrie, QString::fromStdString(champName).toLower());
        }
    }
    file.close();
    QString basePath = QDir::currentPath() +  "/champImgsFull/"; //C:/Users/Hanna/Documents/GitHub/LoLStatsQueryV2/
    QString imgPath = QDir::currentPath() + "/champImgsFull/"; //C:/Users/Hanna/Documents/GitHub/LoLStatsQueryV2/
    QString fileType = ".png";
    QString c_name;
    QHashIterator<QString, QString> i(champDataHash);
    while(i.hasNext()){
        i.next();
        c_name = i.key();
        imgPath.append(c_name);
        imgPath.append(fileType);
        ui->champList->addItem(new QListWidgetItem(QIcon(imgPath), i.key()));
        imgPath = basePath;
    }

}


//updates statistical data, background image, and champ icon upon selection of a champion from the champion list
void MainWindow::on_champList_itemClicked(QListWidgetItem *item)
{
    QString name = item->text();
    QString imgPath = "background-image: url(" + QDir::currentPath() +  "/champImgsFull/"; //C:/Users/Hanna/Documents/GitHub/LoLStatsQueryV2/
    QString fileType = ".png);";
    QString splashfileType = ".jpg);";
    imgPath.append(name);
    imgPath.append(fileType);
    //ui->champImgIcon->setFrameStyle(QFrame::StyledPanel);
    ui->champImgIcon->setStyleSheet(imgPath);

    QString splashPath = "background-image: url(" + QDir::currentPath() +  "/splashImgs/"; //C:/Users/Hanna/Documents/GitHub/LoLStatsQueryV2/
    splashPath.append(name);
    splashPath.append(splashfileType);
    //ui->dataBackground->setFrameStyle(QFrame::StyledPanel);
    ui->dataBackground->setStyleSheet(splashPath);


    ui->champNAME->setText(name);
    //cout << champDataHash.value(item->text()).toStdString() << endl;
    ui->dataDisplay->setText(champDataHash.value(item->text()));



}

//calls trieSearch to filter champList for champions matching the string entered, also facilitates autocomplete to speed up search
void MainWindow::on_champSearch_textChanged()
{
    //reset on new character typed
    champSearch = new QList<QString>();
    QString currText = ui->champSearch->toPlainText().toLower();
    for(int row = 0; row < ui->champList->count(); row++){
        QListWidgetItem *item = ui->champList->item(row);
        item->setHidden(false);
    }


    //perform trie/map search, hide rows not in search result
    trieSearch(champTrie, currText, "", champSearch);
    for(int row = 0; row < ui->champList->count(); row++){
        QListWidgetItem *item = ui->champList->item(row);
        if (!(champSearch->contains(item->text().toLower()))){
            item->setHidden(true);
        }
    }

}


//code for debug purposes
void MainWindow::on_pushButton_2_clicked()
{
   /* char* memblock;
    streampos size;
    ifstream file ("C:/Users/Hanna/Documents/GitHub/LoLStatsQueryV2/data_parsed.txt", ios::in|ios::ate);
    if(file.is_open()){
        size = file.tellg();
        memblock = new char [size];
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();
        cout << size << endl;
        cout << "Read entire file into memory..." << endl;
    }
    char* splitjson = strtok(memblock, ":");
    while(splitjson){
        cout << splitjson << endl;
        splitjson = strtok(NULL, ":");
    }
    */
}


