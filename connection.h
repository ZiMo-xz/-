#ifndef CONNECTION_H
#define CONNECTION_H

#endif // CONNECTION_H
#include <QtSql>
#include <QDebug>
#include <QtXml/qdom.h>

static bool createSqlConnection()
{


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("example.db");
    if(!db.open())
    {
        //提示出错
        qDebug() << "创建数据库失败";
        return false;
    }


    QSqlQuery query;
    //创建厂家表
    //(id varchar primary key, name varchar) 是表的列定义部分。在这里：
    //id varchar primary key 定义了一个名为"id"的列，类型为"varchar"（可变长度字符串），并且将其设置为主键（primary key），意味着该列的值在表中是唯一的。
    //name varchar 定义了另一个名为"name"的列，类型也为"varchar"。

     query.exec("create table factory(id varchar primary key,name varchar)");

    if (query.lastError().isValid()) {
        qDebug() << "Error occurred while creating table:" << query.lastError().text();
    } else {
        qDebug() << "Table created successfully";
    }
    query.exec(QString("insert into factory values('0','请选择厂家')"));
    query.exec(QString("insert into factory values('01','一汽大众')"));
    query.exec(QString("insert into factory values('02','二汽神龙')"));
    query.exec(QString("insert into factory values('03','上海大众')"));

    //创建品牌表
    query.exec("create table brand(id varchar primary key,name varchar,factory varchar,price int,sum int,sell int,last int)");
    query.exec(QString("insert into brand values('01','奥迪A6','一汽大众',36,50,10,40)"));
    query.exec(QString("insert into brand values('02','捷达','一汽大众',34,20,35,15)"));
    query.exec(QString("insert into brand values('03','宝来','一汽大众',41,80,20,60)"));
    query.exec(QString("insert into brand values('04','奔驰','一汽大众',83,40,15,25)"));
    query.exec(QString("insert into brand values('05','毕加索','二汽神龙',39,50,15,35)"));
    query.exec(QString("insert into brand values('06','富康','二汽神龙',28,60,10,50)"));
    query.exec(QString("insert into brand values('07','标准307','二汽神龙',27,70,20,50)"));
    query.exec(QString("insert into brand values('08','桑塔纳','上海大众',25,75,25,50)"));
    query.exec(QString("insert into brand values('09','帕萨特','上海大众',27,65,20,50)"));

    //创建密码表
    query.exec("create table password(pwd varchar primary key)");
    query.exec("insert into password values('123456')");
    return true;
}

static bool creatXML()
{
    QFile file("data.xml");
    if(!file.exists()) return true;
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    // 创建 QDomDocument 对象 //DOM是一种XML文档的编程接口（DomDocument,文档编程接口模型）
    QDomDocument doc;

    // 添加 XML 声明
    QDomNode xmlNode = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(xmlNode);

    // 添加标题元素
    QDomElement title = doc.createElement("日销售清单");
    //QDomText titleText = doc.createTextNode("日销售清单");
    doc.appendChild(title);
    QTextStream out(&file);
    doc.save(out,4);
    file.close();

    return true;
}

