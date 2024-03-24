#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QtSql>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setFixedSize(400,300);
    setWindowTitle(tr("登陆"));
    ui->lineEdit->setFocus();
    ui->pushButton->setDefault(true);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()){
        QMessageBox::information(this,tr("请输入密码"),tr("请先输入秘密在登陆"),QMessageBox::Ok);
    ui->lineEdit->setFocus();
    }
    else{
        QSqlQuery query;
        query.exec("select pwd from password");
        query.next();
        if(query.value(0).toString() == ui->lineEdit->text()){
            QDialog::accept();
        }else{
            QMessageBox::warning(this,tr("密码错误"),tr("请输入正确的密码再登陆!"),QMessageBox::Ok);
            ui->lineEdit->clear();
            ui->lineEdit->setFocus();
        }
    }
}

void Dialog::on_pushButton_2_clicked()
{
    QDialog::reject();
}
