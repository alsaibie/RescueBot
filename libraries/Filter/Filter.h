//Low pass butterworth filter order=2 alpha1=0.5 
#pragma once
class Filter
{
	public:
		Filter()
		{
			v[0]=0.0;
			v[1]=0.0;
		}
	private:
		float v[3];
	public:
		float step(float x) //class II 
		{
			v[0] = v[1];
			v[1] = v[2];
			v[2] = (1.000000000000e+0 * x)
				 + ( -1.0000000000 * v[0])
				 + ( -2.0000000000 * v[1]);
			return 
				 (v[0] + v[2])
				+2 * v[1];
		}
};