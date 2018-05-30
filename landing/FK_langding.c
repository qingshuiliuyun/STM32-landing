#include "FK_langding.h"
#include "landing.h"
#include "usart.h"

#define TRUE 1
#define FALSE 0
unsigned char FK_qiesan = 0;
unsigned char FK_fangsan = 0;
unsigned char FK_brakeL = 0;
unsigned char FK_brakeR = 0;
signed char FK_qianlun = 0;


unsigned char  FK_RxCheckSum(unsigned char buf[], unsigned char len)  
{
	
	unsigned char  sum=0;
	unsigned char  idx = 0;
    unsigned char  check = FALSE;
	
	for (idx=0; idx<len-1; idx++)  
	    sum += buf[idx];
	
	if (buf[len-1] == sum)
	    check = TRUE;	
	else
	    check = FALSE;
	
	return check;
}

unsigned char  FK_TxCheckSum(unsigned char buf[], unsigned char len)
{  
    
    unsigned char  idx = 0;
    unsigned char  sum = 0;
    
	for (idx=0; idx<len-1; idx++ )      
	    sum += buf[idx];	
    
    return sum;
    
}

void  sort(unsigned char a[], unsigned char len)
{
    unsigned char  idx, jdx;
    unsigned char  tmp;
    
    for (jdx=(unsigned char)(len-1); jdx>0; jdx--) 
    {
        for (idx=0; idx<jdx; idx++) 
        {
            if (a[idx] > a[idx+1]) 
            {
                tmp = a[idx];
                a[idx] = a[idx+1];
                a[idx+1] = tmp;
            }
        }
    }
}

void FK_feedback(void)
{
	unsigned char sendbuf[12];
	unsigned char temp=0;
	static unsigned char cnt;
	
	sendbuf[0] = 0xeb;
	sendbuf[1] = 0x90;
	
	temp = 0;
	temp = (GEARDOWN_R<<5) | (GEARUP_R<<4) | (GEARDOWN_L<<3) | (GEARUP_L<<2) | (GEARDOWN_F<<1) | GEARUP_F;
	sendbuf[2] = temp;
	sendbuf[3] = temp;
	sendbuf[4] = temp;
	
	temp = 0;
	temp = (CUTUMBRELLA<<4) | (UMBRELLA<<3) | (ONLAND_R<<2) | (ONLAND_L<<1) | ONLAND_F;
	sendbuf[5] = temp;
	sendbuf[6] = temp;
	sendbuf[7] = temp;
	
	sendbuf[8] = g_airpress[0];
	sendbuf[9] = 0;
	sendbuf[10] = cnt++;
	sendbuf[11] = FK_TxCheckSum(sendbuf+2,sizeof(sendbuf)-2);
	
	USART_SendBuf_notDMA(USART1,sendbuf,sizeof(sendbuf));
}

void M_feedback(void)
{
	unsigned char sendbuf[12];
	unsigned char temp=0;
	static unsigned char cnt;
	
	sendbuf[0] = 0xeb;
	sendbuf[1] = 0x90;
	
	temp = 0;
	temp = (GEARDOWN_R<<5) | (GEARUP_R<<4) | (GEARDOWN_L<<3) | (GEARUP_L<<2) | (GEARDOWN_F<<1) | GEARUP_F;
	sendbuf[2] = temp;
	sendbuf[3] = temp;
	sendbuf[4] = temp;
	
	temp = 0;
	temp = (CUTUMBRELLA<<4) | (UMBRELLA<<3) | (ONLAND_R<<2) | (ONLAND_L<<1) | ONLAND_F;
	sendbuf[5] = temp;
	sendbuf[6] = temp;
	sendbuf[7] = temp;
	
	sendbuf[8] = g_airpress[0];
	sendbuf[9] = 0;
	sendbuf[10] = cnt++;
	sendbuf[11] = FK_TxCheckSum(sendbuf+2,sizeof(sendbuf)-2);
	
	USART_SendBuf_notDMA(USART2,sendbuf,sizeof(sendbuf));
}

void FK_Rx(unsigned char buf[], unsigned char len)
{
	unsigned char a[3];

	
	a[0] = buf[0];
	a[1] = buf[1];
	a[2] = buf[2];
	
	sort(a, 3);
    if((a[0]==a[1]) || (a[1]==a[2]))  //起落架
    {
		if( a[1] == 0x13 )
			g_gearupFK = 1;
		else if( a[1] == 0x14)
			g_gearupFK = 0;
	}
	
	a[0] = buf[3];
	a[1] = buf[4];
	a[2] = buf[5];
	
	sort(a, 3);
    if((a[0]==a[1]) || (a[1]==a[2]))  //放伞 切伞 
    {
		if( a[1] == 0x15 )
		{
			FK_fangsan = 1;
		}
		else if( a[1] == 0x16)
		{
			FK_qiesan = 1;
		}
	}
	
	a[0] = buf[6];
	a[1] = buf[7];
	a[2] = buf[8];
	
	sort(a, 3);
    if((a[0]==a[1]) || (a[1]==a[2]))  //前轮转向
    {
		FK_qianlun = (signed char)a[1];
	}
	
	a[0] = buf[9];
	a[1] = buf[10];
	a[2] = buf[11];
	
	sort(a, 3);
    if((a[0]==a[1]) || (a[1]==a[2]))  //左刹车
    {
		FK_brakeL = a[1];
	}
	
	a[0] = buf[12];
	a[1] = buf[13];
	a[2] = buf[14];
	
	sort(a, 3);
    if((a[0]==a[1]) || (a[1]==a[2]))  //右刹车
    {
		FK_brakeR = a[1];
	}
}

void FK_Rev(void)
{
	unsigned char  rxBufLen, numread;
    unsigned char  idx,  temp[256],buf_use[18];
    unsigned char  tail1, prtail; 

    static unsigned char  head=0, tail=0, buff[256];
    
    rxBufLen = USART1_ReadLen();
    numread = (unsigned char)(rxBufLen>256 ? 256:rxBufLen);
    if (numread) 
    {              
		USART1_ReadIn(temp, numread);
		
		for (idx=0; idx<numread; idx++)
		{
			buff[head++] = temp[idx];
		}
	}
	
	while (((unsigned char)(head-tail)) >= 20) 
    {
		tail1 = (unsigned char)(tail + 1);
		
		if( (buff[tail]==0xEB) && (buff[tail1]==0x90) )
		{
			prtail = (unsigned char)(tail+2);
            for (idx=0; idx<sizeof(buf_use); idx++) 
                buf_use[idx] = buff[prtail++];
			
			if (FK_RxCheckSum(buf_use, sizeof(buf_use)))  //满足校验和
			{
				tail += 20;	
				FK_Rx(buf_use, sizeof(buf_use));
				FK_feedback();
			}
			else
			{
				tail ++; 
			}				
		}
		else
			tail++;
	}
}

void M_Rev(void)
{
	unsigned char  rxBufLen, numread;
    unsigned char  idx,  temp[256],buf_use[18];
    unsigned char  tail1, prtail; 

    static unsigned char  head=0, tail=0, buff[256];
    
    rxBufLen = USART2_ReadLen();
    numread = (unsigned char)(rxBufLen>256 ? 256:rxBufLen);
    if (numread) 
    {              
		USART2_ReadIn(temp, numread);
		
		for (idx=0; idx<numread; idx++)
		{
			buff[head++] = temp[idx];
		}
	}
	
	while (((unsigned char)(head-tail)) >= 20) 
    {
		tail1 = (unsigned char)(tail + 1);
		
		if( (buff[tail]==0x9A) && (buff[tail1]==0xBC) )
		{
			prtail = (unsigned char)(tail+2);
            for (idx=0; idx<sizeof(buf_use); idx++) 
                buf_use[idx] = buff[prtail++];
			
			if (FK_RxCheckSum(buf_use, sizeof(buf_use)))  //满足校验和
			{
				tail += 20;	
				FK_Rx(buf_use, sizeof(buf_use));
				
			}
			else
			{
				tail ++; 
			}				
		}
		else
			tail++;
	}
}



