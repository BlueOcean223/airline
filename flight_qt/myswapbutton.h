#ifndef MYSWAPBUTTON_H
#define MYSWAPBUTTON_H

#include <QPushButton>
#include <QTimer>

class mySwapButton:public QPushButton
{
    Q_OBJECT
public:
    mySwapButton(QWidget* parent=nullptr);

    QString BtnImg;
    QTimer *timer;

    int min=1;
    int max=6;

    void swapChange();
signals:
   void mychange();
};

#endif // MYSWAPBUTTON_H
