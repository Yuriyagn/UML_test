#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase> //用于创建连接并打开数据库
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlQuery> //执行查询操作
#include <QObject>
#include "merchants.h"

#include <QSqlIndex>

#include <QTextStream>
#include <QStringList>
#include <QVariant>

#include <QFile>
#include <QFileDialog>
#include <QSqlTableModel>


enum Identity{Customer,Business,Rider,Admin};
static Identity identity;

//获取数据库中的数据
typedef struct
{
    QString customer;

    QString username;
    QString dish_pix;
    QString dish_id;
    QString dish_name;
    QString dish_price;
    QString dish_description;

    QString merchant_address;
    QString customer_address;
    QString dish_number;
    QString order;
    QString Date;
    QString finishDate;
    QString state;

}CDBPARAMETER;

class database : public QObject
{
    Q_OBJECT
public:

    static database *getdatabase();
    static database *getdatabase(Identity,int a = 0);
    void GetDataOfDBParameter(ParamListVec& paramListVec);
    //QSqlDatabase db;
//函数声明
    bool addRegiserUser(const QString &username,const QString &password,const  QString &phone,const QString &address);
    bool queryphone(const QString &phone);
    bool queryusername(const QString &username);
    bool querypassword(const QString &username,const QString &password);
    bool addUpdaDishes(const QString &account,const QString &dish_path,const QString &dish_ID,const QString &dish_name,const QString &dish_price,const QString &dish_description);
    bool deleteUsername(const QString &username);
signals:

public slots:

private: //构造函数私有化
    explicit database(QObject *parent = nullptr);
//    explicit database(Identity,QObject *parent = nullptr);
    static database *userDb;



};

#endif // DATABASE_H
