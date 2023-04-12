#include "bar_ui.h"
#include "ui_bar_ui.h"

bar_ui::bar_ui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bar_ui)
{
    ui->setupUi(this);
    ui->progressBar->setRange(0,100);
    ui->pushButton->hide();
    static int count = 0;
    mTimer = new QTimer(this);
    //cm = new ChiLeMe;
    connect(mTimer, &QTimer::timeout, this, [=]()
    {

        ui->progressBar->setValue(count);
         count++;
         if(count>ui->progressBar->maximum())
         {
             mTimer->stop();
             count = 0;
             time_out = 1;
             ui->pushButton->show();
             ChiLeMe* ch = new ChiLeMe;
             ch->show();
             hide();

         }
    });
    mTimer->start(30);
}

bar_ui::~bar_ui()
{
    delete ui;
}

void bar_ui::on_pushButton_clicked()
{
    ChiLeMe* ch = new ChiLeMe;
    ch->show();
    hide();
}
