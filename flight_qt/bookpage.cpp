#include "bookpage.h"
#include "myswapbutton.h"
#include <QScrollArea>
#include <QElapsedTimer>

BookPage::BookPage(QDate sDate,QWidget *parent)
    : QWidget(parent) {
    layout = new QVBoxLayout(this);

    // 出发地和目的地布局
    QHBoxLayout *departureArrivalLayout = new QHBoxLayout();

    departureLabel = new QLabel(QString("出发地: %1").arg(flights[0].departureCity), this);
    arrivalLabel = new QLabel(QString("目的地: %1").arg(flights[0].arrivalCity), this);

    mySwapButton *swapButton = new mySwapButton();
    connect(swapButton, &QPushButton::clicked, this, [=]() {
        swapButton->swapChange();
        this->swapDepartureAndArrival();
    });

    departureArrivalLayout->addWidget(departureLabel);
    departureArrivalLayout->addWidget(swapButton);
    departureArrivalLayout->addWidget(arrivalLabel);
    departureArrivalLayout->setAlignment(Qt::AlignCenter);

    layout->addLayout(departureArrivalLayout);

    // 日期按钮区域
    dateButtonLayout = new QHBoxLayout();
    layout->addLayout(dateButtonLayout);

    this->selectedDate = sDate.toString("MM月dd日");
    // 初始加载日期按钮
    QDate baseDate = sDate;
    loadDateButtons(baseDate);

    // 滚动区域：显示卡片布局
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea {  background-color: #ffffff; }");

    QWidget *scrollWidget = new QWidget();
    flightLayout = new QVBoxLayout(scrollWidget);
    flightLayout->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(scrollWidget);
    layout->addWidget(scrollArea);

    // 返回按钮和我的订单按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *returnButton = new QPushButton("返回", this);
    QPushButton *myTicketsButton = new QPushButton("我的订单", this);

    // 按钮样式
    returnButton->setStyleSheet("QPushButton{"
                                " color: rgb(255, 255, 255);"
                                "background-color: rgb(0, 170, 255);"
                                "border-radius: 10px;"
                                "font-size: 16px;  "
                                "height:24px;"
                                "}"
                                "QPushButton:hover{"
                                "color: rgb(255, 255, 255);"
                                "background-color: rgb(38, 143, 255);"
                                "border-radius: 10px;"
                                "height:24px;"
                                "}"
                                "QPushButton:pressed {"
                                "background-color: rgb(38, 143, 255);"
                                "border: 1px solid #555;  /* 按下时改变边框颜色 */"
                                "border-radius: 10px;"
                                "font-size: 12px;  /* 鼠标按下时，字体缩小 */"
                                "height:24px;"
                                "}");
    myTicketsButton->setStyleSheet("QPushButton{"
                                   " color: rgb(255, 255, 255);"
                                   "background-color: rgb(0, 170, 255);"
                                   "border-radius: 10px;"
                                   "font-size: 16px;  "
                                   "height:24px;"
                                   "}"
                                   "QPushButton:hover{"
                                   "color: rgb(255, 255, 255);"
                                   "background-color: rgb(38, 143, 255);"
                                   "border-radius: 10px;"
                                   "height:24px;"
                                   "}"
                                   "QPushButton:pressed {"
                                   "background-color: rgb(38, 143, 255);"
                                   "border: 1px solid #555;  /* 按下时改变边框颜色 */"
                                   "border-radius: 10px;"
                                   "font-size: 12px;  /* 鼠标按下时，字体缩小 */"
                                   "height:24px;"
                                   "}");

    connect(returnButton, &QPushButton::clicked, [=]() {
        this->hide();
        emit this->backToSearchPage();
    });

    connect(myTicketsButton, &QPushButton::clicked, this, [=]{
        this->hide();
        this->openMyTicketsWindow();
    });

    buttonLayout->addWidget(returnButton);
    buttonLayout->addWidget(myTicketsButton);
    layout->addLayout(buttonLayout);

    loadFlightData(flights);

    setLayout(layout);
    setWindowTitle("航班购票");
    setWindowIcon(QIcon(":/imagSoure/sysu.ico"));
    resize(800, 600);
}

BookPage::~BookPage(){}

// 当日期按钮被点击时，更新航班数据并更新日期按钮
void BookPage::onDateButtonClicked(const QDate &date){
    selectedDate = date.toString("MM月dd日");  // 更新选定的日期
    QString departureCity = departureLabel->text().split(": ")[1];
    QString arrivalCity = arrivalLabel->text().split(": ")[1];

    // 更新航班数据
    updateFlightData(departureCity, arrivalCity, selectedDate);

    // 重新加载基于所选日期的按钮
    loadDateButtons(date);
}

//加载航班数据
void BookPage::loadFlightData(const QList<Flight>& flights) {
    // 清理旧的航班布局
    QLayoutItem* child;
    while ((child = flightLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // 遍历航班数据，生成每个航班卡片
    for (const Flight& flight : flights) {
        flightCard* flightcard = new flightCard(flight);
        flightcard->setAttribute(Qt::WA_StyledBackground, true);

        connect(flightcard,&flightCard::buyTicket,this,[=]{
            this->purchaseTicket(flight);
        });

        flightLayout->addWidget(flightcard);
        //qDebug()<<flightcard->styleSheet();
    }

    //  确保卡片居中且整齐
    flightLayout->addStretch();
}


//交换出发地与目的地
void BookPage::swapDepartureAndArrival(){
    // 检查文本内容是否合法
    if (departureLabel->text().contains(": ") && arrivalLabel->text().contains(": ")) {
        // 获取当前的出发地和目的地
        QString departureText = departureLabel->text().split(": ")[1];
        QString arrivalText = arrivalLabel->text().split(": ")[1];

        // 交换文本
        departureLabel->setText(QString("出发地: %1").arg(arrivalText));
        arrivalLabel->setText(QString("目的地: %1").arg(departureText));

        // 清理旧的航班布局
        QLayoutItem* child;
        while ((child = flightLayout->takeAt(0)) != nullptr) {
            delete child->widget();  // 删除子控件
            delete child;
        }

        // 更新航班信息，保留选定的日期
        QString updatedDepartureCity = arrivalText;
        QString updatedArrivalCity = departureText;

        // 调用更新航班数据的方法
        updateFlightData(updatedDepartureCity, updatedArrivalCity, selectedDate);
    } else {
        QMessageBox::warning(this, "错误", "出发地或目的地数据格式不正确，无法交换。");
    }
}

//更新航班数据
void BookPage::updateFlightData(const QString &departureCity, const QString &arrivalCity, const QString &date){
    QSqlDatabase db;
    if (QSqlDatabase::contains("flight_connection")) {
        db = QSqlDatabase::database("flight_connection");
    } else {
        db = QSqlDatabase::addDatabase("QODBC", "flight_connection");
        db.setDatabaseName(DSN);
        db.setUserName(DB_USER);
        db.setPassword(DB_PASSWORD);
        db.setConnectOptions("CLIENT_ENCODING=UTF8");
    }

    if (!db.open()) {
        QMessageBox::critical(this, "数据库连接失败", db.lastError().text());
        return;
    } else {
        qDebug() << " 连接数据库成功 ";
    }

    QList<Flight> matchedFlights;
    QSqlQuery query(db);
    query.exec("SET NAMES 'utf8mb4'");
    query.prepare("SELECT 出发城市, 到达城市, 航空公司, 客机机型, 出发时间, 抵达时间, 机票价格 FROM flight_data WHERE 出发城市 = ? AND 到达城市 = ? AND 出发日期 = ?");
    query.addBindValue(departureCity);
    query.addBindValue(arrivalCity);
    query.addBindValue(date);

    QElapsedTimer timer;
    timer.start();

    if (!query.exec()) {
        QMessageBox::critical(this, "查询失败", query.lastError().text());
        db.close();
        return;
    }
    qDebug() << "查询耗时: " << timer.elapsed() << " 毫秒";

    while (query.next()) {
        Flight flight;
        flight.departureCity = query.value(0).toString();
        flight.arrivalCity = query.value(1).toString();
        flight.airline = query.value(2).toString();
        flight.aircraftType = query.value(3).toString();
        flight.departureTime = query.value(4).toString();
        flight.arrivalTime = query.value(5).toString();
        flight.price = query.value(6).toString();
        flight.date = date;
        matchedFlights.append(flight);
    }

    db.close();

    if (matchedFlights.isEmpty()) {
        // 清理旧的航班布局
        QLayoutItem* child;
        while ((child = flightLayout->takeAt(0)) != nullptr) {
            delete child->widget();  // 删除子控件
            delete child;
        }
        QMessageBox::information(this, "没有找到航班", "没有找到符合条件的航班，请尝试其他出发地和目的地组合。");
        qDebug()<<departureCity<<" "<<arrivalCity<<" "<<date;
    } else {
        loadFlightData(matchedFlights);
    }
}

//购票
void BookPage::purchaseTicket(const Flight& selectedFlight) {
    // 弹出乘客信息对话框
    Passenger passengerDialog(this);
    if (passengerDialog.exec() == QDialog::Accepted) {
        PassengerInfo passengerInfo = passengerDialog.getPassengerInfo();

        // 将机票信息保存到数据库
        QSqlDatabase db;
        if (QSqlDatabase::contains("ticket_connection")) {
            db = QSqlDatabase::database("ticket_connection");
        } else {
            db = QSqlDatabase::addDatabase("QODBC", "ticket_connection");
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

        // 检查是否已经存在记录
        query.prepare("SELECT COUNT(*) FROM ticket_data WHERE passengerName = ? AND passengerIdNumber = ? AND departureCity = ? AND arrivalCity = ? "
                      "AND airline = ? AND aircraftType = ? AND departureTime = ? AND arrivalTime = ? AND date = ?");
        query.addBindValue(passengerInfo.name);
        query.addBindValue(passengerInfo.idNumber);
        query.addBindValue(selectedFlight.departureCity);
        query.addBindValue(selectedFlight.arrivalCity);
        query.addBindValue(selectedFlight.airline);
        query.addBindValue(selectedFlight.aircraftType);
        query.addBindValue(selectedFlight.departureTime);
        query.addBindValue(selectedFlight.arrivalTime);
        query.addBindValue(selectedFlight.date);

        if (!query.exec()) {
            QMessageBox::critical(this, "查询失败", query.lastError().text());
            db.close();
            return;
        }

        // 检查查询结果
        query.next();
        int count = query.value(0).toInt();
        if (count > 0) {
            QMessageBox::warning(this, "重复购票", QString("尊敬的 %1，您已经购买过该航班的机票，请勿重复购买。").arg(passengerInfo.name));
            db.close();
            return;
        }

        // 如果没有记录，则插入新的票据
        query.prepare("INSERT INTO ticket_data (username, departureCity, arrivalCity, airline, aircraftType, departureTime, arrivalTime, price, passengerName, passengerIdNumber, date) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        query.addBindValue(curentUsername);  // 用户名
        query.addBindValue(selectedFlight.departureCity);
        query.addBindValue(selectedFlight.arrivalCity);
        query.addBindValue(selectedFlight.airline);
        query.addBindValue(selectedFlight.aircraftType);
        query.addBindValue(selectedFlight.departureTime);
        query.addBindValue(selectedFlight.arrivalTime);
        query.addBindValue(selectedFlight.price);
        query.addBindValue(passengerInfo.name);
        query.addBindValue(passengerInfo.idNumber);
        query.addBindValue(selectedFlight.date);

        if (!query.exec()) {
            QMessageBox::critical(this, "购票失败", query.lastError().text());
        } else {
            QMessageBox::information(this, "购票成功", "购票成功!");
        }

        db.close();
    }
}



void BookPage::openMyTicketsWindow(){
    this->myTicketsWindow = new MyTicketPage();
    //从我的订单窗口返回购票窗口
    connect(this->myTicketsWindow,&MyTicketPage::backToBookWindow,this,[=]{
        this->myTicketsWindow->hide();
        this->show();
    });
    this->myTicketsWindow->show();
}

// 加载日期按钮的方法，接受一个基准日期
void BookPage::loadDateButtons(const QDate &baseDate){
    // 清空当前按钮
    QLayoutItem *child;
    while ((child = dateButtonLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // 创建新的按钮
    for (int i = -3; i <= 3; ++i) {
        QDate date = baseDate.addDays(i);
        QPushButton *dateButton = new QPushButton(date.toString("MM-dd"), this);
        dateButton->setObjectName("dateButton");
        dateButton->setStyleSheet("#dateButton{"
                                      "background-color: rgb(0, 170, 255);"
                                      "border-radius: 10px;"
                                      "font-size: 16px;  "
                                  "}"
                                  "#dateButton:hover{"
                                      "background-color: rgb(38, 143, 255);"
                                      "border-radius: 10px;"
                                  "}"
                                  "#dateButton:pressed {"
                                      "background-color: rgb(38, 143, 255);"
                                      "border: 1px solid #555;  /* 按下时改变边框颜色 */"
                                      "border-radius: 10px;"
                                      "font-size: 12px;  /* 鼠标按下时，字体缩小 */"
                                  "}");

        connect(dateButton, &QPushButton::clicked, this, [this, date]() {
            this->onDateButtonClicked(date);
        });

        // 如果是当前选择的日期，将其默认样式加粗
        if (i == 0) {
            QFont font = dateButton->font();
            font.setBold(true);
            dateButton->setFont(font);
        }

        dateButtonLayout->addWidget(dateButton);
    }

    dateButtonLayout->setAlignment(Qt::AlignCenter);
}

QIcon BookPage::selectImg(const QString& airline) {
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



