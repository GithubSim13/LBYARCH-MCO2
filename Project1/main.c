#define _CRT_SECURE_NO_WARNINGS // to remove the scanf unsafe warnings
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern void calcAccel(int n, float* inMat, int* outArr);

void cCalc(int n, float* mat, int* out) { // c ver of the asm code for correctness check
    for (int i = 0; i < n; i++) {
        float v1 = mat[i * 3];
        float v2 = mat[i * 3 + 1];
        float t = mat[i * 3 + 2];
        float res = (v2 - v1) / 3.6f / t;

        out[i] = (int)(res + 0.5f); // rounding logic equivalent to cvtss2si (round to nearest)
    }
}

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

    int* outAsm = (int*)malloc(n * sizeof(int)); // alloc mem for asm
    int* outC = (int*)malloc(n * sizeof(int)); // alloc mem for c

    if (!mat || !outAsm || !outC) return;

    for (int i = 0; i < n; i++) {
        scanf(" %f, %f, %f",
            &mat[i * 3],
            &mat[i * 3 + 1],
            &mat[i * 3 + 2]);
    }

    calcAccel(n, mat, outAsm);
    cCalc(n, mat, outC);

    printf("\n--- RESULTS COMPARISON ---\n");
    printf("Row | ASM Result | C Result | Status\n");
    printf("----|------------|----------|--------\n");

    int errors = 0;
    for (int i = 0; i < n; i++) {
        printf(" %d  | %-10d | %-8d | ", i + 1, outAsm[i], outC[i]);

        if (outAsm[i] == outC[i]) printf("MATCH\n");
        else {
            printf("ERROR\n");
            errors++;
        }
    }

    free(mat);
    free(outAsm);
    free(outC);
}

void perf() {
    printf("\n--- PERFORMANCE TEST ---\n");

    int sizes[] = { 10, 100, 1000, 10000 };
    int sCount = 4;
    int runs = 30;

    printf("Running %d loops per size.\n\n", runs);
    printf("| %-11s | %-15s | %-15s |\n", "Vector Size", "ASM Avg (sec)", "C Avg (sec)");
    printf("|-------------|-----------------|-----------------|\n");

    for (int i = 0; i < sCount; i++) {
        int n = sizes[i];

        float* mat = (float*)malloc(n * 3 * sizeof(float));
        int* out = (int*)malloc(n * sizeof(int));

        for (int j = 0; j < n * 3; j++)
            mat[j] = randFloat(0.0f, 200.0f);

        double totalAsm = 0.0;
        for (int r = 0; r < runs; r++) {
            clock_t t1 = clock();
            calcAccel(n, mat, out);
            clock_t t2 = clock();
            totalAsm += ((double)(t2 - t1)) / CLOCKS_PER_SEC;
        }
        double avgAsm = totalAsm / runs;

        double totalC = 0.0;
        for (int r = 0; r < runs; r++) {
            clock_t t1 = clock();
            cCalc(n, mat, out);
            clock_t t2 = clock();
            totalC += ((double)(t2 - t1)) / CLOCKS_PER_SEC;
        }
        double avgC = totalC / runs;

        printf("| %-11d | %-15.6f | %-15.6f |\n", n, avgAsm, avgC);

        free(mat);
        free(out);
    }
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