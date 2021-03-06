/**
 * EE20B129 Sneha Reddy P
 * EE20B130 Soham Roy
 * EE20B131 Souvik Mandal
 *
 * Data		:  24-02-2021
 *
 * Purpose	:  This program finds Vth for the data given in sgfet.csv, and also
 * 		   displays the equation of the approximate lines.
 *
 * Inputs	:  sgfet.csv: The input data file
 * Outputs	:  output.dat: The output data file
 */

#include <stdio.h> 

/* This struct represents two straight lines, one of which passes through origin */
/* and also the sum of squares of errors of the linear representation of data.   */
typedef struct LinePair { double m1, m2, c2, err; } LinePair;	// c1 = 0 because origin

void readData(double arr[12][401]);
void showData(LinePair *lines, double Vth, int i, FILE *fp);
int binaryMin(LinePair[], int l, int r);
LinePair bestFit(double x[], double y[], double Vth);

/* Purpose	:  This is the main method, and calls the other methods as needed
 * Inputs	:  none
 * Outputs	:  none
 */
int main() {
	double arr[12][401];
	readData(arr);
	FILE *fp;
	fp = fopen("output.dat", "w");
	/* output file declared as showData() is called inside loop */
 
	for (int i = 1; i < 12; i++) {
		const double step = 15.0/400;
		LinePair lArr[160];	// stores best linepairs for 160 values of Vth
		/* start from 2 as starting from 0 or 1 results in division by 0! */
		/* upper limit 160 corresponds to 160*15/400 = 6, Vth must be < 6 */
		for (int j = 2; j < 160; j++) {
			lArr[j] = bestFit(arr[0], arr[i], step * j);
		}
		int min = binaryMin(lArr, 2, 159);
		showData(&lArr[min], step * min, i, fp);
	}

	fclose(fp);
	return 0; 
}

/* Purpose	:  This function reads the data given in the file sgfet.csv
 * 		   and stores it in the passed array
 * Inputs	:  arr: array to be set to data in sgfet.csv
 * Outputs	:  sets the values of arr
 */
void readData(double arr[12][401]) {
	FILE *fp;
	fp = fopen("sgfet.csv", "r");
	char waste[100];
	fscanf(fp, "%s", waste);	// throwaway the first line, no data in it
	for (int i = 0; i < 401; i++) {
		fscanf(fp, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &arr[0][i]
		, &arr[1][i], &arr[2][i], &arr[3][i], &arr[4][i], &arr[5][i], &arr[6][i]
		, &arr[7][i], &arr[8][i], &arr[9][i], &arr[10][i], &arr[11][i]);
	}
	fclose(fp);
}

/* Purpose	:  This is the display method, prints and
 * 		   sends to file the output data
 * Inputs	:  lines: pair of lines
 * 		   Vth: Vth found
 * 		   i: Vg number
 * Outputs	:  output.dat: stores Vth vs Vg data
 */
void showData(LinePair *lines, double Vth, int i, FILE *fp) {
	printf("Vg%d:\n", i);	
	printf("y = %e * x\n", lines->m1);
	printf("y = %e * x + %e\n", lines->m2, lines->c2);
	printf("Vth = %f\n\n", Vth);
	fprintf(fp, "%d %f\n", i, Vth);
}

/* Purpose	:  This function finds the index of the Vth
 * 		   with the minimum error in the array
 * Inputs	:  arr[]: array of linepairs
 * 		   l: left bound
 * 		   r: right bound
 * Outputs	:  returns the index as an int
 */
int binaryMin(LinePair arr[], int l, int r) { 
	if (r <= l) return l;		// base case
		
	int mid = (l + r) / 2;

	/* min is in left half if slope at mid is increasing */
	if (arr[mid].err < arr[mid+1].err) 
		return binaryMin(arr, l, mid); 

	/* otherwise min is in right half */
	return binaryMin(arr, mid + 1, r); 
} 

/* Purpose	:  This function calculates the best lines possible
 * Inputs	:  x[]: x axis values (Vd)
 * 		   y[]: y axis values (Vg)
 * 		   Vth: trial value for Vth
 * Outputs	:  lines: the returned LinePair
 */
LinePair bestFit(double x[], double y[], double Vth) {
	int i = 0, j = 0;
	double sum_xy = 0, sum_x2 = 0, Ith;
	LinePair lines;
	
	// first line will pass through origin with slope m1
	// calculate best for m through regression algorithm with c = 0
	while (x[i] < Vth) {
		sum_xy += x[i] * y[i];
		sum_x2 += x[i] * x[i];
		i++;
	}
	lines.m1 = sum_xy / sum_x2;
	Ith = lines.m1 * Vth;
	sum_xy = 0;
	sum_x2 = 0;
	lines.err = 0;

	// calulate error till Vth
	while (j < i) {
		lines.err += (y[j] - lines.m1 * x[i]) * (y[j] - lines.m1 * x[i]);
		j++;
	}

	// second line passes through (Vth, Ith) with slope m2
	// calculate best fit m through regression algorithm with c = 0
	// by first shifting the origin to (Vth, Ith)
	while (i < 401) {
		sum_xy += (x[i] - Vth) * (y[i] - Ith); 
		sum_x2 += (x[i] - Vth) * (x[i] - Vth); 
		i++;
	}
	lines.m2 = sum_xy / sum_x2;
	lines.c2 = Ith - lines.m2 * Vth;

	// calculate error after Vth
	while (j < 401) {
		lines.err += (y[j] - lines.m2 * x[i] - lines.c2) * (y[j] - lines.m2 * x[i] - lines.c2);
		j++;
	}

	return lines;
}
