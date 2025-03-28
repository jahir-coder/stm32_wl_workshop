/*!
 * \file      subghz_phy_app.c
 *
 * \brief     Ping-Pong implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
/**
  ******************************************************************************
  *
  *          Portions COPYRIGHT 2020 STMicroelectronics
  *
  * @file    subghz_phy_app.c
  * @author  MCD Application Team
  * @brief   Application of the SubGHz_Phy Middleware
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "stm32_timer.h"
#include "sys_app.h"
#include "subghz_phy_app.h"
#include "radio.h"
#include "stm32_seq.h"
#include "utilities_def.h"
#include "app_version.h"
#include "adc_if.h"
#include <stdlib.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  RX_DONE,
  RX_TO,
  RX_ERR,
  TX_START,
  TX_DONE,
  RX_START,
  TX_TO,
} States_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

static uint16_t BufferSize = BUFFER_SIZE;
static uint8_t Buffer[BUFFER_SIZE];

static States_t State = RX_START;

static int8_t RssiValue = 0;
static int8_t SnrValue = 0;

static uint32_t WatchDogRx = WATCHDOG_RX_PERIOD;

/* Led Timers objects*/
static  UTIL_TIMER_Object_t timerLedTx;
static  UTIL_TIMER_Object_t timerLedRx;
static  UTIL_TIMER_Object_t timerLedError;
static  UTIL_TIMER_Object_t timerOneSecond;

/* RF collisions avoidance */
bool isChannelFree = true;

/* Radio events function pointer */
static RadioEvents_t RadioEvents;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/*!
 * @brief  Function executed on when led timer elapses
 * @param  LED context
 * @retval none
 */
//static void OnledEvent(void *context);
static void OnLedTxEvent(void *context);
static void OnLedRxEvent(void *context);
static void OnLedErrorEvent(void *context);
static void OnOneSecondElapsedEvent(void *context);

/*!
 * @brief Function to be executed on Radio Tx Done event
 * @param  none
 * @retval none
 */
static void OnTxDone(void);

/*!
 * @brief Function to be executed on Radio Rx Done event
 * @param  payload sent
 * @param  payload size
 * @param  rssi
 * @param  snr
 * @retval none
 */
static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

/*!
 * @brief Function executed on Radio Tx Timeout event
 * @param  none
 * @retval none
 */
static void OnTxTimeout(void);

/*!
 * @brief Function executed on Radio Rx Timeout event
 * @param  none
 * @retval none
 */
static void OnRxTimeout(void);

/*!
 * @brief Function executed on Radio Rx Error event
 * @param  none
 * @retval none
 */
static void OnRxError(void);

/*!
 * @brief PingPong state machine implementation
 * @param  none
 * @retval none
 */
static void BaseStation_Process(void);

/* USER CODE BEGIN PFP */

static uint32_t powInt(uint32_t base, uint32_t exp);

/* USER CODE END PFP */

/* Exported functions ---------------------------------------------------------*/
void BaseStation_Init(void)
{
  /* USER CODE BEGIN SubghzApp_Init_1 */

  /* USER CODE END SubghzApp_Init_1 */

  /* LED TX timer */
  UTIL_TIMER_Create(&timerLedTx,0xFFFFFFFF,UTIL_TIMER_ONESHOT,OnLedTxEvent,NULL);
  UTIL_TIMER_SetPeriod(&timerLedTx, LED_PERIOD_MS);

  /* LED ERROR timer */
  UTIL_TIMER_Create(&timerLedError,0xFFFFFFFF,UTIL_TIMER_ONESHOT,OnLedErrorEvent,NULL);
  UTIL_TIMER_SetPeriod(&timerLedError,LED_ERROR_PERIOD_MS);

  /* LED RX timer */
  UTIL_TIMER_Create(&timerLedRx,0xFFFFFFFF,UTIL_TIMER_ONESHOT,OnLedRxEvent,NULL);
  UTIL_TIMER_SetPeriod(&timerLedRx,LED_PERIOD_MS);

  /* Software watchdog of RX process */
  UTIL_TIMER_Create(&timerOneSecond,0xFFFFFFFF,UTIL_TIMER_PERIODIC,OnOneSecondElapsedEvent,NULL);
  UTIL_TIMER_SetPeriod(&timerOneSecond,1000);
  UTIL_TIMER_Start(&timerOneSecond);

  /* ADC measurement */
  SYS_InitMeasurement();

  /* Radio initialization */
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.RxDone = OnRxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxTimeout = OnRxTimeout;
  RadioEvents.RxError = OnRxError;

  Radio.Init(&RadioEvents);
  srand(Radio.Random());

#if (( USE_MODEM_LORA == 1 ) && ( USE_MODEM_FSK == 0 ))
  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, LORA_IQ_INVERSION_ON, TX_TIMEOUT_VALUE);

  Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                    0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

  Radio.SetMaxPayloadLength(MODEM_LORA, BUFFER_SIZE);

#elif (( USE_MODEM_LORA == 0 ) && ( USE_MODEM_FSK == 1 ))

  Radio.SetTxConfig(MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0,
                    FSK_DATARATE, 0,
                    FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, 0, TX_TIMEOUT_VALUE);

  Radio.SetRxConfig(MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
                    0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
                    0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, true,
                    0, 0, false, true);

  Radio.SetMaxPayloadLength(MODEM_FSK, BUFFER_SIZE);

#else
#error "Please define a frequency band in the sys_conf.h file."
#endif /* USE_MODEM_LORA | USE_MODEM_FSK */

  Radio.SetChannel(RF_FREQUENCY);

  SYS_LED_Init(SYS_LED_BLUE);
  SYS_LED_Init(SYS_LED_GREEN);
  SYS_LED_Init(SYS_LED_RED);

  Radio.Rx(RX_TIMEOUT_VALUE);

  UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_BaseStation_Process), 0, BaseStation_Process);

  APP_LOG(TS_OFF, VLEVEL_L, "\r\nBase Station start -> ");
  APP_LOG(TS_OFF, VLEVEL_L, "RF=%uMHz , SF=%u",RF_FREQUENCY,LORA_SPREADING_FACTOR);
  APP_LOG(TS_OFF,VLEVEL_L," , CS=%ddBm",RF_CHANNEL_FREE_RSSI_TRESHOLD);
  APP_LOG(TS_OFF, VLEVEL_L, "\r\n");

  /* USER CODE BEGIN SubghzApp_Init_2 */

  /* USER CODE END SubghzApp_Init_2 */
}

/* USER CODE BEGIN EF */

/* USER CODE END EF */

/* Private functions ---------------------------------------------------------*/
static void BaseStation_Process(void)
{
int16_t i;
bool isChannelFree = true;
uint32_t backoffTime,carrierSenseTime;
int16_t rssi;

  switch (State)
  {
    case RX_START:
      Radio.SetChannel(RF_FREQUENCY);
	  Radio.Rx(RX_TIMEOUT_VALUE);
	  APP_LOG(TS_OFF,VLEVEL_L, "RX...\r\n\r\n");
	  break;

	case RX_DONE:
	  WatchDogRx = WATCHDOG_RX_PERIOD;
	  UTIL_TIMER_Start(&timerLedRx);
	  SYS_LED_On(SYS_LED_GREEN);
	  if (Buffer[0]<0x20)  /* Print byte format data -> NODE_ID < 0x20 */
	  {
	    APP_LOG(TS_OFF,VLEVEL_L,"\r\nNode ID: %02X | ",Buffer[0]);
		APP_LOG(TS_OFF,VLEVEL_L,"RX data: TS=%ddegC , BAT=%d",(int8_t)(Buffer[1]),Buffer[2]);
		APP_LOG(TS_OFF,VLEVEL_L," , FSentCnt=%u , FAckCnt=%u\r\n",(uint16_t)(Buffer[3]<<8)+Buffer[4],(uint16_t)(Buffer[5]<<8)+Buffer[6]);
		//APP_LOG(TS_OFF,VLEVEL_L,"RSSI: %ddBm\r\n",RssiValue);
		//APP_LOG(TS_OFF,VLEVEL_L,"SNR : %ddB\r\n\r\n",SnrValue);
		APP_LOG(TS_OFF,VLEVEL_L,"\r\n");
	  }
	  else  /* Print ASCII format data -> NODE_ID/Buffer[0] >= 0x20 */
	  {
	    APP_LOG(TS_OFF,VLEVEL_L,"RX CHAR: ");
		for (i=0;i<BufferSize;i++)
		{
		  if (Buffer[i]>=0x20) { APP_LOG(TS_OFF,VLEVEL_L,"%c",Buffer[i]); }
		  else { APP_LOG(TS_OFF,VLEVEL_L,"."); }
		}
		APP_LOG(TS_OFF,VLEVEL_L,"\r\n\r\n");
	  }
	  State = TX_START;
	  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_BaseStation_Process), CFG_SEQ_Prio_0);
	  break;

	case RX_TO:
	  //APP_LOG(TS_ON, VLEVEL_L, "RX timeout\r\n\r\n");
	  State = RX_START;
	  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_BaseStation_Process), CFG_SEQ_Prio_0);
	  break;

	case RX_ERR:
	  UTIL_TIMER_Start(&timerLedError);
	  SYS_LED_On(SYS_LED_RED);
	  APP_LOG(TS_ON, VLEVEL_L, "RX error\r\n\r\n");
	  State = RX_START;
	  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_BaseStation_Process), CFG_SEQ_Prio_0);
	  break;

	case TX_START:
	  i = 0;
	  Buffer[i++] = 'A';
	  Buffer[i++] = 'C';
	  Buffer[i++] = 'K';
	  BufferSize = i;
	  /* RF collisions avoidance and TX section */
	  Radio.SetChannel(RF_FREQUENCY);
	  Radio.Rx(0);
	  HAL_Delay(Radio.GetWakeupTime());
	  for (i=0;i<RF_CHANNEL_FREE_TRIALS_MAX;i++)
	  {
	  //APP_LOG(TS_ON,VLEVEL_L,"RF Channel Sensing #%u ...",i+1);
	  isChannelFree = true;
	  carrierSenseTime = UTIL_TIMER_GetCurrentTime();
	  while( UTIL_TIMER_GetElapsedTime(carrierSenseTime) < RSSI_SENSING_TIME)
	  {
	    rssi = Radio.Rssi(MODEM_LORA);
		if (rssi > RF_CHANNEL_FREE_RSSI_TRESHOLD) { isChannelFree = false; break; }
	  }
	  carrierSenseTime = UTIL_TIMER_GetElapsedTime(carrierSenseTime);
	  //APP_LOG(TS_OFF,VLEVEL_L," CS=%ddBm , CStime=%ums\r\n",rssi,carrierSenseTime);
	  if (isChannelFree)
	  {
	    break; //RF collisions avoidance loop
	  }
	  else
	  {
	    if (i<RF_CHANNEL_FREE_TRIALS_MAX-1)
		{
		  backoffTime = CS_BACKOFF_TIME_UNIT * (1 + (rand() % powInt(2,i+1)));
		  APP_LOG(TS_ON,VLEVEL_L,"RF channel is busy, next attempt after %ums...\r\n",backoffTime);
		  HAL_Delay(backoffTime);
		}
	  }
	}
    /* TX data over the air */
   if (isChannelFree)
   {
     Radio.SetChannel(RF_FREQUENCY);
	 HAL_Delay(Radio.GetWakeupTime() + TCXO_WORKAROUND_TIME_MARGIN);
	 Radio.Send(Buffer,BufferSize);
	 //APP_LOG(TS_ON,VLEVEL_L,"TX ACK\r\n");
	}
    else
    {
	  UTIL_TIMER_Start(&timerLedError);
	  SYS_LED_On(SYS_LED_RED);
	  APP_LOG(TS_ON, VLEVEL_L, "RF channel: %uHz is BUSY\r\n\r\n",RF_FREQUENCY);
	}
	break;

	case TX_DONE:
	  UTIL_TIMER_Start(&timerLedTx);
	  SYS_LED_On(SYS_LED_BLUE);
	  State = RX_START;
	  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_BaseStation_Process), CFG_SEQ_Prio_0);
	  break;

	case TX_TO:
	  UTIL_TIMER_Start(&timerLedError);
	  SYS_LED_On(SYS_LED_RED);
	  APP_LOG(TS_ON, VLEVEL_L, "TX timeout\r\n");
	  State = RX_START;
	  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_BaseStation_Process), CFG_SEQ_Prio_0);
	  break;

	default:
	  break;
  }
}

static void OnTxDone(void)
{
  State = TX_DONE;
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_BaseStation_Process), CFG_SEQ_Prio_0);
}

static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
  BufferSize = size;
  memcpy(Buffer, payload, BufferSize);
  RssiValue = rssi;
  SnrValue = snr;

  State = RX_DONE;
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_BaseStation_Process), CFG_SEQ_Prio_0);
}

static void OnTxTimeout(void)
{
  State = TX_TO;
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_BaseStation_Process), CFG_SEQ_Prio_0);
}

static void OnRxTimeout(void)
{
  State = RX_TO;
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_BaseStation_Process), CFG_SEQ_Prio_0);
}

static void OnRxError(void)
{
  State = RX_ERR;
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_BaseStation_Process), CFG_SEQ_Prio_0);
}

static void OnLedTxEvent(void *context)
{
  SYS_LED_Off(SYS_LED_BLUE) ;
}

static void OnLedRxEvent(void *context)
{
  SYS_LED_Off(SYS_LED_GREEN) ;
}

static void OnLedErrorEvent(void *context)
{
  SYS_LED_Off(SYS_LED_RED) ;
}

static void OnOneSecondElapsedEvent(void *context)
{
  if (WatchDogRx>0)
  {
    WatchDogRx--;
  }
  else
  {
	NVIC_SystemReset();
  }
}

static uint32_t powInt(uint32_t base, uint32_t exp)
{
    uint32_t result = 1;
    while(exp) { result *= base; exp--; }
    return result;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
