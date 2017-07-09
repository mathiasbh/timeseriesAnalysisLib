#include <stdio.h>
#include <math.h>
#include "header.h"

dataStruct* linterpGrid(dataStruct* d, double step) {
  int i, j=0, n = d->size, m = ceil((d->Time[n-1] - d->Time[0])/step);
  dataStruct* gridData = data_alloc(m);
  gridData->size = m;
  for(double z=d->Time[0]; z<d->Time[n-1]; z+=step) {
    i = binary_search(d->size, d->Time, z);
    gridData->Time[j] = z;
    gridData->Data[j] = d->Data[i]
      + (d->Data[i+1]-d->Data[i])/(d->Time[i+1]-d->Time[i]) * (z - d->Time[i]);
    j++;
  }
  return gridData;
}

double autocorr(dataStruct* d, double step, int k) {
  int N = d->size, m1=0, m2=0;
  double meanval1 = 0, meanval2 = 0, rk = 0;
  for(m1=0; m1<N-k; m1++) {
    meanval1 += d->Data[m1];
  } meanval1 /= (double)(m1+1);
    
  for(m2=1+k; m2<N; m2++) {
    meanval2 += d->Data[m2];
  } meanval2 /= (double)(m2+1);
  
  for(int i=0; i<N-k; i++) {
    rk += (d->Data[i] - meanval1)*(d->Data[i+k] - meanval2) / (N-k);
  }
  return rk;
}
