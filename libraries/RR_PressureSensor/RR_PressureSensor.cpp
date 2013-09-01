#include "RR_PressureSensor.h"

RR_PressureSensor::RR_PressureSensor(void):SensorAverage(PRESSURERUNNINGAVGSIZE)
{
	//initialize
	LiftOff_FLAG=false;
	Landing_FLAG=false;
	BaseAltitudeWait=0;
	MaxAltitude=0;
}

void RR_PressureSensor::Enable(void)
{
	

	//Turn digital pin high for sensor VCC
	pinMode(PSENSOR_ENABLE_PIN, OUTPUT);
	digitalWrite(PSENSOR_ENABLE_PIN, HIGH);

	delay(100); //wait for sensor to turn on before starting I2C
	
	//Begin I2C Communication 
	if (!RR_PressureSensor::begin()) 
	{
	Serial.println("Check connection to BMP085 Sensor");
	while (1) {}
	}
	
	
	SensorAverage.clear();
	delay(100);
	//Fill Moving Average Array
	while(BaseAltitudeWait++<=5*PRESSURERUNNINGAVGSIZE)
	{
		
		SensorAverage.addValue(readAltitude(ACTUALSEAPRESSURE));
	}

	//Record Base level pressure after one 
	BaseAltitude=SensorAverage.getAverage();
//	Serial.println(BaseAltitude);
}

void RR_PressureSensor::Update(void)
{
	//Update Pressure Moving Average Array
	SensorAverage.addValue(readAltitude(ACTUALSEAPRESSURE));
	AverageAltitude=SensorAverage.getAverage();
	
	//Determine LiftOff: Check when current pressure reading is higher than BaseAltitude by Threshold amount
	if(AverageAltitude-BaseAltitude>=LIFTOFFTHRESHOLD)
	{
		LiftOff_FLAG=true;
	}

	if(AverageAltitude>MaxAltitude)
	{
		MaxAltitude=AverageAltitude;
	}

	if(LiftOff_FLAG)
	{
		//TODO:
		//check that pressure is not changing for a while within certain percentage then flip Landing Flag
	}

}

float RR_PressureSensor::getAltitude(void)
{
	return AverageAltitude;
}


void RR_PressureSensor::Disable(void)
{
	digitalWrite(PSENSOR_ENABLE_PIN, LOW);
}
