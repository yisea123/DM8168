/****************************************************************/
/* Name :                                                                   				*/
/*       main_cycle		                      								*/
/* Function:                                                               				*/
/*       main_cycle     INT4-INT6                                 					*/
/*  Copyright 2008 by .                           	*/
/*  All rights reserved  .                   		*/ 
/*	Designed by: 										*/
/*   Modified by : 		                                                            */
/****************************************************************/

#include "..\\include\\main_dsp.h"

void interrupt4(void)
{  
	NCAreaPulseToServo_LPMEN1();		
}


void interrupt64(void)
{
	
}

void main_cycle(void)
{
    	while(1)
	{
		Timer0.GenericCounter++;
		if(1)
		{
			static INT32U OutCount=0;
			static INT32U ledtimerstart;
			INT32U ledtimernow;
			INT32U ledtimerDelta;
			INT32U ClockGetPeriod;
			INT32U ClockGetTickPeriod;
			INT32U ClockGetTimerHandle;

			Timer_Object *timer;

			if(1)
			{
				INT40U cpuCounts, timerCounts;
				Types_FreqHz timerfreq, cpufreq;
				INT32U ratio;
				INT32U period;
				INT32U count;

				/* get CPU frequency */
				BIOS_getCpuFreq(&cpufreq);
				cpuCounts = (cpufreq.hi * (1 < 0xffffffff)) + cpufreq.lo;

				/* get Timer frequency */
				//Timer_getFreq(timer0, &timerfreq);
				timerCounts = (timerfreq.hi * (1 < 0xffffffff)) + timerfreq.lo;

				ratio = cpuCounts/timerCounts;
				//period = insts / ratio;
				//count = ratio - (insts % ratio);

				if (period == 0) 
				{
					period = 1;
				}
			}


			if(1)
			{
			
				static INT32U timerCountStart, timerCountEnd, tsCountStart, tsCountEnd;
	    			static INT32U deltaTs, deltaCnt;
					
				Types_FreqHz timerFreq, timestampFreq;
				
				Timestamp_getFreq(&timestampFreq);
				
				//timerCountEnd = Timer_getCount(&timer);
				tsCountEnd = Timestamp_get32();

				deltaTs = tsCountEnd - tsCountStart;
				deltaCnt = timerCountEnd - timerCountStart;

				tsCountStart = tsCountEnd;
				timerCountStart = timerCountEnd;
			}


			ledtimernow=Clock_getTicks();// Time in Clock ticks();
			//ClockGetPeriod = Clock_getPeriod(Clock_Object *obj);
			ClockGetTickPeriod = Clock_getTickPeriod();
			ClockGetTimerHandle = Clock_getTimerHandle();
		
			ledtimerDelta = ledtimernow-ledtimerstart;

			ledtimerstart=ledtimernow;

			//Log_info2("Entering Hwi pwr time = %d,Deltatime = %d\n",ledtimernow,ledtimerDelta);
			
			if((ledtimernow-ledtimerstart)>=500)
			{
				//CPLD_LEDflash(0);
				ledtimerstart=ledtimernow;
				 //Log_info0("-->dsp main_cycle");
			}
		}

		static int Led3FlashCounter=0;  	
		if(Led3FlashCounter < 3000)
		{
			WR_MEM_16(0x110008DC,0xffff);
			WR_MEM_16(0x110008E0,0xffff);
			WR_MEM_32((0x4804C000 + 0x13C),0);
			Led3FlashCounter++;						
		}
		else if(Led3FlashCounter < 6000)
		{
			WR_MEM_16(0x110008DC,0);
			WR_MEM_16(0x110008E0,0);
			WR_MEM_32((0x4804C000 + 0x13C),0xFE00);
			Led3FlashCounter++;
		}	
		else
		{
			Led3FlashCounter = 0;			
			//Log_info_MACH_Position();
		}
		//Log_info0("-->dsp main_cycle");
		OverallSign.Int4IdleSign_1++;
		OverallSign.Int4IdleSign_2++;
		OverallSign.Int4IdleSign_3++;
		//OverallSign.Int4IdleSign_4++;
		//for test
		test_all();

		#if 0
		if(OverallSign.Int4IdleSign_3>30)
		{			
			////IRQ_ENABLE(IRQNUM_PLUSE4);	
			//IRQ_ENABLE(IRQNUM_PLUSE3);		
			//*(GPIO_IRQStatus.GPIO1_IRQStatus_0) = (0x1<<17);			//GPIO1_0	//int66 pulse_interpolation gpio1_0 pin=gpio1_0_bit17;DSP  INT 66
			//*(GPIO_IRQStatus.GPIO1_IRQStatus_RAW_0) = (0x1<<17);		//GPIO1_0
			////*(GPIO_IRQStatus.GPIO1_IRQStatus_1) = (0x1<<19);			//GPIO1_1	int67 reserved gpio1_1 pin=gpio1_1_bit19;DSP  INT 67
			////*(GPIO_IRQStatus.GPIO1_IRQStatus_RAW_1) = (0x1<<19);		//GPIO1_1	int67 reserved gpio1_1 pin=gpio1_1_bit19;DSP  INT 67
		}	
		#endif
		#if 1 
		if(OverallSign.Int4IdleSign_4>30)
		{			
			//IRQ_ENABLE(IRQNUM_PLUSE2);
			IRQ_ENABLE(IRQNUM_PLUSE1);
			*(GPIO_IRQStatus.GPIO0_IRQStatus_0) = (0x1<<24);			//GPIO0_0 int64   reserved  gpio0 pin=gpio0_bit24;DSP  INT 64
			*(GPIO_IRQStatus.GPIO0_IRQStatus_RAW_0) = (0x1<<24);		//GPIO0_0 int64   reserved  gpio0 pin=gpio0_bit24;DSP  INT 64
			//*(GPIO_IRQStatus.GPIO0_IRQStatus_1) = (0x1<<23);			//GPIO0_1 int65 pulse_interpolation  gpio0_1 pin=gpio0_1 bit23; DSP INT 65
			//*(GPIO_IRQStatus.GPIO0_IRQStatus_RAW_1) = (0x1<<23);		//GPIO0_1 int65 pulse_interpolation  gpio0_1 pin=gpio0_1 bit23; DSP INT 65
		}
		#endif
		
		if(1)
		{
			CPLD_Servo_OUTWrite();
			CPLD_Servo_INRead();
			
			//CPLD_GPIO_OUTwrite();	
			Read_AD_In();
			CPLD_GPIO_INRead();
		}

		if(1)
		{
			SearchReference();
			SearchReference_Beta();
			Read_AbsoluteEncoderValue_PSO_Axis();
			get_ABS_coordinate();
			get_PMT_Value();
			CPLD_EncoderJudge();	//判断编码器有无报警
		}
   
		if(Error.MainErrorSign==False)
		{
			if(OverallSign.NCSign != NOCODERUN)
			{
				NC_Run_Gcode(); 
			}
			if(OverallSign.NCSign ==STDCODERUN)
			{
				STD_check_track_run_over();
			}
		
		}
		else
		{
			//�д���ʱ��岹������
			InterpolationBuffer_1[0] = InterpolationBuffer_1[OverallSign.InterpolationRunCnt_1%INTERPOLATIONBUFLENGTH];
			OverallSign.InterpolationSendCnt_1 = 0;
			OverallSign.InterpolationRunCnt_1 = 0;
			InterpolationBuffer_2[0] = InterpolationBuffer_2[OverallSign.InterpolationRunCnt_2%INTERPOLATIONBUFLENGTH];
			OverallSign.InterpolationSendCnt_2 = 0;
			OverallSign.InterpolationRunCnt_2 = 0;
		}
    		
		Dsp_Com();        
		Task_sleep(2);
		OverallSign.InterpolationSendCnt_1 = OverallSign.InterpolationSendCnt_1;
	}
}



void Log_info_MACH_Position(void)
{
	Log_info1("Axis1.MACH_Position = %d\n", Encoder_Axis1.MACH_PositionValue);
	Log_info1("Axis2.MACH_Position = %d\n", Encoder_Axis2.MACH_PositionValue);
	Log_info1("Axis3.MACH_Position = %d\n", Encoder_Axis3.MACH_PositionValue);
	Log_info1("Axis4.MACH_Position = %d\n", Encoder_Axis4.MACH_PositionValue);
	Log_info1("Axis5.MACH_Position = %d\n", Encoder_Axis5.MACH_PositionValue);
	Log_info1("Axis6.MACH_Position = %d\n", Encoder_Axis6.MACH_PositionValue);
	Log_info1("Axis7.MACH_Position = %d\n", Encoder_Axis7.MACH_PositionValue);
	Log_info1("Axis8.MACH_Position = %d\n", Encoder_Axis8.MACH_PositionValue);
	Log_info1("Axis9.MACH_Position = %d\n", Encoder_Axis9.MACH_PositionValue);
	Log_info1("Axis10.MACH_Position = %d\n", Encoder_Axis10.MACH_PositionValue);
	Log_info1("Axis11.MACH_Position = %d\n", Encoder_Axis11.MACH_PositionValue);
	Log_info1("Axis12.MACH_Position = %d\n", Encoder_Axis12.MACH_PositionValue);
	Log_info1("Axis13.MACH_Position = %d\n", Encoder_Axis13.MACH_PositionValue);
	Log_info1("Axis14.MACH_Position = %d\n", Encoder_Axis14.MACH_PositionValue);
	Log_info1("Axis15.MACH_Position = %d\n", Encoder_Axis15.MACH_PositionValue);
	Log_info1("Axis16.MACH_Position = %d\n", Encoder_Axis16.MACH_PositionValue);
	Log_info1("Axis17.MACH_Position = %d\n", Encoder_Axis17.MACH_PositionValue);
	Log_info1("Axis18.MACH_Position = %d\n", Encoder_Axis18.MACH_PositionValue);
	Log_info1("Axis19.MACH_Position = %d\n", Encoder_Axis19.MACH_PositionValue);
	Log_info1("Axis20.MACH_Position = %d\n", Encoder_Axis20.MACH_PositionValue);
	Log_info1("Axis21.MACH_Position = %d\n", Encoder_Axis21.MACH_PositionValue);
	Log_info1("Axis22.MACH_Position = %d\n", Encoder_Axis22.MACH_PositionValue);
	Log_info1("Axis23.MACH_Position = %d\n", Encoder_Axis23.MACH_PositionValue);

}

