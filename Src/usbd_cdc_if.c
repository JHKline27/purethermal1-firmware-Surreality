/**
  ******************************************************************************
  * @file    usbd_cdc_if.c
  * @brief   USB CDC Interface file.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"
#include "usbd_cdc.h"
#include "usbd_core.h"
#include "usbd_cdc.h"
#include "usbd_cdc.h"

/* Private variables ---------------------------------------------------------*/
#define APP_RX_DATA_SIZE  64
#define APP_TX_DATA_SIZE  64

uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

USBD_HandleTypeDef *hUsbDevice_0;
extern USBD_HandleTypeDef hUsbDeviceFS;

/* Forward declarations ------------------------------------------------------*/
static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);

USBD_CDC_ItfTypeDef USBD_CDC_fops_FS = {
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS
};

static int8_t CDC_Init_FS(void)
{
  hUsbDevice_0 = &hUsbDeviceFS;
  USBD_CDC_SetTxBuffer(hUsbDevice_0, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(hUsbDevice_0, UserRxBufferFS);
  return (USBD_OK);
}

static int8_t CDC_DeInit_FS(void)
{
  return (USBD_OK);
}

static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* Implement control requests if needed (SET_LINE_CODING, SET_CONTROL_LINE_STATE, etc.) */
  switch (cmd)
  {
    case CDC_SET_LINE_CODING:
      /* pbuf contains the line coding structure */
      break;
    case CDC_GET_LINE_CODING:
      break;
    case CDC_SET_CONTROL_LINE_STATE:
      break;
    default:
      break;
  }
  return (USBD_OK);
}

static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* Application can process received data here. For now, ignore. */
  USBD_CDC_ReceivePacket(hUsbDevice_0);
  return (USBD_OK);
}

uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
  USBD_CDC_SetTxBuffer(hUsbDevice_0, Buf, Len);
  result = USBD_CDC_TransmitPacket(hUsbDevice_0);
  HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
  return result;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
