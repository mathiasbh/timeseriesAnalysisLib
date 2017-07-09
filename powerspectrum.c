#include <math.h>
#include <stdio.h>
#include "header.h"
#define M_PI 3.14159265358979323846264

outputPwr* powerspectrum(dataStruct* d,
			    double fInitial,
			    double fFinal,
			    double fResolution) {
  if(fFinal == 0) {
    double dt[d->size-1];
    for(int i=0; i<d->size-1; i++) {
      dt[i] = d->Time[i+1] - d->Time[i];
    }
    fFinal = 1.0/(2*median(dt, d->size - 1));
    /* fprintf(stderr, "Using Nyquist frequency: %lg\n", fFinal); */
  }
  if(fResolution == 0) {
    fResolution = 1.0/(4*(d->Time[d->size-1] - d->Time[0]));
    /* fprintf(stderr, "Using resolution frequency: %lg\n", fResolution); */
  }
  if(fResolution < 0) {
    fResolution = 1.0/(10*(d->Time[d->size-1] - d->Time[0]));
    /* fprintf(stderr, "Using resolution frequency: %lg\n", fResolution); */
  }

  int n = ceil((fFinal-fInitial)/fResolution);
  outputPwr* output = outPwr_alloc(n);

  /* Calculate powerspectrum - see equations in associated document */
  int i = 0;
  for(double nu = fInitial; nu<fFinal; nu+=fResolution) {
    double s = 0, c = 0, ss = 0, cc = 0, sc = 0;
    for(int j=0; j<d->size; j++) {
      double sinVal = sin(nu * 2*M_PI * d->Time[j]);
      double cosVal = cos(nu * 2*M_PI * d->Time[j]);
      s += d->Data[j] * sinVal * d->Weight[j];
      c += d->Data[j] * cosVal * d->Weight[j];
      ss+= sinVal*sinVal * d->Weight[j];
      cc+= cosVal*cosVal * d->Weight[j];
      sc+= cosVal*sinVal * d->Weight[j];
    }
    double alpha = (s*cc - c*sc)/(ss*cc - sc*sc);
    double beta  = (c*ss - s*sc)/(ss*cc - sc*sc);
    double Pwr = alpha*alpha + beta*beta;
    
    output->Freq[i]  = nu;
    output->Power[i] = Pwr;
    output->Alpha[i] = alpha;
    output->Beta[i]  = beta;
    i++;
  }
  return output;
}

outputPwr* windowfunction(dataStruct* d, double f, double fInitial,
			  double fFinal, double fResolution) {
  /* Calculates windowfunction for dataset (time, amplitude, weight)
     for a frequency f, from fInitial to fFinal with resolution fResolution */
  int n = d->size;
  dataStruct* datacos = data_alloc(n);
  dataStruct* datasin = data_alloc(n);
  for(int i=0; i<n; i++) {
    datacos->Time[i] = d->Time[i]; datacos->Weight[i] = d->Weight[i];
    datacos->Data[i] = cos(2 * M_PI * f * d->Time[i]);
    
    datasin->Time[i] = d->Time[i]; datasin->Weight[i] = d->Weight[i];
    datasin->Data[i] = sin(2 * M_PI * f * d->Time[i]);
  }
  outputPwr* Powercos = powerspectrum(datacos, fInitial, fFinal, fResolution);
  outputPwr* Powersin = powerspectrum(datasin, fInitial, fFinal, fResolution);
  outputPwr* window = outPwr_alloc(Powercos->size);
  for(int i=0; i<window->size; i++) {
    window->Power[i] = 0.5*(Powercos->Power[i] + Powersin->Power[i]);
    window->Freq[i]  = Powercos->Freq[i];
  }
  data_free(datacos); data_free(datasin);
  outPwr_free(Powercos); outPwr_free(Powersin);
  return window;
}
