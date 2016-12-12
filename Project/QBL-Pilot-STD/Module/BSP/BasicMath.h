#ifndef __BASICMATH_H__
#define __BASICMATH_H__

#include <stdint.h>

#ifndef MIN
#define MIN(a, b)  (((a) > (b)) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a, b)  (((a) < (b)) ? (b) : (a))
#endif

#ifndef ABS
#define ABS(x)  ((x)<0?(-(x)):(x))
#endif

/* q[0] = w q[0] = x q[0] = y q[0] = z */

/* normalize the quaternion */
void quatNormalize(float *pq);

/* normalize the vector */
void vectNormalize(float *pvect, uint16_t dim);

/* quaternion to rotation matrix */
void quat2rot(const float *pq, float *pr);

/* rotation matrix to Euler angle (yaw, roll, pitch) */
void rot2euler(const float *pr, float *pe);

/* quaternion to Euler angle (yaw, roll, pitch) */
void quat2euler(const float *pq, float *pe);

/* Euler angle (yaw, roll, pitch) to rotation matrix */
void euler2rot(const float *pe, float *pr);

/* Euler angle (yaw, roll, pitch) to quaternion */
void euler2quat(const float *pe, float *pq);

/* multiply rotation matrix */
void rotMultirot(const float *prs0, const float *prs1, float *prd);

/* transpose rotation matrix */
void rotTranspose(const float *prs, float *prd);

/* multiply rotation matrix with vector */
void rotMultivect(const float *pr, const float *pvsrc, float *pvdst);

/* rotation matrix from bodylevel to ground */
void rot_from_bodylevel_to_ground(const float *pq, float *pr);

/* Right product matrix of quaternion */
void quat2RightComp(const float *pq, float *prc);

/* left product matrix of quaternion */
void quat2LightComp(const float *pq, float *plc);

/* constrain input between threshold_min and threshold_max */
void float_limit(float *input, const float threshold_min, const float threshold_max);

/* constrain input between threshold_min and threshold_max */
void int16_limit(int16_t *input, const int16_t threshold_min, const int16_t threshold_max);

#endif
