#include "Basic_Type.h"
#include "QBL_FPU_BasicMath.h"

float INT16_XYZNorm(const INT16_XYZ_t *input)
{
	float norm;
	
	norm = (float)sqrt_f32(input->X*input->X + input->Y*input->Y + input->Z*input->Z);

	return norm;
}

void INT16_XYZNormalize(const INT16_XYZ_t *input, FLOAT_XYZ_t *result)
{
	float norm;
	
	norm = (float)sqrt_f32(input->X*input->X + input->Y*input->Y + input->Z*input->Z);

	result->X = input->X / norm;
	result->Y = input->Y / norm;
	result->Z = input->Z / norm;
}


