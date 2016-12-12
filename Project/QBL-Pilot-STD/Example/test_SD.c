#include "QBL_Type.h"
#include "QBL_USART1.h"
#include "QBL_SD.h"
#include "SDcard.h"
#include "LED.h"
#include "ff.h"
#include "diskio.h"
#include <stdio.h>


static uint8_t SD_Write_test(void)
{
	FATFS fs;
	FIL fdst;
	UINT bw;
	BYTE textFileBuffer[] = "\rQBL SD card experiment";

	FRESULT fresult;
	int res;

	{
		fresult = f_mount(&fs, "", 0);// Mount a logical drive
		if (fresult != FR_OK) {
			printf("mount fail!\n");
			return 0;
		}
	}

	{
		res = f_open(&fdst, "Test.txt", FA_WRITE | FA_OPEN_EXISTING);//open existed file
		if(res != FR_OK) {
			res = f_open(&fdst, "Test.txt", FA_WRITE | FA_CREATE_NEW);//new file
		}

		if (res == FR_OK) {
			res = f_write(&fdst, textFileBuffer, sizeof(textFileBuffer), &bw);
			res = f_lseek(&fdst, f_size(&fdst));//move the read/write pointer

			f_close(&fdst);
		}else {
			printf("make the file fail!\n");
			return 0;
		}
	}

	{		
		res = f_mkdir("QBL_Dir");

		res = f_open(&fdst, "/QBL_Dir/Test.txt", FA_WRITE | FA_OPEN_EXISTING);//open existed file
		if(res != FR_OK)
			res = f_open(&fdst, "/QBL_Dir/Test.txt", FA_WRITE | FA_CREATE_NEW);//new file
		if (res == FR_OK) {
			res = f_write(&fdst , textFileBuffer, sizeof(textFileBuffer), &bw);
			res = f_lseek(&fdst, f_size(&fdst)); 
			f_close(&fdst);
		}else {
			printf("mkdir the floder file fail!\n");
			return 0;
		}
	}

	{
		fresult  = f_mount(NULL, "", 1);// Unmount a logical drive
		if (fresult != FR_OK) {
			printf("umount fail!\n");
			return 0;
		}
	}

	printf("test mkdir successfully\n");
	
	return 1;
}

static uint8_t SD_Read_test(void)
{
	FRESULT fresult;
	FATFS fs;
	int res;
	FIL fdst;
	{
		fresult = f_mount(&fs, "", 0);// Mount a logical drive
		if (fresult != FR_OK) {
			printf("mount fail!\n");
			return 0;
		}
	}

	{
		res = f_open(&fdst, "Test.txt", FA_READ | FA_OPEN_EXISTING);
		if (res != FR_OK){
			printf("the file can not find!\n");
			return 0;
		}
	}

	{
		/* Read all lines and display it */
		char line[100]; /* Line buffer */
    while (f_gets(line, sizeof line, &fdst))
        printf("%s\n", line);
	}

	f_close(&fdst);

	{
		fresult  = f_mount(NULL, "", 0);// Unmount a logical drive
		if (fresult != FR_OK) {
			printf("umount fail!\n");
			return 0;
		}
	}

	printf("reading the file successful\n");

	return 1;
}

void test_SD_main(void)
{
	QBL_SDIO_Init();
	QBL_USART1_Init();

	printf("test SDcard!\n");

	uint32_t status = SD_Init();
	if (SD_OK!=status) {
		printf("status=%d\n", status);
		while(1) {;}
	}

	SD_Write_test();
	SD_Read_test();

	printf("test SDcard finished!\n");
}
