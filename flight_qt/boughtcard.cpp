#include "boughtcard.h"
#include "ui_boughtcard.h"

boughtCard::boughtCard(const Flight& flight,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::boughtCard)
{
    this->setFixedHeight(140);
    ui->setupUi(this);

    QIcon icon = flightIcon::getFlightIcon(flight.airline);
    ui->iconLabel->setPixmap(icon.pixmap(90,90));

    ui->airlineLabel->setText(QString("%1 | %2").arg(flight.airline).arg(flight.aircraftType));
    ui->arrivalLabel->setText(flight.arrivalTime);
    ui->departimeLabel->setText(flight.departureTime);
    ui->dataLabel->setText(flight.date);
    ui->deparcityLabel->setText(QString("%1 ——> %2").arg(flight.departureCity).arg(flight.arrivalCity));
    ui->price->setText(QString("￥%1").arg(flight.price));
    ui->usernameLabel->setText(QString("姓名: %1").arg(flight.passengerName));

    connect(ui->tuipushButton,&QPushButton::clicked,this,[=]{
        emit this->tuiTicket();
    });

    connect(ui->gaipushButton,&QPushButton::clicked,this,[=]{
        emit this->gaiTicket();
    });
}

boughtCard::~boughtCard()
{
    delete ui;
}
