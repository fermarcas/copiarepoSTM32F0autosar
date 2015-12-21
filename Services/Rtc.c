/**
 ******************************************************************************
 * @file    template.c
 * @author  Max Mustermann
 * @version V1.0.0
 * @date    dd.MM.yyy
 * @brief   TODO
 ******************************************************************************
 */
 
#include "stm32f0xx_rtc.h"
#include "stm32f0xx_conf.h"
#include "Services/Rtc.h"

/****************************************************************
 * Type definitions
 ***************************************************************/

/****************************************************************
 * Macros
 ***************************************************************/

#define RTC_CLOCK_SOURCE_LSE /* LSE used as RTC source clock */

/****************************************************************
 * Function declarations
 ***************************************************************/
static void Rtc_Config(void);

/****************************************************************
 * Global variables
 ***************************************************************/

RTC_InitTypeDef RTC_InitStructure;

__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

/****************************************************************
 * User functions
 ***************************************************************/

/****************************************************************
 * Internal functions/callbacks
 ***************************************************************/

/****************************************************************
 * Static functions
 ***************************************************************/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Rtc_Init( void )
{
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
  { 
    /* RTC configuration */
    Rtc_Config();
    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    /* Check on RTC init */
    if (RTC_Init(&RTC_InitStructure) == ERROR)
     {
    //  printf("\n\r /!\\***** RTC Prescaler Config failed ********/!\\ \n\r");
    }
    /* Configure the time register */
    //RTC_TimeRegulate();
   }
    //else
    //{
    /* Check if the Power On Reset flag is set */
    //if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    //{
    //printf("\r\n Power On Reset occurred....\n\r");
    //}
    /* Check if the Pin Reset flag is set */
    //else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    //{
    //printf("\r\n External Reset occurred....\n\r");
    //}
    //printf("\r\n No need to configure RTC....\n\r");
    
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);
    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();
   
}

/**
* @brief Configure the RTC peripheral by selecting the clock source.
* @param None
* @retval None
*/
static void Rtc_Config(void)
{
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
 
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }
  /* Select LSE as the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  SynchPrediv = 0xFF; /* Prescaler 255 for LSE 32.768 kHz*/
  AsynchPrediv = 0x7F; /* Prescaler 127 for LSE 32.768 kHz*/
  
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);
  
  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
  
  /* Enable The TimeStamp */
  RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, ENABLE);
}


