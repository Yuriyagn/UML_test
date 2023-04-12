#ifndef MERCHANTS_H
#define MERCHANTS_H
#include <QList>
#include <QString>
typedef struct
{
    QString username;
    QString Dish_pix;
    QString Dish_id;
    QString Dish_name;
    QString Dish_price;
    QString Dish_description;
}Merchant_dishes;
typedef std::vector<Merchant_dishes*>ParamListVec;
//static ParamListVec paramListVec;
class merchants
{
public:
    merchants(){};
    merchants(QString ac,QString pa);
    QString account;  //账户
    QString password;  //密码
    QList<QString> dishes;
    //获取数据库中的数据
};

#endif // MERCHANTS_H
