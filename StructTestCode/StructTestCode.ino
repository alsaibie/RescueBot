struct IMUdata_t
{
 struct gyrodata_t

    {
        float x,y,z,;
    }gyrodata;
 
 struct magdata_t

    {
        float x,y,z,;
    }magdata;

};


void setup()
{
	IMUdata_t IMUData;
	IMUData.gyrodata.y=5.5;


  /* add setup code here */

}

void loop()
{

  /* add main program code here */

}
