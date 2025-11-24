#define _CRT_SECURE_NO_WARNINGS // to remove the scanf unsafe warnings
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern void calcAccel(int n, float* inMat, int* outArr);

float randFloat(float min, float max) { // random num gen for the performance test
    float s = rand() / (float)RAND_MAX;
    return min + s * (max - min);
}

void check() {
    int n = 0;

    printf("\n--- CORRECTNESS CHECK ---\n");
    printf("Input: Matrix Rows, single float matrix values\n");
    if (scanf("%d", &n) != 1) return;

    float* mat = (float*)malloc(n * 3 * sizeof(float));
    int* out = (int*)malloc(n * sizeof(int));

    if (!mat || !out) return;

    for (int i = 0; i < n; i++) {
        scanf(" %f, %f, %f",
            &mat[i * 3],
            &mat[i * 3 + 1],
            &mat[i * 3 + 2]);
    }

    calcAccel(n, mat, out);

    printf("\nOutput: Integer acceleration values (m/s^2)\n");
    for (int i = 0; i < n; i++) 
        printf("%d\n", out[i]);

    free(mat);
    free(out);
}

void perf() {
    printf("\n--- PERFORMANCE TEST ---\n");

    int sizes[] = { 10, 100, 1000, 10000 };
    int sCount = 4;
    int runs = 30;

    printf("Running 30 loops per size. Please wait...\n\n");
    printf("| %-10s | %-15s |\n", "Vector Size", "Avg Time (sec)");
    printf("|------------|-----------------|\n");

    for (int i = 0; i < sCount; i++) {
        int n = sizes[i];

        float* mat = (float*)malloc(n * 3 * sizeof(float));
        int* out = (int*)malloc(n * sizeof(int));

        if (!mat || !out) {
            printf("| %-10d | MEMORY ERROR    |\n", n);
            continue;
        }

        for (int j = 0; j < n * 3; j++)
            mat[j] = randFloat(0.0f, 200.0f);

        double total = 0.0;

        for (int r = 0; r < runs; r++) { // gets time and adds to total
            clock_t t1 = clock();
            calcAccel(n, mat, out);
            clock_t t2 = clock();
            total += ((double)(t2 - t1)) / CLOCKS_PER_SEC;
        }

        double avg = total / runs;  // get avg here
        printf("| %-10d | %-15.6f |\n", n, avg);

        free(mat);
        free(out);
    }
    printf("\nDone.\n");
}

int main() {
    int sel = 0;
    srand((unsigned int)time(NULL)); // so that the rng actually rngs

    printf("Select Mode:\n");
    printf("1. Correctness Check\n");
    printf("2. Performance Test\n");
    printf("Enter choice: ");
    scanf("%d", &sel);

    if (sel == 1) check();
    else if (sel == 2) perf();
    else printf("Invalid choice.\n");

    printf("\nPress Enter to exit...");
    getchar(); 
    getchar();
    return 0;
}