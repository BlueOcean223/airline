#ifndef BOOKPAGE_H
#define BOOKPAGE_H

#include <QWidget>
#include "allSoure.h"
#include "myticketpage.h"
#include "flightcard.h"
#include "dbhelper.h"
#include "flighticon.h"

// 乘客信息对话框
class Passenger : public QDialog {
    Q_OBJECT

public:
    Passenger(QWidget *parent = nullptr) : QDialog(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *nameLabel = new QLabel("姓名:", this);
        nameEdit = new QLineEdit(this);

        QLabel *idLabel = new QLabel("身份证号:", this);
        idEdit = new QLineEdit(this);

        QLabel *phLabel = new QLabel("电话号码:",this);
        phEdit = new QLineEdit(this);

        QPushButton *confirmButton = new QPushButton("确认", this);
        confirmButton->setObjectName("ConfirmButton");
        confirmButton->setStyleSheet("#ConfirmButton{"
                                         " color: rgb(255, 255, 255);"
                                         "background-color: rgb(0, 170, 255);"
                                         "border-radius: 10px;"
                                         "font-size: 16px;  "
                                     "}"
                                     "#ConfirmButton:hover{"
                                         "color: rgb(255, 255, 255);"
                                         "background-color: rgb(38, 143, 255);"
                                         "border-radius: 10px;"
                                     "}"
                                     "#ConfirmButton:pressed {"
                                         "background-color: rgb(38, 143, 255);"
                                         "border: 1px solid #555;  /* 按下时改变边框颜色 */"
                                         "border-radius: 10px;"
                                         "font-size: 12px;  /* 鼠标按下时，字体缩小 */"
                                     "}");
        connect(confirmButton, &QPushButton::clicked, this, &Passenger::confirm);

        layout->addWidget(nameLabel);
        layout->addWidget(nameEdit);

        layout->addWidget(idLabel);
        layout->addWidget(idEdit);

        layout->addWidget(phLabel);
        layout->addWidget(phEdit);

        layout->addWidget(confirmButton);

        setLayout(layout);
        setWindowTitle("乘客信息");
        resize(300, 150);
    }

    PassengerInfo getPassengerInfo() const {
        PassengerInfo info;
        info.name = nameEdit->text();
        info.idNumber = idEdit->text();
        info.phNumber=phEdit->text();
        return info;
    }

private slots:
    void confirm() {
        if (nameEdit->text().isEmpty() || idEdit->text().isEmpty() || phEdit->text().isEmpty()) {
            QMessageBox::warning(this, "输入错误", "姓名，身份证号和电话号码不能为空!");
        } else {
            accept();
        }
    }

private:
    QLineEdit *nameEdit;
    QLineEdit *idEdit;
    QLineEdit *phEdit;
};


class BookPage:public QWidget
{
    Q_OBJECT
public:
    BookPage(QDate sDate,QWidget *parent = nullptr);
    ~BookPage();
signals:
    void backToSearchPage();
private slots:
    // 当日期按钮被点击时，更新航班数据并更新日期按钮
    void onDateButtonClicked(const QDate &date);

    void loadFlightData(const QList<Flight> &flights);

    void swapDepartureAndArrival();

    void updateFlightData(const QString &departureCity, const QString &arrivalCity, const QString &date);

    void purchaseTicket(const Flight& selectedFlight);

    void openMyTicketsWindow();

private:
    // 加载日期按钮的方法，接受一个基准日期
    void loadDateButtons(const QDate &baseDate);

    QTableWidget *flightTable;
    QVBoxLayout *layout;
    MyTicketPage *myTicketsWindow;

    // 出发地和目的地标签
    QLabel *departureLabel;
    QLabel *arrivalLabel;
    QString selectedDate; // 选择的出发日期
    QHBoxLayout *dateButtonLayout;  // 保存日期按钮的布局

    QVBoxLayout *flightLayout;
};

#endif // BOOKPAGE_H
