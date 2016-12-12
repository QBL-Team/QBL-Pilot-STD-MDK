#include "SDcardWriteAndRead.h"

#include "SDcard.h"
#include "ff.h"
#include "diskio.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

void SDcard_reset(uint8_t flag)
{
	static FATFS fs;
	if(flag == 1)
	{
		// Mount a logical drive
		FRESULT fresult;
		fresult = f_mount(&fs, "", 0);
		if (fresult != FR_OK) {
			while(1);
		}
	}
	if(flag == 0)
	{
		// Unmount a logical drive
		FRESULT fresult;
		fresult = f_mount(NULL, "", 0);
		if (fresult != FR_OK) {
			while(1);
		}
	}
}

/* log the debug informations */
void SDcard_log(const char *format, ...)
{
	uint8_t log_buffer[255];
	uint32_t length;

	va_list args;
	va_start(args, format);
	length = vsnprintf((char*)log_buffer, sizeof(log_buffer), (char*)format, args);
	va_end(args);

	static FIL fdst;
	static uint8_t open_flag = 0;
	if(open_flag==0)
	{
		open_flag = 1;

		//if file not exist, new file and open
		f_open(&fdst, "log.txt", FA_WRITE | FA_OPEN_ALWAYS);
	}

	UINT bw;
	f_lseek(&fdst, f_size(&fdst));
	f_write(&fdst, log_buffer, length, &bw);
	f_sync(&fdst);
}

/* omit the charactors(c) of the string */
static bool trim(char *str, char c)
{
	if(str==NULL) return false;

	char *front = str;
	char *last = str;

	/* omit the charactors(c) in the begin */
	while((*front)!='\0')
	{
		if((*front)!=c)
		{
			*last = *front;
			last ++;
		}
		front ++;
	}
	if(last==str)
	{
		return false;
	}
	else
	{
		*last = '\0';
		return true;
	}
}

/* splite the one string to two sub string by charactor(c) */
static bool split(char *str, char c, char p[2][50])
{
	if(str==NULL) return false;

	char *ptr = str;

	while((*ptr)!='\0')
	{
		if((*ptr)==c)
		{
			break;
		}
		ptr ++;
	}

	if((*ptr)=='\0')
	{
		return false;
	}
	else
	{
		int count = 0;
		while(str!=ptr)
		{
			p[0][count++] = *str++;
		}
		p[0][count] = '\0';

		str ++;

		count = 0;
		while((*str)!='\0')
		{
			p[1][count++] = *str++;
		}
		p[1][count] = '\0';
	}

	return true;
}

static char params[50][2][50];
bool SDcard_readParamFile(const char *filename)
{
	FIL fdst;
	uint8_t res;
	res = f_open(&fdst, filename, FA_READ | FA_OPEN_EXISTING);
	if (res != FR_OK)
	{
		return false;
	}
	
	char line[100]; /* Line buffer */
	char lineNum = 0;
	bool ret;
	while (f_gets(line, sizeof(line), &fdst))
	{
		/* omit the space of string */
		ret = trim(line, ' ');
		if(ret==false)
		{
			return false;
		}
		/* omit the tab of string */
		ret = trim(line, '	');
		if(ret==false)
		{
			return false;
		}

		split(line, '=', params[lineNum]);

		lineNum ++;
	}

	f_close(&fdst);
	
	return true;
}

