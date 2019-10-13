#ifndef BINARIZATION_H
#define BINARIZATION_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void threshold(unsigned char* src, unsigned char* out, uint_fast16_t width, uint_fast16_t height);


#ifdef __cplusplus
}
#endif

#endif /*BINARIZATION_H*/
