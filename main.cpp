#include "mainwindow.h"

#include <QApplication>
#include "connection.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!createSqlConnection() || !creatXML()) return 0; //加载数据库
    MainWindow w;
    Dialog dlg;
    if(dlg.exec() == QDialog::Accepted){
        w.show();
        return a.exec();
    }else{
        return 0;
    }
}
