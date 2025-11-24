# x86-64 Assembly to C Interface: Car Acceleration Calculator

## Group Members

* Simbillo, Jose Miguel B.

## Project Description

This project implements a hybrid C and x86-64 Assembly program to calculate the acceleration of cars based on Initial Velocity ($V_i$), Final Velocity ($V_f$), and Time ($T$).

* **C:** Handles memory allocation, random data generation, and performance timing. It also includes a C implementation of the logic for correctness checking.
* **Assembly:** Handles the core logic using **Scalar SIMD (single-precision floating point)** instructions to perform the calculation: $A = (V_f - V_i) / 3.6 / T$, and converts the result to an integer.

---

## Correctness Check

To verify the correctness of the Assembly function, the program runs the same input data through both the x86-64 Assembly function (`calcAccel`) and a C implementation (`cCalc`). The results are compared side-by-side to ensure they match.

### Methodology
1.  User inputs the number of cars and the matrix data (Vi, Vf, T).
2.  The program calculates results using Assembly.
3.  The program calculates results using C.
4.  If `ASM Result == C Result`, the status is "MATCH".

<img width="570" height="527" alt="image" src="https://github.com/user-attachments/assets/db83edfa-3891-462b-98e6-2838dd4c41ab" />

---

## Performance Analysis

We measured the execution time of both the Assembly function and the C function across different vector sizes ($N$). The test was run 30 times per size to calculate the average execution time in seconds.

### Execution Results

<img width="633" height="443" alt="image" src="https://github.com/user-attachments/assets/eaafdcdc-277b-4c30-a496-9a7cc5554562" />

### Analysis

For input sizes up to 1,000, both implementations executed instantly (`0.000000` seconds) due to the high speed of modern processors handling small datasets.

However, at the vector size of **10,000**, a performance divergence appeared:

1.  **Assembly Efficiency:** The x86-64 Assembly implementation remained at `0.000000` seconds. This indicates that the manual use of scalar SIMD instructions (`subss`, `divss`) was efficient enough to complete the 10,000 calculations within the system clock's resolution threshold (under ~1ms).
2.  **C Performance:** The C implementation registered a measurable time of `0.000033` seconds.
3.  **Conclusion:** This demonstrates that for this specific workload, the hand-optimized Assembly code performed measurably faster than the C code. While C compilers are highly optimized, the direct register control in Assembly provided a slight edge in execution speed that became visible as the dataset size increased.

## Files

* `main.c`: C driver program (Input and Output, Memory Allocation, Correctness Checking, Performance Testing).
* `calc_accel.asm`: x86-64 NASM Assembly implementation of the acceleration logic.
