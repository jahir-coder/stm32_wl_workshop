/**
  ******************************************************************************
  * @file    sgfx_eeprom_if.h
  * @author  MCD Application Team
  * @brief   eeprom interface to the upper module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SGFX_EEPROM_IF_H__
#define __SGFX_EEPROM_IF_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sigfox_types.h"
#include "st_sigfox_api.h"
#include "se_nvm.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
#if defined(__CC_ARM) /* when changing tool chain the EEPROM shall be re-initialized */
  E2P_SET = (uint32_t)0xAA,   /* Set by Keil */
#elif defined(__ICCARM__)
  E2P_SET = (uint32_t) 0xBB,   /* Set by IAR */
#else  /* __GNUC__ */
  E2P_SET = (uint32_t) 0xCC,   /* Set by GCC */
#endif /* __CC_ARM | __ICCARM__ | __GNUC__ */
  E2P_RST = (uint32_t)  0     /* EEPROM has not yet been set or has been erased */
} E2P_flagStatus_t;

typedef enum
{
  E2P_OK = (uint32_t)1,   /* Set by Keil */
  E2P_KO = (uint32_t)0     /* EEPROM has not yet been set or has been erased */
} E2P_ErrorStatus_t;

/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
typedef enum
{
  EE_INIT_ID = 0,
  EE_SE_NVM_0_ID,
  EE_SE_NVM_1_ID,
  EE_SE_NVM_2_ID,
  EE_SE_NVM_3_ID,
  EE_SE_NVM_4_ID,
  EE_MCU_NVM_0_ID,
  EE_MCU_NVM_1_ID,
  EE_MCU_NVM_2_ID,
  EE_MCU_NVM_3_ID,
  EE_RSSI_CAL_ID,
  EE_AT_ECHO_ID,
  EE_TX_POWER_RC1_ID,
  EE_TX_POWER_RC2_ID,
  EE_TX_POWER_RC3A_ID,
  EE_TX_POWER_RC3C_ID,
  EE_TX_POWER_RC4_ID,
  EE_TX_POWER_RC5_ID,
  EE_TX_POWER_RC6_ID,
  EE_TX_POWER_RC7_ID,
  EE_SGFX_RC_ID,
  EE_SGFX_KEYTYPE_ID,
  EE_MACROCH_CONFIG_WORD0_RC2_ID,
  EE_MACROCH_CONFIG_WORD1_RC2_ID,
  EE_MACROCH_CONFIG_WORD2_RC2_ID,
  EE_MACROCH_CONFIG_WORD0_RC3A_ID,
  EE_MACROCH_CONFIG_WORD1_RC3A_ID,
  EE_MACROCH_CONFIG_WORD2_RC3A_ID,
  EE_MACROCH_CONFIG_WORD0_RC3C_ID,
  EE_MACROCH_CONFIG_WORD1_RC3C_ID,
  EE_MACROCH_CONFIG_WORD2_RC3C_ID,
  EE_MACROCH_CONFIG_WORD0_RC4_ID,
  EE_MACROCH_CONFIG_WORD1_RC4_ID,
  EE_MACROCH_CONFIG_WORD2_RC4_ID,
  EE_MACROCH_CONFIG_WORD0_RC5_ID,
  EE_MACROCH_CONFIG_WORD1_RC5_ID,
  EE_MACROCH_CONFIG_WORD2_RC5_ID,
  EE_SGFX_ENCRYPTIONFLAG_ID,
  EE_SGFX_VERBOSELEVEL_ID,
  EE_ID_COUNT,
} e_EE_ID;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/*!
 * @brief Initializes the Eeprom
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  None
 * @retval None
 */
void E2P_Init(void);

/*!
 * @brief Initializes the Eeprom
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  None
 * @retval None
 */
void E2P_RestoreFs(void);

/*!
 * @brief Read Output Power
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  Output Power
 * @retval None
 */
int8_t E2P_Read_Power(sfx_rc_enum_t SgfxRc);

/*!
 * @brief Write Output Power to eeprom
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  Output Power
 * @retval None
 */
void E2P_Write_Power(sfx_rc_enum_t SgfxRc, int8_t power);

/*!
 * @brief Read Region Configuration RC from eeprom
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  None
 * @retval Region Configuration RC
 */
sfx_rc_enum_t E2P_Read_Rc(void);

/*!
 * @brief Write Region Configuration RC from eeprom
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  Region Configuration RC
 * @retval None
 */
void E2P_Write_Rc(sfx_rc_enum_t SgfxRc);

/*!
 * @brief Read Region Configuration words depending on RC
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  RC
 * @param  Configuration words
 * @retval None
 */
void E2P_Read_ConfigWords(sfx_rc_enum_t sfx_rc, sfx_u32 config_words[3]);

/*!
 * @brief Write Region Configuration words depending on RC
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  RC
 * @param  Configuration words
 * @retval None
 */
void E2P_Write_ConfigWords(sfx_rc_enum_t sfx_rc, sfx_u32 config_words[3]);

/*!
 * @brief Read rssi calibration value from eeprom
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  None
 * @retval rssi calibration value
 */
int16_t E2P_Read_RssiCal(void);

/*!
 * @brief Writes rssi calibration value to eeprom
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  rssi calibration value
 * @retval None
 */
void E2P_Write_RssiCal(int16_t rssi_cal);

/*!
 * @brief Read Region At echo setting
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  None
 * @retval at_echo
 */
uint32_t E2P_Read_AtEcho(void);

/*!
 * @brief Write Region At echo setting
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  at_echo
 * @retval None
 */
void E2P_Write_AtEcho(uint32_t at_echo);

/*!
 * @brief Read KeyType setting
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  None
 * @retval KeyType
 */
sfx_key_type_t E2P_Read_KeyType(void);

/*!
 * @brief Write KeyType setting
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  KeyType
 * @retval None
 */
void E2P_Write_KeyType(sfx_key_type_t key_type);

/*!
 * @brief Read EncryptionFlag setting
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  None
 * @retval encryption_flag
 */
uint8_t E2P_Read_EncryptionFlag(void);

/*!
 * @brief Write EncryptionFlag  setting
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  encryption_flag
 * @retval None
 */
void E2P_Write_EncryptionFlag(sfx_u8 encryption_flag);

/*!
 * @brief Read Region SEnvm setting
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  SEnvm
 * @param  len
 * @retval error status
 */
E2P_ErrorStatus_t E2P_Read_SeNvm(sfx_u8 *read_data, uint32_t len);

/*!
 * @brief Write Region SEnvm setting
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  SEnvm
 * @param  len
 * @retval error status
 */
E2P_ErrorStatus_t E2P_Write_SeNvm(sfx_u8 *data_to_write, uint32_t len);

/*!
 * @brief Read MCUnvm setting
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  MCUnvm
 * @param  len
 * @retval error status
 */
E2P_ErrorStatus_t E2P_Read_McuNvm(sfx_u8 *read_data,  uint32_t len);

/*!
 * @brief Write Region MCUnvm setting
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  MCUnvm
 * @param  len
 * @retval error status
 */
E2P_ErrorStatus_t E2P_Write_McuNvm(sfx_u8 *data_to_write, uint32_t len);

/*!
 * @brief Read Region SEnvm setting
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param   None
 * @retval the VL
 */
uint8_t E2P_Read_VerboseLevel(void);

/*!
 * @brief Read verbose levelsetting
 * @note  DEFAULT_FACTORY_SETTINGS are written E2pData
 * @param  verboselevel
 * @retval None
 */
void E2P_Write_VerboseLevel(uint8_t verboselevel);

/* USER CODE BEGIN EFP */
/*Active application*/
/* USER CODE END EFP */

#endif /* __SGFX_EEPROM_IF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
