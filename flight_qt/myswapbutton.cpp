#include "myswapbutton.h"
#include <QPixmap>

mySwapButton::mySwapButton(QWidget* parent)
    :QPushButton(parent)
{
    BtnImg=":/imagSoure/swap1.png";
    timer = new QTimer(this);

    QPixmap pix;
    bool bet=pix.load(BtnImg);
    if(!bet){
        qDebug()<<"文件载入失败！";
        return;
    }

    //设置按键大小
    this->setFixedSize(pix.width(),pix.height());

    //设置不规则图片的边框
    this->setStyleSheet("QPushButton{border:0px;}");//调整其边框为零像素

    //设置图标
    this->setIcon(pix);

    //设置图标大小
    this->setIconSize(QSize(pix.width(),pix.height()));


    connect(this->timer,&QTimer::timeout,[=](){
        //qDebug()<<"切换";
        QString str=QString(":/imagSoure/swap%1.png").arg(this->min++);
        QPixmap pix=QPixmap(str);

        //设置按键大小
        this->setFixedSize(pix.width(),pix.height());
        //去除图片边框
        this->setStyleSheet("QPushButton{border:0px;}");
        //设置图标
        this->setIcon(QIcon(pix));
        //设置图标大小
        this->setIconSize(QSize(pix.width(),pix.height()));

        if(this->min > this->max){
            this->min=1;
            timer->stop();
        }
    });
}

void mySwapButton::swapChange(){
    timer->start(50);
}
