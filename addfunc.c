/* Simple but horribly optimized function to
   calculate the median of an array x[] */
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "header.h"

double median(double x[], int n) {
  /* Sort in ascending order */
  double temporaryElement;
  for(int i=0; i<n; i++) {
    for(int j=i+1; j<n; j++) {
      if(x[j] < x[i]) {
	temporaryElement = x[i];
	x[i] = x[j];
	x[j] = temporaryElement;
      }
    }
  }
  if(n%2 == 0) {
    /* If array consists of an even number of elements, 
       return mean of two middle elements. */
    return (x[n/2] + x[n/2 - 1]) / 2.0;
  } else {
    return x[n/2];
  }
}
//(N+2-1)/2
void movingMedian(double x[], double medianarr[], int size, int N) {
  for(int i=0; i<N; i++) medianarr[i] = 0.00;
  for(int i=N; i<size-N; i++) {
    double tempArr[2*N];
    int tempInt = 0;
    for(int j=i-N; j<i+N; j++) {
      tempArr[tempInt] = x[j];
      tempInt++;
    }
    medianarr[i] = median(tempArr, 2*N);
  }
  for(int i=size-N; i<size; i++) medianarr[i] = 0.00;
}

void movingMean(double x[], double meanarr[], int size, int N) {
  for(int i=0; i<N; i++) meanarr[i] = 0.00;
  for(int i=N; i<size-N; i++) {
    double sum = 0;
    for(int j=i-N; j<i+N; j++) {
      sum += x[j]/(2*N);
    }
    meanarr[i] = sum;
  }
  for(int i=size-N; i<size; i++) meanarr[i] = 0.00;
}

void scatter2(double x[], double scatter[], int size, int N) {
  double meanarr[size];
  movingMean(x, meanarr, size, N);
  for(int i=0; i<N; i++) scatter[i] = (double)1e9;
  for(int i=N; i<size-N; i++) {
    double sum = 0;
    for(int j=i-N; j<i+N; j++) {
      sum += (x[j] - meanarr[j])*(x[j] - meanarr[j])/(2*N+1);
    }
    scatter[i] = sum;
  }
  for(int i=size-(N+2-1)/2; i<size; i++) scatter[i] = (double)1e9;
}

int findMaximum(int size, double y[]) {
  /* Returns element index */
  double max = y[0];
  int index = 0;
  for(int i=1; i<size; i++) {
    if(y[i] > max) {index = i; max = y[i];}
  }
  return index;
}

int findMaximumLimx(int size, double x[], double y[], double limit0, double limit1) {
  /* Returns element index, searching only with xvalue limit0 and limit1 */
  double max = 0;
  int index = 0;
  for(int i=0; i<size; i++) {
    if(x[i] > limit0 && x[i] < limit1) {
      if(y[i] > max) {index = i; max = y[i];
      }
    }
  }
  return index;
}

/* Find jump and correct */
void correctJump(dataStruct* d, int numPeaks) {
  double dataJump[d->size-1];
  for(int i=0; i<d->size-1; i++) {
    dataJump[i] = fabs(d->Data[i] - d->Data[i+1]);
  }
  for(int k=0; k<numPeaks; k++) { // does not work yet
    int intJump = findMaximum(d->size, dataJump);
    double addJump = d->Data[intJump-1] - d->Data[intJump+1];
    for(int i=intJump+1; i<d->size; i++) {
      d->Data[i] += addJump;
    }
  }
}

void difCalc(dataStruct* d, double dif[], double cutoff, int n) {
  /* Locate bad data */
  double dMedian[d->size];
  movingMedian(d->Data, dMedian, d->size, n);
  
  for(int i=0; i<d->size; i++) {
    dif[i] = dMedian[i]/d->Data[i] - 1;
    if(fabs(dif[i]) > cutoff) d->Data[i] = dMedian[i];
  }
}

int binary_search(int n, double *x, double z) {
  int IndexInitial = 0; 
  int IndexFinal = n-1;

  while(IndexFinal - IndexInitial > 1) {
    int IndexMid = (IndexInitial + IndexFinal)/2;

    if(z > x[IndexMid]) IndexInitial = IndexMid; 
    else IndexFinal = IndexMid;
  }

  return IndexInitial;
}

