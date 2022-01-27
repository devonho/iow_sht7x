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

	printf("%.2f deg. C\n", sht.read());
	//printf("%.2f RH\n", sht.readTempHumidity());

	sht.close(); 


}