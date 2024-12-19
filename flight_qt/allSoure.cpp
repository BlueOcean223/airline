#include "allSoure.h"

const QString DSN = "persondb"; // 数据源名称，需在ODBC数据源管理器中设置
const QString DB_USER = "root";
const QString DB_PASSWORD = "123456";

QString curentUsername = "";

QList<Flight> flights;
QList<Flight> userTickets;
