#include <stdlib.h>
#include <stdio.h>
#include "header.h"

/* Holds input data */
dataStruct* data_alloc(int n) {
  dataStruct* d = (dataStruct*)malloc(sizeof(dataStruct));
  d->size  = n;
  d->Time  = (double*)malloc(n*sizeof(double));
  d->Data  = (double*)malloc(n*sizeof(double));
  d->Weight= (double*)malloc(n*sizeof(double));
  if( d==NULL ) fprintf(stderr,"error in data_alloc\n");
  return d;
}

void data_free(dataStruct* d) {
  free(d->Time); 
  free(d->Data);
  free(d->Weight);
  free(d);
}

/* Collects results from powerspectrum */
outputPwr* outPwr_alloc(int n) {
  outputPwr* o = (outputPwr*)malloc(sizeof(outputPwr));
  o->size = n;
  o->Freq = (double*)malloc(n*sizeof(double));
  o->Power= (double*)malloc(n*sizeof(double));
  o->Alpha= (double*)malloc(n*sizeof(double));
  o->Beta = (double*)malloc(n*sizeof(double));
  if( o==NULL ) fprintf(stderr,"error in outPwr_alloc\n");
  return o;
}

void outPwr_free(outputPwr* d) {
  free(d->Freq);
  free(d->Alpha);
  free(d->Beta);
  free(d->Power);
  free(d);
}


/* Collects results from powerspectrum */
outputClean* outClean_alloc(int n) {
  outputClean* o = (outputClean*)malloc(sizeof(outputClean));
  o->size = n; // Number of extracted frequencies
  o->Freq = (double*)malloc(n*sizeof(double));
  o->Ampl = (double*)malloc(n*sizeof(double));
  if( o==NULL ) fprintf(stderr,"error in outClean_alloc\n");
  return o;
}

void outClean_free(outputClean* d) {
  free(d->Freq);
  free(d->Ampl);
  free(d);
}
