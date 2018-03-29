#ifndef GCOLOR_H_
#define GCOLOR_H_

class GColor
{
public:
	GColor(int iReg, int iRed, int green, int blue);
	
	int GetBlue();
	void SetBlue(int iBlue);

	int GetGreen(); 
	void SetGreen(int iGreen); 

	int GetRed();
	void SetRed(int iRed);	

	int GetRegister();
	
private:
	int m_iBlue;
	int m_iGreen;	
	int m_iRed;
	int m_iRegister;	 
};

#endif
