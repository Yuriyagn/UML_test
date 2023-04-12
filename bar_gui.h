#ifndef BAR_GUI_H
#define BAR_GUI_H

#include <QWidget>
#include <QTimer>
namespace Ui {
class bar_gui;
}

class bar_gui : public QWidget
{
    Q_OBJECT

public:
    explicit bar_gui(QWidget *parent = nullptr);
    ~bar_gui();
    QTimer *mTimer;
public:
    Ui::bar_gui *ui;
};

#endif // BAR_GUI_H
