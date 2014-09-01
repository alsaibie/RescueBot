// ConsoleTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "GPSConversion.h"
using namespace std;

struct MyStruct
{
	float value;
	//static const string valueCode="HGH";
	

}mystruct;


int _tmain(int argc, _TCHAR* argv[])
{
	float utm_E, utm_N;
	int utmZone;
	wgs2utm(3347.342,- 8424.003,&utm_E,&utm_N,&utmZone);
	char str[80];
	sprintf_s(str,"Easting: %f Northing: %f Zone: %i\n",utm_E,utm_N,utmZone);
	//sprintf_s(str,"Easting: %f Northing: %f Zone: %i\n",3347.342,3347.342,40);
	//cout<<utm_E<<endl;
	printf(str);

	while(1);
	return 0;
}

