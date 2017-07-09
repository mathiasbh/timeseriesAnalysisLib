#include <stdio.h>
#include <math.h>
#include "header.h"
#define M_PI 3.14159265358979323846264

outputClean* clean(dataStruct* dataset, double f0, double f1, double df, int numberPeaks) {
  fprintf(stderr, "CLEANing started (%i peaks)! May take some time.\n", numberPeaks);
  /* dataStruct* recover = data_alloc(dataset->size); */
  outputClean* cleanSeries = outClean_alloc(numberPeaks);
  
  for(int i=0; i<numberPeaks; i++) {
    /* Calculate powerspectrum and find maximum */
    outputPwr* pOut = powerspectrum(dataset, f0, f1, df);
    int intPeak = findMaximum(pOut->size, pOut->Power);
    
    /* Zoom in on max, find new refined maximum */
    double f0Zoom = pOut->Freq[intPeak]*0.9, f1Zoom = pOut->Freq[intPeak]*1.1;
    pOut = powerspectrum(dataset, f0Zoom, f1Zoom, -1);
    intPeak = findMaximum(pOut->size, pOut->Power);
    cleanSeries->Freq[i] = pOut->Freq[intPeak];
    cleanSeries->Ampl[i] = sqrt(pOut->Power[intPeak]);
    fprintf(stderr, "Peak removed (%i): freq = %lg, ", i+1, cleanSeries->Freq[i]); 
    fprintf(stderr, "Ampl = %lg\n", cleanSeries->Ampl[i]);
    for(int j=0; j<dataset->size; j++) {
      dataset->Data[j] = dataset->Data[j]
    	- pOut->Alpha[intPeak]*sin(2*M_PI*pOut->Freq[intPeak]*dataset->Time[j])
    	- pOut->Beta [intPeak]*cos(2*M_PI*pOut->Freq[intPeak]*dataset->Time[j]);
    }
    /* for(int j=0; j<dataset->size; j++) { */
    /*   recover->Data[j] = recover->Data[j] */
    /* 	+ pOut->Alpha[intPeak]*sin(2*M_PI*pOut->Freq[intPeak]*dataset->Time[j]) */
    /* 	+ pOut->Beta [intPeak]*cos(2*M_PI*pOut->Freq[intPeak]*dataset->Time[j]); */
    /* } */
  }
  /* for(int j=0; j<dataset->size; j++) { */
  /*   dataset->Data[j] = recover->Data[j]; */
  /* } */
  fprintf(stderr, "CLEANing done!\n");
  return cleanSeries;
}
