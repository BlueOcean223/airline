#ifndef FLIGHTICON_H
#define FLIGHTICON_H

#include<QIcon>

class flightIcon
{
public:
    flightIcon();
    static QIcon getFlightIcon(const QString& airline);
};

#endif // FLIGHTICON_H
