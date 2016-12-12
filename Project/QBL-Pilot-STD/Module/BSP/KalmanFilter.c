#include "KalmanFilter.h"
#include "QBL_FPU_BasicMath.h"
#include "BasicMath.h"
#include <stdlib.h>
#include <stdint.h>

void PredictStatesFromKinetics(float *Xk_1, float *F, uint16_t dim, float *Xk)
{
    /* Xk = A * Xk_1 */
    MatrixMultiply(F,dim,dim, Xk_1,dim,1, Xk,dim,1);
}

void PredictCovariance(float *Pk_1, float *F, float *G, float *Qk_1, uint16_t dim, float *Pk)
{
    /* Pk = F*Pk_1*F' + G*Qk_1*G' */

    float *temp1 = (float *)malloc(sizeof(float)*dim*dim);
    float *temp2 = (float *)malloc(sizeof(float)*dim*dim);
    float *temp3 = (float *)malloc(sizeof(float)*dim*dim);

    /* temp1 = F*Pk_1*F' */
    MatrixTranspose(F,dim,dim, temp1,dim,dim);
    MatrixMultiply(Pk_1,dim,dim, temp1,dim,dim, temp2,dim,dim);
    MatrixMultiply(F,dim,dim, temp2,dim,dim, temp1,dim,dim);

    if(G!=NULL && Qk_1!=NULL)
		{
		    /* temp2 = G*Qk_1*G' */
		    MatrixTranspose(G,dim,dim, temp2,dim,dim);
        MatrixMultiply(Qk_1,dim,dim, temp2,dim,dim, temp3,dim,dim);
        MatrixMultiply(G,dim,dim, temp3,dim,dim, temp2,dim,dim);

			  /* Pk = temp1 + temp2 */
        MatrixAdd(temp1,dim,dim, temp2,dim,dim, Pk,dim,dim);
		}
		else
    {
		    /* Pk = temp1 */
		    MatrixCopy(temp1,dim,dim, Pk);
		}

    free(temp1);
    free(temp2);
    free(temp3);
}

void enforcePSD(float *input, uint16_t dim)
{
    uint16_t i, j;

    for(i=0;i<dim;i++)
    {
		    for(j=0;j<dim;j++)
				{
				    input[i*dim+j] = ABS(input[i*dim+j]);
				}
		}

    for(i=0;i<dim;i++)
    {
		    for(j=i+1;j<dim;j++)
				{
				    input[i*dim+j] = input[i*dim+j] + input[j*dim+i];
				    input[j*dim+i] = input[i*dim+j];
				}
		}
}

void KalmanGain(float *Pk, uint16_t dimPk, float *H, uint16_t dimrowsH, uint16_t dimcolsH, float *V, float *K)
{
    /* K = P*H'*(H*P*H'+V)^-1 */

    if(dimPk!=dimcolsH)
		{
		   while(1);
		}

    uint16_t rowsMax = MAX(dimPk, dimrowsH);
    uint16_t colsMax = MAX(dimPk, dimcolsH);

    float *temp1 = (float *)malloc(sizeof(float)*rowsMax*colsMax);
    float *temp2 = (float *)malloc(sizeof(float)*rowsMax*colsMax);
    float *temp3 = (float *)malloc(sizeof(float)*rowsMax*colsMax);

    /* temp1 = H' */
    MatrixTranspose(H,dimrowsH,dimcolsH, temp1,dimcolsH,dimrowsH);
    /* temp2 = P*H' */
    MatrixMultiply(Pk,dimPk,dimPk, temp1,dimcolsH,dimrowsH, temp2,dimPk,dimrowsH);
    /* temp3 = H*P*H' */
    MatrixMultiply(H,dimrowsH,dimcolsH, temp2,dimPk,dimrowsH, temp3,dimrowsH,dimrowsH);
    /* temp2 = H*P*H'+V */
    MatrixAdd(temp3,dimrowsH,dimrowsH, V,dimrowsH,dimrowsH, temp2,dimrowsH,dimrowsH);
    /* temp3 = (H*P*H'+V)^-1 */
    MatrixInverse(temp2,dimrowsH, temp3,dimrowsH);
    /* temp2 = H'*(H*P*H'+V)^-1 */
    MatrixMultiply(temp1,dimcolsH,dimrowsH, temp3,dimrowsH,dimrowsH, temp2,dimcolsH,dimrowsH);
		/* K = P*H'*(H*P*H'+V)^-1 */
    MatrixMultiply(Pk,dimPk,dimPk, temp2,dimcolsH,dimrowsH, K,dimPk,dimrowsH);

    free(temp1);
    free(temp2);
    free(temp3);
}

void PredictMeasureFromeStates(float *H, uint16_t dimrowsH, uint16_t dimcolsH, float *Xk, float *HX)
{
    /* HX = H*Xk */
    MatrixMultiply(H,dimrowsH,dimcolsH, Xk,dimcolsH,1, HX,dimrowsH,1);
}

void UpdateStatesByMeasurement(float *K, uint16_t dimrowsK, uint16_t dimcolsK, float *HX, float *Z, float *trueXk)
{
    /* trueXk = K*(Z-HX) */

    float *temp = (float *)malloc(sizeof(float)*dimrowsK);

    /* temp1 = Z-HX */
    MatrixSub(Z,dimrowsK,1, HX,dimrowsK,1, temp,dimrowsK,1);

    /* trueXk = K*(Z-HX) */
    MatrixMultiply(K,dimrowsK,dimcolsK, temp,dimrowsK,1, trueXk,dimrowsK,1);

    free(temp);
}

void UpdateCovarianceByMeasurement(float *Pk, uint16_t dimPk, float *H, uint16_t dimrowsH, uint16_t dimcolsH,
                                   float *V, float *K, float *truePk)
{
    /* truePk = Pk-K*(H*Pk*H'+V)*K' */

    if(dimPk!=dimrowsH)
		{
		   while(1);
		}

    uint16_t rowsMax = MAX(dimPk, dimrowsH);
    uint16_t colsMax = MAX(dimPk, dimcolsH);

    float *temp1 = (float *)malloc(sizeof(float)*rowsMax*colsMax);
    float *temp2 = (float *)malloc(sizeof(float)*rowsMax*colsMax);
    float *temp3 = (float *)malloc(sizeof(float)*rowsMax*colsMax);
    float *temp4 = (float *)malloc(sizeof(float)*rowsMax*colsMax);

    /* temp1 = H' */
    MatrixTranspose(H,dimrowsH,dimcolsH, temp1,dimcolsH,dimrowsH);
    /* temp2 = P*H' */
    MatrixMultiply(Pk,dimPk,dimPk, temp1,dimcolsH,dimrowsH, temp2,dimPk,dimrowsH);
    /* temp3 = H*P*H' */
    MatrixMultiply(H,dimrowsH,dimcolsH, temp2,dimPk,dimrowsH, temp3,dimrowsH,dimrowsH);
    /* temp2 = H*P*H'+V */
    MatrixAdd(temp3,dimrowsH,dimrowsH, V,dimrowsH,dimrowsH, temp2,dimrowsH,dimrowsH);
    /* temp4 = K' */
    MatrixTranspose(K,dimrowsH,dimcolsH, temp4,dimrowsH,dimPk);
    /* temp3 = (H*Pk*H'+V)*K' */
    MatrixMultiply(temp2,dimrowsH,dimrowsH, temp2,dimPk,dimrowsH, temp3,dimrowsH,dimrowsH);
    /* temp2 = K*(H*Pk*H'+V)*K' */
    MatrixMultiply(K,dimPk,dimrowsH, temp3,dimrowsH,dimrowsH, temp2,dimPk,dimrowsH);
		/* truePk = Pk-K*(H*Pk*H'+V)*K' */
    MatrixSub(truePk,dimPk,dimPk, temp2,dimPk,dimrowsH, truePk,dimPk,dimPk);

    free(temp1);
    free(temp2);
    free(temp3);
    free(temp4);
}

