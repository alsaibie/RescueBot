//Low pass butterworth filter order=2 alpha1=0.5 
#pragma once
class Filter
{
	public:
		Filter()
		{
			for(int i=0; i <= 3; i++)
				v[i]=0.0;
		}
	private:
		float v[4];
	public:
		float step(float x) //class II 
		{
			v[0] = v[1];
			v[1] = v[2];
			v[2] = v[3];
			v[3] = (1.809893300751e-2 * x)
				 + (  0.2780599176 * v[0])
				 + ( -1.1828932620 * v[1])
				 + (  1.7600418803 * v[2]);
			return 
				 (v[0] + v[3])
				+3 * (v[1] + v[2]);
		}
};