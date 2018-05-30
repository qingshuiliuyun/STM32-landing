/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>

#include "bsp.h"
#include "ADS1248.h"
#include "GPIO.h"
#include "landing.h"
#include "FK_langding.h"


int g_CNT = 0;

void InitUSART_Config(void)
{
	USART_Config(USART1,115200,USART_WordLength_9b,USART_StopBits_1,USART_Parity_Odd);
	USART_Config(USART2,115200,USART_WordLength_9b,USART_StopBits_1,USART_Parity_Odd);
// 	USART_Config(USART3,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No);
// 	USART_Config(UART4,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No);
// 	USART_Config(UART5,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No);
// 	USART_Config(USART6,115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No);
}

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{

	uint64_t time_us;
	uint64_t pre_time_us;
	unsigned int detime=0;
    InitBSP();
    
    LEDOn();

    if (SysTick_Config(SystemCoreClock / 2000))
    { 
        /* Capture error */ 
        while (1);
    }
	
	
	g_startupstate = 1;		//上电检查标志
	
	
    while (1)
    {

        if(tag_1ms)          
        {
            tag_1ms = FALSE;
			time_us = get_absolute_time();
			
			Check_PrintfBuf_1ms();
			detime = time_us - pre_time_us;
			
			pre_time_us = time_us;
                
        }
        if(tag_2ms)          
        {
            tag_2ms = FALSE; 
			Read_ADCData();
        }
		
		if(tag_2_5ms)          
        {
            tag_2_5ms = FALSE;            
        }
		
        if(tag_5ms)          
        {
            tag_5ms = FALSE;
			Landing_PWMin();
			FK_Rev();
			M_Rev();
			Landing_main();		
        }
                
        if(tag_10ms && (tag_5ms == FALSE))
        {
            tag_10ms = FALSE;
        }

        if(tag_20ms && (tag_10ms== FALSE) && (tag_5ms == FALSE)) 
        {
            tag_20ms = FALSE;
        }

        if(tag_25ms) 
        {
            tag_25ms = FALSE; 
            
        }    
                
        if(tag_40ms) 
        {
            tag_40ms = FALSE;
        }

        if(tag_50ms && (tag_10ms == FALSE)&& (tag_5ms == FALSE))
        {
            tag_50ms = FALSE;
        }
       
        if(tag_80ms)
        {
            tag_80ms = FALSE;
        }
        
        if(tag_100ms && (tag_10ms == FALSE) && (tag_5ms == FALSE) && (tag_50ms == FALSE))   
        {
            tag_100ms = FALSE;
			M_feedback();
        }    
      
        if(tag_125ms)
        {
            tag_125ms = FALSE; 
        }

        if(tag_200ms)
        {
            tag_200ms = FALSE; 
        }
        
        if(tag_500ms && (tag_10ms == FALSE))
        {
            tag_500ms = FALSE;
            LEDToggle();
        }
        
        if(tag_1s && (tag_1ms == FALSE))
        {    
            tag_1s = FALSE;
        }
    }
}

 
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
