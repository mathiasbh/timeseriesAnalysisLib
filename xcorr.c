/* 
   Cross-correlation model 
   model(i,P,phi) = 1 (|t_i - mP - phi| < Delta T / 2)
                  = 0 elsewhere 

  i = index
  P = orbital period
  phi = phase
  Delta T = length of transit
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "header.h"

void findMaximumMatrix(int rowsize, int colsize, double y[], int *ind1, int *ind2) {
  /* Returns element indices */
  int m;
  double max = y[0];// y[0];
  for(m=0; m<colsize*rowsize; m++) {
    if(y[m] > max) {
      max = y[m];
      *ind1 = m/colsize; //convert m to i,j (row/column)
      *ind2 = m%colsize;
    }
  }
}


dataStruct* ccmodel(dataStruct* d, double period, double phase, double deltaT, int numPeaks) {
  dataStruct* modelData = data_alloc(d->size);
  for(int i=0; i<d->size; i++) {
    modelData->Data[i] = 0.00;
    modelData->Time[i] = d->Time[i] - d->Time[0];
  }
  
  for(int m=0; m<numPeaks; m++) {
    for(int i=0; i<d->size; i++) {
      if(fabs(modelData->Time[i] - m*period - phase) < deltaT/2.0) {
	modelData->Data[i] = 1.00;
      }
    }
  }
  return modelData;
}

void xcorr(dataStruct* d, double p1, double p2, double periodRes, double phaseRes,
	   double deltaT, int numPeaks, double *periodResult, double *phaseResult) {
  fprintf(stderr, "Cross-correlation initiated - it may take some time!\n");
  double phaseEnd = d->Time[d->size-1] - d->Time[0];
  double mean1, mean2, r, rdenom1, rdenom2;
  int rowsize = ceil((p2-p1)/periodRes) + 1;
  int colsize = ceil(phaseEnd/phaseRes);
  int rcsize = rowsize*colsize;
  /* Allocate memory to sCorr, sCorr[rowsize][colsize] */
  double *sCorr = (double*)malloc(rcsize*sizeof(double));
  double periodCorr[rowsize]; // holds period
  double phaseCorr[colsize]; // holds phase
  int i=0, j, m;
  for(double period=p1; period<p2; period+=periodRes) {
    j=0;
    for(double phase=0; phase<phaseEnd; phase+=phaseRes) {
      m = i * colsize + j; // index for matrix
      dataStruct* modelData = ccmodel(d, period, phase, deltaT, numPeaks);
      mean1=0; mean2=0, r=0; rdenom1=0; rdenom2=0;
      for(int k=0; k<d->size; k++) {
	mean1 += d->Data[k];
	mean2 += modelData->Data[k];
      }
      mean1 /= d->size;
      mean2 /= modelData->size;

      for(int k=0; k<d->size; k++) {
	r += (d->Data[k] - mean1) * (modelData->Data[k] - mean2);
	rdenom1 += (d->Data[k] - mean1) * (d->Data[k] - mean1);
	rdenom2 += (modelData->Data[k] - mean2) * (modelData->Data[k] - mean2);
      }

      r /= sqrt(rdenom1 * rdenom2);
      sCorr[m] = r;
      
      phaseCorr[j] = phase; // unnecessarily repeated...
      data_free(modelData);
      j++; // phase in columns
    }
    periodCorr[i] = period;
    fprintf(stderr, "Period step: %i/%i\n", i, rowsize-1);
    i++; // Period in rows
  }
  int iMax=0, jMax=0;
  findMaximumMatrix(rowsize, colsize, sCorr, &iMax, &jMax);
  *periodResult = periodCorr[iMax];
  *phaseResult = phaseCorr[jMax];
  fprintf(stderr, "Cross-correlation completed - found P=%.10lg, phi=%.10lg\n",
	  periodCorr[iMax], phaseCorr[jMax]);
  free(sCorr);
}
