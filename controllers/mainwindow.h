#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "categorylistwidget.h"
#include "notemodellist.h"
#include "configmodel.h"
#include "settingdialog.h"
#include "aboutdialog.h"

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QListWidgetItem>
#include <QTimer>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

signals:
    void noteListItemClicked(QListWidgetItem *item);

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_splitter_splitterMoved(int pos, int index);

    void on_pushButton_noteAdd_clicked();

    void on_action_saveNote_triggered();

    void on_action_deleteNote_triggered();

private:
    Ui::MainWindow *ui;


private:
    void setupUi();
    void setNoteList();
    void setCategoryList();
    void setTagList();
    void setOpenedNoteModel();
};

#endif // MAINWINDOW_H
