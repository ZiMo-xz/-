#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QStandardItemModel>
#include <QtXml>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QMenu *managerMenu;
    QMenu *passwordMenu;
    QAction *managerAction;
    QAction *chartAction;
    QAction *quitAction;
    void on_managerMenu_clicked();
    void on_chartMenu_clicked(); //销售统计
    void on_quitMenu_clicked();
    void on_password_clicked();
    enum DateTimeType{Time,Date,DateTime};
    QString getDateTime(DateTimeType type);

private slots:


    void on_factoycomboBox_currentIndexChanged(const QString &arg1);

    void on_sellCancelBtn_clicked();

    void on_brandcomboBox_currentIndexChanged(const QString &arg1);

    void on_sellNumspinBox_valueChanged(int arg1);

    void on_sellOkBtn_clicked();

    void on_chartpagecomboBox_currentIndexChanged(const QString &arg1);

    void on_updateBtn_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    void createMenuBar(); //生成菜单栏
    QStandardItemModel *chartModel;
    void createChartModelView();
    void showChart();
    QDomDocument doc;
    bool docRead();
    bool docWrite();
    void writeXml();
    void createNodes(QDomElement &date);
    void showDailyList();
};
#endif // MAINWINDOW_H
