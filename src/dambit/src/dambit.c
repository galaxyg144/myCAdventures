#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// Check if a number is prime
bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// Count divisors (excluding 1 and itself)
int divisorCount(int x) {
    int count = 0;
    for (int d = 2; d < x; d++) {
        if (x % d == 0) count++;
    }
    return count;
}

// Full scoring function with breakdown printing
int score(int x) {
    // Stage 1: Initial formula
    int initial = (int)ceil((2 * x) + 0.83);
    printf("Stage 1 (initial): %d\n", initial);

    // Stage 2: Prime rule
    int stage2 = isPrime(x) ? (2 * initial + 5) : initial;
    printf("Stage 2 (prime rule): %d\n", stage2);

    // Stage 3: Bigness relative to 100
    int stage3 = (stage2 > 100) ? stage2 + (stage2 - 100) : stage2;
    printf("Stage 3 (bigness adjustment): %d\n", stage3);

    // Stage 3a: WOAH TOO BIG (random chop 10–20%)
    double stage3a = stage3;
    if (x > 1000) {
        double chopPercent = 0.10 + ((double)rand() / RAND_MAX) * 0.10; // random between 10–20%
        stage3a = stage3 * (1.0 - chopPercent);
        printf("Stage 3a (WOAH TOO BIG): %.2f (chopped %.1f%%)\n", stage3a, chopPercent * 100);
    } else {
        printf("Stage 3a (WOAH TOO BIG): %.2f (no chop)\n", stage3a);
    }

    // Stage 4: Divisor penalty
    int divisors = divisorCount(x);
    double stage4 = stage3a - (divisors * 1.5);
    printf("Stage 4 (divisor penalty): %.2f (divisors=%d)\n", stage4, divisors);

    // Stage 5: Smallness adjustment
    int l = 100 - x;
    double b = round((double)x / 25.0) - (5.0 * l / 10.0);
    double stage5 = stage4 + b;
    printf("Stage 5 (smallness adjustment): %.2f (b=%.2f)\n", stage5, b);

    // Stage 6: Final ceiling
    int finalScore = (int)ceil(stage5);
    printf("Final Score (ceiling): %d\n", finalScore);

    return finalScore;
}

int main() {
    srand(time(NULL)); // seed random once

    int x;
    printf("Enter a number: ");
    scanf("%d", &x);

    printf("\n--- Score Breakdown for %d ---\n", x);
    int result = score(x);
    printf("\nFinal score for %d is %d\n", x, result);

    return 0;
}
