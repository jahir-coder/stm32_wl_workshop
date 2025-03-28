/*!
 * \file      main.c
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
  * @file    main.c
  * @author  MCD Application Team
  * @brief   this is the main!
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "hw.h"
#include "radio.h"
#include "timeServer.h"
#include "low_power_manager.h"
#include "vcom.h"

#if defined( REGION_AS923 )

#define RF_FREQUENCY                                923000000 // Hz

#elif defined( REGION_AU915 )

#define RF_FREQUENCY                                915000000 // Hz

#elif defined( REGION_CN470 )

#define RF_FREQUENCY                                470000000 // Hz

#elif defined( REGION_CN779 )

#define RF_FREQUENCY                                779000000 // Hz

#elif defined( REGION_EU433 )

#define RF_FREQUENCY                                433000000 // Hz

#elif defined( REGION_EU868 )

#define RF_FREQUENCY                                868000000 // Hz

#elif defined( REGION_KR920 )

#define RF_FREQUENCY                                920000000 // Hz

#elif defined( REGION_IN865 )

#define RF_FREQUENCY                                865000000 // Hz

#elif defined( REGION_US915 )

#define RF_FREQUENCY                                915000000 // Hz

#elif defined( REGION_RU864 )

#define RF_FREQUENCY                                864000000 // Hz

#else
#error "Please define a frequency band in the compiler options."
#endif

#define TX_OUTPUT_POWER                             10        // dBm

#if defined( USE_MODEM_LORA )

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
//  1: 250 kHz,
//  2: 500 kHz,
//  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
//  2: 4/6,
//  3: 4/7,
//  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         5         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false

#elif defined( USE_MODEM_FSK )

#define FSK_FDEV                                    25000     // Hz
#define FSK_DATARATE                                50000     // bps
#define FSK_BANDWIDTH                               50000     // Hz
#define FSK_AFC_BANDWIDTH                           83333     // Hz
#define FSK_PREAMBLE_LENGTH                         5         // Same for Tx and Rx
#define FSK_FIX_LENGTH_PAYLOAD_ON                   false

#else
#error "Please define a modem in the compiler options."
#endif

typedef enum
{
    IDLE,
    RX,
    RX_TIMEOUT,
    RX_ERROR,
    TX_START,
    TX_TIMEOUT,
}States_t;

#define RX_TIMEOUT_VALUE                            2000
#define BUFFER_SIZE                                 32 // Define the payload size here
#define LED_PERIOD_MS               200

#define LEDS_OFF   do{ \
                   LED_Off( LED_BLUE ) ;   \
                   LED_Off( LED_RED ) ;    \
                   LED_Off( LED_GREEN1 ) ; \
                   LED_Off( LED_GREEN2 ) ; \
                   } while(0) ;


uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];
uint32_t i;

States_t State = IDLE;

int8_t RssiValue = 0;
int8_t SnrValue = 0;

/* Led Timers objects*/
static TimerEvent_t timerLedRx;     //GREEN led
static TimerEvent_t timerLedTx;	    //BLUE led			
static TimerEvent_t timerLedErr;    //RED led	

/* Private function prototypes -----------------------------------------------*/
/*!
 * Radio events function pointer
 */
static RadioEvents_t RadioEvents;

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
void OnTxDone(void);

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout(void);

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout(void);

/*!
 * \brief Function executed on Radio Rx Error event
 */
void OnRxError(void);

/*!
 * \brief Function executed on when led timer elapses
 */
static void OnTimerLedRxEvent(void *context);
static void OnTimerLedTxEvent(void *context);
static void OnTimerLedErrEvent(void *context);
/**
 * Main application entry point.
 */
int main( void )
{
	
  HAL_Init( );
  
  SystemClock_Config( );
  
  DBG_Init( );

  HW_Init( );  
  
  /* Led Timers*/
  TimerInit(&timerLedRx, OnTimerLedRxEvent);   
  TimerSetValue(&timerLedRx,LED_PERIOD_MS);
  TimerInit(&timerLedTx, OnTimerLedTxEvent);   
  TimerSetValue(&timerLedTx,LED_PERIOD_MS);
  TimerInit(&timerLedErr, OnTimerLedErrEvent);   
  TimerSetValue(&timerLedErr,LED_PERIOD_MS);
	
  LPM_SetOffMode(LPM_APPLI_Id , LPM_Disable );

  // Radio initialization
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.RxDone = OnRxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxTimeout = OnRxTimeout;
  RadioEvents.RxError = OnRxError;

  Radio.Init( &RadioEvents );

  Radio.SetChannel( RF_FREQUENCY );

#if defined( USE_MODEM_LORA )

  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                 LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000000 );
    
  Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

#elif defined( USE_MODEM_FSK )

  Radio.SetTxConfig( MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0,
                                  FSK_DATARATE, 0,
                                  FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
                                  true, 0, 0, 0, 3000000 );
    
  Radio.SetRxConfig( MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
                                  0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
                                  0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, true,
                                  0, 0,false, true );

#else
    #error "Please define a frequency band in the compiler options."
#endif
																	
  PRINTF("\n\rBase Station start -> ");	
  PRINTF("RF=%uMHz , SF=%u\r\n",RF_FREQUENCY,LORA_SPREADING_FACTOR);
  PRINTF("B-L072Z-LRWAN1 Discovery board , I-CUBE-LRWAN v1.3.1\r\n\r\n");																				
                                  
  Radio.Rx( RX_TIMEOUT_VALUE );
                                  
  while( 1 )
  {
    switch( State )
    {
      case RX:
      case RX_TIMEOUT:
      case RX_ERROR:
        PRINTF("RX...\r\n");
        HAL_Delay(10);
        Radio.SetChannel(RF_FREQUENCY);
        Radio.Rx( RX_TIMEOUT_VALUE );
        State = IDLE;
        break;
      case TX_TIMEOUT:
        PRINTF("TX timeout");
        State = IDLE;
        break;
      case TX_START:
        i = 0;
	Buffer[i++] = 'A';
	Buffer[i++] = 'C';
	Buffer[i++] = 'K';
	BufferSize = i;
        HAL_Delay(10);
        Radio.SetChannel(RF_FREQUENCY);	
	Radio.Send(Buffer,BufferSize);
        State = IDLE;
        break;
      case IDLE:
        default:
        break;
    }
    
    DISABLE_IRQ( );
    /* if an interupt has occured after __disable_irq, it is kept pending 
     * and cortex will not enter low power anyway  */
    if (State == IDLE)
    {
#ifndef LOW_POWER_DISABLE
      LPM_EnterLowPower( );
#endif
    }
    ENABLE_IRQ( );
       
  }
}


void OnTxDone( void )
{
  Radio.Sleep( );
  State = RX;
  LED_On(LED_BLUE);
  TimerStart(&timerLedTx);
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
  Radio.Sleep( );
  BufferSize = size;
  memcpy( Buffer, payload, BufferSize );
  RssiValue = rssi;
  SnrValue = snr;
  State = RX;
  
  LED_On(LED_GREEN);
  TimerStart(&timerLedRx);

  if (Buffer[0]<0x20)  /* Print byte format data -> NODE_ID < 0x20 */
  {
    PRINTF("\r\nNode ID: %02X | ",Buffer[0]);
    PRINTF("RX data: TS=%ddegC , BAT=%d",(int8_t)(Buffer[1]),Buffer[2]);
    PRINTF(" , FSentCnt=%u , FAckCnt=%u\r\n",(uint16_t)(Buffer[3]<<8)+Buffer[4],(uint16_t)(Buffer[5]<<8)+Buffer[6]);
    PRINTF("RSSI: %ddBm\r\n",RssiValue);
    PRINTF("SNR : %ddB\r\n\r\n",SnrValue);
  }
  else  /* Print ASCII format data -> NODE_ID/Buffer[0] >= 0x20 */
  {
    PRINTF("RX CHAR: ");
    for (i=0;i<BufferSize;i++)
    {
      if (Buffer[i]>=0x20) { PRINTF("%c",Buffer[i]); }
      else { PRINTF("."); }
     }
     PRINTF("\r\n\r\n");
   }
    State = TX_START;
		
}

void OnTxTimeout( void )
{
    State = RX;
    LED_On(LED_RED2);
    TimerStart(&timerLedErr);
}

void OnRxTimeout( void )
{
    Radio.Sleep( );
    State = RX_TIMEOUT;
}

void OnRxError( void )
{
    Radio.Sleep( );
    State = RX_ERROR;
    LED_On(LED_RED2);
    TimerStart(&timerLedErr);
}

static void OnTimerLedRxEvent( void *context)
{
  LED_Off(LED_GREEN) ;   
}

static void OnTimerLedTxEvent( void *context)
{
  LED_Off(LED_BLUE) ;   
}

static void OnTimerLedErrEvent( void *context)
{
  LED_Off(LED_RED2) ;   
}
