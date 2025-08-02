#include "rebookpage.h"

rebookPage::rebookPage(Flight preFlight,QDate sDate,QWidget *parent)
    :QWidget(parent)
{

    this->preFlight = preFlight;
    // 主布局：垂直排列
    layout = new QVBoxLayout(this);

    // 出发地和目的地布局
    QHBoxLayout *departureArrivalLayout = new QHBoxLayout();

    departureLabel = new QLabel(QString("出发地: %1").arg(preFlight.departureCity), this);
    QLabel *lalala = new QLabel(QString("——>"),this);
    arrivalLabel = new QLabel(QString("目的地: %1").arg(preFlight.arrivalCity), this);

    departureArrivalLayout->addWidget(departureLabel);
    departureArrivalLayout->addWidget(lalala);
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

    // 返回按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *returnButton = new QPushButton("返回", this);

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

    connect(returnButton, &QPushButton::clicked, [=]() {
        this->hide();
        emit this->backToMyTicket();
    });

    buttonLayout->addWidget(returnButton);
    layout->addLayout(buttonLayout);

    updateFlightData(this->preFlight.departureCity,this->preFlight.arrivalCity,this->preFlight.date);

    setLayout(layout);
    setWindowTitle("航班购票");
    setWindowIcon(QIcon(":/imagSoure/sysu.ico"));
    resize(800, 600);
}
rebookPage::~rebookPage(){}


// 当日期按钮被点击时，更新航班数据并更新日期按钮
void rebookPage::onDateButtonClicked(const QDate &date){
    selectedDate = date.toString("MM月dd日");  // 更新选定的日期
    QString departureCity = departureLabel->text().split(": ")[1];
    QString arrivalCity = arrivalLabel->text().split(": ")[1];

    // 更新航班数据
    updateFlightData(departureCity, arrivalCity, selectedDate);

    // 重新加载基于所选日期的按钮
    loadDateButtons(date);
}

void rebookPage::loadFlightData(const QList<Flight> &flights){
    // 清理旧的航班布局
    QLayoutItem* child;
    while ((child = flightLayout->takeAt(0)) != nullptr) {
        delete child->widget();  // 删除子控件
        delete child;
    }

    // 遍历航班数据，生成每个航班卡片
    for (const Flight& flight : flights) {
        flightCard* flightcard = new flightCard(flight,QString("改签"));
        flightcard->setAttribute(Qt::WA_StyledBackground, true);

        connect(flightcard,&flightCard::rebookTicket,this,[=]{
            this->rebookTicket(flight);
        });

        flightLayout->addWidget(flightcard);
        //qDebug()<<flightcard->styleSheet();
    }

    // 添加一个伸缩项，确保卡片居中且整齐
    flightLayout->addStretch();
}

void rebookPage::updateFlightData(const QString &departureCity, const QString &arrivalCity, const QString &date){
    QSqlDatabase db;
    if (QSqlDatabase::contains("flight_connection")){
        db = QSqlDatabase::database("flight_connection");
    }else{
        db = DBHelper::getConnection();
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

    if (!query.exec()) {
        QMessageBox::critical(this, "查询失败", query.lastError().text());
        db.close();
        return;
    }

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

void rebookPage::rebookTicket(const Flight& selectedFlight) {
    if(this->preFlight.airline==selectedFlight.airline && this->preFlight.aircraftType==selectedFlight.aircraftType
        && this->preFlight.departureTime==selectedFlight.departureTime && this->preFlight.arrivalTime==selectedFlight.arrivalTime
        && this->preFlight.price==selectedFlight.price){
        QMessageBox::warning(this,"警告","您的航班与选择的航班相同，不支持改签");
        return;
    }
    // 创建或获取数据库连接
    QSqlDatabase db;
    if (QSqlDatabase::contains("flight_connection")) {
        db = QSqlDatabase::database("flight_connection");
    } else {
        db = DBHelper::getConnection();
    }

    if (!db.open()) {
        QMessageBox::critical(this, "数据库连接失败", db.lastError().text());
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE ticket_data SET airline = ?, aircraftType = ?, departureTime = ?, arrivalTime = ?, price = ?, date = ? "
                  "WHERE username = ? AND departureCity = ? AND arrivalCity = ? AND airline = ? AND aircraftType = ? "
                  "AND departureTime = ? AND arrivalTime = ? AND price = ? AND passengerName = ? AND passengerIdNumber = ? AND date = ?");
    query.addBindValue(selectedFlight.airline);
    query.addBindValue(selectedFlight.aircraftType);
    query.addBindValue(selectedFlight.departureTime);
    query.addBindValue(selectedFlight.arrivalTime);
    query.addBindValue(selectedFlight.price);
    query.addBindValue(selectedFlight.date);
    query.addBindValue(curentUsername);
    query.addBindValue(this->preFlight.departureCity);
    query.addBindValue(this->preFlight.arrivalCity);
    query.addBindValue(this->preFlight.airline);
    query.addBindValue(this->preFlight.aircraftType);
    query.addBindValue(this->preFlight.departureTime);
    query.addBindValue(this->preFlight.arrivalTime);
    query.addBindValue(this->preFlight.price);
    query.addBindValue(this->preFlight.passengerName);
    query.addBindValue(this->preFlight.passengerIdNumber);
    query.addBindValue(this->preFlight.date);


    // qDebug() << "SQL Query:" << query.lastQuery();
    // qDebug() << "Bound Values:";
    // for (int i = 0; i < query.boundValues().size(); ++i) {
    //     qDebug() << "Parameter" << i + 1 << ":" << query.boundValues().value(i);
    // }

    // 执行查询
    if (!query.exec()) {
        qDebug() << "SQL Query Error:" << query.lastError().text();
        qDebug() << "Executed Query:" << query.lastQuery();
        QMessageBox::critical(this, "改签失败", query.lastError().text());
    } else {
        qDebug() << preFlight.airline <<preFlight.aircraftType<<preFlight.date;
        qDebug() << selectedFlight.airline <<selectedFlight.aircraftType<<selectedFlight.date;
        QMessageBox::information(this, "改签成功", "改签成功!");
        emit this->backToMyTicket();
    }

    db.close();
}


// 加载日期按钮的方法，接受一个基准日期
void rebookPage::loadDateButtons(const QDate &baseDate){
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

