#include "mainwindow.h"
#include "loginwidget.h"
#include "globals.h"
#include "tools.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gConfigModel->init();

    qApp->setAttribute(Qt::AA_UseHighDpiPixmaps);
    qApp->setStyleSheet(Tools::readerFile(":/theme/default.qss"));

    QDir::setCurrent(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));

    if (gConfigModel->getLocalRepoStatus() > Gitnoter::NoneRepo) {
        MainWindow *w = new MainWindow;
        w->show();
    }
    else {
        LoginWidget *l = new LoginWidget;
        l->show();
    }

    return a.exec();
}
