#ifndef DBHELPER_H
#define DBHELPER_H

#include<QSqlDatabase>
#include"allSoure.h"

class DBHelper
{
public:
    DBHelper();
    static QSqlDatabase getConnection();
};

#endif // DBHELPER_H
