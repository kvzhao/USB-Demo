/*
 * main.h
 *
 *  Created on: 10 jul 2012
 *      Author: BenjaminVe
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f4_discovery.h"
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* TIM2 Autoreload and Capture Compare register values */
#define TIM_ARR                          (uint16_t)1999
#define TIM_CCR                          (uint16_t)1000

/* MEMS Microphone SPI Interface
#define SPI_SCK_PIN                   GPIO_Pin_10
#define SPI_SCK_GPIO_PORT             GPIOB
#define SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define SPI_SCK_SOURCE                GPIO_PinSource10
#define SPI_SCK_AF                    GPIO_AF_SPI2

#define SPI_MOSI_PIN                  GPIO_Pin_3
#define SPI_MOSI_GPIO_PORT            GPIOC
#define SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define SPI_MOSI_SOURCE               GPIO_PinSource3
#define SPI_MOSI_AF                   GPIO_AF_SPI2
*/
/* Exported macro ------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x
#define MAX(a,b)       (a < b) ? (b) : a
/* Exported functions ------------------------------------------------------- */
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
void timing_handler();
#endif /* MAIN_H_ */
