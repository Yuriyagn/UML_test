#ifndef CHILEME_H
#define CHILEME_H
#include <QWidget>
#include <QDebug>
#include "database.h"
#include <QMessageBox>
#include <QLabel>
#include <QPixmap>
#include <QFileDialog>
#include <QKeyEvent>
#include <QFile>
#include <QResource>
#include <QSqlTableModel>
#include <QFileInfo>
#include<QDateTime>
#include <QSpinBox>
#include "Riders.h"
QT_BEGIN_NAMESPACE
namespace Ui { class ChiLeMe; }
QT_END_NAMESPACE

class ChiLeMe : public QWidget
{
    Q_OBJECT

public:
    ChiLeMe(QWidget *parent = nullptr);
    ~ChiLeMe();
    Identity check_identity();
    virtual void keyPressEvent(QKeyEvent* event);

private slots:
    void onCellClicked(int row, int column);
    void onCellClicked2(int row, int column);
    void on_pushButton_clicked();

    void on_radioButton_4_clicked(bool checked);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_pushButton_3_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_4_clicked();   //确认注册

    void on_pushButton_2_clicked();//登录

    void on_pushButton_5_clicked();//退出登录

    void Dishes();

    void Dishes_menu();
    void on_pushButton_8_clicked();//添加菜品

    void on_pushButton_9_clicked(); //删除菜品


    void on_pushButton_7_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    //
    void create_CustomerTable();

    void create_BusinessTable();

    void create_RiderTable();

    void on_radioButton_5_clicked();

    void createModel();

    void on_adduser_clicked();

    void on_save_clicked();

    void on_cancel_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_deleteuser_clicked();

    void on_checkBox_2_clicked(bool checked);

    void on_radioButton_6_clicked();

    void on_radioButton_7_clicked();

    //
    void on_pushButton_6_clicked();

    void on_openshoppingcart_clicked();

    void on_back_clicked();

    void opendishlist(QString Name);

    void on_pushButton_10_clicked();

    void on_addshoppingcart_clicked();

    void CreatShoppingCart();

    void on_submit_order_clicked();

    void on_outcart_clicked();

    void Cal_Total_prices();

    //骑手

    void create_OrderView();

    void create_OrderDelView();

    //控件

    void on_tableView_2_clicked(const QModelIndex &index);

    void on_screen_clicked();

    void on_viewdetails_clicked();

    void on_confirm_clicked();

    void on_finished_clicked();

    void on_deltailsReply_clicked();

     void Delete_table();

     void TableFresh();
private:
    Ui::ChiLeMe *ui;
    merchants* merchant;
    Riders* rider;
    QString currentMerchant;
    QString currentCustomer;
    QString currentAddress;
    QLabel* lab;

};
#endif // CHILEME_H
