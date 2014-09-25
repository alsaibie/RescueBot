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
	float coordinate = 4433.154;
	  float min = fmod(coordinate, 100.0)/60.0;
		float deg = float(int(coordinate/100));

	//wgs2utm(3347.342,- 8424.003,&utm_E,&utm_N,&utmZone);
	char str[80];
	sprintf_s(str,"Deg: %f Min: %f \n",deg,min);
	//sprintf_s(str,"Easting: %f Northing: %f Zone: %i\n",3347.342,3347.342,40);
	//cout<<utm_E<<endl;
	printf(str);

	while(1);
	return 0;
}

