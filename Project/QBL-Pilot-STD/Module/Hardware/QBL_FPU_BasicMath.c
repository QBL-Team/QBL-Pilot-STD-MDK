#include "arm_math.h"
#include "QBL_FPU_BasicMath.h"
#include <stdlib.h>

float A_f32[2*2] = {4.0, 0.0, 0.0, 2.0};
float AI_f32[2*2];
arm_status status;
float* FPU_BasicMath_test(void)
{
	arm_matrix_instance_f32 A;
	arm_matrix_instance_f32 AI;

	uint8_t srcRows, srcColumns;

	srcRows = 2;
	srcColumns = 2;
	arm_mat_init_f32(&A, srcRows, srcColumns, (float32_t *)A_f32);

	srcRows = 2;
	srcColumns = 2;
  float *tempAI = (float *)malloc(sizeof(float)*srcRows*srcColumns);
	arm_mat_init_f32(&AI, srcRows, srcColumns, (float32_t *)tempAI);

	status = arm_mat_inverse_f32(&A, &AI);
	
	memcpy(AI_f32, tempAI, sizeof(float)*srcRows*srcColumns);
	free(tempAI);

	if (status != ARM_MATH_SUCCESS)
	{
		while(1);
	}

	return AI_f32;
}

//====================================================
float sqrt_f32(float x)
{
	 float result = 0;

	 arm_sqrt_f32(x, &result);

	 return result;
}

//====================================================
void MatrixClear(float *Amat_original, uint8_t Amat_rows, uint8_t Amat_cols)
{
    memset(Amat_original, 0, sizeof(float)*Amat_rows*Amat_cols);
}

void MatrixCopy(float *Amat_original, uint8_t Amat_rows, uint8_t Amat_cols, float *ATmat_result)
{
    memcpy(ATmat_result, Amat_original, sizeof(float)*Amat_rows*Amat_cols);
}

void MatrixAdd(float *Amat_original, uint8_t Amat_rows, uint8_t Amat_cols,
               float *Bmat_original, uint8_t Bmat_rows, uint8_t Bmat_cols,
               float *AaddBmat_result, uint8_t AaddBmat_rows, uint8_t AaddBmat_cols)
{
	if (   Amat_rows!=Bmat_rows
	    || Amat_cols!=Bmat_cols
	    || Amat_rows!=AaddBmat_rows
	    || Amat_cols!=AaddBmat_cols
	  )
	{
		while(1);
	}

  float *tempAaddBmat_result = (float *)malloc(sizeof(float)*AaddBmat_rows*AaddBmat_cols);

	arm_matrix_instance_f32 A;      
	arm_matrix_instance_f32 B;
	arm_matrix_instance_f32 AaddB;    

	arm_status status;

	arm_mat_init_f32(&A, Amat_rows, Amat_cols, (float32_t *)Amat_original);
	arm_mat_init_f32(&B, Bmat_rows, Bmat_cols, (float32_t *)Bmat_original);
	arm_mat_init_f32(&AaddB, Amat_rows, Amat_cols, tempAaddBmat_result);

	status = arm_mat_add_f32(&A, &B, &AaddB);

	memcpy(AaddBmat_result, tempAaddBmat_result, sizeof(float)*AaddBmat_rows*AaddBmat_cols);
	free(tempAaddBmat_result);

	if (status != ARM_MATH_SUCCESS)
	{
		while(1);
	}
}

void MatrixSub(float *Amat_original, uint8_t Amat_rows, uint8_t Amat_cols,
               float *Bmat_original, uint8_t Bmat_rows, uint8_t Bmat_cols,
               float *AsubBmat_result, uint8_t AsubBmat_rows, uint8_t AsubBmat_cols)
{
  if (   Amat_rows!=Bmat_rows
	    || Amat_cols!=Bmat_cols
	    || Amat_rows!=AsubBmat_rows
	    || Amat_cols!=AsubBmat_cols
	  ) 
	{
		while(1);
	}

	float *tempAsubBmat_result = (float *)malloc(sizeof(float)*AsubBmat_rows*AsubBmat_cols);

	arm_matrix_instance_f32 A;
	arm_matrix_instance_f32 B;
	arm_matrix_instance_f32 AsubB;

	arm_status status;

	arm_mat_init_f32(&A, Amat_rows, Amat_cols, (float32_t *)Amat_original);
	arm_mat_init_f32(&B, Bmat_rows, Bmat_cols, (float32_t *)Bmat_original);
	arm_mat_init_f32(&AsubB, Amat_rows, Amat_cols, tempAsubBmat_result);
	
	status = arm_mat_sub_f32(&A, &B, &AsubB);

	memcpy(AsubBmat_result, tempAsubBmat_result, sizeof(float)*AsubBmat_rows*AsubBmat_cols);
	free(tempAsubBmat_result);
	
	if (status != ARM_MATH_SUCCESS)
	{
		while(1);
	}
}

void MatrixGetE(float* Amat_original, uint8_t n)
{
	uint8_t index_i;
	uint8_t index_j;

	for (index_i=0;index_i<n;index_i++)
	{
		for (index_j=0;index_j<n;index_j++)
		{
			if (index_i==index_j)
				*(Amat_original+index_i*n+index_j) = 1;
			else
				*(Amat_original+index_i*n+index_j) = 0;
		}
	}
}

void MatrixMultiScale(float *Amat_original, uint8_t Amat_rows, uint8_t Amat_cols,
                      float Scale, float *result)
{
	float *tempresult = (float *)malloc(sizeof(float)*Amat_rows*Amat_cols);

	uint8_t i, j;
	for(i=0;i<Amat_rows;i++)
	{
		for(j=0;j<Amat_cols;j++)
		{
			Amat_original[i*Amat_cols+j] *= Scale;
		}
	}

	memcpy(result, tempresult, sizeof(float)*Amat_rows*Amat_cols);

	free(tempresult);
}

void MatrixMultiply(float *Amat_original, uint8_t Amat_rows, uint8_t Amat_cols,
                    float *Bmat_original, uint8_t Bmat_rows, uint8_t Bmat_cols,
                    float *ABmat_result, uint8_t ABmat_rows, uint8_t ABmat_cols)
{
	if (  Amat_cols!=Bmat_rows
	    ||Amat_rows!=ABmat_rows
	    ||Bmat_cols!=ABmat_cols
	  )
	{
		while(1);
	}

	float *tempABmat_result = (float *)malloc(sizeof(float)*ABmat_rows*ABmat_cols);

	arm_matrix_instance_f32 A;
	arm_matrix_instance_f32 B;
	arm_matrix_instance_f32 AB;

	arm_status status;

	arm_mat_init_f32(&A, Amat_rows, Amat_cols, (float32_t *)Amat_original);
	arm_mat_init_f32(&B, Bmat_rows, Bmat_cols, (float32_t *)Bmat_original);
	arm_mat_init_f32(&AB, Amat_rows, Bmat_cols, (float32_t *)ABmat_result);

	status = arm_mat_mult_f32(&A, &B, &AB);

	memcpy(ABmat_result, tempABmat_result, sizeof(float)*ABmat_rows*ABmat_cols);
	free(tempABmat_result);

	if (status != ARM_MATH_SUCCESS)
	{
		while(1);
	}
}

void MatrixTranspose(float *Amat_original, uint8_t Amat_rows, uint8_t Amat_cols,
                     float *ATmat_result, uint8_t ATmat_rows, uint8_t ATmat_cols)
{
	if (  Amat_rows!=ATmat_cols
	    ||Amat_cols!=ATmat_rows
	  )
	{
		while(1);
	}

	float *tempATmat_result = (float *)malloc(sizeof(float)*ATmat_rows*ATmat_cols);

	arm_matrix_instance_f32 A;
	arm_matrix_instance_f32 AT;
	
	arm_status status;	

	arm_mat_init_f32(&A, Amat_rows, Amat_cols, (float32_t *)Amat_original);
	arm_mat_init_f32(&AT, Amat_cols, Amat_rows, (float32_t *)ATmat_result);

	status = arm_mat_trans_f32(&A, &AT);

	memcpy(ATmat_result, tempATmat_result, sizeof(float)*ATmat_rows*ATmat_cols);
	free(tempATmat_result);

	if (status != ARM_MATH_SUCCESS)
	{
		while(1);
	}
}

void MatrixInverse(float *Amat_original, uint8_t Amat_n,
                   float *AImat_result, uint8_t AImat_n)
{
	if (Amat_n!=AImat_n)
	{
		while(1);
	}

	float *tempAImat_result = (float *)malloc(sizeof(float)*AImat_n*AImat_n);

	arm_matrix_instance_f32 A;
	arm_matrix_instance_f32 AI;

	arm_status status;

	arm_mat_init_f32(&A, Amat_n, Amat_n, (float32_t *)Amat_original);
	arm_mat_init_f32(&AI, Amat_n, Amat_n, (float32_t *)AImat_result);

	status = arm_mat_inverse_f32(&A, &AI);

	memcpy(AImat_result, tempAImat_result, sizeof(float)*AImat_n*AImat_n);
	free(tempAImat_result);

	if (status != ARM_MATH_SUCCESS)
	{
		while(1);
	}
}

float MatrixDet2(float* fMatrixA)
{
	return (*fMatrixA)*(*(fMatrixA+3))-(*(fMatrixA+1))*(*(fMatrixA+2));
}

void MatrixInverse2(float* fMatrixA, float* fMatrixB)
{
	float fmatrix_vlaue = 0.0;
	float tempfMatrixB[2*2];

	fmatrix_vlaue = MatrixDet2(fMatrixA);

	if (fmatrix_vlaue == 0)
	{
		while(1);
	}

	tempfMatrixB[0] = fMatrixA[3]/fmatrix_vlaue;
	tempfMatrixB[1] = -fMatrixA[2]/fmatrix_vlaue;
	tempfMatrixB[2] = -fMatrixA[1]/fmatrix_vlaue;
	tempfMatrixB[3] = -fMatrixA[0]/fmatrix_vlaue;

	memcpy(fMatrixB, tempfMatrixB, sizeof(float)*2*2);
}


