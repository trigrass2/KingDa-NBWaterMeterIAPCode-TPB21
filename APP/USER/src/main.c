/*********************************************************************************
//概述：
//作者：随风飘跃     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "include.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/
/*********************************************************************************
外部变量声明区
*********************************************************************************/
/*********************************************************************************
私有变量定义区
*********************************************************************************/ 
/*********************************************************************************
测试变量定义区
*********************************************************************************/
/*********************************************************************************
内部函数定义区
*********************************************************************************/
/*********************************************************************************
功能代码定义区
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void main(void)
{ 
  disableInterrupts();                                      //关总中断
  RCC_Configuration();
  GPIO_Configuration();
  Rtc_Config();
  Set_Alarm();
  Pulse_Acquire_Config();
  
  IWDG_INIT(); 
  enableInterrupts();                                       //开总中断
/////////////////////////////////////////////////////////    
  Check_Version();                                      //验证APP版本
  APPValid = APP_VALID;
  Save_APP_Valid();                                     //保存APP程序有效标志
  Read_Upgrade_Info();                                  //读取升级信息
  Read_ACUM_Flow(ADD_FLOW_ADD,&Cal.Water_Data);         //读取当前累积流量
  Read_Meter_Parameter();                               //读取水表参数
  Read_History_Save_Index();                            //读取历史数据保存索引
  
  TPB21_Power_On();

  
  while (1)
  {
//    RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
//    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
    
    IWDG_ReloadCounter();//重载看门狗计数器
    
    Magnetic_Interference_Detection();
      
    //上报失败2次则复位
    if( TPB21.FailTimes >= 2 )
    { 
      Save_Add_Flow(ADD_FLOW_ADD,&Cal.Water_Data);       //保存当前水量
      Run_BLD();
//      WWDG->CR = 0x80;  //看门狗复位
    }
    
    if(MeterParameter.DeviceStatus == SLEEP)     //设备进入睡眠状态
    {
      Sleep();
    } 
    else
    {
      Sys_Timer_Process();
      TPB21_Process(); 
    }
  }
}

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void Sleep(void)
{
  HistoryData.ReadIndex = 0;
  TPB21.Report_Bit = 0;
  TPB21.Start_Process = TPB21_POWER_DOWN;
  MeterParameter.DeviceRunTiming = 0;

//  CLK_HaltConfig(CLK_Halt_FastWakeup,ENABLE);   //快速唤醒后时钟为HSI  
//  PWR_FastWakeUpCmd(ENABLE);                    //开启电源管理里的快速唤醒  
  PWR_UltraLowPowerCmd(ENABLE);                 //使能电源的低功耗模式          //开启后内部参考电压获取值变小
//  CLK_HSICmd(DISABLE);                          //关闭内部高速时钟

  halt();
}
/**
*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void IWDG_INIT(void)  //看门狗初始化
{ 
  IWDG_Enable();//启动看门狗
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//允许写预分频器和重载寄存器
  IWDG_SetPrescaler(IWDG_Prescaler_256);//设置IWDG预分频值
  IWDG_SetReload(0xFF);//设置重载值1.7s：(255+1)*256/38K = 1.72s
  IWDG_ReloadCounter();//重载计数器
}
/*********************************************************************************
 Function:      //
 Description:   //验证APP版本
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void Check_Version(void)
{
  unsigned char APPVersion[16] = {0};
  
  Read_Version(APP_VERSION_ADD,APPVersion);
  if(strstr((char const*)APPVersion,"NB_APP_V") == NULL)
  {
    Save_Version(APP_VERSION_ADD,"NB_APP_V2.0");
  }
}
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

