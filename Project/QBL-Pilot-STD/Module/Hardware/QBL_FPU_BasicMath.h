#ifndef __QBL_FPU_Basic_H__
#define __QBL_FPU_Basic_H__

#include <stdint.h>

/* Note:input and output memeroy can be same */

float* FPU_BasicMath_test(void);

#define sin_f32 arm_sin_f32

#define cos_f32 arm_cos_f32

float sqrt_f32(float x);

void MatrixClear(float *Amat_original, uint8_t Amat_rows, uint8_t Amat_cols);

void MatrixCopy(float *Amat_original, uint8_t Amat_rows, uint8_t Amat_cols, float *ATmat_result);

void MatrixAdd(float *Amat_original, uint8_t Amat_rows, uint8_t Amat_cols,
               float *Bmat_original, uint8_t Bmat_rows, uint8_t Bmat_cols,
               float *AaddBmat_result, uint8_t AaddBmat_rows, uint8_t AaddBmat_cols);

void MatrixSub(float *Amat_original, uint8_t Amat_rows, uint8_t Amat_cols,
               float *Bmat_original, uint8_t Bmat_rows, uint8_t Bmat_cols,
               float *AsubBmat_result, uint8_t AsubBmat_rows, uint8_t AsubBmat_cols);

void MatrixGetE(float* Amat_original, uint8_t n);

void MatrixMultiScale(float *Amat_original, uint8_t Amat_rows, uint8_t Amat_cols,
                      float Scale, float *result);

void MatrixMultiply(float *Amat_original, uint8_t Amat_rows, uint8_t Amat_cols,
                    float *Bmat_original, uint8_t Bmat_rows, uint8_t Bmat_cols,
                    float *ABmat_result, uint8_t ABmat_rows, uint8_t ABmat_cols);

void MatrixTranspose(float *Amat_original, uint8_t Amat_rows, uint8_t Amat_cols,
                     float *ATmat_result, uint8_t ATmat_rows, uint8_t ATmat_cols);

void MatrixInverse(float *Amat_original, uint8_t Amat_n,
                   float *AImat_result, uint8_t AImat_n);

void MatrixInverse2(float* fMatrixA, float* fMatrixB);


#endif

