#ifndef BOUGHTCARD_H
#define BOUGHTCARD_H

#include <QWidget>
#include "allSoure.h"

namespace Ui {
class boughtCard;
}

class boughtCard : public QWidget
{
    Q_OBJECT

public:
    explicit boughtCard(const Flight& flight,QWidget *parent = nullptr);
    ~boughtCard();
    QIcon selectImg(const QString& airline);
signals:
    void tuiTicket();
    void gaiTicket();
private:
    Ui::boughtCard *ui;
};

#endif // BOUGHTCARD_H
