#include "myticketpage.h"
#include "allSoure.h"
#include <QScrollArea>

MyTicketPage::MyTicketPage(QWidget* parent)
    :QWidget(parent)
{

    QVBoxLayout *layout = new QVBoxLayout(this);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { background-color: #ffffff; }");

    QWidget *scrollWidget = new QWidget();
    this->flightLabel = new QVBoxLayout(scrollWidget);
    this->flightLabel->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(scrollWidget);
    layout->addWidget(scrollArea);


    loadTicketData();

    QPushButton *returnButton = new QPushButton("返回", this);
    returnButton->setObjectName("returnButton");
    returnButton->setStyleSheet("#returnButton{"
                                    " color: rgb(255, 255, 255);"
                                    "background-color: rgb(0, 170, 255);"
                                    "border-radius: 10px;"
                                    "font-size: 16px;  "
                                "}"
                                "#returnButton:hover{"
                                    "color: rgb(255, 255, 255);"
                                    "background-color: rgb(38, 143, 255);"
                                    "border-radius: 10px;"
                                "}"
                                "#returnButton:pressed {"
                                    "background-color: rgb(38, 143, 255);"
                                    "border: 1px solid #555;  /* 按下时改变边框颜色 */"
                                    "border-radius: 10px;"
                                    "font-size: 12px;  /* 鼠标按下时，字体缩小 */"
                                "}");
    connect(returnButton, &QPushButton::clicked, [this]() {
        emit this->backToBookWindow();
        emit this->backToSearchWindow();
    });

    layout->addWidget(returnButton);

    setLayout(layout);
    setWindowTitle("我的订单");
    setWindowIcon(QIcon(":/imagSoure/sysu.ico"));
    resize(800, 600);
}

void MyTicketPage::loadTicketData(){

    // 从数据库中查询当前用户的机票信息
    QSqlDatabase db;
    if (QSqlDatabase::contains("ticket_connection")) {
        db = QSqlDatabase::database("ticket_connection");
    } else {
        db = QSqlDatabase::addDatabase("QODBC");
        db.setDatabaseName(DSN);
        db.setUserName(DB_USER);
        db.setPassword(DB_PASSWORD);
        db.setConnectOptions("CLIENT_ENCODING=UTF8");
    }

    if (!db.open()) {
        QMessageBox::critical(this, "数据库连接失败", db.lastError().text());
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT * FROM ticket_data WHERE username = ?");
    query.addBindValue(curentUsername);

    if (!query.exec()) {
        QMessageBox::critical(this, "查询失败", query.lastError().text());
        db.close();
        return;
        }

    QList<Flight> matchedFlights;
    while (query.next()) {
        Flight flight;
        //按列获取数据
        flight.departureCity = query.value(1).toString();
        flight.arrivalCity = query.value(2).toString();
        flight.airline = query.value(3).toString();
        flight.aircraftType = query.value(4).toString();
        flight.departureTime = query.value(5).toString();
        flight.arrivalTime = query.value(6).toString();
        flight.price = query.value(7).toString();
        flight.passengerName=query.value(8).toString();
        flight.passengerIdNumber=query.value(9).toString();
        flight.date=query.value(10).toString();

        matchedFlights.append(flight);
    }

    db.close();

    this->addBoughtCard(matchedFlights);
    qDebug()<<matchedFlights.length();
}

void MyTicketPage::addBoughtCard(const QList<Flight>& userflights){
    QLayoutItem* child;
    while((child = flightLabel->takeAt(0)) != nullptr){
        delete child->widget();
        delete child;
    }

    for(const Flight& flight : userflights){
        boughtCard* boughtcard = new boughtCard(flight);
        boughtcard->setAttribute(Qt::WA_StyledBackground,true);

        connect(boughtcard,&boughtCard::tuiTicket,this,[=]{
            this->cancelTicket(flight);
        });

        connect(boughtcard,&boughtCard::gaiTicket,this,[=]{
            this->hide();

            QString format = "MM月dd日";
            QDate ddate = QDate::fromString(flight.date,format);

            this->rebookWindow = new rebookPage(flight,ddate);
            //返回我的订单窗口
            connect(this->rebookWindow,&rebookPage::backToMyTicket,this,[=]{
                this->rebookWindow->hide();
                this->loadTicketData();
                this->show();
            });
            this->rebookWindow->show();
        });

        flightLabel->addWidget(boughtcard);
    }
    flightLabel->addStretch();
}

void MyTicketPage::cancelTicket(const Flight& flight){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认退票", "您确定要退掉这张票吗?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // 从数据库中删除机票信息
        QSqlDatabase db;
        db = QSqlDatabase::addDatabase("QODBC");
        db.setDatabaseName(DSN);
        db.setUserName(DB_USER);
        db.setPassword(DB_PASSWORD);

        if (!db.open()) {
            QMessageBox::critical(this, "数据库连接失败", db.lastError().text());
            return;
        }

        QSqlQuery query(db);
        query.prepare("DELETE FROM ticket_data WHERE username = ? AND departureCity = ? AND arrivalCity = ? AND departureTime = ? AND arrivalTime = ? "
                      "AND price = ? AND passengerName = ? AND passengerIdNumber = ? AND date = ?");
        query.addBindValue(curentUsername);
        query.addBindValue(flight.departureCity);
        query.addBindValue(flight.arrivalCity);
        query.addBindValue(flight.departureTime);
        query.addBindValue(flight.arrivalTime);
        query.addBindValue(flight.price);
        query.addBindValue(flight.passengerName);
        query.addBindValue(flight.passengerIdNumber);
        query.addBindValue(flight.date);

        if (!query.exec()) {
            QMessageBox::critical(this, "退票失败", query.lastError().text());
        } else {
            this->loadTicketData();
            QMessageBox::information(this, "退票成功", "您的订单已取消。");
        }

        db.close();
    }
}

QIcon MyTicketPage::selectImg(const QString& airline){
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

