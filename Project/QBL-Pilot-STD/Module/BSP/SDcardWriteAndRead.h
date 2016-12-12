#ifndef __SDCARDWRITEANDREAD_H__
#define __SDCARDWRITEANDREAD_H__

#include <stdint.h>

void SDcard_reset(uint8_t flag);

void SDcard_log(const char *format, ...);

#endif
