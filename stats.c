/**
 * Program:			ststs.c
 * Date:			Feb 13, 2020
 * Author:			Priscilla Peron
 * Description:     this console application compiles a series of statistics on a list of real number pairs.
					Signalized by comment code excerpts were taken from Garth Santor's codes made during classes
					while working on Dynamic Memory example.
 */

// from Garth Santor's code during classes
#define _CRT_SECURE_NO_WARNINGS
#include "stats_t.h"
#include <stdio.h>
#include <ctype.h>
#include <math.h>

/**
	* Method:		swap
	* Purpose:		to swap the contents of the two memory locations
	* Accepts:		two double variables to be swaped
	* Returns:		void
*/
// from Garth Santor's code during classes
void swap(double* pa, double* pb) {
	double hold = *pa;
	*pa = *pb;
	*pb = hold;
}

/**
	* Method:		calculateMedian
	* Purpose:		calculate the median of an array of values.
	* Accepts:		one array_t structure holding values to be calculated and a size_t size
					to determine the middle of the for loop and the index to be used for the median
	* Returns:		a double representing the median of the array
*/
double calculateMedian(array_t arr, size_t size)
{
	if (size % 2 == 0)
		return (arr.data[(size - 1) / 2] + arr.data[size / 2]) / 2.0;
	else
		return  arr.data[size / 2];
}

/**
	* Method:		shell_sort
	* Purpose:		to sort an array using Shell's sort.
	* Accepts:		two array_t structures to be sorted and a size_t size to determine the middle of the arrays when divided by 2
	* Returns:		void
*/
// from Garth Santor's code during classes
void shell_sort(array_t arr)
{/*
	size_t stepSize = size / 2;
	while (stepSize > 0) {
		for (size_t idxLastInSegment = stepSize; idxLastInSegment < size; ++idxLastInSegment) {
			size_t idxCurrent = idxLastInSegment;
			while (idxCurrent >= stepSize && x.data[idxCurrent] < x.data[idxCurrent - stepSize]) {
				swap(&x.data[idxCurrent], &x.data[idxCurrent - stepSize]);
				idxCurrent -= stepSize;
			}
		}
		stepSize /= 2;
	}*/
	double m = calculateMedian(arr, arr.nSize);
	double left = arr.data[0];
	double right = arr.data[arr.nSize - 1];
	double n1 = m - left + 1;
	double n2 = right - m;
	size_t i, j, k = 0;

	/* create temporary arrays to hold the sorted halves */
	array_t L = array();
	array_t R = array();

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		array_push_back(&L, arr.data[i]);
	for (j = 0; j < n2; j++)
		array_push_back(&R, arr.data[(int)m + 1 + j]);

	/* Merge the temp arrays back into arr[l..r]*/
	while (i < n1 && j < n2)
	{
		if (L.data[i] <= R.data[j])
		{
			arr.data[k] = L.data[i];
			i++;
		}
		else
		{
			arr.data[k] = R.data[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there
		are any */
	while (i < n1)
	{
		arr.data[k] = L.data[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
		are any */
	while (j < n2)
	{
		arr.data[k] = R.data[j];
		j++;
		k++;
	}
	
}

void mergeSort(array_t arr)
{
	double left = arr.data[0];
	double right = arr.data[arr.nSize - 1]; 

	if (left < right)
	{
		// Same as (l+r)/2, but avoids overflow for 
		// large l and h 
		double m = arr.data[(int)(left + (right - left) / 2.0)];

		// Sort first and second halves 
		mergeSort(arr, left, m);
		mergeSort(arr, (m + 1), right);

		shell_sort(arr, left, m, right);
	}
}

/**
	* Method:		calculateMean
	* Purpose:		calculate the mean of an array of values.
	* Accepts:		one array_t structure holding values to be calculated and a size_t size 
					to determine the end of the for loop and divide the sum of the array_t elements
	* Returns:		a double representing the mean of the array
*/

double calculateMean(array_t arr, size_t size)
{
	double sum = 0.0;

	for (size_t i = 0; i < size; i++)
		sum += arr.data[i];

	return sum / size;
}
//
///**
//	* Method:		calculateMedian
//	* Purpose:		calculate the median of an array of values.
//	* Accepts:		one array_t structure holding values to be calculated and a size_t size 
//					to determine the middle of the for loop and the index to be used for the median
//	* Returns:		a double representing the median of the array
//*/
//double calculateMedian(array_t arr, size_t size)
//{	
//	if (size % 2 == 0)
//		return (arr.data[(size-1)/2] + arr.data[size/2])/2.0;
//	else
//		return  arr.data[size/2];
//}

/**
	* Method:		calculateMode
	* Purpose:		calculate the mode of an array of values.
	* Accepts:		two array_t structures, one holding values to be calculated and one empty array
					to hold the mode values and a size_t size to determine the end of the for loops
	* Returns:		a double representing the mean absolute deviation of the array
*/
int calculateMode(array_t arr, array_t* values, size_t size)
{
	int c = 1, frequency = 2;
	double valuem;

	for (size_t i = 1; i < size; i++)
	{
		if (arr.data[i] == arr.data[i - 1])
			++c;
		else if (c > 1 && c == frequency)
		{
			frequency = c;
			c = 1;
			valuem = arr.data[i - 1];
			array_push_back(values, valuem);
		}
		else if (c > frequency)
		{
			frequency = c;
			c = 1;
			valuem = arr.data[i - 1];
			array_free(values);
			array_push_back(values, valuem);
		}
		if (i == (size - 1))
		{
			if (c > 1 && c == frequency)
			{
				frequency = c;
				c = 1;
				valuem = arr.data[i];
				array_push_back(values, valuem);
			}
			else if (c > frequency)
			{
				frequency = c;
				c = 1;
				valuem = arr.data[i];
				array_free(values);
				array_push_back(values, valuem);
			}
		}
	}

	if ((frequency * (*values).nSize) == arr.nSize)
	{
		frequency = 0;
		array_free(values);
	}
	return frequency;
}

void printMode(array_t valuesx, array_t valuesy, int freqx, int freqy) 
{

	printf("\nmode");

	if (freqx == 0)
		printf("%*s", 41, "no mode");
	else
		printf("%*s%d", 40, "freq.=", freqx);

	if (freqy == 0)
		printf("%*s", 20, "no mode");
	else
		printf("%*s%d", 19, "freq.=", freqy);

	if (freqx != 0)
	{
		for (size_t i = 0; i < valuesx.nSize; i++)
			printf("\n%45.6lf", valuesx.data[i]);

	}

	if (freqy != 0)
	{
		if (freqx != 0)
		{
			for (size_t i = 0; i < valuesy.nSize; i++)
				printf("%20.6lf", valuesy.data[i]);

		}
		else {
			for (size_t i = 0; i < valuesy.nSize; i++)
				printf("\n%60.6lf", valuesy.data[i]);

		}
	}
}


/**
	* Method:		calculateMAD
	* Purpose:		calculate the absolute deviation of an array of values.
	* Accepts:		one array_t structure holding values to be calculated and a size_t size 
					to determine the end of the for loop and divide the sum of the absolute
					values of array_t elements 
	* Returns:		a double representing the mean absolute deviation of the array
*/
double calculateMAD(array_t arr, size_t size, double m)
{
	// Calculate the sum of absolute deviation. 
	double absSum = 0;
	for (size_t i = 0; i < size; i++)
	{
		if ((arr.data[i] - m) > 0)
			absSum += arr.data[i] - m;
		else
			absSum += (arr.data[i] - m) *-1;
	}
	// Return absolute deviation. 
	return absSum / size;
}

void printMADMode(array_t x, array_t y, size_t size, array_t modex, array_t modey) 
{
	printf("\n-> about the mode");
	if (modex.nSize == 0) 
		printf("%*s%", 28, "no mode");
	else if (modex.nSize > 1) 
		printf("%*s%", 28, "multi-modal");
	else 
		printf("%28.3lf", calculateMAD(x, size, modex.data[0]));

	if (modey.nSize == 0)
		printf("%*s%", 20, "no mode");
	else if (modey.nSize > 1)
		printf("%*s%", 20, "multi-modal");
	else
		printf("%20.3lf", calculateMAD(y, size, modey.data[0]));
}

/**
	* Method:		calculateVariance
	* Purpose:		calculate the variance of an array of values.
	* Accepts:		one array_t structure and a size_t size to determine the end of the for loops
	* Returns:		a double representing the variance of the array
*/
double calculateVariance(array_t arr, size_t size)
{
	// Compute mean (average of elements) 
	double sum = 0;
	for (size_t i = 0; i < size; i++)
		sum += arr.data[i];
	double mean = (double)sum /
		(double)size;

	// Compute sum squared differences with mean. 
	double differencesq = 0;
	for (size_t i = 0; i < size; i++)
		differencesq += (arr.data[i] - mean) *
		(arr.data[i] - mean);
	return differencesq / (double)size;
}

/**
	* Method:		calculateStdDev
	* Purpose:		calculate the standard deviation of an array of values.
	* Accepts:		one array_t structure holding the values to be calculated 
					and a size_t size to determine the end of the for loops
	* Returns:		a double representing the standard deviation of the array
*/
double calculateStdDev(array_t arr, size_t size) 
{
	return sqrt(calculateVariance(arr, size));
}

/**
	* Method:		calculateRegressionLine
	* Purpose:		calculate the regression line of an array of values.
	* Accepts:		one array_t structure holding the values to be calculated
					and a size_t size to determine the end of the for loops
	* Returns:		a double representing the standard deviation of the array
*/
void calculateRegressionLine(array_t* x, array_t* y, size_t size, double* a, double* b) 
{
	double sumBoth = 0.0, sumx = 0.0, sumy = 0.0, sumx2 = 0.0;
	
	for (size_t i = 0; i < size; i++) 
	{
		sumx += (*x).data[i];
		sumy += (*y).data[i];
		sumBoth += (*x).data[i] * (*y).data[i];
		sumx2 += pow((*x).data[i],2);
	}

	*b = (size * sumBoth - sumx * sumy) / (size * sumx2 - pow(sumx,2));
	*a = (sumy - *b * sumx) / size;

}

double midPoint(array_t arr, size_t size, double* midX) 
{

	*midX = (arr.data[0] + arr.data[size - 1]) / 2;

	return 0;
}


int calculateOutliers2x(array_t x, array_t y, array_t* outlier1, array_t* outlier2, size_t size)
{
	int numOutliers = 0;
	double xstdDeviation2x = calculateStdDev(x, x.nSize) * 2;
	double ystdDeviation2x = calculateStdDev(y, x.nSize) * 2;
	double xmean = calculateMean(x, x.nSize);
	double ymean = calculateMean(y, y.nSize);

	// Verify if there are outliers on x
	for (size_t i = 0; i < x.nSize; i++)
	{
		if (((xmean - x.data[i]) * -1) >= xstdDeviation2x || (xmean - x.data[i]) >= xstdDeviation2x)
		{
			numOutliers++;
			array_push_back(outlier1, x.data[i]);
			array_push_back(outlier2, y.data[i]);
		}
	}

	// Verify if there are outliers on y
	for (size_t i = 0; i < y.nSize; i++)
	{
		if (((ymean - y.data[i]) * -1) >= ystdDeviation2x || (ymean - y.data[i]) >= ystdDeviation2x)
		{
			numOutliers++;
			array_push_back(outlier1, x.data[i]);
			array_push_back(outlier2, y.data[i]);
		}
	}

	if (numOutliers == 0) {
		printf("%*s%", -30, "no outliers");
	}
	else {
		printf("%*s %-30.d", -30, "# outliers =", numOutliers);
		for (size_t i = 0; i < x.nSize; i++) {
			printf(
				"\n%38.3lf %20.3lf", y.data[i], x.data[i]);
		}
	}

	return numOutliers;
}


int calculateOutliers3x(array_t x, array_t y, array_t* outlier1, array_t* outlier2, size_t size)
{
	int numOutliers = 0;
	double xstdDeviation3x = calculateStdDev(x, x.nSize) * 3;
	double ystdDeviation3x = calculateStdDev(y, x.nSize) * 3;
	double xmean = calculateMean(x, x.nSize);
	double ymean = calculateMean(y, y.nSize);

	// Verify if there are outliers on x
	for (size_t i = 0; i < x.nSize; i++)
	{
		if (((xmean - x.data[i]) * -1) >= xstdDeviation3x || (xmean - x.data[i]) >= xstdDeviation3x)
		{
			numOutliers++;
			array_push_back(outlier1, x.data[i]);
			array_push_back(outlier2, y.data[i]);
		}
	}
	// Verify if there are outliers on y
	for (size_t i = 0; i < y.nSize; i++)
	{
		if (((ymean - y.data[i]) * -1) >= ystdDeviation3x || (ymean - y.data[i]) >= ystdDeviation3x)
		{
			numOutliers++;
			array_push_back(outlier1, x.data[i]);
			array_push_back(outlier2, y.data[i]);
		}
	}

	if (numOutliers == 0) {
		printf("%*s%", -30, "no outliers");
	}
	else {
		printf("%*s %-30.d", -30, "# outliers =", numOutliers);
		for (size_t i = 0; i < x.nSize; i++) {
			printf(
				"\n%38.3lf %20.3lf", y.data[i], x.data[i]);
		}
	}

	return numOutliers;
}


/*	Process entry point receives command line arguments.
    argc counts the parameters and argv stores the parameters.
*/
int main(int argc, char* argv[])
{
// from Garth Santor's code during classes
#if defined(_DEBUG)
	int dbgFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	dbgFlags |= _CRTDBG_CHECK_ALWAYS_DF;
	dbgFlags |= _CRTDBG_DELAY_FREE_MEM_DF; // whenever said "free memory" don't throw away but set aside; it helps us save and catch pointers
	dbgFlags |= _CRTDBG_LEAK_CHECK_DF; // checks for memory leaks
#endif
	FILE* stream = stdin; // default

	
	array_t x = array();
	array_t y = array();
	array_t valuesx = array();
	array_t valuesy = array();
	array_t outlier1 = array();
	array_t outlier2 = array();
	char ch;
	double number1 = 0, number2 = 0;
	double a = 0; 
	double b = 0;
	double yAtMidX;
	double midX;
	int space = -30;
	int spc = 9;
	int spc1 = 6;
	
	// from Garth Santor's code during classes (parts of if else statements)
	// checking for too many arguments
	if (argc > 2) 
	{
		printf("Error: too many arguments (%d). Try again\n", argc);
		return EXIT_FAILURE;
	}
	else if (argc == 2) 
	{
		printf("Opening file: %s\n", argv[1]);
		stream = fopen(argv[1], "rb");
		if (stream == NULL)
		{
			printf("Error: the file could not be opened.");
			return EXIT_FAILURE;
		}

		while (fscanf_s(stream, " %lf , %lf", &number1, &number2) != EOF)
		{
			array_push_back(&x, number1);
			array_push_back(&y, number2);
		}
	}
	else if (argc == 1)
	{
		printf("Enter a list of comma-separated real number pairs terminated by ^Z:\n");

		while (fscanf_s(stream, " %lf , %lf", &number1, &number2))
		{
			ch = getchar();
			
			if (ch == EOF || isalpha(ch))
			{
				printf("STOP\n");
				break;
			}
			else if (isspace(ch) || ch == '\n')
			{
				while (isspace(ch) || (ch == '\n'))
					ch = getchar();
				ungetc(ch, stream);
			}
			array_push_back(&x, number1);
			array_push_back(&y, number2);
		}
	}
	
	mergeSort(x);
	mergeSort(y);

	for (size_t i = 0; i < x.nSize; i++)
		printf("SORTED: X\t%lf, Y\t%lf", x.data[i], y.data[i]);

	double modex = calculateMode(x, &valuesx, x.nSize);
	double modey = calculateMode(y, &valuesy, y.nSize);
	
	printf("Results:\n");
	printf("------------------------------------------------------------------------------\n");
	printf("\n%*s%15u%20u", space, "# elements", x.nSize, y.nSize);
	printf("\n%*s%15.3lf%20.3lf", space, "minimum", x.data[0], y.data[0]);
	printf("\n%*s%15.3lf%20.3lf", space, "maximum", x.data[x.nSize-1], y.data[y.nSize-1]);
	printf("\n%*s%15.3lf%20.3lf", space, "mean", calculateMean(x, x.nSize), calculateMean(y, y.nSize));
	printf("\n%*s%15.3lf%20.3lf", space, "median", calculateMedian(x, x.nSize), calculateMedian(y, y.nSize));
	printMode(valuesx, valuesy, modex, modey);
	printf("\n%*s%15.3lf%20.3lf", space, "variance", calculateVariance(x, x.nSize), calculateVariance(y, y.nSize));
	printf("\n%*s%15.3lf%20.3lf", space, "stdDeviation", calculateStdDev(x, x.nSize), calculateStdDev(y, y.nSize));
	printf("\nmean absolute deviations:");
	printf("\n%*s%15.3lf%20.3lf", space, "-> about the mean", calculateMAD(x, x.nSize, calculateMean(x, x.nSize)), calculateMAD(y, y.nSize, calculateMean(y, y.nSize)));
	printf("\n%*s%15.3lf%20.3lf", space, "-> about the median", calculateMAD(x, x.nSize, calculateMedian(x, x.nSize)), calculateMAD(y, y.nSize, calculateMedian(y, y.nSize)));
	printMADMode(x, y, x.nSize, valuesx, valuesy);
	calculateRegressionLine(&x, &y, x.nSize, &a, &b);
	printf("\n%*s%*s%-15.3lf%*s%-10.3lf", space, "regression line", spc, "a= ", a, spc1, "b= ", b);
	midPoint(x, x.nSize, &midX);
	yAtMidX = b * midX + a;
	printf("\n%*s%*s%-15.3lf%*s%-10.3lf", space, "Y at mid(X)", spc - 1, "x= ", midX, spc1, "y= ", yAtMidX);
	printf("\nOutliers(2x)=");
	calculateOutliers2x(x, y, &outlier1, &outlier2, x.nSize), calculateOutliers2x(y, x, &outlier2, &outlier1, y.nSize);
	array_free(&outlier1);
	array_free(&outlier2);
	printf("\nOutliers(3x)=");
	calculateOutliers3x(x, y, &outlier1, &outlier2, x.nSize), calculateOutliers3x(y, x, &outlier2, &outlier1, y.nSize);
	
	array_free(&x);
	array_free(&y);
	array_free(&valuesx);
	array_free(&valuesy);
	array_free(&outlier1);
	array_free(&outlier2);

	fclose(stream);

	return 0;
}