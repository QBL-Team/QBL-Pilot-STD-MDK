#ifndef __KALMANFILTER_H__
#define __KALMANFILTER_H__

#include <stdint.h>

/* Xk = A * Xk_1 */
void PredictStatesFromKinetics(float *Xk_1, float *F, uint16_t dim, float *Xk);

/* Pk = F*Pk_1*F' + G*Qk_1*G' */
void PredictCovariance(float *Pk_1, float *F, float *G, float *Qk_1, uint16_t dim, float *Pk);

/* input = 0.5*(|input|+|input|') */
void enforcePSD(float *input, uint16_t dim);

/* K = P*H'*(H*P*H'+V)^-1 */
void KalmanGain(float *Pk, uint16_t dimPk, float *H, uint16_t dimrowsH, uint16_t dimcolsH, float *V, float *K);

/* HX = H*Xk */
void PredictMeasureFromeStates(float *H, uint16_t dimrowsH, uint16_t dimcolsH, float *Xk, float *HX);

/* trueXk = K*(Z-HX) */
void UpdateStatesByMeasurement(float *K, uint16_t dimrowsK, uint16_t dimcolsK, float *HX, float *Z, float *trueXk);

/* truePk = Pk-K*(H*Pk*H'+V)*K' */
void UpdateCovarianceByMeasurement(float *Pk, uint16_t dimPk, float *H, uint16_t dimrowsH, uint16_t dimcolsH, 
                                   float *V, float *K, float *truePk);

#endif
