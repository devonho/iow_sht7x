#include "iowkit.h"
#include <string>

class IOWSHT7x
{
    double _temperature;
    double _humidity;
    double _dewpoint;
    std::string  _last_error_msg;

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

    std::string get_last_error();

};
