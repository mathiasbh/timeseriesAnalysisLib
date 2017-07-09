/* ******************************
   Read data of three columns:
   time1 data1 weight1
   time2 data2 weight2
   ...
   timeN dataN weightN

 * ****************************** */
#include <stdio.h>
#include "header.h"

int NumberDatapoints(char* filename) {
  FILE *fdata = fopen(filename,"r");
  if(fdata == NULL) {
    fprintf(stderr, "File not found!");
  }
  
  int n = 0;
  double x, y, z;
  while( fscanf(fdata, "%lg\t%lg\t%lg\n", &x, &y, &z) == 3) {
    n++;
  }
  fclose(fdata);
  return n;
}

dataStruct* readdata(char* filename, int columns, int subMean) {
  int n = NumberDatapoints(filename);
  FILE *fdata = fopen(filename,"r");
  dataStruct* dataset = data_alloc(n);
  if(columns == 3) {
    double t, d, w;
    for(int i = 0; i<n; i++) {
      if(fscanf(fdata, "%lg\t%lg\t%lg\n", &t, &d, &w) == 3);
      dataset->Time[i]   = t;
      dataset->Data[i]   = d;
      dataset->Weight[i] = w;
    }
  }

  if(columns == 2) {
    double t, d;
    for(int i = 0; i<n; i++) {
      if(fscanf(fdata, "%lg\t%lg\n", &t, &d) == 2);
      dataset->Time[i]   = t;
      dataset->Data[i]   = d;
      dataset->Weight[i] = 1.0;
    }
  }

  if(subMean == 1) {
    /* Subtract mean of time to avoid "zero-frequency" */
    double MeanValue = 0;
    for(int i=0; i<dataset->size; i++) MeanValue += dataset->Data[i]/dataset->size;
    for(int i=0; i<dataset->size; i++) dataset->Data[i] -= MeanValue;
  }
  fclose(fdata);
  return dataset;
}
