#ifndef MYTICKETPAGE_H
#define MYTICKETPAGE_H

#include "allSoure.h"
#include "boughtcard.h"
#include "rebookpage.h"
#include <QWidget>

class MyTicketPage:public QWidget
{
    Q_OBJECT
public:
    MyTicketPage(QWidget* parent = nullptr);
signals:
    void backToBookWindow();
    void backToSearchWindow();
private slots:
    void loadTicketData();

    void cancelTicket(const Flight& flight);
    void addBoughtCard(const QList<Flight>& userflights);
    QIcon selectImg(const QString& airline);
private:
    QTableWidget *ticketTable;
    QVBoxLayout *flightLabel;
    rebookPage* rebookWindow;
};

#endif // MYTICKETPAGE_H
