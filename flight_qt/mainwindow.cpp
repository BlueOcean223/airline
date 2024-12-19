#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsOpacityEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->searcheView = new searchPage();
    //链接登录按键
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::login);

    //链接注册按钮和注册页面
    connect(ui->register_2,&QPushButton::clicked,this,&MainWindow::showRegisterPage);

    //链接忘记密码按扭和忘记密码页面
    connect(ui->forget,&QPushButton::clicked,this,&MainWindow::showForgetPage);

    //链接返回按钮和登录页面
    connect(ui->back1,&QPushButton::clicked,this,&MainWindow::backLogin);
    connect(ui->back2,&QPushButton::clicked,this,&MainWindow::backLogin);

    //链接注册页面的注册按钮
    connect(ui->zhuce,&QPushButton::clicked,this,&MainWindow::registerUser);

    //链接忘记密码页面的重置按钮
    connect(ui->chongzhi,&QPushButton::clicked,this,&MainWindow::forgetUser);

    //链接查询窗口中的自定义信号和显示登录窗口的槽函数
    connect(this->searcheView,&searchPage::goToLogin,this,&MainWindow::showLogin);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showLogin(){
    //重置信息
    ui->username->setText("");
    ui->password->setText("");
    curentUsername = "";
    this->show();
}

void MainWindow::login(){
    QString username = ui->username->text();
    QString password = ui->password->text();

    //连接数据库
    QSqlDatabase db;
    if(QSqlDatabase::contains("login_connection")){
        db = QSqlDatabase::database("login_connection");
    }else{
        db = QSqlDatabase::addDatabase("QODBC","login_connection");
        db.setDatabaseName(DSN);
        db.setUserName(DB_USER);
        db.setPassword(DB_PASSWORD);
    }

    if(!db.open()){
        QMessageBox::critical(this,"数据库连接失败",db.lastError().text());
        return;
    }

    // 查询用户信息
    QSqlQuery query(db);
    query.prepare("SELECT * FROM user_data WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);

    // query.exec() 执行sql语句
    if(query.exec() && query.next()){
        qDebug() << "登陆成功！";
        curentUsername = username;
        QMessageBox::information(this,"登陆成功","登陆成功！");
        this->hide();
        this->searcheView->show();
    }else{
        QMessageBox::warning(this, "登录失败", "用户名或密码错误!");
    }
    db.close();
}

void MainWindow::showRegisterPage(){
    ui->stackedWidget->setCurrentIndex(1);
}



void MainWindow::showForgetPage(){
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow:: backLogin(){
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::registerUser(){
    QString username = ui->registerUsername->text();
    QString password = ui->registerPassword->text();
    QString confirPassword = ui->registerComfirPass->text();
    QString problem = ui->comfirProblem->text();


    if(username.length() == 0|| password.length() == 0 || problem.length() == 0 || problem.isEmpty()){
        QMessageBox::warning(this,"注册失败","用户名/密码/密保问题不能为空！");
        return;
    }

    if(password != confirPassword){
        QMessageBox::warning(this,"注册失败","两次输入的密码不相同！");
        return;
    }

    //qDebug()<<username;
    //连接数据库
    QSqlDatabase db;
    if(QSqlDatabase::contains("login_connection")){
        db = QSqlDatabase::database("login_connection");
    }else{
        db = QSqlDatabase::addDatabase("QODBC","login_connection");
        db.setDatabaseName(DSN);
        db.setUserName(DB_USER);
        db.setPassword(DB_PASSWORD);
    }

    if(!db.open()){
        QMessageBox::critical(this,"数据库连接失败",db.lastError().text());
        return;
    }

    //检查用户名是否已经存在
    QSqlQuery query(db);
    query.prepare("SELECT * FROM user_data WHERE username = ?");
    query.addBindValue(username);

    if(query.exec() && query.next()){
        QMessageBox::warning(this,"注册失败","用户名已存在，请选择其他用户名!");
        db.close();
        return;
    }

    // 将用户信息插入数据库
    query.prepare("INSERT INTO user_data (username, password,protectproblem) VALUES (?, ?,?)");
    query.addBindValue(username);
    query.addBindValue(password);
    query.addBindValue(problem);

    if (!query.exec()) {
        QMessageBox::critical(this, "注册失败", query.lastError().text());
    } else {
        QMessageBox::information(this, "注册成功", "注册成功， 即将返回登录页面登录!");
        //清空输入框
        ui->registerUsername->setText("");
        ui->registerPassword->setText("");
        ui->registerComfirPass->setText("");
        ui->comfirProblem->setText("");

        ui->stackedWidget->setCurrentIndex(0);
    }

    db.close();
}

void MainWindow::forgetUser(){
    QString username = ui->forgetUsername->text();
    QString password = ui->forgetPassword->text();
    QString confirPassword = ui->forgetComfirPassword->text();
    QString problem = ui->forgetProblem->text();


    if(username.length() == 0|| password.length() == 0 || problem.length() == 0 || problem.isEmpty()){
        QMessageBox::warning(this,"重置失败","用户名/密码/密保问题不能为空！");
        return;
    }

    if(password != confirPassword){
        QMessageBox::warning(this,"重置失败","两次输入的密码不相同！");
        return;
    }

    //连接数据库
    QSqlDatabase db;
    if(QSqlDatabase::contains("login_connection")){
        db = QSqlDatabase::database("login_connection");
    }else{
        db = QSqlDatabase::addDatabase("QODBC","login_connection");
        db.setDatabaseName(DSN);
        db.setUserName(DB_USER);
        db.setPassword(DB_PASSWORD);
    }

    if(!db.open()){
        QMessageBox::critical(this,"数据库连接失败",db.lastError().text());
        return;
    }

    // 查询用户信息
    QSqlQuery query(db);
    query.prepare("SELECT * FROM user_data WHERE username = ? AND protectproblem = ?");
    query.addBindValue(username);
    query.addBindValue(problem);

    if (query.exec() && query.next()) {
        // 重置用户密码
        query.clear();
        query.prepare("UPDATE user_data SET password = ? WHERE username = ?");
        query.addBindValue(password);
        query.addBindValue(username);
        //qDebug()<<username<<" "<<password;
        if (!query.exec()) {
            QMessageBox::critical(this, "重置失败", query.lastError().text());
        } else {
            QMessageBox::information(this, "重置成功", "重置成功!  即将返回登陆界面");
            //清空输入框
            ui->forgetUsername->setText("");
            ui->forgetPassword->setText("");
            ui->forgetComfirPassword->setText("");
            ui->forgetProblem->setText("");

            ui->stackedWidget->setCurrentIndex(0);
        }
    } else {
        QMessageBox::warning(this, "重置失败", "用户名或密保问题错误!");
    }

    db.close();
}
