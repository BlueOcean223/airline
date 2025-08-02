#ifndef REBOOKPAGE_H
#define REBOOKPAGE_H

#include "allSoure.h"
#include "flightcard.h"
#include "dbhelper.h"
#include "flighticon.h"
#include <QScrollArea>
#include <QWidget>

class rebookPage: public QWidget
{
    Q_OBJECT
public:
    rebookPage(Flight preFlight,QDate sDate,QWidget *parent = nullptr);
    ~rebookPage();
signals:
    void backToMyTicket();
private slots:
    // 当日期按钮被点击时，更新航班数据并更新日期按钮
    void onDateButtonClicked(const QDate &date);

    void loadFlightData(const QList<Flight> &flights);

    void updateFlightData(const QString &departureCity, const QString &arrivalCity, const QString &date);

    void rebookTicket(const Flight& selectedFlight);

private:
    // 加载日期按钮的方法，接受一个基准日期
    void loadDateButtons(const QDate &baseDate);

    QVBoxLayout *layout;
    // 出发地和目的地标签
    QLabel *departureLabel;
    QLabel *arrivalLabel;
    QString selectedDate; // 选择的出发日期
    QHBoxLayout *dateButtonLayout;  // 保存日期按钮的布局

    QVBoxLayout *flightLayout;
    Flight preFlight;//存储需改签的机票信息
};

#endif // REBOOKPAGE_H
