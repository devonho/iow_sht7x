#include <cstring>
#include <iostream>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include "sht7x.h"

#define HUMIDITY 0x05
#define TEMPERATURE 0x03

IOWSHT7x::IOWSHT7x()
{
	_temperature = _humidity = _dewpoint = 0;
	_ioHandle = NULL;
}

bool IOWSHT7x::open()
{
	try
	{
		_ioHandle = IowKitOpenDevice();	//open IO-Warrior

		if (_ioHandle == NULL) throw "ioHandle is NULL";
			
		IOWKIT_SPECIAL_REPORT report;
		memset(&report, 0x00, IOWKIT_SPECIAL_REPORT_SIZE);

		report.ReportID = 0x01; //I2C-Mode
		report.Bytes[0] = 0x01; //Enable
		report.Bytes[1] = 0xC0; //Disable Pull-Up resistors, Enable Bus

		IowKitWrite(_ioHandle, IOW_PIPE_SPECIAL_MODE, (char*)&report, IOWKIT_SPECIAL_REPORT_SIZE);

		report.ReportID = 0x02; 
		report.Bytes[0] = 0x82; //Generate start, data count = 1 byte
		report.Bytes[1] = 0x06; 

		IowKitWrite(_ioHandle, IOW_PIPE_SPECIAL_MODE, (char*)&report, IOWKIT_SPECIAL_REPORT_SIZE);
		IowKitRead(_ioHandle, IOW_PIPE_SPECIAL_MODE, (char*)&report, IOWKIT_SPECIAL_REPORT_SIZE);
		return true;
	}
	catch(const std::exception& e)
	{
		_last_error_msg = std::string("open:") +  std::string(e.what());
		return false;
	}
	catch(const char* e)
	{
		_last_error_msg = std::string("open:") +  std::string(e);
		return false;
	}
}

void IOWSHT7x::close()
{
	try
	{
		if (_ioHandle != NULL)
		{
			IOWKIT_SPECIAL_REPORT report;
			memset(&report, 0x00, IOWKIT_SPECIAL_REPORT_SIZE);

			report.ReportID = 0x01; //I2C-Mode
			report.Bytes[0] = 0x00; //Disable

			IowKitWrite(_ioHandle, IOW_PIPE_SPECIAL_MODE, (char*)&report, IOWKIT_SPECIAL_REPORT_SIZE);

			IowKitCloseDevice(_ioHandle);	//close all IO-Warrior
			_ioHandle = NULL;		
		}
	}
	catch(const std::exception& e)
	{
		_last_error_msg = std::string("close:") +  std::string(e.what());
	}
	catch(const char* e)
	{
		_last_error_msg = std::string("close:") +  std::string(e);
	}
}

bool IOWSHT7x::read()
{
	try
	{
		if (_ioHandle == NULL) throw "ioHandle is NULL";		

		_humidity = readI2C(HUMIDITY);						//Read the humidity over I2C
		_temperature = readI2C(TEMPERATURE);				//Read the temperature over I2C

		calc_trh(_humidity, _temperature);					//calc humidity and temperature
		calc_dewpoint(_humidity, _temperature);	//calc dew point
		return true;
	}
	catch(const std::exception& e)
	{
		_last_error_msg = std::string("read:") +  std::string(e.what());
		return false;
	}
	catch(const char* e)
	{
		_last_error_msg = std::string("read:") +  std::string(e);
		return false;
	}
}

double IOWSHT7x::getTemperature()
{	
	try
	{
		if (_ioHandle == NULL) throw "ioHandle is NULL";		
		return _temperature;
	}
	catch(const std::exception& e)
	{
		_last_error_msg = std::string("getTemperature:") +  std::string(e.what());
		return 0.0;
	}
	catch(const char* e)
	{
		_last_error_msg = std::string("getTemperature:") +  std::string(e);
		return false;
	}
}

double IOWSHT7x::getHumidity()
{
	try
	{
		if (_ioHandle == NULL) throw "ioHandle is NULL";		
		return _humidity;
	}
	catch(const std::exception& e)
	{
		_last_error_msg = std::string("getHumidity:") +  std::string(e.what());
		return 0.0;
	}
	catch(const char* e)
	{
		_last_error_msg = std::string("getHumidity:") +  std::string(e);
		return false;
	}
}

short IOWSHT7x::readI2C(UCHAR call)
{
	try
	{
		if (_ioHandle == NULL) throw "ioHandle is NULL";		
		
		IOWKIT_SPECIAL_REPORT report;
		memset(&report, 0x00, IOWKIT_SPECIAL_REPORT_SIZE);
		short result = 0;

		report.ReportID = 0x03;			//I2C-Read
		report.Bytes[0] = 0x03;			//Read 3 Bytes
		report.Bytes[1] = call;			//Address for humidity or temperature

		IowKitWrite(_ioHandle, IOW_PIPE_SPECIAL_MODE, (char*)&report, IOWKIT_SPECIAL_REPORT_SIZE);
		IowKitRead(_ioHandle, IOW_PIPE_SPECIAL_MODE, (char*)&report, IOWKIT_SPECIAL_REPORT_SIZE);

		result = (report.Bytes[1] << 8) | report.Bytes[2];	//create a 16bit value

		return result;		
	}
	catch(const std::exception& e)
	{
		_last_error_msg = std::string("readI2C:") +  std::string(e.what());
		return 0;
	}
	catch(const char* e)
	{
		_last_error_msg = std::string("readI2C:") +  std::string(e);
		return false;
	}
}

bool IOWSHT7x::calc_trh(double humidity, double temperature)
{
	try
	{
		if (_ioHandle == NULL) throw "ioHandle is NULL";		

		const double C1 = -4.0;              // for 12 Bit
		const double C2 = +0.0405;           // for 12 Bit
		const double C3 = -0.0000028;        // for 12 Bit
		const double T1 = +0.01;             // for 14 Bit @ 5V
		const double T2 = +0.00008;          // for 14 Bit @ 5V	

		double rh = humidity;				 // rh:      Humidity [Ticks] 12 Bit 
		double t = temperature;				 // t:       Temperature [Ticks] 14 Bit
		double rh_lin;						 // rh_lin:  Humidity linear
		double rh_true;						 // rh_true: Temperature compensated humidity
		double t_C;							 // t_C   :  Temperature [deg. C]

		t_C = t * 0.01 - 40;						     //calc. temperature from ticks to [deg. C]
		rh_lin = C3 * rh * rh + C2 * rh + C1;			 //calc. humidity from ticks to [%RH]
		rh_true = (t_C - 25) * (T1 + T2 * rh) + rh_lin;	 //calc. temperature compensated humidity [%RH]

		if (rh_true > 100) rh_true = 100;		//cut if the value is outside of
		if (rh_true < 0.1) rh_true = 0.1;		//the physical possible range

		_temperature = t_C;		    //return temperature [C]
		_humidity = rh_true;		//return humidity[%RH]
		return true;
	}
	catch(const std::exception& e)
	{
		_last_error_msg = std::string("calc_trh:") +  std::string(e.what());
		return false;
	}
	catch(const char* e)
	{
		_last_error_msg = std::string("calc_trh:") +  std::string(e);
		return false;
	}
}

bool IOWSHT7x::calc_dewpoint(double h, double t)
{
	try
	{
		if (_ioHandle == NULL) throw "ioHandle is NULL";		

		double logEx;
		logEx = 0.66077 + 7.5 * t / (237.3 + t) + (log10(h) - 2);
		_dewpoint = (logEx - 0.66077) * 237.3 / (0.66077 + 7.5 - logEx);
		return true;
	}
	catch(const std::exception& e)
	{
		_last_error_msg = std::string("calc_dewpoint:") +  std::string(e.what());
		return false;
	}
	catch(const char* e)
	{
		_last_error_msg = std::string("calc_dewpoint:") +  std::string(e);
		printf("calc_dewpoint: %s\n", e);
		return false;
	}
}

std::string IOWSHT7x::get_last_error()
{
	return _last_error_msg;
}
