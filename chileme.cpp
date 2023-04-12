#include "ChiLeMe.h"
#include "ui_chileme.h"
static bool isBusiness=0;
static int lastClickRow=-1;
static int lastClickOrder=-1;
static int finishedOrder=-1;
QSqlTableModel *model;
QSqlQueryModel *model1;
QSqlDatabase my_db;
QList<QString> File_Path;
ChiLeMe::ChiLeMe(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChiLeMe)
{
    ui->setupUi(this);

    ui->pushButton_2->setShortcut(Qt::Key_Return);//设置按钮关联到Enter键
    ui->screen->hide();
    ui->viewdetails->hide();
   // show();
    ui->stackedWidget->setCurrentIndex(0);
    ui->radioButton->setChecked(1);
    ui->lineEdit->setAlignment(Qt::AlignRight);
    ui->lineEdit_2->setAlignment(Qt::AlignRight);
    ui->lineEdit_3->setAlignment(Qt::AlignRight);
    ui->lineEdit_4->setAlignment(Qt::AlignRight);
    ui->lineEdit_5->setAlignment(Qt::AlignRight);
    ui->lineEdit_6->setAlignment(Qt::AlignRight);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    Dishes_menu();
    // Connect the cellClicked signal to a slot function
    connect(ui->tableWidget_2, SIGNAL(cellClicked(int, int)), this, SLOT(onCellClicked(int, int)));
    connect(ui->dish_list, SIGNAL(cellClicked(int, int)), this, SLOT(onCellClicked2(int, int)));
    //qDebug()<<ui->radioButton_4->isChecked();
}
ChiLeMe::~ChiLeMe()
{
    delete ui;
}

void ChiLeMe::on_pushButton_clicked()//注册
{
    //qDebug()<<"nihao";
     //获取输入的用户名、密码、电话分别存入username、password、phone对象中；
    ui->stackedWidget->setCurrentIndex(1);
    if(check_identity()!=Business)
    {
        ui->label_14->hide();
        ui->lineEdit_7->hide();
    }
    else
    {
        ui->label_14->show();
        ui->lineEdit_7->show();
    }
}

void ChiLeMe::on_radioButton_4_clicked(bool checked)
{
    if(checked == true)
        {
            ui->pushButton->hide();
        }
        else
        {
            qDebug()<<"222";//取消选中
             ui->pushButton->show();
    }
}

void ChiLeMe::on_radioButton_clicked()
{
    ui->pushButton->show();
}

void ChiLeMe::on_radioButton_2_clicked()
{
    ui->pushButton->show();
}

void ChiLeMe::on_radioButton_3_clicked()
{
    ui->pushButton->show();
}

void ChiLeMe::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void ChiLeMe::on_checkBox_stateChanged(int arg1)
{
    qDebug()<<arg1;
    if(arg1==2)
    {
        ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
    }
    else if(arg1==0)
    {
        ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    }
}

void ChiLeMe::on_pushButton_4_clicked()
{
    if(ui->lineEdit_4->text()!=ui->lineEdit_5->text())
        QMessageBox::information(NULL,"注册失败","两次密码不一致",QMessageBox::Ok);
    else
    {
    QString username = ui->lineEdit_3->text();
       QString password = ui->lineEdit_4->text();
       QString phone = ui->lineEdit_6->text();
       QString address = ui->lineEdit_7->text();
       ui->lineEdit_7->setText("");
       //控制台输出测试部分
           qDebug() << "username : " << username;
           qDebug() << "password : " << password;
           qDebug() << "phone    : " << phone;
            qDebug() << "address    : " << address;
    database *userDb = database::getdatabase(check_identity());

    //判断用户名在数据库中是否存在，违规弹出注册失败弹窗
        if(userDb->queryusername(username)){
            QMessageBox::information(NULL,"注册","用户名已被注册",QMessageBox::Ok);
            return;
        }
    //判断手机号在数据库中是否存在，违规弹出注册失败弹窗
        if(userDb->queryphone(phone)){
            QMessageBox::information(NULL,"注册","手机号已被注册",QMessageBox::Ok);
            return;
        }
    //打开数据库
        bool ok = userDb->addRegiserUser(username,password,phone,address);
        if(!ok){
            QMessageBox::information(NULL,"注册","注册失败",QMessageBox::Ok);
        }else{
            QMessageBox::information(NULL,"注册","注册成功",QMessageBox::Ok);
        }
    }
}
Identity ChiLeMe::check_identity()
{
    if(ui->stackedWidget->currentIndex()!=2)
    {
        if(ui->radioButton->isChecked() )
        {
            return Customer;
        }
        if(ui->radioButton_2->isChecked() )
        {
            return Business;
        }
        if(ui->radioButton_3->isChecked() )
        {
            return Rider;
        }
        return Admin;
    }
    else{
        if( ui->radioButton_5->isChecked())
        {
            return Customer;
        }
        if( ui->radioButton_6->isChecked())
        {
            return Rider;
        }
        if( ui->radioButton_7->isChecked())
        {
            return Business;
        }
        return Admin;
    }
}

void ChiLeMe::keyPressEvent(QKeyEvent *event)
{
    if(ui->stackedWidget->currentIndex()==0)
    {
        if(event->key() == Qt::Key_Enter) {
            on_pushButton_2_clicked();
        }
    }
}

void ChiLeMe::on_pushButton_2_clicked()
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    database *userDb;
    //控制台输出测试部分
    qDebug() << "username : " << username;
    qDebug() << "password : " << password;
    switch(check_identity())
    {
    case Admin:
        userDb = database::getdatabase(Admin);
        if(ui->lineEdit->text()!="admin")
        {
            ui->label_9->setText("账户名错误！");
            break;
        }
        if(ui->lineEdit_2->text()!="123")
        {
            ui->label_9->setText("密码错误！");
            break;
        }
        ui->stackedWidget->setCurrentIndex(2);
        ui->pushButton_5->show();
        break;
    case Customer:
              currentCustomer = username;
        userDb = database::getdatabase(Customer);
           if(userDb->queryusername(username)){
               //在数据库中查询到了就跳转到主页
               if(userDb->querypassword(username,password)){
                   ui->stackedWidget->setCurrentIndex(3);
                   ui->pushButton_5->show();
                   //major->show();//跳转到major页面
                   //this->close(); //关闭登陆页面
                   QSqlDatabase db = QSqlDatabase::database("smart-home-db");

                   if(!db.isOpen())
                   {
                       qDebug()<<"db未打开";
                       return;
                   }
                   //删除所有行
                   int temp = ui->tableWidget->rowCount();
                   for(int row = 0;row < temp;row++)
                   {
                       ui->tableWidget->removeRow(0);
                   }
                   QStringList tables;
                   QString tabName,sqlString;
                   tables = db.tables(QSql::Tables);

                   //菜品表名称
                   sqlString = "select * from Business";
                   QSqlQuery q(sqlString,db);

                   //获取字段域的值集合
                   QSqlRecord rec = q.record();

                   //创建表头
                   ui->tableWidget->setColumnCount(2);
                   QStringList header;
                   header<<"商家"<<"商家地址";
                   ui->tableWidget->verticalHeader()->setHidden(true);
                   ui->tableWidget->setHorizontalHeaderLabels(header);
                   ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
                   ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
                   ui->tableWidget->setColumnWidth(0, 202);
                   ui->tableWidget->setColumnWidth(1, 606);
                   ui->tableWidget->horizontalHeader()->setFixedHeight(45);
                   ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条
                   ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  //去掉垂直滚动条
                   ui->tableWidget->horizontalHeader()->setStyleSheet(
                               "QLabel{font:10pt '宋体';color: black;};");

                   QString fieldName;

                   int available_count = 0;//表格行数

                   while(q.next())//每一行的数据
                   {
                       CDBPARAMETER* data = new CDBPARAMETER;
                       data->username = q.value(0).toString();
                       data->merchant_address = q.value(3).toString();

                       QTableWidgetItem *available_item[2];      //QTableWidget控件

                       ui->tableWidget->insertRow(available_count);
                       available_item[0]=new QTableWidgetItem(data->username);
                       available_item[1]=new QTableWidgetItem(data->merchant_address);
                       ui->tableWidget->setItem(available_count,0,available_item[0]);
                       ui->tableWidget->setItem(available_count,1,available_item[1]);
                       available_count++;
                   }
               }
               //在数据库中查询不到即弹出信息错误窗口
               else{
                   QMessageBox::information(NULL,"登录","密码错误",QMessageBox::Ok);
                   break;
               }
           }
           else{
               QMessageBox::information(NULL,"登录","用户名不存在",QMessageBox::Ok);
               break;
           }
        break;

    case Business:
        userDb = database::getdatabase(Business);
           if(userDb->queryusername(username)){
               //在数据库中查询到了就跳转到主页
               if(userDb->querypassword(username,password)){
                   merchant = new merchants(username,password);
                   ui->stackedWidget->setCurrentIndex(4);
                   ui->pushButton_5->show();

                   //删除所有行
                   int temp = ui->tableWidget_2->rowCount();
                   for(int row = 0;row < temp;row++)
                   {
                       ui->tableWidget_2->removeRow(0);
                   }

                   ui->tableWidget_2->setColumnCount(5);

                    Dishes_menu();
                   QStringList header;
                   header<<"菜品"<<"菜品ID"<<"菜品名称"<<"菜品价格"<<"菜品描述";
                   ui->tableWidget_2->verticalHeader()->setHidden(true);

                   ui->tableWidget_2->setHorizontalHeaderLabels(header);
                   ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
                   ui->tableWidget_2->setColumnWidth(0, 60);
                   ui->tableWidget_2->setColumnWidth(1,60);
                   ui->tableWidget_2->setColumnWidth(2,100);
                   ui->tableWidget_2->setColumnWidth(3,80);
                   ui->tableWidget_2->setColumnWidth(4,500);
                  //  qDebug()<<ui->tableWidget_2->columnWidth(1);
                   ui->tableWidget_2->horizontalHeader()->setFixedHeight(45);
                   ui->tableWidget_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条
                   ui->tableWidget_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  //去掉垂直滚动条
                   ui->tableWidget_2->horizontalHeader()->setStyleSheet(
                       "QLabel{font:10pt '宋体';color: black;};");

                   //major->show();//跳转到major页面
                   //this->close(); //关闭登陆页面
                   database *getmessage;
                   ParamListVec paramListVec;
                   getmessage->GetDataOfDBParameter(paramListVec);
                   for(auto i:paramListVec)
                   {
                       if(i->username!=merchant->account)
                           continue;
                        ui->tableWidget_2->setEditTriggers(QAbstractItemView::CurrentChanged);

                        int rowIndex = ui->tableWidget_2->rowCount();//当前表格的行数
                        ui->tableWidget_2->insertRow(rowIndex);//在最后一行的后面插入一行

                        QLabel *label = new QLabel("");
                        //QString str = "dish1.jpg";
                        // 获取当前工作目录的绝对路径
                        //QString absolutePath = QDir::currentPath();
                        QString str = i->Dish_pix;
                        merchant->dishes.append(str);
                        //qDebug()<<absolutePath;
                        label->setPixmap(QPixmap(str).scaled(50,40));
                        ui->tableWidget_2->setCellWidget(rowIndex, 0,label);


                        //ui->tableWidget_2->setItem(rowIndex, 0, new QTableWidgetItem("123")); //添加到界面
                        ui->tableWidget_2->setItem(rowIndex, 1, new QTableWidgetItem(i->Dish_id)); //添加到界面
                        ui->tableWidget_2->setItem(rowIndex, 2, new QTableWidgetItem(i->Dish_name)); //添加到界面
                        ui->tableWidget_2->setItem(rowIndex, 3, new QTableWidgetItem(i->Dish_price)); //添加到界面
                        ui->tableWidget_2->setItem(rowIndex, 4, new QTableWidgetItem(i->Dish_description)); //添加到界面
                   }

               }
               //在数据库中查询不到即弹出信息错误窗口
               else{
                   QMessageBox::information(NULL,"登录","密码错误",QMessageBox::Ok);
                   break;
               }
           }
           else{
               QMessageBox::information(NULL,"登录","用户名不存在",QMessageBox::Ok);
               break;
           }
        break;
    case Rider:
        userDb = database::getdatabase(Rider);
           if(userDb->queryusername(username)){
               //在数据库中查询到了就跳转到主页
               if(userDb->querypassword(username,password)){
                   rider = new Riders(username);
                   ui->stackedWidget->setCurrentIndex(5);
                   ui->pushButton_5->show();
                   //major->show();//跳转到major页面
                   //this->close(); //关闭登陆页面
                   TableFresh();
                   //model->select();
//                   for(int i = 0;i<13;i++)
//                   {
//                       if(i!=7)
//                           ui->tableView_2->hideColumn(i);
//                   }
               }
               //在数据库中查询不到即弹出信息错误窗口
               else{
                   QMessageBox::information(NULL,"登录","密码错误",QMessageBox::Ok);
                   break;
               }
           }
           else{
               QMessageBox::information(NULL,"登录","用户名不存在",QMessageBox::Ok);
               break;
           }
        break;
    }
    CreatShoppingCart();
}

void ChiLeMe::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->pushButton_5->hide();
    Dishes_menu();
    Delete_table();
}

void ChiLeMe::Dishes(){
    ui->tableWidget_2->setColumnCount(5);
    ui->tableWidget_2->setColumnWidth(1,200);
    QStringList header;
    header<<"菜品"<<"菜品ID"<<"菜品名称"<<"菜品价格"<<"菜品描述";
    ui->tableWidget_2->setHorizontalHeaderLabels(header);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setFixedHeight(45);
    ui->tableWidget_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条
    ui->tableWidget_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  //去掉垂直滚动条
    ui->tableWidget_2->horizontalHeader()->setStyleSheet(
        "QLabel{font:10pt '宋体';color: black;};");
}

void ChiLeMe::Dishes_menu()
{
    int a = ui->stackedWidget->currentIndex();
    if(a==4)
    {
        ui->pushButton_5->show();
        //ui->pushButton_6->show();
        ui->pushButton_7->show();
        ui->pushButton_8->show();
        ui->pushButton_9->show();
    }
    else
    {
        ui->pushButton_5->hide();
        //ui->pushButton_6->hide();
        ui->pushButton_7->hide();
        ui->pushButton_8->hide();
        ui->pushButton_9->hide();
    }
}

void ChiLeMe::on_pushButton_8_clicked()
{
    qDebug()<<"nihaoaoa";
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::CurrentChanged);

    int rowIndex = ui->tableWidget_2->rowCount();//当前表格的行数
    ui->tableWidget_2->insertRow(rowIndex);//在最后一行的后面插入一行

    QLabel *label = new QLabel("");
    //QString str = "dish1.jpg";
    // 获取当前工作目录的绝对路径
    QString absolutePath = QDir::currentPath();
    QString str = ":/dish1.jpg";
    merchant->dishes.append(str);
    qDebug()<<absolutePath;
    label->setPixmap(QPixmap(str).scaled(50,40));
    ui->tableWidget_2->setCellWidget(rowIndex, 0,label);


    //ui->tableWidget_2->setItem(rowIndex, 0, new QTableWidgetItem("123")); //添加到界面
    ui->tableWidget_2->setItem(rowIndex, 1, new QTableWidgetItem("123")); //添加到界面
    ui->tableWidget_2->setItem(rowIndex, 2, new QTableWidgetItem("123")); //添加到界面
    ui->tableWidget_2->setItem(rowIndex, 3, new QTableWidgetItem("123")); //添加到界面
    ui->tableWidget_2->setItem(rowIndex, 4, new QTableWidgetItem("123")); //添加到界面
    //ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void ChiLeMe::on_pushButton_9_clicked()
{
    int rowIndex = ui->tableWidget_2->currentRow();
    if (rowIndex!=-1)
    {
        ui->tableWidget_2->removeRow(rowIndex);
    }

}

void ChiLeMe::on_pushButton_7_clicked()  //商家完成编辑
{
    database *userDb = database::getdatabase(check_identity(),1);

    //先删除之前账户下的所有菜品信息
    bool ok = userDb->deleteUsername(merchant->account);
    if(!ok){
        QMessageBox::information(NULL,"更新菜单","不可更新",QMessageBox::Ok);

    }else{
        QMessageBox::information(NULL,"更新菜单","可以更新",QMessageBox::Ok);
        // 遍历每一行
        //循环读取数据
        for(int i=0;i<ui->tableWidget_2->rowCount();i++){
            for(int j=0;j<ui->tableWidget_2->columnCount();j++){
                if(ui->tableWidget_2->item(i,j)!=NULL){               //一定要先判断非空，否则会报错
                    qDebug()<<ui->tableWidget_2->item(i,j)->text();

                }
            }
            QString dish_path = merchant->dishes[i];
            QString dish_ID = ui->tableWidget_2->item(i,1)->text();
            QString dish_name = ui->tableWidget_2->item(i,2)->text();
            QString dish_price = ui->tableWidget_2->item(i,3)->text();
            QString dish_description = ui->tableWidget_2->item(i,4)->text();
            //打开数据库
                bool ok = userDb->addUpdaDishes(merchant->account,dish_path,dish_ID,dish_name,dish_price,dish_description);
                if(!ok){
                    QMessageBox::information(NULL,"更新菜单","更新失败",QMessageBox::Ok);
                }else{
                    QMessageBox::information(NULL,"更新菜单","更新成功",QMessageBox::Ok);
                }
        }
    }

}


void ChiLeMe::on_tableWidget_cellClicked(int row, int column)
{
    qDebug()<<row<<" "<<column;
    qDebug()<<" no";
}

void ChiLeMe::onCellClicked(int row, int column)
{
    // Do something when a cell is clicked
    qDebug()<<row<<" "<<column;
    if(column==0)
    {
        qDebug()<<"niubi";
        //auto widget = ui->tableWidget_2->cellWidget(row,column);

        QLabel *label = new QLabel("");
        QString filepath = QFileDialog::getOpenFileName(this,"选择jpg文件", "", "JPEG Files (*.png *.jpeg *.jpg *.bmp)");

        QFileInfo fileInfo = QFileInfo(filepath);
        QString filename = fileInfo.fileName();
        QString absolutePath = QDir::currentPath();
        QString newfilep = absolutePath + "/" +filename;
        QFile::copy(filepath, newfilep);

        qDebug()<<filename;
        if(filepath=="")
            return;
        qDebug()<<filepath;
        merchant->dishes[row] = filename;
        label->setPixmap(QPixmap(filepath).scaled(50,40));
        ui->tableWidget_2->setCellWidget(row, 0,label);

        QImage img = QImage(filepath);
        QPixmap pixmap = QPixmap::fromImage(img);
        lab = new QLabel;
        lab->setScaledContents(true);
        lab->setPixmap(pixmap);
        lab->show();
    }
    for(auto i : merchant->dishes)
    {
        qDebug()<<i;
    }
    //auto item = ui->tableWidget_2->currentItem()->text();
}

void ChiLeMe::onCellClicked2(int row, int column)
{
    // Do something when a cell is clicked
    qDebug()<<row<<" "<<column;
    if(column==0)
    {
        qDebug()<<"niubi";
        //auto widget = ui->tableWidget_2->cellWidget(row,column);
        QImage img = QImage(File_Path[row]);
        QPixmap pixmap = QPixmap::fromImage(img);
        lab = new QLabel;
        lab->setScaledContents(true);
        lab->setPixmap(pixmap);
        lab->show();
    }
//    for(auto i : merchant->dishes)
//    {
//        qDebug()<<i;
//    }
    //auto item = ui->tableWidget_2->currentItem()->text();
}

//
//连接数据库，创建表模型
void ChiLeMe::createModel(){

    if(QSqlDatabase::contains("smart-home-db")){
     my_db = QSqlDatabase::database("smart-home-db");
      //qDebug()<<"包含连接"<<endl;
    }
   else{
      //qDebug()<<"不包含连接"<<endl;
      my_db = QSqlDatabase::addDatabase("smart-home-db");
    }
    if(!my_db.open())
        qDebug()<<"打开失败";

    if(!model)
        model = new QSqlTableModel(ui->tableView,my_db);

    //model->setEditStrategy(QSqlTableModel::OnFieldChange);
}

void ChiLeMe::create_CustomerTable(){
    createModel();
    model->setTable("Customers");//数据表名
    //查询数据
    if(!model->select())
    {
        qDebug()<<"查询失败";
    }
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("username"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("phone"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("password"));
    model->setSort(0, Qt::AscendingOrder);
    model->select();
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setModel(model);
}

void ChiLeMe::create_BusinessTable(){
    createModel();
    model->setTable("Business");//数据表名
    //查询数据
    if(!model->select())
    {
        qDebug()<<"查询失败";
    }
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("username"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("phone"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("password"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("address"));
    model->setSort(0, Qt::AscendingOrder);
    model->select();
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setModel(model);
}

void ChiLeMe::create_RiderTable(){
    createModel();
    model->setTable("Rider");//数据表名
    //查询数据
    if(!model->select())
    {
        qDebug()<<"查询失败";
    }
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("username"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("phone"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("password"));
    model->setSort(0, Qt::AscendingOrder);
    model->select();
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setModel(model);
}
//管理员界面，显示顾客信息
void ChiLeMe::on_radioButton_5_clicked(){
    isBusiness=0;
    ui->address->hide();
    ui->label_19->hide();
    create_CustomerTable();
}
//管理员界面，显示骑手信息
void ChiLeMe::on_radioButton_6_clicked(){
    isBusiness=0;
     ui->address->hide();
     ui->label_19->hide();
     create_RiderTable();
}
//管理员界面，显示商家信息
void ChiLeMe::on_radioButton_7_clicked(){
     isBusiness=1;

     ui->address->show();
     ui->label_19->show();
     create_BusinessTable();
}



//获取当前点击的行
void ChiLeMe::on_tableView_clicked(const QModelIndex &index){
    lastClickRow = index.row();
    //qDebug()<<lastClickRow<<endl;
}

void ChiLeMe::on_adduser_clicked(){

    int rowCount = model->rowCount();
    if(ui->username->text().isEmpty()||ui->userphone->text().isEmpty()||ui->userpassword->text().isEmpty()
            ){
        QMessageBox::information(this, "提示", "请输入要添加的用户信息,不能为空");
        return;
    }
    if(isBusiness==1){
        if(ui->address->text().isEmpty()){
            QMessageBox::information(this, "提示", "请输入要添加的用户信息,不能为空");
            return;
        }
    }
    model->insertRow(rowCount);
    //获取新建记录上的值并设置
    QModelIndex curIndex = model->index(model->rowCount()-1,1);
    //添加新行到curRow值的位置
    int curRow = curIndex.row();
    //管理员添加用户判断用户名和手机号是否已经存在
    database *userDb = database::getdatabase(check_identity());

    //判断用户名在数据库中是否存在，违规弹出注册失败弹窗
        if(userDb->queryusername(ui->username->text())){
            QMessageBox::information(NULL,"注册","用户名已被注册",QMessageBox::Ok);
            on_cancel_clicked();
            return;
        }
    //判断手机号在数据库中是否存在，违规弹出注册失败弹窗
        if(userDb->queryphone(ui->userphone->text())){
            QMessageBox::information(NULL,"注册","手机号已被注册",QMessageBox::Ok);
            on_cancel_clicked();
            return;
        }

    model->setData(model->index(curRow, 0), ui->username->text());
    model->setData(model->index(curRow, 1), ui->userphone->text());
    model->setData(model->index(curRow, 2), ui->userpassword->text());

    if(isBusiness==1){
        model->setData(model->index(curRow, 3), ui->address->text());
    }
    if(ui->checkBox_2->isChecked())
    {
        on_save_clicked();
    }
}

void ChiLeMe::on_deleteuser_clicked(){
    //createModel();

    int rowCount = model->rowCount();

    //没有记录时，将不能删除
    if(rowCount == 0){
        QMessageBox::information(this, "提示", "没有记录");
        return;
    }
    //移除最后点击的行
    if(lastClickRow!=-1){
        int ok  =  QMessageBox::warning(this,tr("删除当前行!"),tr("你确定删除当前行吗？"), QMessageBox::Yes,QMessageBox::No);
        //如确认删除
        if(ok == QMessageBox::Yes)
        {
            model->removeRow(lastClickRow);
            QMessageBox::information(this, "删除成功", "所选信息删除成功");
            lastClickRow=-1;
            model->select();
        }
    }
    else
        QMessageBox::information(this, "提示", "请选择你要删除的信息行");
}
//保存所有修改的值
void ChiLeMe::on_save_clicked(){
    model->submitAll();
    create_CustomerTable();

}
//取消所有未保存的修改
void ChiLeMe::on_cancel_clicked(){
    model->revertAll();
}

void ChiLeMe::on_checkBox_2_clicked(bool checked){
    //设置保存，取消按钮是否可用
    ui->save->setEnabled(!checked);
    ui->cancel->setEnabled(!checked);
    //QSqlTableModel *Model=createModel();
    if(checked){
        //自动更新
        //model->submitAll();
        model->setEditStrategy(QSqlTableModel::OnFieldChange);
    }
    else{
        //手动更新
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    }
}


void ChiLeMe::on_pushButton_6_clicked(){
    ui->stackedWidget->setCurrentIndex(6);
    int rowIndex = ui->tableWidget->currentRow();
    QString Merchantname = ui->tableWidget->model()->index(rowIndex,0).data().toString();
    currentMerchant = Merchantname;
    QString Address = ui->tableWidget->model()->index(rowIndex,1).data().toString();
    currentAddress = Address;
    opendishlist(Merchantname);
}

//打开购物车
void ChiLeMe::on_openshoppingcart_clicked(){
    Cal_Total_prices();
    ui->stackedWidget->setCurrentIndex(7);
}

void ChiLeMe::on_back_clicked(){
    ui->stackedWidget->setCurrentIndex(6);
}

void ChiLeMe::opendishlist(QString Name){
    //读取菜品表
    QSqlDatabase db = QSqlDatabase::database("smart-home-db");

    if(!db.isOpen())
    {
        qDebug()<<"db未打开";
        return;
    }

    QStringList tables;
    QString tabName,sqlString;
    tables = db.tables(QSql::Tables);

    //菜品表名称
    sqlString = "select * from Merchants" ;
    QSqlQuery q(sqlString,db);
    //获取字段域的值集合
    QSqlRecord rec = q.record();

    //创建表头
    ui->dish_list->setColumnCount(6);
    QStringList header;
    header<<"菜品图片"<<"菜品id"<<"菜品名称"<<"菜品价格"<<"菜品描述"<<"购买数量";
    ui->dish_list->verticalHeader()->setHidden(true);
    ui->dish_list->setHorizontalHeaderLabels(header);
    ui->dish_list->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->dish_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->dish_list->setColumnWidth(0, 100);
    ui->dish_list->setColumnWidth(1,100);
    ui->dish_list->setColumnWidth(2,100);
    ui->dish_list->setColumnWidth(3,100);
    ui->dish_list->setColumnWidth(4,292);
    ui->dish_list->setColumnWidth(5,100);
    ui->dish_list->horizontalHeader()->setFixedHeight(45);
    ui->dish_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条
    ui->dish_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  //去掉垂直滚动条
    ui->dish_list->horizontalHeader()->setStyleSheet(
        "QLabel{font:10pt '宋体';color: black;};");

    QString fieldName;
    int available_count = 0;//表格行数
    File_Path.clear();
    while(q.next())//每一行的数据
    {
        QString namevalue = q.value(0).toString();
        if(namevalue==Name){
            CDBPARAMETER* data = new CDBPARAMETER;
            data->dish_pix = q.value(1).toString();
            qDebug()<<data->dish_pix;
            data->dish_id = q.value(2).toString();
            data->dish_name = q.value(3).toString();
            data->dish_price = q.value(4).toString();
            data->dish_description = q.value(5).toString();

            QTableWidgetItem *available_item[6];      //QTableWidget控件

            ui->dish_list->insertRow(available_count);

            QLabel *label = new QLabel("");

            QString str = data->dish_pix;
            File_Path.push_back(str);
            label->setPixmap(QPixmap(str).scaled(50,40));
            ui->dish_list->setCellWidget(available_count, 0,label);

            available_item[0]=new QTableWidgetItem(data->dish_pix);
            available_item[1]=new QTableWidgetItem(data->dish_id);
            available_item[2]=new QTableWidgetItem(data->dish_name);
            available_item[3]=new QTableWidgetItem(data->dish_price);
            available_item[4]=new QTableWidgetItem(data->dish_description);


            ui->dish_list->setItem(available_count,1,available_item[1]);
            ui->dish_list->setItem(available_count,2,available_item[2]);
            ui->dish_list->setItem(available_count,3,available_item[3]);
            ui->dish_list->setItem(available_count,4,available_item[4]);

            //添加QSpinbox控件
            QSpinBox *editor2 = new  QSpinBox;
            editor2->setMinimum(0);
            //设置最大值
            editor2->setMaximum(200);
            ui->dish_list->setCellWidget(available_count,5,editor2);

            available_count++;
        }
    }
}

void ChiLeMe::on_pushButton_10_clicked(){
    ui->pushButton_5->show();
    ui->stackedWidget->setCurrentIndex(3);
    int numoflist = ui->dish_list->rowCount();
    for(int i=0;i<numoflist;i++){
        ui->dish_list->removeRow(0);
    }
}

void ChiLeMe::on_addshoppingcart_clicked(){
    int rowIndex = ui->dish_list->currentRow();
    int numofrow = ui->shoppingcart->rowCount();
    int insertrow = numofrow;
    QTableWidgetItem *available_item[1];      //QTableWidget控件

    //先插入购买数量（判断数量是否为0）
    QSpinBox* rdish_number = (QSpinBox*)ui->dish_list->cellWidget(rowIndex,5);
    int temp = rdish_number->value();
    if(temp==0){
        QMessageBox::information(this,"提示","所选商品数量不能为空!");
        return;
    }
    else{
        ui->shoppingcart->insertRow(insertrow);
        QString dish_number = QString::number(temp,10);
        available_item[0] = new QTableWidgetItem(dish_number);
        ui->shoppingcart->setItem(insertrow,6,available_item[0]);
    }

    //插入商家名称
    available_item[0] = new QTableWidgetItem(currentMerchant);
    ui->shoppingcart->setItem(insertrow,0,available_item[0]);

    //再插入菜品属性
    for(int i=0;i<5;i++){
        QString Merchantname = ui->dish_list->model()->index(rowIndex,i).data().toString();
        available_item[0]=new QTableWidgetItem(Merchantname);
        if(i==0)
        {
            QLabel *label = new QLabel("");
              //QLabel*  label_tep = (QLabel*)ui->dish_list->cellWidget(rowIndex,0);
              //QLabel *label = label_tep;
//            QString str = Merchantname;
//            qDebug()<<Merchantname;
            label->setPixmap(QPixmap(File_Path[rowIndex]).scaled(50,40));
            //ui->dish_list->setCellWidget(available_count, 0,label);
              ui->shoppingcart->setCellWidget(insertrow,i+1,label);
        }
        else
        ui->shoppingcart->setItem(insertrow,i+1,available_item[0]);
    }

    //插入店铺地址
    available_item[0] = new QTableWidgetItem(currentAddress);
    ui->shoppingcart->setItem(insertrow,7,available_item[0]);
}

void ChiLeMe::CreatShoppingCart(){
    //创建表头
    ui->shoppingcart->setColumnCount(8);
    QStringList header;
    header<<"商家"<<"菜品图片"<<"菜品id"<<"菜品名称"<<"菜品价格"<<"菜品描述"<<"购买数量"<<"店铺地址";
    ui->shoppingcart->verticalHeader()->setHidden(true);
    ui->shoppingcart->setHorizontalHeaderLabels(header);
    ui->shoppingcart->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->shoppingcart->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->shoppingcart->setColumnWidth(0, 80);
    ui->shoppingcart->setColumnWidth(1,80);
    ui->shoppingcart->setColumnWidth(2,80);
    ui->shoppingcart->setColumnWidth(3,80);
    ui->shoppingcart->setColumnWidth(4,80);
    ui->shoppingcart->setColumnWidth(5,176);
    ui->shoppingcart->setColumnWidth(6,80);
    ui->shoppingcart->setColumnWidth(7,176);
    ui->shoppingcart->horizontalHeader()->setFixedHeight(45);
    ui->shoppingcart->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉水平滚动条
    ui->shoppingcart->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  //去掉垂直滚动条
    ui->shoppingcart->horizontalHeader()->setStyleSheet(
        "QLabel{font:10pt '宋体';color: black;};");

}

void ChiLeMe::on_submit_order_clicked(){

    QDateTime dateTime = QDateTime::currentDateTime();//获取系统当前的时间
    QString Date = dateTime .toString("yyyy-MM-dd hh:mm:ss");//格式化时间

//    orderNumber++;//订单号
//    //订单号转为QString
//    QString sorderNumber = QString::number(orderNumber, 10);

    QSqlDatabase db;
    if(QSqlDatabase::contains("smart-home-db"))
        db = QSqlDatabase::database("smart-home-db");
    else
        db = QSqlDatabase::addDatabase("smart-home-db");
    bool isOk = db.open();
    QSqlQuery query(db);
    if(!isOk){
        qDebug()<<"error info :"<<db.lastError();
    }
    else{
        bool HaveCreatedTable = false;
        QStringList tables;
        QString tabName,sqlString;
        tables = db.tables(QSql::Tables);
        for (int i = 0; i < tables.size(); ++i)//读表中记录
        {
            tabName = tables.at(i);//表名
            //qDebug()<<"table.size()为："<<tables.size()<<endl;
            if(tabName == "MyTakeawaylist2"){
                HaveCreatedTable = true;
            }
        }
        if(HaveCreatedTable == false){
            QString creatTableStr;
            creatTableStr = "CREATE TABLE MyTakeawaylist2    \
                    (                                       \
                        用户      char(50)  NOT NULL ,     \
                        商家      char(50)  NOT NULL ,     \
                        菜品id    char(20)  NOT NULL  ,    \
                        菜品名称   char(20)  NOT NULL ,     \
                        菜品价格   char(20)  NOT NULL ,     \
                        菜品描述   char(50)  NOT NULL ,     \
                        购买数量   char(20)  NOT NULL ,    \
                        订单号     char(20)  NOT NULL ,    \
                        下单时间   char(20)  NOT NULL ,    \
                        送达时间   char(20)  NOT NULL ,    \
                        订单状态   char(20)  NOT NULL ,   \
                        用户地址   char(50)  NOT NULL ,   \
                        商家地址   char(50)  NOT NULL ,   \
                        骑手       char(20)  NOT NULL  \
                        );";
            query.prepare(creatTableStr);
            if(!query.exec()){
                qDebug()<<"query error :"<<query.lastError();
            }
            else{
                qDebug()<<"creat table success!";
            }
        }

        //插入数据
        QString customer_address = ui->Customer_address->text();
        int sumofrow = ui->shoppingcart->rowCount();

        //获取不重复订单号
        int orderNumber = 0;
        query.prepare("select distinct 订单号 from MyTakeawaylist2");//获取年级，不重复
        query.exec();//运行数据库
        QSqlRecord rec =query.record();
        while(query.next())
        {
            //IndexorderNumber是获取某列的位置
            int IndexorderNumber = rec.indexOf("订单号");
            QString strorderNumber = query.value(IndexorderNumber).toString();
            int temp = strorderNumber.toInt();
            orderNumber = qMax(orderNumber,temp);
         }
        orderNumber++;
        QString sorderNumber = QString::number(orderNumber, 10);
        qDebug()<<"订单号为:"<<sorderNumber;

        for(int i=0;i<sumofrow;i++){
            QSqlQuery query2(db);
            query2.prepare("INSERT INTO MyTakeawaylist2 (用户,商家,菜品id,菜品名称,菜品价格,菜品描述,购买数量,订单号,下单时间,送达时间,订单状态,用户地址,商家地址,骑手)"
                           "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                QString username = ui->shoppingcart->model()->index(i,0).data().toString();
                //菜品图片不添加到订单
                QString dish_id = ui->shoppingcart->model()->index(i,2).data().toString();
                QString dish_name = ui->shoppingcart->model()->index(i,3).data().toString();
                QString dish_price = ui->shoppingcart->model()->index(i,4).data().toString();
                QString dish_discription = ui->shoppingcart->model()->index(i,5).data().toString();
                QString dish_number = ui->shoppingcart->model()->index(i,6).data().toString();
                QString merchantAddress = ui->shoppingcart->model()->index(i,7).data().toString();

                query2.addBindValue(currentCustomer);
                query2.addBindValue(username);
                query2.addBindValue(dish_id);
                query2.addBindValue(dish_name);
                query2.addBindValue(dish_price);
                query2.addBindValue(dish_discription);
                query2.addBindValue(dish_number);//购买数量
                query2.addBindValue(sorderNumber);//订单号
                query2.addBindValue(Date);//下单时间
                query2.addBindValue("");//送达时间为空，表为送达
                query2.addBindValue("1");//订单状态,1代表未接单
                query2.addBindValue(customer_address);//用户地址
                query2.addBindValue(merchantAddress);//商家地址
                query2.addBindValue("");//骑手
                if(!query2.exec()){
                    qDebug()<<"query error :"<<query2.lastError();
                }
                else{
                    qDebug()<<"input data success!";
                    QMessageBox::information(this,"提示","订单提交成功");
                }
        }
    }
    int cnumofrow = ui->shoppingcart->rowCount();
    int numofrow = cnumofrow;
    for(int i=0;i<numofrow;i++){
        ui->shoppingcart->removeRow(0);
    }

}

void ChiLeMe::on_outcart_clicked(){
    int rowIndex = ui->shoppingcart->currentRow();
    ui->shoppingcart->removeRow(rowIndex);
    Cal_Total_prices();
}

void ChiLeMe::Cal_Total_prices(){
    double total_prices = 0;
    int sumofrow = ui->shoppingcart->rowCount();
    for(int i=0;i<sumofrow;i++){
        QString Qsingle_price = ui->shoppingcart->model()->index(i,4).data().toString();
        QString Qsingle_number = ui->shoppingcart->model()->index(i,6).data().toString();
        double single_price = Qsingle_price.toDouble();
        double single_number = Qsingle_number.toDouble();
        total_prices+=(single_price*single_number);
    }
    QString Qtotal_prices = QString::number(total_prices,10,5);
    ui->Total_prices->setText(Qtotal_prices);
}


//骑手
void ChiLeMe::create_OrderView(){
    createModel();
    model->setTable("MyTakeawaylist2");//订单表名
    //查询数据
//    if(!model->select())
//    {
//        qDebug()<<"查询失败";
//    }
    model->setFilter("select 订单号,订单状态 from Takeawaylist \
                     where 订单状态 = 1 \
                     group by 订单号   \
                     ");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("订单号")); //订单属性
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("订单状态"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr(" "));
    model->setSort(0, Qt::AscendingOrder);
    //model->select();
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_2->setModel(model);

};

void ChiLeMe::create_OrderDelView(){
    createModel();
    model->setTable(" ");//订单详情表名
    //查询数据
    if(!model->select())
    {
        qDebug()<<"查询失败";
    }
    model->setHeaderData(0, Qt::Horizontal, QObject::tr(" ")); //订单详情属性
    model->setHeaderData(1, Qt::Horizontal, QObject::tr(" "));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr(" "));
    model->setSort(0, Qt::AscendingOrder);
    model->select();
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    ui->tableViewOrderDel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewOrderDel->setModel(model);
};

//控件

//获取点击的订单
void ChiLeMe::on_tableView_2_clicked(const QModelIndex &index){
    lastClickOrder = index.row();
};
//筛选未配送订单
void ChiLeMe::on_screen_clicked(){
    create_OrderView();
    //筛选未配送订单
    //model->setFilter("");  //筛选条件
};
//查看订单详情，跳转
void ChiLeMe::on_viewdetails_clicked(){
    int orderCount = model->rowCount();
    if(orderCount == 0){
        QMessageBox::information(this, "提示", "没有订单");
        return;
    }
    //跳转到所点击的订单详情页
    if(lastClickOrder!=-1){
          ui->stackedWidget->setCurrentIndex(7);
    }
};
//确认接单
void ChiLeMe::on_confirm_clicked(){
    if(QSqlDatabase::contains("smart-home-db")){
     my_db = QSqlDatabase::database("smart-home-db");
    }
   else{
      my_db = QSqlDatabase::addDatabase("smart-home-db");
    }
    if(!my_db.open())
        qDebug()<<"打开失败";
    QModelIndex curIndex = ui->tableView_2->currentIndex();

    //获取当前记录
    QSqlRecord curRec = model1->record(curIndex.row());

    if(curRec.isEmpty())
        return;

    //获取id
    QString id = curRec.value("订单号").toString();
    QString str = rider->account;
    qDebug()<<id;
    qDebug()<<curIndex.row();
    QSqlQuery query(my_db);
    QString sql = QString("update MyTakeawaylist2 set 订单状态='%1' ,骑手='%2' where 订单号='%3'")
            .arg("2")
            .arg(str)
            .arg(id);
    if(query.exec(sql))
    {
        qDebug()<<"修改成功";
        QMessageBox::information(NULL,"确认接单","接单成功",QMessageBox::Ok);
        TableFresh();
    }
    else
    {
        qDebug()<<"修改失败:"<<query.lastError().text();
    }


//    query.prepare("delete from students where id = :ID");
//    query.bindValue(":ID",id);

//    if(!query.exec())
//        return;
//    int orderCount = model->rowCount();
//    if(orderCount == 0){
//        QMessageBox::information(this, "提示", "没有订单");
//        return;
//    }
//    //修改orderView中订单状态为已接单static int lastClickOrder=-1;

//    if(lastClickOrder!=-1){
//         finishedOrder=lastClickOrder;
//         model->setData(model->index(finishedOrder, 0),"000"); //0改为订单状态所在列
//    }
//    model->submitAll();//提交修改
};
//配送完成
void ChiLeMe::on_finished_clicked(){


    if(QSqlDatabase::contains("smart-home-db")){
     my_db = QSqlDatabase::database("smart-home-db");
    }
   else{
      my_db = QSqlDatabase::addDatabase("smart-home-db");
    }
    if(!my_db.open())
        qDebug()<<"打开失败";
    QModelIndex curIndex = ui->tableView_2->currentIndex();

    //获取当前记录
    QSqlRecord curRec = model1->record(curIndex.row());

    if(curRec.isEmpty())
        return;

    //获取id
    QString id = curRec.value("订单号").toString();
    QString state = curRec.value("订单状态").toString();
    QString str;
    qDebug()<<id;
    qDebug()<<curIndex.row();
    //获取当前时间，赋给配送订单的完成时间
    if(state=="2"){
        QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间

        str = dateTime .toString("yyyy-MM-dd hh:mm:ss");//格式化时间
        //model->setData(model->index(finishedOrder, 0),str); //0改为配送时间所在列
        //model->submitAll();//提交修改
    }
    else {
        QMessageBox::information(this, "提示", "没有接单");
        return;
    }
    QSqlQuery query(my_db);
    QString sql = QString("update MyTakeawaylist2 set 订单状态='%1' ,送达时间='%2' where 订单号='%3'")
            .arg("3")
            .arg(str)
            .arg(id);
    if(query.exec(sql))
    {
        qDebug()<<"修改成功";
        QMessageBox::information(NULL,"确认送达","完成送达",QMessageBox::Ok);
        TableFresh();
    }
    else
    {
        qDebug()<<"修改失败:"<<query.lastError().text();
    }

};

//返回骑手主页
void ChiLeMe::on_deltailsReply_clicked(){
    ui->stackedWidget->setCurrentIndex(6);
};



void ChiLeMe::Delete_table(){
    int sumofrow = ui->dish_list->rowCount();
    int sumofrow2 = ui->shoppingcart->rowCount();
    for(int i=0;i<sumofrow;i++){
        ui->dish_list->removeRow(0);
    }
    for(int j=0;j<sumofrow2;j++){
        ui->shoppingcart->removeRow(0);
    }
    ui->Total_prices->clear();
}

void ChiLeMe::TableFresh()
{
    if(QSqlDatabase::contains("smart-home-db")){
     my_db = QSqlDatabase::database("smart-home-db");
    }
   else{
      my_db = QSqlDatabase::addDatabase("smart-home-db");
    }
    if(!my_db.open())
        qDebug()<<"打开失败";
    if(!model1)
        model1 = new QSqlQueryModel(this);

    //model->setEditStrategy(QSqlTableModel::OnFieldChange);
    // model->setTable("MyTakeawaylist2");//订单表名
    //查询数据
//    if(!model->select())
//    {
//        qDebug()<<"查询失败";
//    }
     //QSqlQuery query1;
     //query1.exec("SELECT DISTINCT 订单号 FROM MyTakeawaylist2 WHERE 订单状态 = '1'");
     QString arg1 = "1";

    model1->setQuery(tr("SELECT DISTINCT 订单号,订单状态,商家地址,用户地址,骑手 FROM MyTakeawaylist2 WHERE 订单状态 = '1' OR 订单状态 = '2'"),my_db);

    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_2->verticalHeader()->setHidden(true);
    ui->tableView_2->setModel(model1);
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);
}
