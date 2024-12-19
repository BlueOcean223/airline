#ifndef SEARCHPAGE_H
#define SEARCHPAGE_H

#include <QWidget>
#include "allSoure.h"
#include "bookpage.h"
#include "myticketpage.h"


namespace Ui {
class searchPage;
}



class searchPage : public QWidget
{
    Q_OBJECT

public:
    explicit searchPage(QWidget *parent = nullptr);
    ~searchPage();

    void backLogin();
    void searchTicket();
    void showBookWindow();
    void showMyTicketWindow();
    void comeBack();
signals:
    void goToLogin();
private:
    Ui::searchPage *ui;
    BookPage* bookWindow;
    MyTicketPage* myTicketWindow;
};

#endif // SEARCHPAGE_H
