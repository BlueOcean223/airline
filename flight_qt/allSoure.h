#ifndef ALLSOURE_H
#define ALLSOURE_H

#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QDebug>
#include <QMap>
#include <QStringList>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateEdit>
#include <QComboBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QPainter>
#include <QIcon>

//现在登录的用户名
extern QString curentUsername;

// 航班信息结构体
struct Flight {
    QString airline;
    QString aircraftType;
    QString departureTime;
    QString arrivalTime;
    QString departureCity;
    QString arrivalCity;
    QString price;
    QString passengerName;
    QString passengerIdNumber;
    QString date;
    QString username; // 新增，用于区分用户
};

//加载航班信息
extern QList<Flight> flights;
// 存储用户购票信息
extern QList<Flight> userTickets;

// 乘客信息结构体
struct PassengerInfo {
    QString name;
    QString idNumber;
    QString phNumber;
};



#endif // ALLSOURE_H


