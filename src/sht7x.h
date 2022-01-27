#include "iowkit.h"

class IOWSHT7x
{
    double _temperature;
    double _humidity;
    double _dewpoint;

    IOWKIT_HANDLE _ioHandle;

    short readI2C(UCHAR call);
    bool  calc_trh(double humidity, double temperature);
    bool  calc_dewpoint(double h, double t);

public:
    IOWSHT7x();
    bool open();
    void close();
    bool read();
    double getTemperature();
    double getHumidity();

};
