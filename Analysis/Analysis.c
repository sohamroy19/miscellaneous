/**
 * Author	:	@sohamroy19
 * Date		:	06-03-2021
 *
 * Purpose	:	This program analyses the data provided by the flow cyclometer and
 * 			finds the location and width of each droplet, to then find the mean and
 * 			the standard deviation of the arrival times between the droplets.
 *
 * Inputs	:	data.txt: two columns of data
 * Outputs	:	output.txt: 3 columns of output
 * 			histo2.txt: histogram data for column 2
 * 			histo1.txt: histogram data for column 1
 * 			Tabulated data outputted to the terminal
 */

#include <math.h>		// include math functions
#include <stdio.h>		// include standard input/output
#include <stdlib.h>		// include standard library

/* function prototypes */
void readData(double *col1, double *col2);
double movingAverage(double *data, double *averages, int window);
void writeData(double *avg1, double *avg2, double pDev2, int *peaks1, int *peaks2);
void col2Analysis(double *col2, double *avg2, double pDev2, int *peaks2);
void col1Analysis(double *col1, double *avg1, int *peaks1);
int isMaximum(double *avg1, int index, int limit);
void statistics(int *peaks, FILE *fp);

/* Purpose	:	It is the main function- calls other functions as needed
 * 			also allocates and frees large arrays as needed
 */
int main() {
	// arrays store columns of data and properties
	double *col1 = malloc(sizeof(double) * 9800);
	double *col2 = malloc(sizeof(double) * 9800);
	double *avg1 = malloc(sizeof(double) * 9800);
	double *avg2 = malloc(sizeof(double) * 9800);
	int peaks1[54], peaks2[54];

	readData(col1, col2);
	/* large window is taken for column 2 so that minimas stand out prominently */
	/* small window is taked for column 1 so that the trend is shown accurately */
	double pDev2 = 2.4 * movingAverage(col2, avg2, 101);	// 101 = window, 2.4 = p
	movingAverage(col1, avg1, 29);				// 29  = window
	col2Analysis(col2, avg2, pDev2, peaks2);
	col1Analysis(col1, avg1, peaks1);
	writeData(avg1, avg2, pDev2, peaks1, peaks2);

	free(col1); free(col2); free(avg1); free(avg2);
	return 0;
}

/* Purpose	:	Reads data from data.txt into the two arrays col1 and col2
 * Inputs	:	col1, col2: array pointers for 2 columns of data.
 * Outputs	:	reads data into col1, col2
 */
void readData(double *col1, double *col2) {
	FILE * fp;
	fp = fopen("data.txt", "r");

	for (int i = 0; i < 9800; i++) {
		fscanf(fp, "%lf %lf", (col1 + i), (col2 + i));
		if (col1[i] < 0) col1[i] = -col1[i];
	}
	fclose(fp);
}

/* Purpose	:	Writes the output to file output.txt,
 * 			calls statistics() to perform statistics
 * Inputs	:	columns of data to be outputted
 * Outputs	:	output.txt: 3 columns of data
 * 			histo2.txt: histogram data for column 2
 * 			histo1.txt: histogram data for column 1
 */
void writeData(double *avg1, double *avg2, double pDev2, int *peaks1, int *peaks2) {
	FILE *fp;
	fp = fopen("output.txt", "w");

	for (int i = 0; i < 9800; i++) {
		fprintf(fp, "%lf %lf %lf\n", avg2[i], avg2[i] - pDev2, avg1[i]);
	}
	fclose(fp);

	fp = fopen("histo2.txt", "w");
	printf("\nColumn 2 Stats -\n");
	statistics(peaks2, fp);			// perform necessary statistics
	fclose(fp);
	
	fp = fopen("histo1.txt", "w");
	printf("\nColumn 1 Stats -\n");
	statistics(peaks1, fp);
	fclose(fp);
}

/* Purpose	:	Applies a moving average (low pass) filter to the data,
 * 			stores the averages in the passed array and returns stdev
 * Inputs	:	data: array of input data
 * 			averages, stdevs: arrays to store averages and stdevs
 * Outputs	:	sets the values of averages and returns stdev
 */
double movingAverage(double *data, double *averages, int window) {
	int h = (window - 1) / 2;			// half the window
	double mAvg = 0, stdev = 0;			// initialise moving avg, stdev

	for (int i = 0; i < 9800 + h; i++) {
		if (i < h) {				// if window not yet complete
			mAvg += data[i] / window;	// add to it
			continue;
		} else if (i < window) {		// if window is just complete
			mAvg += data[i] / window;	// average over visible part
			averages[i - h] = mAvg * window / i;
		} else if (i < 9800) {			// traverse regularly
			mAvg += data[i] / window;	// add to it, then subtract
			mAvg -= data[i - window] / window;	
			averages[i - h] = mAvg;
		} else {				// at end of array
			mAvg -= data[i - window] / window;
			averages[i - h] = mAvg * window / (window + 9799 - i);
		}

		stdev += ( (data[i - h] - averages[i - h]) * (data[i - h] - averages[i - h]) );
	}

	stdev = sqrt(stdev / (9800 + 1 - window));
	return stdev;
}

/* Purpose	:	Analyses the column 2 data by thresholding with p = 2.4
 * 			Bollinger band and taking minima of each valid interval
 * Inputs	:	*col2: data of column 2
 * 			*avg2: moving averages of column 2
 * 			pDev2: 2.4 * global standard deviation
 * 			*peaks2: array to store peak locations
 * Outputs	:	sets peak values in peaks array
 */
void col2Analysis(double *col2, double *avg2, double pDev2, int *peaks2) {
	int min = -1, count = 0, current = 0;		// -1 indicates no min

	for (int i = 0; i < 9800; i++) {
		if (col2[i] < avg2[i] - pDev2) {	// check if outside threshold
			if (min == -1 || col2[min] > col2[i])
				min = i;		// set min if i is min
			count++;
		} else {
			if (count >= 3) {		// if 3 or more consecutive peaks
				peaks2[current] = min;	// final answer is the current min
				current++;
			}
			min = -1;
			count = 0;
		}
	}
}

/* Purpose	:	Analyses the column 1 data by first finding the body of
 * 			the droplet (thresholding below 0.1). Then, we find the
 * 			first significant maxima (by avg) to each side of it.
 * Inputs	:	*col1: data of column 1
 * 			*avg1: moving averages of column 1
 * 			*peaks1: array to store peak locations
 * Outputs	:	sets peak values in peaks array
 */
void col1Analysis(double *col1, double *avg1, int *peaks1) {
	int count = 0, current = 0, droplets[28];

	for (int i = 0; i < 9800; i++) {
		if (avg1[i] < 0.1) count++;
		else count = 0;
		
		if (count == 10) {		// if 10 continuous points < 0.1
			droplets[current] = i;	// then that is a droplet
			current++;
		}
	}
	droplets[27] = 9800;			// dummy droplet at the end

	int bigPeaks[27];
	for (current = 0; current < 27; current++) {
		/* choose the first significant maxima to right of droplet */
		for (int i = droplets[current]; i < droplets[current + 1]; i++) {
			if (avg1[i] > 0.1 && isMaximum(avg1, i, 10)) {
				bigPeaks[current] = i;
				break;
			}
		}
	}
	
	int smallPeaks[27];
	smallPeaks[0] = 0;			// first small peak is at 0
	for (current = 1; current < 27; current++) {
		/* choose the first significant maxima to left of droplet */
		for (int i = droplets[current]; i > droplets[current - 1]; i--) {
			if (avg1[i] > 0.1 && isMaximum(avg1, i, 4)) {
				smallPeaks[current] = i;
				break;
			}
		}
	}

	for (int i = 0; i < 27; i++) {		// set peaks
		peaks1[2*i] = smallPeaks[i];
		peaks1[2*i + 1] = bigPeaks[i];
	}
}

/* Purpose	:	Checks whether a point data[index] is a maxima, where
 * 			maxima is defined as a point larger than 'limit'
 * 			number of points on either side of it
 * Inputs	:	*data: array of data to be considered
 * 			index: index of element to be considered
 * 			limit: range on either side to be considered
 * Outputs	:	returns 1 if maximum, 0 if not
 */
int isMaximum(double *data, int index, int limit) {
	for (int i = 1; i < limit; i++) {
		if (data[index - i] > data[index] || data[index] < data[index + i]) return 0;
	}
	return 1;			// if no points within limit is larger
}

/* Purpose	:	Tabulates and finds mean, stdev and writes to file
 * Inputs	:	*peaks: array of peaks
 * 			*fp: file to be outputted to
 * Outputs	:	tabulates data and prints to file pointed to by *fp
 */
void statistics(int *peaks, FILE *fp) {
	double widthMean = 0, widthStdev = 0, timeMean = 0, timeStdev = 0;
	for (int i = 0; i < 27; i++) {
		/* size is the distance between consecutive small peak and big peak */
		int size = peaks[2*i + 1] - peaks[2*i];
		widthMean += size;
		widthStdev += size * size;
		printf("Location: %d\t\tWidth: %d\n", peaks[2*i], size);
		fprintf(fp, "\n%d", size);

		if (i < 26) {
			/* time is the distance between consecutive short peaks */
			int time = peaks[2*i + 2] - peaks[2*i];
			timeMean += time;
			timeStdev += time * time;
			fprintf(fp, "\t%d", time);
		}
	}
	widthMean /= 27; widthStdev /= 27; timeMean /= 26; timeStdev /= 26;
	printf ("Mean Width = %.2f\t\t", widthMean);
	printf ("StDev of Width = %.2f\n", sqrt(widthStdev - widthMean*widthMean));
	printf ("Mean Arrival Time = %.2f\t", timeMean);
	printf ("StDev of Time = %.2f\n", sqrt(timeStdev - timeMean*timeMean));
}
