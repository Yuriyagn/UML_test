#ifndef BAR_UI_H
#define BAR_UI_H
#include "ChiLeMe.h"
#include <QMainWindow>
#include <QTimer>

namespace Ui {
class bar_ui;
}

class bar_ui : public QMainWindow
{
    Q_OBJECT

public:
    explicit bar_ui(QWidget *parent = nullptr);
    ~bar_ui();
    QTimer *mTimer;
public:
    int time_out = 0;
    Ui::bar_ui *ui;

private slots:
    void on_pushButton_clicked();
};

#endif // BAR_UI_H
