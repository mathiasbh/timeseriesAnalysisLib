#include <math.h>
#include <stdio.h>
#include "header.h"
#define M_PI 3.14159265358979323846264

void bandpass(dataStruct* dataset, double fInitial, double fFinal, double df,
	      double f, double freqA, double freqB) {
  outputPwr* pWin = windowfunction(dataset, f, fInitial, fFinal, df);
  int nf1 = binary_search(pWin->size, pWin->Freq, freqA);
  int nf2 = binary_search(pWin->size, pWin->Freq, freqB);
  
  double sumPwindow = 0;
  for(int i=0; i<pWin->size; i++) sumPwindow+=pWin->Power[i];
  outputPwr* pOut = powerspectrum(dataset, fInitial, fFinal, df);
  
  for(int j=0; j<dataset->size; j++) {
    double bandj = 0;
    for(int i=nf1; i<nf2; i++) {
      bandj += (pOut->Alpha[i]*sin(2*M_PI*pOut->Freq[i]*dataset->Time[j])
	      + pOut->Beta[i]*cos(2*M_PI*pOut->Freq[i]*dataset->Time[j]))/sumPwindow;
    }
    dataset->Data[j] = bandj;
  }
}

void highpass(dataStruct* dataset, double fInitial, double fFinal, double df,
	      double f, double freqHigh) {
  outputPwr* pWin = windowfunction(dataset, f, fInitial, fFinal, df);
  int nf1 = 0;
  int nf2 = binary_search(pWin->size, pWin->Freq, freqHigh);
  
  double sumPwindow = 0;
  for(int i=0; i<pWin->size; i++) sumPwindow+=pWin->Power[i];
  outputPwr* pOut = powerspectrum(dataset, fInitial, fFinal, df);
  
  for(int j=0; j<dataset->size; j++) {
    double bandj = 0;
    for(int i=nf1; i<nf2; i++) {
      bandj += (pOut->Alpha[i]*sin(2*M_PI*pOut->Freq[i]*dataset->Time[j])
		+ pOut->Beta[i]*cos(2*M_PI*pOut->Freq[i]*dataset->Time[j]))/sumPwindow;
    }
    dataset->Data[j] -= bandj;
  }
}


void lowpass(dataStruct* dataset, double fInitial, double fFinal, double df,
	     double f, double freqLow) {
  bandpass(dataset, fInitial, fFinal, df, f, 0, freqLow);
}
