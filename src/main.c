#include "main.h"
#include "core_cm4.h"
#include "stm32f4xx_conf.h"
#include <string.h>

// Macros
#define f_tell(fp)		((fp)->fptr)
#define BUTTON			(GPIOA->IDR & GPIO_Pin_0)

// Variables
volatile uint32_t		time_var1, time_var2;
USB_OTG_CORE_HANDLE		USB_OTG_Core;
USBH_HOST				USB_Host;
RCC_ClocksTypeDef		RCC_Clocks;
volatile int			enum_done = 0;
FIL file;
char* nfile = "Hacking.txt";


// Private function prototypes
static void AudioCallback(void *context,int buffer);
static void play_mp3(char* filename);
static FRESULT put_file_directory (const char* path, unsigned char seek);

/*
 * Main function. Called when startup code is done with
 * copying memory and setting up clocks.
 */
int main(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;

	// SysTick interrupt each 1ms
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	// GPIOD Peripheral clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    // Initialize LEDS (12:Green/13:Orange/14:Red/15:Blue)
	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Initialize USB Host Library
	USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_Callbacks);

	for(;;) {
		USBH_Process(&USB_OTG_Core, &USB_Host);

		if (enum_done >= 2) {
			enum_done = 0;
			put_file_directory("", 0);
		}
	}
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

static FRESULT put_file_directory (const char* path, unsigned char seek) {
	FRESULT res;
	FILINFO fno;
	DIR dir;
	char *fn; /* This function is assuming non-Unicode cfg. */
	char buffer[200];
#if _USE_LFN
	static char lfn[_MAX_LFN + 1];
	fno.lfname = lfn;
	fno.lfsize = sizeof(lfn);
#endif


	res = f_opendir(&dir, path); /* Open the directory */
	if (res == FR_OK) {
        const char* nfile = "Hacking.txt";
        res = f_open(&file, nfile, FA_CREATE_NEW | FA_WRITE);
        GPIO_SetBits(GPIOD, GPIO_Pin_13);
        if (res == FR_OK) {
            char *wrt_content = "MemoryStick Hacked!";
            f_printf(&file, wrt_content);
            f_close(&file);
            GPIO_SetBits(GPIOD, GPIO_Pin_15);
        }
    }
        /*
		for (;;) {
			res = f_readdir(&dir, &fno); // Read a directory item
			if (res != FR_OK || fno.fname[0] == 0)
                break; // Break on error or end of dir
            }
			if (fno.fname[0] == '.') continue; // Ignore dot entry
#if _USE_LFN
			fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			fn = fno.fname;
#endif
			if (fno.fattrib & AM_DIR) { // It is a directory

			} else { // It is a file.
				//sprintf(buffer, "%s/%s", path, fn);
				}
		}
    */
	return res;
}

static void play_mp3(char* filename) {
	unsigned int br, btr;
	FRESULT res;

	//bytes_left = FILE_READ_BUFFER_SIZE;
	//read_ptr = file_read_buffer;

        char *wrt_content = "MemoryStick Hacked!";
        f_printf(nfile, wrt_content);
        GPIO_SetBits(GPIOD, GPIO_Pin_14);

        f_close(&file);
	if (FR_OK == f_open(&file, filename, FA_OPEN_EXISTING | FA_WRITE)) {

		// Read ID3v2 Tag
//		Mp3ReadId3V2Tag(&file, szArtist, sizeof(szArtist), szTitle, sizeof(szTitle));

		// Fill buffer
//		f_read(&file, file_read_buffer, FILE_READ_BUFFER_SIZE, &br);/
//        f_write(&file, wrt_content, );
        f_close(&file);
		}
}

/*
 * Called by the audio driver when it is time to provide data to
 * one of the audio buffers (while the other buffer is sent to the
 * CODEC using DMA). One mp3 frame is decoded at a time and
 * provided to the audio driver.
 */

/*
 * Dummy function to avoid compiler error
 */
void _init(void) {

}

/*
 * Called by the SysTick interrupt
 */
void TimingDelay_Decrement(void) {
	if (time_var1) {
		time_var1--;
	}
	time_var2++;
}

/*
 * Delay a number of systick cycles (1ms)
 */
void Delay(volatile uint32_t nTime) {
	time_var1 = nTime;
	while(time_var1){};
}
