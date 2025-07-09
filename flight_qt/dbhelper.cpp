#include "dbhelper.h"

const QString DSN = "persondb"; // 数据源名称
const QString DB_USER = "root";
const QString DB_PASSWORD = "123456";

DBHelper::DBHelper() {}

QSqlDatabase DBHelper::getConnection(){
    static QSqlDatabase db = QSqlDatabase::addDatabase("QODBC","flight_connection");

    db.setDatabaseName(DSN);
    db.setUserName(DB_USER);
    db.setPassword(DB_PASSWORD);
    db.setConnectOptions("CLIENT_ENCODING=UTF8");

    return db;
}
