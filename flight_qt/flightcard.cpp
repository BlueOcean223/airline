#include "flightcard.h"
#include "ui_flightcard.h"

flightCard::flightCard(const Flight& flight,QString buttonName,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::flightCard)
{
    this->setFixedHeight(100);
    ui->setupUi(this);

    QIcon icon = this->selectImg(flight.airline);
    ui->iconLabel->setPixmap(icon.pixmap(100,100));

    ui->airlineLabel->setText(QString("%1 | %2").arg(flight.airline).arg(flight.aircraftType));
    ui->departimeLabel->setText(flight.departureTime);
    ui->arrivalLabel->setText(flight.arrivalTime);
    ui->price->setText(QString("￥%1").arg(flight.price));

    if(buttonName.length()==0){
        connect(ui->buyPushButton,&QPushButton::clicked,this,[=]{
            emit this->buyTicket();
        });
    }else{
        ui->buyPushButton->setText(buttonName);
        connect(ui->buyPushButton,&QPushButton::clicked,this,[=]{
            emit this->rebookTicket();
        });
    }
}

flightCard::~flightCard()
{
    delete ui;
}

QIcon flightCard::selectImg(const QString& airline){
    QIcon icon;

    if (airline == "上海航空") icon = QIcon(":/imagSoure/shanghai.png");
    else if (airline == "东方航空") icon = QIcon(":/imagSoure/dongfang.png");
    else if (airline == "东海航空") icon = QIcon(":/imagSoure/donghai.png");
    else if (airline == "中国国航") icon = QIcon(":/imagSoure/guohang.png");
    else if (airline == "中国联合航空") icon = QIcon(":/imagSoure/zhongguolianhe.png");
    else if (airline == "华夏航空") icon = QIcon(":/imagSoure/huaxia.png");
    else if (airline == "南方航空") icon = QIcon(":/imagSoure/nanfang.png");
    else if (airline == "厦门航空") icon = QIcon(":/imagSoure/xiamen.png");
    else if (airline == "吉祥航空") icon = QIcon(":/imagSoure/jixiang.png");
    else if (airline == "四川航空") icon = QIcon(":/imagSoure/sichuan.png");
    else if (airline == "成都航空") icon = QIcon(":/imagSoure/chengdu.png");
    else if (airline == "昆明航空") icon = QIcon(":/imagSoure/kunming.png");
    else if (airline == "春秋航空") icon = QIcon(":/imagSoure/chunqiu.png");
    else if (airline == "河北航空") icon = QIcon(":/imagSoure/hebei.png");
    else if (airline == "海南航空") icon = QIcon(":/imagSoure/hainan.png");
    else if (airline == "深圳航空") icon = QIcon(":/imagSoure/shenzhen.png");
    else if (airline == "西藏航空") icon = QIcon(":/imagSoure/xizang.png");
    else if (airline == "长龙航空") icon = QIcon(":/imagSoure/changlong.png");
    else icon = QIcon(":/imagSoure/haihangqixia.png");

    return icon;
}
