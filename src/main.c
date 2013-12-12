#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "main.h"

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

#include "stm32f4_discovery_lis302dl.h"
// Private variables
volatile uint32_t time_var1, time_var2;
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

// Private function prototypes
void Delay(volatile uint32_t nCount);
void init();
void calculation_test();

// MEMS
LIS302DL_InitTypeDef  LIS302DL_InitStruct;
LIS302DL_FilterConfigTypeDef LIS302DL_FilterStruct;
__IO int8_t X_Offset, Y_Offset, Z_Offset  = 0x00;
uint8_t Buffer[6];

int main(void) {
	init();

	/*
	 * Disable STDOUT buffering. Otherwise nothing will be printed
	 * before a newline character or when the buffer is flushed.
	 */
	setbuf(stdout, NULL);

	calculation_test();

	for(;;) {
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		Delay(500);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		Delay(500);
	}

	return 0;
}

void calculation_test() {

	for(;;) {

		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		Delay(5);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		//Delay(500);

		time_var2 = 0;

        LIS302DL_Read(Buffer, LIS302DL_OUT_X_ADDR, 6);
        X_Offset = Buffer[0];
        Y_Offset = Buffer[2];
        Z_Offset = Buffer[4];

        printf("%i",X_Offset);
        Delay(10);
	}
}

void init() {
	GPIO_InitTypeDef  GPIO_InitStructure;

	// ---------- SysTick timer -------- //
	if (SysTick_Config(SystemCoreClock / 1000)) {
		// Capture error
		while (1){};
	}

	// ---------- GPIO -------- //
	// GPIOD Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// ------------- USB -------------- //
	USBD_Init(&USB_OTG_dev,
	            USB_OTG_FS_CORE_ID,
	            &USR_desc,
	            &USBD_CDC_cb,
	            &USR_cb);

    // MEMS configuration
    LIS302DL_InitStruct.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
    LIS302DL_InitStruct.Output_DataRate = LIS302DL_DATARATE_400; // 100 or 400 Hz
    LIS302DL_InitStruct.Axes_Enable = LIS302DL_XYZ_ENABLE;
    LIS302DL_InitStruct.Full_Scale = LIS302DL_FULLSCALE_2_3;
    LIS302DL_InitStruct.Self_Test = LIS302DL_SELFTEST_NORMAL;
    LIS302DL_Init(&LIS302DL_InitStruct);

    // Required delay for the MEMS Accelerometre: Turn-on time = 3/Output data Rate = 3/100 = 30ms
    Delay(30);

    // MEMS High Pass Filter configuration
    LIS302DL_FilterStruct.HighPassFilter_Data_Selection = LIS302DL_FILTEREDDATASELECTION_OUTPUTREGISTER;
    LIS302DL_FilterStruct.HighPassFilter_CutOff_Frequency = LIS302DL_HIGHPASSFILTER_LEVEL_1;
    LIS302DL_FilterStruct.HighPassFilter_Interrupt = LIS302DL_HIGHPASSFILTERINTERRUPT_1_2;
    LIS302DL_FilterConfig(&LIS302DL_FilterStruct);
}

/*
 * Called from systick handler
 */
void timing_handler() {
	if (time_var1) {
		time_var1--;
	}
	time_var2++;
}

/*
 * Delay a number of systick cycles (1ms)
 */
void Delay(volatile uint32_t nCount) {
	time_var1 = nCount;
	while(time_var1){};
}

/**
  * @brief  MEMS accelerometre management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t LIS302DL_TIMEOUT_UserCallback(void)
{
  /* MEMS Accelerometer Timeout error occured during Test program execution */
    /* Timeout error occured for SPI TXE/RXNE flags waiting loops.*/
    //Fail_Handler();
  /* MEMS Accelerometer Timeout error occured during Demo execution */
  return 0;
}

/*
 * Dummy function to avoid compiler error
 */
void _init() {

}

