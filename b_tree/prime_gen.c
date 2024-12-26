#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int isPrime(int num) {
    if (num <= 1) return 0;
    if (num <= 3) return 1;
    if (num % 2 == 0 || num % 3 == 0) return 0;
    for (int i = 5; i * i <= num; i += 6) {
	if (num % i == 0 || num % (i + 2) == 0) return 0;
    }
    return 1;
}

int main() {
    FILE *file = fopen("primes.txt", "w");
    if (file == NULL) {
	perror("Failed to create primes.txt");
	return EXIT_FAILURE;
    }

    int count = 0;
    int num = 2;
    while (count < 10000) {
	if (isPrime(num)) {
	    fprintf(file, "%d\n", num);
	    count++;
	}
	num++;
    }

    fclose(file);
    return 0;
}