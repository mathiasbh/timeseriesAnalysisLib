typedef struct {int size; double *Time, *Data, *Weight;} dataStruct;
typedef struct {int size; double *Freq, *Power, *Alpha, *Beta;} outputPwr;
typedef struct {int size; double *Freq, *Ampl;} outputClean;

dataStruct*  data_alloc(int n);
outputPwr*   outPwr_alloc(int n);
outputClean* outClean_alloc(int n);
void data_free(dataStruct* d);
void outPwr_free(outputPwr* d);
void outClean_free(outputClean* d);

dataStruct* readdata(char* filename, int columns, int subMean);
outputPwr*  powerspectrum(dataStruct* d, double f0, double f1, double df);
outputPwr*  windowfunction(dataStruct* d, double f, double f0, double f1, double df);

outputClean* clean(dataStruct* data, double f0, double f1, double df, int numberPeaks);

void bandpass(dataStruct* dataset, double f0, double f1, double df, double f, double freqA,
	      double freqB);
void highpass(dataStruct* dataset, double f0, double f1, double df, double f, double freqHigh);
void lowpass (dataStruct* dataset, double f0, double f1, double df, double f, double freqLow);

double median(double arr[], int n);
void movingMedian(double x[], double meanarr[], int size, int N);
void movingMean(double x[], double medianarr[], int size, int N);
void scatter2(double x[], double scatter[], int size, int N);
int findMaximum(int size, double y[]);
int findMaximumLimx(int size, double x[], double y[], double limit0, double limit1);
void correctJump(dataStruct* d, int numPeaks);
void difCalc(dataStruct* d, double dif[], double cutoff, int n);
int binary_search(int n, double *x, double z);

dataStruct* ccmodel(dataStruct* d, double period, double phase, double deltaT, int numPeaks);
void xcorr(dataStruct* d, double p1, double p2, double periodRes, double phaseRes,
	   double deltaT, int numPeaks, double *periodResult, double *phaseResult);

dataStruct* linterpGrid(dataStruct* d, double step);
double autocorr(dataStruct* d, double step, int k);
