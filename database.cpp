#include "database.h"

database* database::userDb = NULL;
database *database::getdatabase()
{
    if(!userDb){
        userDb = new database;
    } //new一个构造函数

    return userDb;
}
database *database::getdatabase(Identity ident,int a)
{

    identity = ident;
    if(!userDb){
        userDb = new database;
    } //new一个构造函数
    QSqlDatabase db;
    if(QSqlDatabase::contains("smart-home-db"))
      db = QSqlDatabase::database("smart-home-db");
    else
      db = QSqlDatabase::addDatabase("smart-home-db");
    bool isOk = db.open();
        if(!isOk){
            qDebug()<<"error info :"<<db.lastError();
        }
        else{
            QSqlQuery query(db);
            QString creatTableStr;
            switch(identity)
            {
            case Customer:
                qDebug()<<"0";
                creatTableStr = "CREATE TABLE Customers \
                        (                                       \
                            username    char(50)  NOT NULL ,     \
                            phone       char(24)  NOT NULL ,     \
                            password    char(20) NOT NULL      \
                        );";
                break;
            case Business:
                qDebug()<<"1";
                if(a==1)
                {
                    qDebug()<<"a1";
                    creatTableStr = "CREATE TABLE Merchants \
                            (                                       \
                                username    char(50)  NOT NULL ,     \
                                dish_pix       char(50)  NOT NULL ,     \
                                dish_id    char(20) NOT NULL,      \
                                dish_name  char(20) NOT NULL,       \
                                dish_price char(20) NOT NULL,    \
                                dish_description char(50) NOT NULL   \
                            );";
                    break;
                }
                creatTableStr = "CREATE TABLE Business \
                        (                                       \
                            username    char(50)  NOT NULL ,     \
                            phone       char(24)  NOT NULL ,     \
                            password    char(20) NOT NULL  ,    \
                            address     char(50)  NOT NULL      \
                        );";
                break;
            case Rider:
                qDebug()<<"2";
                creatTableStr = "CREATE TABLE Rider \
                        (                                       \
                            username    char(50)  NOT NULL ,     \
                            phone       char(24)  NOT NULL ,     \
                            password    char(20) NOT NULL      \
                        );";
                break;
            }



            query.prepare(creatTableStr);
            if(!query.exec()){
                qDebug()<<"query error :"<<query.lastError();
                qDebug()<<"nihao";
            }
            else{
                qDebug()<<"creat table success!";
            }
        }

    return userDb;
}
//注册功能
bool database::addRegiserUser(const QString &username, const QString &password, const QString &phone,const QString &address)
{
    //调用QSqlDatabase的静态函数databas创建了一个名为smart-home-db的连接
    QSqlDatabase db;
    if(QSqlDatabase::contains("smart-home-db"))
      db = QSqlDatabase::database("smart-home-db");
    else
      db = QSqlDatabase::addDatabase("smart-home-db");
   // QSqlDatabase db = QSqlDatabase::database("smart-home-db");

    //创建一个关联的QSqlQuery对象，由QSqlQuery来实现以下工作
    QSqlQuery  query(db);

    //将输入username、password、phone 的数据插入表中；
    switch(identity)
    {
    case Customer:
        query.prepare("INSERT INTO Customers (username,password,phone) "
                      "VALUES (?, ?, ?)");
        query.addBindValue(username);
        query.addBindValue(password);
        query.addBindValue(phone);
        break;
    case Business:
        query.prepare("INSERT INTO Business (username,password,phone,address) "
                      "VALUES (?, ?, ?, ?)");
        query.addBindValue(username);
        query.addBindValue(password);
        query.addBindValue(phone);
        query.addBindValue(address);
        break;
    case Rider:
        query.prepare("INSERT INTO Rider (username,password,phone) "
                      "VALUES (?, ?, ?)");
        query.addBindValue(username);
        query.addBindValue(password);
        query.addBindValue(phone);
        break;
    }




    bool ok = query.exec(); //判断打开数据库是否成功；
    if(!ok){
        qDebug() << "Fail add regiser user : " << query.lastError().text();
    }

    return ok;
}


//登陆的电话检测
bool database::database::queryphone(const QString &phone)
{
    QSqlDatabase db = QSqlDatabase::database("smart-home-db");
    QSqlQuery query(db);

    //判断数据库中是否存在所输入号码
    switch(identity)
    {
    case Customer:
        query.prepare("select *from Customers where username=?");
        break;
    case Business:
        query.prepare("select *from Business where username=?");
        break;
    case Rider:
        query.prepare("select *from Rider where username=?");
        break;
    }
    query.addBindValue(phone);
    bool ok = query.exec();
    if(!ok){
        qDebug() << "Fail query register phone" << db.lastError().text();
        return false;
    }
    if(query.next()){
        return true;
    }
    else{
        return false;
    }
}

//登陆的用户名检测
bool database::database::queryusername(const QString &username)
{
    QSqlDatabase db = QSqlDatabase::database("smart-home-db");
    QSqlQuery query(db);
    switch(identity)
    {
    case Customer:
        query.prepare("select *from Customers where username=?");
        break;
    case Business:
        query.prepare("select *from Business where username=?");
        break;
    case Rider:
        query.prepare("select *from Rider where username=?");
        break;
    }


    query.addBindValue(username);
    bool ok = query.exec();
    if(!ok){
        qDebug() << "Fail query register username" << db.lastError().text();
        return false;
    }
    if(query.next()){
        return true;
    }
    else{
        return false;
    }
}


//登陆的密码检测
bool database::querypassword(const QString &username,const QString &password)
{
    QSqlDatabase db = QSqlDatabase::database("smart-home-db");

    QSqlQuery query(db);
    switch(identity)
    {
    case Customer:
        query.prepare("select *from Customers where username=? and password=?");
        break;
    case Business:
        query.prepare("select *from Business where username=? and password=?");
        break;
    case Rider:
        query.prepare("select *from Rider where username=? and password=?");
        break;
    }
    query.addBindValue(username);
    query.addBindValue(password);
    bool ok = query.exec();
    if(!ok){
        qDebug() << "Fail query register password" << db.lastError().text();
        return false;
    }
    if(query.next()){

        return true;
    }
    else{
        return false;
    }

}

bool database::addUpdaDishes(const QString &account,const QString &dish_path, const QString &dish_ID, const QString &dish_name, const QString &dish_price, const QString &dish_description)
{
    //调用QSqlDatabase的静态函数databas创建了一个名为smart-home-db的连接
    QSqlDatabase db;
    if(QSqlDatabase::contains("smart-home-db"))
      db = QSqlDatabase::database("smart-home-db");
    else
      db = QSqlDatabase::addDatabase("smart-home-db");
   // QSqlDatabase db = QSqlDatabase::database("smart-home-db");

    //创建一个关联的QSqlQuery对象，由QSqlQuery来实现以下工作
    QSqlQuery  query(db);

    query.prepare("INSERT INTO Merchants (username,dish_pix,dish_id,dish_name,dish_price,dish_description) "
                  "VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(account);
    query.addBindValue(dish_path);
    query.addBindValue(dish_ID);
    query.addBindValue(dish_name);
    query.addBindValue(dish_price);
    query.addBindValue(dish_description);
    bool ok = query.exec(); //判断打开数据库是否成功；
    if(!ok){
        qDebug() << "Fail add regiser user : " << query.lastError().text();
    }

    return ok;
}

bool database::deleteUsername(const QString &username)
{
    //调用QSqlDatabase的静态函数databas创建了一个名为smart-home-db的连接
    QSqlDatabase db;
    if(QSqlDatabase::contains("smart-home-db"))
      db = QSqlDatabase::database("smart-home-db");
    else
      db = QSqlDatabase::addDatabase("smart-home-db");
   // QSqlDatabase db = QSqlDatabase::database("smart-home-db");

    //创建一个关联的QSqlQuery对象，由QSqlQuery来实现以下工作
    QSqlQuery  query(db);
    query.prepare("DELETE FROM Merchants WHERE username=:username");
    query.bindValue(":username", username);

    bool ok = query.exec(); //判断打开数据库是否成功；
    if(!ok){
        qDebug() << "Fail add regiser user : " << query.lastError().text();
    }

    return ok;
}



database::database(QObject *parent) : QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","smart-home-db");
        db.setDatabaseName("chileme.db"); //这里我的路径是/Users/dongjiahuizi/Desktop/dong.db
        bool isOk = db.open();
            if(!isOk){
                qDebug()<<"error info :"<<db.lastError();
            }
            else{

            }

}



void database::GetDataOfDBParameter(ParamListVec& paramListVec)
{

    //QSqlDatabase db = this->GetDatabase();
    QSqlDatabase db = QSqlDatabase::database("smart-home-db");

    if(!db.isOpen())
    {
        qDebug()<<"db未打开";
        return;
    }

    QStringList tables;
    QString tabName,sqlString;
    tables = db.tables(QSql::Tables);

    paramListVec.clear();

    sqlString = "select * from Merchants" ;
    QSqlQuery q(sqlString,db);
    //获取字段域的值集合
    QSqlRecord rec = q.record();
    int fieldCount = rec.count();//表列数

    QString fieldName;
    for(int j=0;j<fieldCount;j++)//列的名字
    {
        fieldName = rec.fieldName(j);
        qDebug()<<"第"<<j<<"列的列名为:"<<fieldName;
    }

    while(q.next())//每一行的数据
    {
        QString qvalue = q.value(0).toString();
        Merchant_dishes* data = new Merchant_dishes;
        data->username = q.value(0).toString();
        data->Dish_pix = q.value(1).toString();
        data->Dish_id = q.value(2).toString();
        data->Dish_name = q.value(3).toString();
        data->Dish_price = q.value(4).toString();
        data->Dish_description = q.value(5).toString();
//        qDebug()<<"data->username="<<data->username;
//        qDebug()<<"data->dish_pix="<<data->Dish_pix;
//        qDebug()<<"data->dish_id="<<data->Dish_id;
//        qDebug()<<"data->dish_name="<<data->Dish_name;
//        qDebug()<<"data->dish_price="<<data->Dish_price;
//        qDebug()<<"data->dish_description="<<data->Dish_description;
        paramListVec.push_back(data);
    }
//    for(auto i:paramListVec)
//    {
//        qDebug()<<i->username;
//        qDebug()<<i->Dish_pix;
//        qDebug()<<i->Dish_id;
//        qDebug()<<i->Dish_name;
//        qDebug()<<i->Dish_price;
//        qDebug()<<i->Dish_description;
//    }

    return;
}

