#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSplitter>
#include <QTableView>
#include "pieview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    setWindowTitle(tr("汽车销售管理系统"));
    createMenuBar();
    setFixedSize(750,500);
    move(500,300);
    QSqlQueryModel *factorModel = new QSqlQueryModel(this);
    factorModel->setQuery("select name from factory");
    ui->factoycomboBox->setModel(factorModel);
    ui->chartpagecomboBox->setModel(factorModel);
    createChartModelView();
    showDailyList();
}


void MainWindow::createMenuBar()
{
    managerAction = new QAction("品牌车管理",this);
    chartAction = new QAction("销售统计",this);
    quitAction = new QAction("退出",this);
    managerAction->setShortcut(tr("Ctrl+M"));
    chartAction->setShortcut(tr("Ctrl+C"));
    quitAction->setShortcut(tr("Ctrl+Q"));
    managerMenu = menuBar()->addMenu("销售管理");
    managerMenu->addAction(managerAction);
    managerMenu->addAction(chartAction);
    managerMenu->addSeparator();
    managerMenu->addAction(quitAction);
    passwordMenu = menuBar()->addMenu("修改密码");
    QAction *passwordAction = new QAction("修改密码",this);
    passwordMenu->addAction(passwordAction);
    //建立信号和槽
    connect(managerAction,&QAction::triggered,this,&MainWindow::on_managerMenu_clicked);
    connect(chartAction,&QAction::triggered,this,&MainWindow::on_chartMenu_clicked);
    connect(quitAction,&QAction::triggered,this,&MainWindow::on_quitMenu_clicked);
    connect(passwordAction,&QAction::triggered,this,&MainWindow::on_password_clicked);
}

void MainWindow::on_managerMenu_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_chartMenu_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_quitMenu_clicked()
{
    this->close();
}

void MainWindow::on_password_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QSqlQuery query;
    query.exec("select pwd from password");
    query.next();
    //qDebug() << query.value(0).toString();
    if(arg1 != query.value(0).toString()){
        ui->errorlabel->setText("原密码错误!");
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(true);
    }
    else
    {
        ui->lineEdit_2->setEnabled(true);
        ui->errorlabel->setText("");
        ui->lineEdit_2->setFocus();
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(false);
    }
}

void MainWindow::on_pushButton_clicked()
{

    if(ui->lineEdit_2->text().isEmpty()){
        QMessageBox::warning(this,"","新密码不能为空！",QMessageBox::Ok);
    }
    else{
        QSqlQuery query;

        query.exec(QString("delete from password"));
        query.exec(QString("insert into password values('%1')").arg(ui->lineEdit_2->text()));
        if (query.lastError().isValid()) {
            qDebug() << "执行语句的时候出现错误" << query.lastError().text();
        } else {
            qDebug() << "语句执行成功";
        }
        QMessageBox::information(this,"","修改成功!",QMessageBox::Ok);
        ui->pushButton_2->setEnabled(true);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_factoycomboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "请选择厂家"){
        //进行其他部件的状态设置
        on_sellCancelBtn_clicked();
        ui->brandcomboBox->clear();
    }
    else{
        ui->brandcomboBox->setEnabled(true);
        QSqlQueryModel *brandModel = new QSqlQueryModel(this);
        brandModel->setQuery(QString("select name from brand where factory='%1'").arg(arg1));
        ui->brandcomboBox->setModel(brandModel);
        ui->sellCancelBtn->setEnabled(true);
    }
}

void MainWindow::on_sellCancelBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->brandcomboBox->clear();
   // ui->brandcomboBox->setModel(0);
}

// 当品牌下拉框项目改变时，改变报价编辑框
void MainWindow::on_brandcomboBox_currentIndexChanged(const QString &arg1)
{
    ui->sellNumspinBox->setValue(0);
    ui->sellNumspinBox->setEnabled(false);
    ui->sellSumlineEdit->clear();
    ui->sellSumlineEdit->setEnabled(false);
    ui->sellOkBtn->setEnabled(false);
    QSqlQuery query;
    //获取价格
    query.exec(QString("select price from brand where name ='%1' and factory = '%2'").arg(arg1).arg(ui->factoycomboBox->currentText()));
    query.next(); //遍历
    ui->sellPricelineEdit->setEnabled(true);
    ui->sellPricelineEdit->setReadOnly(true);
    ui->sellPricelineEdit->setText(query.value(0).toString());
    //获取数量
    query.exec(QString("select last from brand where name ='%1' and factory = '%2'").arg(arg1).arg(ui->factoycomboBox->currentText()));
    query.next();
    int num = query.value(0).toInt();
    if(num == 0)
    {
        QMessageBox::information(this,"提示","该品牌汽车已经售完！",QMessageBox::Ok);
    }
    else
    {
        ui->sellNumspinBox->setEnabled(true);
        ui->sellNumspinBox->setMaximum(num);
        ui->sellLastlabel->setText(tr("剩余数量：%1").arg(num));
        ui->sellLastlabel->setVisible(true);
    }
}

//计算金额 当Spinbox改变时
void MainWindow::on_sellNumspinBox_valueChanged(int arg1)
{
    if(arg1 == 0)
    {
        ui->sellSumlineEdit->clear();
        ui->sellSumlineEdit->setEnabled(false);
        ui->sellOkBtn->setEnabled(false);
    }
    else
    {
        ui->sellSumlineEdit->setEnabled(true);
        ui->sellSumlineEdit->setReadOnly(true);
        qreal sum = arg1 * ui->sellPricelineEdit->text().toInt(); //报价*数量 = 总价
        ui->sellSumlineEdit->setText(QString::number(sum));
        ui->sellOkBtn->setEnabled(true);
    }
}

void MainWindow::on_sellOkBtn_clicked()
{
    QString factory = ui->factoycomboBox->currentText(); //工厂
    QString name = ui->brandcomboBox->currentText(); //品牌
    int value = ui->sellNumspinBox->value(); //数量
    //sellNumSpinBox的最大值就是以前的剩余量
    int last = ui->sellNumspinBox->maximum()-value;//更新后剩余量
    //获取以前的销售量 sell 之前卖掉的数量
    QSqlQuery query;
    query.exec(QString("select sell from brand where name ='%1' and factory = '%2'").arg(name).arg(factory));
    query.next();
    int sell = query.value(0).toInt() + value; //将之前卖掉的与现在卖掉的相加，得到新的卖掉的
    //事务操作
    QSqlDatabase::database().transaction();
    bool rtn = query.exec(QString("update brand set sell=%1,last=%2 where name='%3' and factory='%4'").arg(sell).arg(last).arg(name).arg(factory));
    if(rtn)
    {
        QSqlDatabase::database().commit();
        QMessageBox::information(this,"提示","购车成功",QMessageBox::Ok);
        writeXml();
        showDailyList();
        on_sellCancelBtn_clicked();
    }
    else
    {
        QSqlDatabase::database().rollback();
    }
}


void MainWindow::createChartModelView()
{
    //创建模型
    chartModel =new QStandardItemModel(this);
    chartModel->setColumnCount(2); //设置两列（标题）
    chartModel->setHeaderData(0,Qt::Horizontal,QString("品牌")); //设置标题
    chartModel->setHeaderData(1,Qt::Horizontal,QString("销售数量"));
    QSplitter *splitter =new QSplitter(ui->chartpage); //第二页
    splitter->resize(700,320);
    splitter->move(0,80); //往下移动

    //-----------1----------------
    QTableView *table =new QTableView; //创建表格视图
    QItemSelectionModel *selectionModel =new QItemSelectionModel (chartModel);
    splitter->addWidget(table);
    splitter->setStretchFactor(0,1);
    table->setModel(chartModel);
    table->setSelectionModel(selectionModel);

    //-----------2-----------------
    PieView *pieChart =new PieView; //创建方形视图
    splitter->addWidget(pieChart);
    splitter->setStretchFactor(1,2);
    pieChart->setModel(chartModel); //这里用到的模型用于后面的遍历
    pieChart->setSelectionModel(selectionModel);
}

void MainWindow::showChart() //放入图形数据
{
    QSqlQuery query;
    query.exec(QString("select name,sell from brand where factory='%1'").arg(ui->chartpagecomboBox->currentText()));
    chartModel->removeRows(0,chartModel->rowCount(QModelIndex()),QModelIndex());
    int row = 0;
    //遍历数据
    while(query.next()){
    //创建一行三列的数据
    int r = qrand() %256;
    int g = qrand() %256;
    int b = qrand() %256;
    chartModel->insertRows(row,1,QModelIndex()); //新建一行
    chartModel->setData(chartModel->index(row,0,QModelIndex()),QColor(r,g,b),Qt::DecorationRole); //设置方框颜色
    chartModel->setData(chartModel->index(row,0,QModelIndex()),query.value(0).toString()); //品牌
    chartModel->setData(chartModel->index(row,1,QModelIndex()),query.value(1).toInt());  //数量
    row++;
    }
}



void MainWindow::on_chartpagecomboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 !="请选择厂家")
           showChart();
}




void MainWindow::on_updateBtn_clicked()
{
    if(ui->chartpagecomboBox->currentText() !="请选择厂家")
            showChart();
}


QString MainWindow::getDateTime(DateTimeType type)
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString date = datetime.toString("yyyy-MM-dd");
    QString time = datetime.toString("hh:mm");
    QString dateAndTime = datetime.toString("yyyy-MM-dd dddd hh:mm");
    if(type == Date)return date;
    else if(type == Time) return time;
    else return dateAndTime;
}

//读取 XML 文档
bool MainWindow::docRead()
{
    QFile file("data.xml");
    if(!file.open(QIODevice::ReadOnly))
            return false;
    if(!doc.setContent(&file)){
        file.close();
        return false;
    }
    file.close();
    return true;
}

//写入 XML 文档
bool MainWindow::docWrite()
{
    QFile file("data.xml");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
            return false;
    QTextStream out(&file);
    doc.save(out,4);
    file.close();
    return true;
}

//将销售车辆信息保存到 XML 文件中
void MainWindow::writeXml()
{
    //先从文件读取
    if(docRead()){
        QString currentDate = getDateTime(Date);
        QDomElement root = doc.documentElement();
        //根据是否有日期节点进行处理
        if(!root.hasChildNodes()){
            QDomElement date = doc.createElement(QString("日期"));
            QDomAttr curDate = doc.createAttribute("date");
            curDate.setValue(currentDate);
            date.setAttributeNode(curDate);
            root.appendChild(date);
            createNodes(date);
        }else{
            QDomElement date = root.lastChild().toElement();
            //根据是否已经有今天的日期节点进行处理
            if(date.attribute("date") == currentDate){
                createNodes(date);
            }else{
                QDomElement date = doc.createElement(QString("日期"));
                QDomAttr curDate = doc.createAttribute("date");
                curDate.setValue(currentDate);
                date.setAttributeNode(curDate);
                root.appendChild(date);
                createNodes(date);
            }
        }
        //写入到文件
        docWrite();
    }
}

//创建销售车辆信息的节点
void MainWindow::createNodes(QDomElement &date)
{
    QDomElement time = doc.createElement(QString("时间"));
    QDomAttr curTime = doc.createAttribute("time");
    curTime.setValue(getDateTime(Time));
    time.setAttributeNode(curTime);
    date.appendChild(time);
    QDomElement factory = doc.createElement(QString("厂家"));
    QDomElement brand = doc.createElement(QString("品牌"));
    QDomElement price = doc.createElement(QString("报价"));
    QDomElement num = doc.createElement(QString("数量"));
    QDomElement sum = doc.createElement(QString("金额"));
    QDomText text;
    text = doc.createTextNode(QString("%1").arg(ui->factoycomboBox->currentText()));
    factory.appendChild(text);
    text = doc.createTextNode(QString("%1").arg(ui->brandcomboBox->currentText()));
    brand.appendChild(text);
    text = doc.createTextNode(QString("%1").arg(ui->sellPricelineEdit->text()));
    price.appendChild(text);
    text = doc.createTextNode(QString("%1").arg(ui->sellNumspinBox->value()));
    num.appendChild(text);
    text = doc.createTextNode(QString("%1").arg(ui->sellSumlineEdit->text()));
    sum.appendChild(text);
    time.appendChild(factory);
    time.appendChild(brand);
    time.appendChild(price);
    time.appendChild(num);
    time.appendChild(sum);
}

//显示日销售清单
void MainWindow::showDailyList()
{
    ui->listWidget->clear();
    if(docRead()){
        qDebug() << "hello";
        QDomElement root = doc.documentElement();
        QString title = root.tagName();
        QListWidgetItem *titleItem = new QListWidgetItem;
        titleItem->setText(QString("-----%1-----").arg(title));
        titleItem->setTextAlignment(Qt::AlignCenter);
        ui->listWidget->addItem(titleItem);
        if(root.hasChildNodes()){
            QString currentDate = getDateTime(Date);
            QDomElement dateElement = root.lastChild().toElement();
            QString date = dateElement.attribute("date");
            if(date == currentDate){
                ui->listWidget->addItem("");
                ui->listWidget->addItem(QString("日期:%1").arg(date));
                ui->listWidget->addItem("");
                QDomNodeList children = dateElement.childNodes();
                //遍历当日销售的所有汽车
                for(int i=0;i<children.count();i++){
                    QDomNode node = children.at(i);
                    QString time = node.toElement().attribute("time");
                    QDomNodeList list = node.childNodes();
                    QString factory = list.at(0).toElement().text();
                    QString brand = list.at(1).toElement().text();
                    QString price = list.at(2).toElement().text();
                    QString num = list.at(3).toElement().text();
                    QString sum = list.at(4).toElement().text();
                    QString str = time + "出售" + brand + factory + " " + num + "辆，" + "成交价:" + price + "万，共" + sum + "万元";
                    QListWidgetItem *tempItem = new QListWidgetItem;
                    tempItem->setText("**************************");
                    tempItem->setTextAlignment(Qt::AlignCenter);
                    ui->listWidget->addItem(tempItem);
                    ui->listWidget->addItem(str);
                }
            }
        }
    }
}

















