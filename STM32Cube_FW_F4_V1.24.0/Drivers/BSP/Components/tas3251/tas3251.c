/*
 * TAS3251.c
 *
 *  Created on: 24 mars 2019
 *      Author: louis
 */
#include <stdint.h>

#include "stm32f4xx_hal.h"
#include <stm32f4xx_hal_gpio.h>
#include "stm32f4_discovery_audio.h"
#include "tas3251_registers.h"
#include "tas3251.h"

/* AUDIO IO functions */
void      AUDIO_IO_Init(void);
void      AUDIO_IO_DeInit(void);
void      AUDIO_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value);
uint8_t   AUDIO_IO_Read(uint8_t Addr, uint8_t Reg);

/**
  * @brief  Writes/Read a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address
  * @param  Value: Data to be written
  * @retval None
  */
static uint8_t CODEC_IO_Write(uint8_t Addr, uint8_t Reg, uint8_t Value)
{
  uint32_t result = 0;

  AUDIO_IO_Write(Addr, Reg, Value);

#ifdef VERIFY_WRITTENDATA
  /* Verify that the data has been correctly written */
  result = (AUDIO_IO_Read(Addr, Reg) == Value)? 0:1;
#endif /* VERIFY_WRITTENDATA */

  return result;
}

/******************************* I2C Routines**********************************/

/**
  * @brief Initializes the audio codec and the control interface.
  * @param DeviceAddr: Device address on communication Bus.
  * @param OutputDevice: can be OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO .
  * @param Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t tas3251_Init(uint16_t DeviceAddr, uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{
	  uint32_t ret = AUDIO_OK;

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOC_CLK_ENABLE();

	  /* Initialize the Control interface of the Audio Codec */
	  AUDIO_IO_Init();

	  /* startup sequence according to SLASEG6A – MAY 2018 – REVISED NOVEMBER 2018 §8.3.11.1 */

	  /* 1. Apply power to DAC_DVDD, DAC_AVDD, GVDD_x, and PVDD_x */
	  /* 2. Apply I2S or TDM clocks to the device to enable the internal system clocks */
	  /* 3. Mute the left and right DAC channels */
	  CODEC_IO_Write(DeviceAddr, TAS3251_REG_PAGE, 0x00);
	  HAL_Delay(1);
	  CODEC_IO_Write(DeviceAddr, TAS3251_REG_BOOK, 0x00);
	  HAL_Delay(1);

	  /* mute right and left */
	  //CODEC_IO_Write(DeviceAddr, TAS3251_REG_MUTE, TAS3251_MUTE_LEFT|TAS3251_MUTE_RIGHT);
	  //HAL_Delay(1);

	  /* standby */
	  //CODEC_IO_Write(DeviceAddr, TAS3251_REG_STANDBY, TAS3251_STANDBY_STANDBY);
	  //uint8_t status = AUDIO_IO_Read(DeviceAddr, TAS3251_REG_STANDBY);
	  //HAL_Delay(1);

	  /* enable */
	  //CODEC_IO_Write(DeviceAddr, TAS3251_REG_STANDBY, TAS3251_STANDBY_ENABLE);
	  //HAL_Delay(1);

	  /* 5.1. set digital volume */
	  CODEC_IO_Write(DeviceAddr, TAS3251_REG_LEFT_DIGITAL_VOLUME, (uint8_t)135);
	  HAL_Delay(1);
	  CODEC_IO_Write(DeviceAddr, TAS3251_REG_RIGHT_DIGITAL_VOLUME, (uint8_t)135);
	  HAL_Delay(1);

	  /* dsp out of standby */
	  //CODEC_IO_Write(DeviceAddr, TAS3251_REG_STANDBY, TAS3251_STANDBY_DSPR);
	  //HAL_Delay(1);

	  /* enable */
	  CODEC_IO_Write(DeviceAddr, TAS3251_REG_STANDBY, TAS3251_STANDBY_ENABLE);
	  HAL_Delay(1);

	  /* unmute right and left */
	  //CODEC_IO_Write(DeviceAddr, TAS3251_REG_MUTE, 0);

	  return ret;
}

void tas3251_DeInit(void)
{}

uint32_t tas3251_ReadID(uint16_t DeviceAddr)
{
	return 0;
}

uint32_t tas3251_Play(uint16_t DeviceAddr, uint16_t* pBuffer, uint16_t Size)
{
	return 0;
}

uint32_t tas3251_Pause(uint16_t DeviceAddr)
{
	return 0;
}

uint32_t tas3251_Resume(uint16_t DeviceAddr)
{
	return 0;
}

uint32_t tas3251_Stop(uint16_t DeviceAddr, uint32_t CodecPdwnMode)
{
	return 0;
}

uint32_t tas3251_SetVolume(uint16_t DeviceAddr, uint8_t Volume)
{
	return 0;
}

uint32_t tas3251_SetFrequency(uint16_t DeviceAddr, uint32_t AudioFreq)
{
	return 0;
}

uint32_t tas3251_SetMute(uint16_t DeviceAddr, uint32_t Cmd)
{
	return 0;
}

uint32_t tas3251_SetOutputMode(uint16_t DeviceAddr, uint8_t Output)
{
	return 0;
}

uint32_t tas3251_Reset(uint16_t DeviceAddr)
{
	return 0;
}

/* Audio codec driver structure initialization */
AUDIO_DrvTypeDef tas3251_drv =
{
  tas3251_Init,
  tas3251_DeInit,
  tas3251_ReadID,

  tas3251_Play,
  tas3251_Pause,
  tas3251_Resume,
  tas3251_Stop,

  tas3251_SetFrequency,
  tas3251_SetVolume,
  tas3251_SetMute,
  tas3251_SetOutputMode,
  tas3251_Reset,
};
