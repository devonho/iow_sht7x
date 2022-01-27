#include <iostream>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "sht7x.h"

int main()
{
	IOWSHT7x sht;

	sht.open();
	sht.read();
	printf("%.2f deg. C\n", sht.getTemperature());
	printf("%.2f RH\n", sht.getHumidity());

	sht.close(); 


}