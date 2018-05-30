#include "landing.h"
#include "PWM.h"


unsigned int g_landtime_d5ms = 0;

unsigned char g_startupstate = 0;

unsigned char g_cmdstate = 0;
unsigned char g_gearupFK = 0;
unsigned char g_gearupPWM = 0;

float g_airpress[2] = {0.0f,0.0f};

void Landing_PWMin(void)
{
	if((CH1Capture < 2200) && (CH1Capture > 800))
	{
		g_cmdstate = 1;
		if(CH1Capture > 1500)
		{
			g_gearupPWM = 1;
		}
		else
		{	g_gearupPWM = 0;
		}
		CH1Capture = 0;
	}
	
	if((CH2Capture < 2200) && (CH2Capture > 800))
	{
		g_cmdstate = 1;
		if(CH2Capture > 1500)
		{
			;
		}
		else
		{	;
		}
		CH2Capture = 0;
	}
	
	if((CH3Capture < 2200) && (CH3Capture > 800))
	{
		g_cmdstate = 1;
		if(CH3Capture > 1500)
		{
			;
		}
		else
		{	;
		}
		CH3Capture = 0;
	}
}

#define LANGDINGDELAY 5000			//5s

//0 未完成  1 完成
unsigned char langding_downCMD(void)
{
	static unsigned char state = 0;
	
	switch(state)
	{
		case 0:
			//开舱门
			PWM_OUT.CCR10_Val = 20000;
			PWMOUT_Set();
		
			state = 1;
			g_landtime_d5ms = LANGDINGDELAY*2;
			break;
		case 1:
			if(g_landtime_d5ms == 0)
			{
				state = 2;
				
				//放起落架
				PWM_OUT.CCR10_Val = 20000;
				PWM_OUT.CCR9_Val = 20000;
				PWMOUT_Set();

				g_landtime_d5ms = LANGDINGDELAY*2;
			}
			break;
		case 2:
			if(g_landtime_d5ms == 0)
			{
				state = 0;
				return 1;
			}
			else
			{
				if((GEARDOWN_F == 1) && (GEARDOWN_L == 1) && (GEARDOWN_R == 1))
				{
					state = 0;
					return 1;
				}
			}
			break;
		default:
			state = 0;
			return 0;
	}


	return 0;
}

//0 未完成  1 完成
unsigned char langding_upCMD(void)
{
	static unsigned char state = 0;
	
	switch(state)
	{
		case 0:
			//收起落架
			PWM_OUT.CCR9_Val = 0;	
			PWMOUT_Set();
		
			state = 1;
			g_landtime_d5ms = LANGDINGDELAY*2;
			break;
		case 1:
			if(g_landtime_d5ms == 0)
			{
				state = 2;
				
				//关舱门
				PWM_OUT.CCR10_Val = 0;
				PWM_OUT.CCR9_Val = 0;
				PWMOUT_Set();
					
				g_landtime_d5ms = LANGDINGDELAY*2;
			}
			break;
		case 2:
			if(g_landtime_d5ms == 0)
			{
				state = 0;
				return 1;
			}
			else
			{
				if((GEARUP_F == UP_OK) && (GEARUP_L == UP_OK) && (GEARUP_R == UP_OK))
				{
					state = 0;
					return 1;
				}
			}
			break;
		default:
			state = 0;
			return 0;
	}

	
	return 0;
}


unsigned char langding_downCheck(void)
{
	static unsigned char state = 0;
	
	if(g_startupstate == 1)
	{
		//初始上电条件通过
		if((state == 1) || (GEARDOWN_F != DOWN_OK) || (GEARDOWN_L != DOWN_OK) || (GEARDOWN_R != DOWN_OK))
		{
			state = 1;
			if(langding_downCMD())
			{
				state = 0;
				return 1;
			}
		}
	}
	
	return 0;
}

#define ENERGENCY_UP 1000.0f		//Kpa
#define ENERGENCY_DOWN 300.0f

unsigned char landing_downEmergency(void)
{
	static unsigned char state = 0;
	
	if((state == 0)&&((g_airpress[0] > ENERGENCY_UP) || (g_airpress[1] > ENERGENCY_UP)))
	{
		state = 1;
	}
	
	
	if((state == 1) && ((g_airpress[0] < ENERGENCY_DOWN) || (g_airpress[1] < ENERGENCY_DOWN)) )
	{
		if(langding_upCMD())
		{
			state = 0;
			return 1;		//应急放起落架
		}
	}
	
	return 0;
}

unsigned char langding_upCheck(void)
{
	static unsigned char state = 0;
	
	if(g_startupstate == 1)
	{
		//初始上电条件通过
		if((state == 1) || (((ONLAND_F != ONLAND)&&(ONLAND_L != ONLAND)&&(ONLAND_R != ONLAND)) 
							&& ((GEARUP_F != UP_OK) || (GEARUP_L != UP_OK) || (GEARUP_R != UP_OK))) )
		{
			state = 1;
			if(langding_upCMD())
			{
				state = 0;
				return 1;
			}
		}
	}
	
	return 0;
}

void Landing_main(void)
{
	// init setup check
	
	//接收指令判断
	if(g_cmdstate == 1)
	{
		switch(g_gearupPWM)
		{
			case 0:
				langding_downCMD();
				break;
			case 1:
				langding_upCMD();
				break;
			default:
				break;
		}
	}
	else
	{		
		switch(g_gearupFK)
		{
			case 0:
				langding_downCheck();
				break;
			case 1:
				langding_upCheck();
				break;
			default:
				break;
		}
	}
	
	landing_downEmergency();

}

