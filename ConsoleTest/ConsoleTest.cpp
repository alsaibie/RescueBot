// ConsoleTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;

struct MyStruct
{
	float value;
	//static const string valueCode="HGH";
	

}mystruct;


int _tmain(int argc, _TCHAR* argv[])
{
	mystruct.value=60.0;
	//mystruct.valueCode=";
	cout<<mystruct.value<<endl;
	//sprintf(mystruct.valueCode);
	//cout<<mystruct.valueCode<<endl;
	while(1);
	return 0;
}

