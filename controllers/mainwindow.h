#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "document.h"
#include "categorieswidget.h"
#include "notemodel.h"
#include "configtablemodel.h"

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *size);

public slots:
    void onChangeCategories();

private slots:
    void on_tableWidget_list_doubleClicked(const QModelIndex &index);

    void onSaveFile();
    void onNewFile();
    void onRemoveFile();

    void on_pushButton_deleteNote_clicked();
    void on_tableWidget_list_itemClicked(QTableWidgetItem *item);

    void on_headerView_sortIndicatorChanged(int logicalIndex, Qt::SortOrder order);

    void on_pushButton_categories_clicked();

    void on_pushButton_folder_clicked();

    void on_listWidget_categories_itemClicked(QListWidgetItem *item);

    void on_listWidget_categories_itemDoubleClicked(QListWidgetItem *item);

signals:
    void resizeChildWindow(QSize size);

private:
    Ui::MainWindow *ui;
    Document m_content;
    QMap<QString, QPushButton *> menuPushButtons;
    CategoriesWidget *categoriesWidget;
    QList<NoteTableModel *> *sidebarNoteList;
    QList<CategoriesTableModel *> m_categoriesList;

    void textChanged();
    void updatePreview();
    void menuPushButtonClicked();

    void initNotesToDatabases();
    void setDefaultNote();
    void setSidebarTable();
    void setEditText();

    bool isModified();
    void setModified(bool m);

    void setTagsdata();
    void setMainWindowData();
};

#endif // MAINWINDOW_H
