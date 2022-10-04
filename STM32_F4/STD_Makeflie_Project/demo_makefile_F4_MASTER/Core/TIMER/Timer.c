	/*
	 * TIMER.h
	 *
	 *  Created on: May 3, 2016
	 *      Author: AnhTuan
	 */

#include "include.h"

/* Public function prototypes -----------------------------------------------*/

void Creat_Pulse(int pulse , uint32_t frequency ,  uint16_t joint);

/* Private function prototypes -----------------------------------------------*/
static void Config_GPIO_Driver_Pulse(void);
static void ConfigTimer(void);
static void Pull_Pulse(int pulse , int32_t frequency ,int32_t accel,  int16_t joint); 
static void TIM9_Config(void);
static void TIM4_Config(void);
static void timer3_config(void);
static void  Process_Pulse_Generator(void);

/*******************
* Parameter for Timer 
* Create Pluse for Driver
*
*
*********************/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/


//* Public variables -------------------------------------------------------*/
volatile int16_t pointer_openloop=1;// kieu volatile cho cac ham cac co the tham chieu thong qua tu khoa extern va thay doi gia tri bien
//volatile int32_t Servo_Pluse_buffer[buffer_point][6] =
//{{ 0, 0, 0, 0, 0, 0} ,
//{150000,150000, 150000, 150000, 150000, 150000},
//{ 450000, 450000,110000, 0, 150000, -500000} ,
//{ -850000, -850000, 110000, -40000, -90000, 500000} ,
//{ 100000, 100000,110000, 0, 50000, 500000} ,
//{ 0, 0, -70000, 0, 150000, 0}, 
// { 150000, 150000,-130000, 0, 150000, 0} ,
//{ 0, 0, -130000, -30000, -1000, 0} ,
//{ 100000, 100000,110000, 0, 50000, 500000} ,
//{ 450000, 450000,110000, 0, 500000, -500000}};

volatile int32_t Servo_Pluse_buffer[buffer_point][6] =
{{ 0, 0, 0, 0, 0, 0} ,
{ 0,150000, 150000, 150000, 150000, 15000},
{ 100000, 200000,200000, 200000, 200000, 20000} ,
{ 200000, 100000, 100000, 100000, 100000, 100000} ,
{ 200000, 0,0, 0, 0, 0},
{ 0, 100000,100000, 10000, 100000, 100000},
{ -250000, 25000,25000, 250000, 25000, 25000},
{ 0, 0,0, 0, 0, 0}} ;


volatile bool Servo_Flag[6]={ false,false,false,false,false,false}; // for purpose Motor moving or idle
volatile bool Flag_Robot_Run=true;
volatile int32_t ServoPulse[6] ={ 0, 0, 0, 0, 0, 0 };

//* Private variables ------------------------------------------------------*/
static GPIO_TypeDef * ServoBase_Pin_Pulse[6] = { Servo_Pulse_Port_1, Servo_Pulse_Port_2, Servo_Pulse_Port_3, Servo_Pulse_Port_4, Servo_Pulse_Port_5, Servo_Pulse_Port_6 };
static uint32_t ServoPin_Pulse[6] = { Servo_Pulse_Pin_1, Servo_Pulse_Pin_2, Servo_Pulse_Pin_3, Servo_Pulse_Pin_4, Servo_Pulse_Pin_5, Servo_Pulse_Pin_6 };
//PD12 YELLO PD13 ORGANGE PD14 RED PD15 BLUE 

static GPIO_TypeDef * ServoBase_Pin_Dir[6] ={ Servo_Dir_Port_1, Servo_Dir_Port_2, Servo_Dir_Port_3, Servo_Dir_Port_4, Servo_Dir_Port_5, Servo_Dir_Port_6 };
static uint32_t ServoPin_Dir[6] ={ Servo_Dir_Pin_1, Servo_Dir_Pin_2, Servo_Dir_Pin_3, Servo_Dir_Pin_4, Servo_Dir_Pin_5, Servo_Dir_Pin_6 }; 

/* Private variables ---------------------------------------------------------*/
static int32_t Qei_Pos_temp[6] ={ 0, 0, 0, 0, 0, 0 }; // save old value of position
static int32_t Qei_Speed[6] ={ 0, 0, 0, 0, 0, 0 }; // speed of each joint

static int i ,j ,k ;

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

void Config_Create_Pluse(void)
{

		Config_GPIO_Driver_Pulse();	
		ConfigTimer();
}
void Display_Ram(void)
{
	int i,j;
	 Print(USARTx,"/****************************************/");	
	 Print(USARTx," Possition  in Ram ");	
	 for(j=0;j<buffer_point;j++)
	{
		Print(USARTx,"* Possiton ");
    UARTPutnlong(USARTx,j);	
		USART_SendData(USARTx,(uint16_t)':'); 
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx,(uint16_t)' '); 
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		for(i=0;i<6;i++)
		{			
			UARTPutnlong(USARTx,Servo_Pluse_buffer[j][i]);
		  USART_SendData(USARTx,(uint16_t)' '); 
		  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		}
   }
	 USART_SendData(USARTx,(uint16_t)10);
   while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
}

void TIM7_IRQHandler(void) //interupt Timer7 For push pluse for Driver 
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
				TIM_Cmd(TIM7, DISABLE);					
				TIM_ClearITPendingBit(TIM7, TIM_IT_Update);	
				// chu ki ngat qua lon thi se ko du thoi gian cho Ngat lam xong cong viec 
				//se dan den chua lam xong ma da co ngat tiep theo .Nen phai lam xong moi enable ngat moi
				TIM_Cmd(TIM7, ENABLE);
	}
}

int32_t Get_Qei_Speed(int joint )
{
	return Qei_Speed[joint-1];
}

void ConfigTimer(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the TIM7 gloabal Interrupt */
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_SetPriorityGrouping(3);
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* TIM7 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 168 - 1;  // 84 MHz down to 0.2MHZ
	TIM_TimeBaseStructure.TIM_Prescaler = 1 - 1; // 84 MHz 
	/*
	 84*1/Frequently= TIM_Period
	 840--0.1MHZ
	 420--0.2MHZ
	 280--0.3MHZ
	 210--0.4MHZ
	 168--0.5MHZ
	 84---1MHZ

	 Timer_Period must different 0 
	 Prescaler mean clock Timer =Max soure clock Timer(84MHZ) /Prescaler 
	 period of Timer = clock Timer/ Timer_Period
	 
	 */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	/* TIM IT enable */
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	/* TIM7 enable counter */
	TIM_Cmd(TIM7, ENABLE);
}

void Config_GPIO_Driver_Pulse(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;
  /* GPIOD Periph clock enable */
   RCC_AHB1PeriphClockCmd(Servo_Pulse_Clk_1, ENABLE);
	 RCC_AHB1PeriphClockCmd(Servo_Pulse_Clk_2, ENABLE);
	 RCC_AHB1PeriphClockCmd(Servo_Pulse_Clk_3, ENABLE);
	 RCC_AHB1PeriphClockCmd(Servo_Pulse_Clk_4, ENABLE);
	 RCC_AHB1PeriphClockCmd(Servo_Pulse_Clk_5, ENABLE);
	 RCC_AHB1PeriphClockCmd(Servo_Pulse_Clk_6, ENABLE);
	
	RCC_AHB1PeriphClockCmd(Servo_Dir_Clk_1, ENABLE);
	RCC_AHB1PeriphClockCmd(Servo_Dir_Clk_2, ENABLE);
	RCC_AHB1PeriphClockCmd(Servo_Dir_Clk_3, ENABLE);
	RCC_AHB1PeriphClockCmd(Servo_Dir_Clk_4, ENABLE);
	RCC_AHB1PeriphClockCmd(Servo_Dir_Clk_5, ENABLE);
	RCC_AHB1PeriphClockCmd(Servo_Dir_Clk_6, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//Pin Pulse	
	GPIO_InitStructure.GPIO_Pin = Servo_Pulse_Pin_1;
  GPIO_Init(Servo_Pulse_Port_1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_Pulse_Pin_2;
  GPIO_Init(Servo_Pulse_Port_2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_Pulse_Pin_3;
  GPIO_Init(Servo_Pulse_Port_3, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_Pulse_Pin_4;
  GPIO_Init(Servo_Pulse_Port_4, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_Pulse_Pin_5;
  GPIO_Init(Servo_Pulse_Port_5, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_Pulse_Pin_6;
  GPIO_Init(Servo_Pulse_Port_6, &GPIO_InitStructure);
////Pin Dir	
	GPIO_InitStructure.GPIO_Pin = Servo_Dir_Pin_1;
  GPIO_Init(Servo_Dir_Port_1, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_Dir_Pin_2;
  GPIO_Init(Servo_Dir_Port_2, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_Dir_Pin_3;
  GPIO_Init(Servo_Dir_Port_3, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_Dir_Pin_4;
  GPIO_Init(Servo_Dir_Port_4, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = Servo_Dir_Pin_5;
  GPIO_Init(Servo_Dir_Port_5, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = Servo_Dir_Pin_6; //error
//  GPIO_Init(Servo_Dir_Port_6, &GPIO_InitStructure);
}


 int round_float (float x)
{
 	return (int)(x+0.5);
 }
	void delay_us(uint16_t period)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
		TIM6->PSC=83;
		TIM6->ARR=period-1;
		TIM6->CNT=0;
		TIM6->EGR=1;//update register
		TIM6->SR=0;//clr overflow flag
		TIM6->CR1=1;// enable tim6 
		while (!TIM6->SR);
		TIM6->CR1=0; 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,DISABLE);
			
	}
	void delay_ms(uint16_t period)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
		TIM6->PSC=8399;
		TIM6->ARR=period-1;
		TIM6->CNT=0;
		TIM6->EGR=1;//update register
		TIM6->SR=0;//clr overflow flag
		TIM6->CR1=1;// enable tim6 
		while (!TIM6->SR);
		TIM6->CR1=0; 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,DISABLE);		
	}
	void Delay(__IO uint32_t nCount)
	{
		while(nCount--)
		{
		}
	}





