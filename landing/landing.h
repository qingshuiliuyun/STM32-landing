/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ENGINE_H
#define __ENGINE_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
#include "GPIO.h"
	 
	 
extern unsigned char g_gearupFK;
extern float g_airpress[2];	 
	 
/*	
			g_sw[0] = GetGPIOData(1);		��ɡ״̬				1�г� 0δ�г�
			g_sw[1] = GetGPIOData(2);		
			g_sw[2] = GetGPIOData(3);		
			g_sw[3] = GetGPIOData(4);		ǰ����ܷ��µ�λ		0���µ�λ 1δ���µ�λ
			g_sw[4] = GetGPIOData(5);		��������ܷ��µ�λ
			g_sw[5] = GetGPIOData(6);		��������ܷ��µ�λ
			g_sw[6] = GetGPIOData(7);		ǰ���������λ		0���ϵ�λ 1δ���ϵ�λ
			g_sw[7] = GetGPIOData(8);		�������������λ
			g_sw[8] = GetGPIOData(9);		�������������λ
			g_sw[9] = GetGPIOData(10);		ǰ�ֳ���				0���� 1������
			g_sw[10] = GetGPIOData(11);		���ֳ���
			g_sw[11] = GetGPIOData(12);		���ֳ���
			g_sw[12] = GetGPIOData(13);		��ɡ״̬				1�� 0δ��

			CH1Capture 			PWM�ź����޼��    ���� 1500 ʹ��
			CH2Capture
			CH3Capture
			PWM_OUT.CCR9_Val 		��������  ������շŵ�ŷ�    0 ��     20000 ��
			PWM_OUT.CCR10_Val					�����շŵ�ŷ�		0 ��	 20000 ��
			PWM_OUT.CCR11_Val
			PWM_OUT.CCR12_Val
			PWM_OUT.CCR13_Val	
*/
#define CUTUMBRELLA	GetGPIOData(1)
// #define GATEUP_L	GetGPIOData(2)
// #define GATEUP_R	GetGPIOData(3)
#define GEARDOWN_F	GetGPIOData(4)
#define GEARDOWN_L	GetGPIOData(5)
#define GEARDOWN_R	GetGPIOData(6)
#define GEARUP_F	GetGPIOData(7)
#define GEARUP_L	GetGPIOData(8)
#define GEARUP_R	GetGPIOData(9)
#define ONLAND_F	GetGPIOData(10)
#define ONLAND_L	GetGPIOData(11)
#define ONLAND_R	GetGPIOData(12)
#define UMBRELLA	GetGPIOData(13)

#define ONLAND		0
#define FLYING		1

#define UP_OK		0
#define DOWN_OK		0
#define UP_NOTOK	1
#define DOWN_NOTOK	1
#define OK			1
#define notOK		0	 
	 

void Landing_main(void);
void Landing_PWMin(void);
void M_feedback(void);

extern unsigned char g_startupstate;
     

#ifdef __cplusplus
}
#endif

#endif 	 
	 
	 
