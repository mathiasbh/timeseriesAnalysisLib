//
//   Example 1
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "header.h"


int main() {
  // Load data into data structure, readdata(filename, number of columns, subtract mean(1=True,0=False))
  char* filename = "Example_data/Example1.txt";
  dataStruct* dataSt = readdata(filename, 2, 0);

  // Print time series
  printf("#raw: time data\n");
  for(int i=0; i<dataSt->size; i++) {
    printf("%7.10lg %7.10lg\n", dataSt->Time[i] - dataSt->Time[0], dataSt->Data[i]);
  } printf("\n\n");

  // Correct for jump in data, second argument indicates how many jumps
  correctJump(dataSt, 1);

  // Print fixed jump
  printf("#jump: time data\n");
  for(int i=0; i<dataSt->size; i++) {
    printf("%7.10lg %7.10lg\n", dataSt->Time[i], dataSt->Data[i]);
  } printf("\n\n");

  // Calculate powerspectrum
  double freq_initial = 0.0;
  double freq_final = 8.0;
  double freq_resolution = 0.0; // using: 1/(4*(tN - t0))
  outputPwr* outputSt = powerspectrum(dataSt, freq_initial, freq_final, freq_resolution);

  // Print powerspectrum
  printf("#powerspectrum: freq, power, alpha, beta\n");
  for(int i=0; i<outputSt->size; i++) {
    printf("%7.5lg %7.5lg %7.5lg %7.5lg\n",
  	   outputSt->Freq[i],
  	   outputSt->Power[i],
  	   outputSt->Alpha[i],
  	   outputSt->Beta[i]);
  } printf("\n\n");

  // Free allocated data
  data_free(dataSt);
  outPwr_free(outputSt);

  return 0;
}
