/**
 * Author	:	@sohamroy19
 * Date		:	24-04-21
 * 
 * Purpose	:	This is a program that takes an integer n from the user
 * 			and prints all subsets of the set {1, 2, 3, ..., n},
 * 			without repeating the same subset.
 * 
 * Note		:	The bottleneck seems to be the I/O.
 * 			Time complexity n * 2^n
 * 			Space complexity of O(n).
 */

#include <stdio.h>		// for I/O
#include <stdlib.h>		// for malloc()

void middleMan(int n);
int increment(int *subset, int n);
void display(int *subset, int n);

int main(int argc, char *argv[]) {
	printf("\n"
		   "This is a program that takes an integer n from the user\n"
		   "and prints all subsets of the set {1, 2, 3, ..., n},\n"
		   "without repeating the same subset.\n\n");

	int n;
	printf("Enter integer : ");
	scanf("%d", &n);
	getchar();
	printf("Subsets : \n");
	middleMan(n);

	getchar();
	return 0;
}

/**
 * Purpose	:	Simply allocates and initialises the subset array,
 * 			and increments until we get our complete array.
 */
void middleMan(int n) {
	// holds the subset of current interest, with trailing zeros as placeholder
	int *subset = malloc(sizeof(int) * n);

	for (int i; i < n; i++) {
		subset[i] = 0;
	}

	// increments until the increment() returns 0,
	// i.e. cannot increment further
	while (increment(subset, n))
		;
	
	free(subset);
}

/**
 * Purpose	:	Increments the subset to the next subset, and prints it.
 * Outputs	:	Returns 1 if this is possible, 0 otherwise.
 */
int increment(int *subset, int n) {
	int last = 0;

	// identify the last non-zero element
	while (subset[last + 1]) {
		last++;
		if (last == n - 1)
			break;
	}

	// simply increment, if possible
	for (int i = last; i >= 0; i--) {
		if (subset[i] < n + i - last) {
			int temp = subset[i];

			// set the rest of the subset to the lowest possible allowed
			// eg. 1 2 5 6 0 0  ->  1 3 4 5 0 0
			for (int j = i; j <= last; j++) {
				subset[j] = ++temp;
			}

			display(subset, n);
			return 1;
		}
	}

	// if there are no more elements, then exit by returning 0
	if (last == n - 1)
		return 0;

	// otherwise increase size of subset by 1
	for (int i = 0; i <= last + 1; i++) {
		subset[i] = i + 1;
	}
	display(subset, n);
	return 1;
}

/**
 * Purpose	:	Displays the non-zero elements of the subset
 */
void display(int *subset, int n) {
	for (int i = 0; i < n && subset[i]; i++) {
		printf("%d ", subset[i]);
	}
	printf("\n");
}
