#ifndef FLIGHTCARD_H
#define FLIGHTCARD_H

#include <QWidget>
#include "allSoure.h"

namespace Ui {
class flightCard;
}

class flightCard : public QWidget
{
    Q_OBJECT

public:
    explicit flightCard(const Flight& flight,QString buttonName="",QWidget *parent = nullptr);
    ~flightCard();
    QIcon selectImg(const QString& airline);

private:
    Ui::flightCard *ui;
signals:
    void buyTicket();
    void rebookTicket();
};

#endif // FLIGHTCARD_H
