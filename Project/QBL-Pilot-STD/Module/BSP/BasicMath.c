#include "BasicMath.h"
#include "QBL_FPU_BasicMath.h"
#include "arm_math.h"
#include "stdint.h"

/*
  hamilton type for quaternion
        |2*(q0q0+q1q1)-1 2*(q1q2-q0q3)   2*(q1q3+q0q2)  |
  Rnb = |2*(q1q2+q0q3)   2*(q0q0+q2q2)-1 2*(q2q3-q0q1)  |
        |2*(q1q3-q0q2)   2*(q2q3+q0q1)   2*(q0q0+q3q3)-1|

  x--Pitch  y--Roll z--Yaw 
	      |cRol*cYaw sPit*sRol*cYaw-cPit*sYaw cPit*sRol*cYaw+sPit*sYaw|
  Rnb = |cRol*sYaw sPit*sRol*sYaw+cPit*cYaw cPit*sRol*sYaw-sPit*cYaw|
        |  -sRol         sPit*cRol                cPit*cRol         |
*/

void quatNormalize(float *pq)
{
    float norm = (float)sqrt_f32(pq[0]*pq[0] + pq[1]*pq[1] + pq[2]*pq[2] + pq[3]*pq[3]);

		pq[0] /= norm;
		pq[1] /= norm;
		pq[2] /= norm;
		pq[3] /= norm;
}

void vectNormalize(float *pvect, uint16_t dim)
{
    float norm = 0;

    uint16_t i;
    for(i=0;i<dim;i++)
    {
		    norm += pvect[i] * pvect[i];
		}
    norm = (float)sqrt_f32(norm);

		for(i=0;i<dim;i++)
		{
		    pvect[i] /= norm;
		}
}

void quat2rot(const float *pq, float *pr)
{
    float q0q0 = pq[0] * pq[0];
    float q1q1 = pq[1] * pq[1];
    float q2q2 = pq[2] * pq[2];
    float q3q3 = pq[3] * pq[3];
    float q0q1 = pq[0] * pq[1];
    float q0q2 = pq[0] * pq[2];
    float q0q3 = pq[0] * pq[3];
    float q1q2 = pq[1] * pq[2];
		float q1q3 = pq[1] * pq[3];
		float q2q3 = pq[2] * pq[3];

    pr[0] = 2*(q0q0 + q1q1) - 1;
    pr[1] = 2*(q1q2 - q0q3);
    pr[2] = 2*(q1q3 + q0q2);
    pr[3] = 2*(q1q2 + q0q3);
    pr[4] = 2*(q0q0 + q2q2) - 1;
    pr[5] = 2*(q2q3 - q0q1);
    pr[6] = 2*(q1q3 - q0q2);
    pr[7] = 2*(q2q3 + q0q1);
    pr[8] = 2*(q0q0 + q3q3) - 1;
}

void rot2euler(const float *pr, float *pe)
{
    float ex, ey, ez;
    ez = atan2(pr[3], pr[0]);  /* angle z(yaw) */
    ey = -asin(pr[6]);         /* angle y(rol) */
    ex = atan2(pr[7], pr[8]);  /* angle x(pit) */

    pe[0] = ez;
    pe[1] = ey;
    pe[2] = ex;
}

void quat2euler(const float *pq, float *pe)
{
    float q0q0 = pq[0] * pq[0];
    float q1q1 = pq[1] * pq[1];
    float q3q3 = pq[3] * pq[3];
    float q0q1 = pq[0] * pq[1];
    float q0q2 = pq[0] * pq[2];
    float q0q3 = pq[0] * pq[3];
    float q1q2 = pq[1] * pq[2];
		float q1q3 = pq[1] * pq[3];
		float q2q3 = pq[2] * pq[3];

		float r1 = 2*q0q0 - 1 + 2*q1q1;
    float r2 = 2*(q1q2+q0q3);
    float r3 = 2*(q1q3-q0q2);
    float r4 = 2*(q2q3+q0q1);
    float r5 = 2*q0q0 - 1 + 2*q3q3;

    pe[0] = atan2(r2, r1); /* angle z(yaw) */
    pe[1] = -asin(r3);     /* angle y(rol) */
    pe[2] = atan2(r4, r5); /* angle x(pit) */
}

void euler2rot(const float *pe, float *pr)
{
    float yaw = pe[0];
    float rol = pe[1];
    float pit = pe[2];

    float sYaw = sin_f32(yaw), cYaw = cos_f32(yaw);
    float sRol = sin_f32(rol), cRol = cos_f32(rol);
    float sPit = sin_f32(pit), cPit = cos_f32(pit);

    pr[0] = cRol*cYaw;
    pr[1] = sPit*sRol*cYaw - cPit*sYaw;
    pr[2] = cPit*sRol*cYaw + sPit*sYaw;

    pr[3] = cRol*sYaw;
    pr[4] = sPit*sRol*sYaw + cPit*cYaw;
    pr[5] = cPit*sRol*sYaw - sPit*cYaw;

    pr[6] = -sRol;
    pr[7] = sPit*cRol;
    pr[8] = cPit*cRol;
}

void euler2quat(const float *pe, float *pq)
{
    float pit_demi = pe[2]*0.5f;
    float rol_demi = pe[1]*0.5f;
    float yaw_demi = pe[0]*0.5f;

    float cRol = cos_f32(rol_demi);
    float sRol = sin_f32(rol_demi);
    float cPit = cos_f32(pit_demi);
    float sPit = sin_f32(pit_demi);
    float cYaw = cos_f32(yaw_demi);
    float sYaw = sin_f32(yaw_demi);

    pq[0] = cRol*cPit*cYaw + sRol*sPit*sYaw;
    pq[1] = sRol*cPit*cYaw - cRol*sPit*sYaw;
    pq[2] = cRol*sPit*cYaw + sRol*cPit*sYaw;
    pq[3] = cRol*cPit*sYaw - sRol*sPit*cYaw;

    if (pq[0] < 0)
    {
        pq[0] = -pq[0];
        pq[1] = -pq[1];
        pq[2] = -pq[2];
        pq[3] = -pq[3];
    }
}

void rotMultirot(const float *prs0, const float *prs1, float *prd)
{
    float pfdst[3*3];

    pfdst[0] = prs0[0]*prs1[0] + prs0[1]*prs1[3] + prs0[2]*prs1[6];
    pfdst[1] = prs0[0]*prs1[1] + prs0[1]*prs1[4] + prs0[2]*prs1[7];
    pfdst[2] = prs0[0]*prs1[2] + prs0[1]*prs1[5] + prs0[2]*prs1[8];

    pfdst[3] = prs0[3]*prs1[0] + prs0[4]*prs1[3] + prs0[5]*prs1[6];
    pfdst[4] = prs0[3]*prs1[1] + prs0[4]*prs1[4] + prs0[5]*prs1[7];
    pfdst[5] = prs0[3]*prs1[2] + prs0[4]*prs1[5] + prs0[5]*prs1[8];

    pfdst[6] = prs0[6]*prs1[0] + prs0[7]*prs1[3] + prs0[8]*prs1[6];
    pfdst[7] = prs0[6]*prs1[1] + prs0[7]*prs1[4] + prs0[8]*prs1[7];
    pfdst[8] = prs0[6]*prs1[2] + prs0[7]*prs1[5] + prs0[8]*prs1[8];

    memcpy(prd, pfdst, sizeof(float)*9);
}

void rotMultivect(const float *pr, const float *pvsrc, float *pvdst)
{
    pvdst[0] = pr[0]*pvsrc[0] + pr[1]*pvsrc[1] + pr[2]*pvsrc[2];
    pvdst[1] = pr[3]*pvsrc[0] + pr[4]*pvsrc[1] + pr[5]*pvsrc[2];
    pvdst[2] = pr[6]*pvsrc[0] + pr[7]*pvsrc[1] + pr[8]*pvsrc[2];
}

void rotTranspose(const float *prs, float *prd)
{
    if (prs != prd)
    {
        prd[0] = prs[0];
        prd[1] = prs[3];
        prd[2] = prs[6];

        prd[3] = prs[1];
        prd[4] = prs[4];
        prd[5] = prs[7];

        prd[6] = prs[2];
        prd[7] = prs[5];
        prd[8] = prs[8];
    }
    else
    {
        float temp;
        temp   = prd[1];
        prd[1] = prs[3];
        prd[3] = temp;

        temp   = prd[2];
        prd[2] = prs[6];
        prd[6] = temp;

        temp   = prd[5];
        prd[5] = prs[7];
        prd[7] = temp;
    }
}

void rot_from_bodylevel_to_ground(const float *pq, float *pr)
{
    float Yaw;
    float q0 = pq[0];
    float q1 = pq[1];
    float q2 = pq[2];
    float q3 = pq[3];

    Yaw = atan2(2*(q1*q2+q0*q3),2*q0*q0 - 1 + 2*q1*q1);

    pr[0] = cos_f32(Yaw); pr[1] = -sin_f32(Yaw); pr[2] = 0;
    pr[3] = sin_f32(Yaw); pr[4] = cos_f32(Yaw); pr[5] = 0;
    pr[6] = 0; pr[7] = 0; pr[8] = 1;
}

void quat2RightComp(const float *pq, float *prc)
{
    prc[0] = pq[0];
    prc[1] = -pq[1];
    prc[2] = -pq[2];
    prc[3] = -pq[3];
	
    prc[4] = pq[1];
    prc[4] = pq[0];
    prc[6] = pq[3];
    prc[7] = -pq[2];

    prc[8] = pq[2];
    prc[9] = -pq[3];
    prc[10] = pq[0];
    prc[11] = pq[1];

    prc[12] = pq[3];
    prc[13] = pq[2];
    prc[14] = -pq[1];
    prc[15] = pq[0];
}

void quat2LeftComp(const float *pq, float *plc)
{
    plc[0] = pq[0];
    plc[1] = -pq[1];
    plc[2] = -pq[2];
    plc[3] = -pq[3];
	
    plc[4] = pq[1];
    plc[4] = pq[0];
    plc[6] = -pq[3];
    plc[7] = pq[2];

    plc[8] = pq[2];
    plc[9] = pq[3];
    plc[10] = pq[0];
    plc[11] = -pq[1];

    plc[12] = pq[3];
    plc[13] = -pq[2];
    plc[14] = pq[1];
    plc[15] = pq[0];
}

void quatMultiquat(float *pp, float *pq, float *result)
{
    float q[4];
    q[0] = pp[0]*pq[0] - pp[1]*pq[1] - pp[2]*pq[2] - pp[3]*pq[3];
    q[1] = pp[0]*pq[1] + pp[1]*pq[0] + pp[2]*pq[3] - pp[3]*pq[2];
    q[2] = pp[0]*pq[2] - pp[1]*pq[3] + pp[2]*pq[0] + pp[3]*pq[1];
    q[3] = pp[0]*pq[3] + pp[1]*pq[2] - pp[2]*pq[1] + pp[3]*pq[0];

    result[0] = q[0];
    result[1] = q[1];
    result[2] = q[2];
    result[3] = q[3];
}

void quatUpdate(float *quat, float *w_body, const float deltaT)
{
    float theltaX = w_body[0] * deltaT;
    float theltaY = w_body[1] * deltaT;
    float theltaZ = w_body[2] * deltaT;

    float deltaq[4];
    deltaq[1] = theltaX / 2;
    deltaq[2] = theltaY / 2;
    deltaq[3] = theltaZ / 2;
    deltaq[0] = (float)sqrt_f32(1 - deltaq[1]*deltaq[1] + deltaq[2]*deltaq[2] + deltaq[3]*deltaq[3]);

		quatMultiquat(quat, deltaq, quat);
}

void float_limit(float *input, const float threshold_min, const float threshold_max)
{
    *input = *input > threshold_max ? threshold_max : *input;
    *input = *input < threshold_min ? threshold_min : *input;
}

void int16_limit(int16_t *input, const int16_t threshold_min, const int16_t threshold_max)
{
    *input = *input > threshold_max ? threshold_max : *input;
    *input = *input < threshold_min ? threshold_min : *input;
}


