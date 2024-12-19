#include "searchpage.h"
#include "ui_searchpage.h"

searchPage::searchPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::searchPage)
{
    ui->setupUi(this);
    //链接交互按钮
    connect(ui->myswapButton,&QPushButton::clicked,this,[=]{
        qDebug()<<"我被点击了";
        ui->myswapButton->swapChange();
        int departureIndex = ui->StartcomboBox->currentIndex();
        int arrivalIndex = ui->EndcomboBox->currentIndex();

        // 交换当前选中项
        ui->StartcomboBox->setCurrentIndex(arrivalIndex);
        ui->EndcomboBox->setCurrentIndex(departureIndex);
    });
    //链接返回按键
    connect(ui->back1,&QPushButton::clicked,this,&searchPage::backLogin);
    //链接查询按键
    connect(ui->searchPushButton,&QPushButton::clicked,this,&searchPage::searchTicket);


    //加载地址选择
    ui->StartcomboBox->addItems({"北京","上海","广州", "深圳", "厦门","成都","青岛","南京","杭州","昆明","重庆","珠海","香港","大理"});

    ui->EndcomboBox->addItems({"北京","上海","广州", "深圳", "厦门","成都","青岛","南京","杭州","昆明","重庆","珠海","香港","大理"});

    connect(ui->myTickpushButton,&QPushButton::clicked,this,&searchPage::showMyTicketWindow);
}

searchPage::~searchPage()
{
    delete ui;
}

void searchPage::backLogin(){
    this->hide();
    emit this->goToLogin();
}

void searchPage::comeBack(){
    this->show();
}

void searchPage::showMyTicketWindow(){
    this->myTicketWindow = new MyTicketPage();
    //从我的订单窗口返回查询窗口
    connect(this->myTicketWindow,&MyTicketPage::backToSearchWindow,this,[=]{
        this->myTicketWindow->hide();
        this->show();
    });
    this->myTicketWindow->show();
}

void searchPage::searchTicket(){
    flights.clear();

    QString departureCity = ui->StartcomboBox->currentText();
    QString arrivalCity = ui->EndcomboBox->currentText();
    QString date = ui->dateEdit->date().toString("MM月dd日");

    if(departureCity == arrivalCity){
        QMessageBox::warning(this,"警告","请选择不同的出发地和目的地！");
        return;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC"); // 使用唯一的连接名称
    db.setDatabaseName(DSN);
    db.setUserName(DB_USER);
    db.setPassword(DB_PASSWORD);
    db.setConnectOptions("CLIENT_ENCODING=UTF8");

    if (!db.open()) {
        QMessageBox::critical(this, "数据库连接失败", db.lastError().text());
        return;
    } else {
        qDebug() << " 连接数据库成功 ";
    }

    QList<Flight> matchedFlights;
    QSqlQuery query(db);
    query.prepare("SELECT 出发城市, 到达城市, 航空公司, 客机机型, 出发时间,抵达时间, 机票价格 FROM flight_data WHERE 出发城市 = ? AND 到达城市 = ? AND 出发日期 = ?");
    query.addBindValue(departureCity);
    query.addBindValue(arrivalCity);
    query.addBindValue(date);

    qDebug() << departureCity << " " << arrivalCity << " " << date;
    if (!query.exec()) {
        QMessageBox::critical(this, "查询失败", query.lastError().text());
        db.close(); // 确保在失败时关闭连接
        qDebug() << "连接失败";
        return;
    }


    while (query.next()) {
        Flight flight;
        //按列获取数据
        flight.departureCity = query.value(0).toString();
        flight.arrivalCity = query.value(1).toString();
        flight.airline = query.value(2).toString();
        flight.aircraftType = query.value(3).toString();
        flight.departureTime = query.value(4).toString();
        flight.arrivalTime = query.value(5).toString();
        flight.price = query.value(6).toString();
        flight.date=date;
        matchedFlights.append(flight);
    }
    qDebug() << "成功自数据库载入数据至程序中";

    db.close(); // 确保在成功后关闭连接

    flights = matchedFlights;
    if(flights.isEmpty()){
        QMessageBox::warning(this,"警告","没有该时间的飞机！");
        return;
    }

    this->showBookWindow();
}

void searchPage::showBookWindow(){
    //链接从购票窗口返回该窗口的自定义信号和槽函数
    this->bookWindow = new BookPage(ui->dateEdit->date());
    connect(this->bookWindow,&BookPage::backToSearchPage,this,[=]{
        this->comeBack();
        qDebug()<<"我回来了";
    });
    this->hide();
    this->bookWindow->show();
}
