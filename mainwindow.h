#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QProcess>
#include <QListWidgetItem>


struct trieNode{
    QMap<QChar, trieNode*> node;
    bool isLeaf;
    trieNode() {isLeaf = false;}
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_champList_itemClicked(QListWidgetItem *item);


    void on_champSearch_textChanged();

    void displayChange();

    void clearDisplay();

    void on_historyBox_activated(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QHash<QString, QString> champDataHash;
    trieNode *champTrie;
    QList<QString> *champSearch;
    QMap<QString, QHash<QString, QString>> searchHistory;

};



#endif // MAINWINDOW_H
