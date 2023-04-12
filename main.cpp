#include "ChiLeMe.h"
#include "bar_ui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bar_ui b_ui;
    b_ui.show();

    //ChiLeMe w;
    //w.show();


    return a.exec();
}
