/**
  ******************************************************************************
  * @file    st_device.h
  * @author  AMG/IPC Application Team
  * @brief   This file contains all the functions prototypes for metrology
  @verbatim
  @endverbatim

  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ST_DEVICE_H
#define __ST_DEVICE_H

#ifdef __cplusplus
 extern "C" {
#endif
 
   
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#define UART_XFER_STPM3X 1
/* Communication */
#define SPI_STPM                    SPI1
#define SPI_STPM_SPEED              SPI_BAUDRATEPRESCALER_16
#define SPI_TIMEOUT		    10

#define USART_STPM                  UART4
#define USART_SPEED		    9600
#define USART_TIMEOUT		    10
   
/* GPIOs */
#define CS_GPIO_type                GPIOA
#define CS_GPIO_pin                 GPIO_PIN_4
#define SYN_GPIO_type               GPIOA
#define SYN_GPIO_pin                GPIO_PIN_9
#define LED2_GPIO_type              GPIOC
#define LED2_GPIO_pin               GPIO_PIN_7
   
#define UART4_TX_Pin GPIO_PIN_10
#define UART4_TX_GPIO_Port GPIOC
#define UART4_RX_Pin GPIO_PIN_11
#define UART4_RX_GPIO_Port GPIOC   
   

/**** No Enable pin used in case of IPE023 but if you can use it */
/* used here only to mimic the use case */
#define EN_GPIO_type                GPIOA
#define EN_GPIO_pin                 GPIO_PIN_3
    
   
/*----------------------------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


#ifdef __cplusplus
}
#endif

#endif /* __ST_DEVICE_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
