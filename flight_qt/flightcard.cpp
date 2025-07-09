#include "flightcard.h"
#include "ui_flightcard.h"

flightCard::flightCard(const Flight& flight,QString buttonName,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::flightCard)
{
    this->setFixedHeight(100);
    ui->setupUi(this);

    QIcon icon = flightIcon::getFlightIcon(flight.airline);
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

