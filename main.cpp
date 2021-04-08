#include <iostream>
#include <ctime>
#define TEST 0

struct pair {
    int value;
    int index;
};

typedef struct pair Pair;

Pair *createPair(int v, int i) {
    Pair *out = new Pair();
    out->index = i;
    out->value = v;
    return out;
}

void mergeSort(Pair **arr, int size) {
    if (size > 1) {
        mergeSort(arr, size / 2);
        mergeSort(arr + size / 2, size - size / 2);

        int i = 0, j = size / 2, z = 0;

        Pair **buf = new Pair *[size];

        while (i < size / 2 && j < size) {
            if (arr[i]->value <= arr[j]->value) {
                buf[z] = arr[i];
                i++;
            } else {
                buf[z] = arr[j];
                j++;
            }
            z++;
        }

        if (i >= size / 2) {
            std::copy(arr + j, arr + size, buf + z);
        }

        if (j >= size) {
            std::copy(arr + i, arr + size / 2, buf + z);
        }

        std::copy(buf, buf + size, arr);
        delete[] buf;
    }
}

int D(int j, int l, int k, const int* A, const int* S){
    return S[k + l] + S[l == 0 ? 0 : l - 1] - 2 * l * A[j] + (2 * j - k) * A[j] - S[j] - S[j == 0 ? 0 : j - 1];
}

int findMin(int j, int k, int left, int right, const int* A, const int* S){
    if (left == right){
        return D(j, left, k, A, S);
    }

    if (right - left == 1){
        int lSum = D(j, left, k, A, S);
        int rSum = D(j, right, k, A, S);
        return lSum < rSum ? lSum : rSum;
    }

    int medium = (left + right) / 2;

    int mSum = D(j, medium, k, A, S);
    int mrSum = D(j, medium + 1, k, A, S);
    int mlSum = D(j, medium - 1, k, A, S);

    if (mrSum < mSum && mSum <= mlSum){
        return findMin(j, k, medium + 1, right, A, S);
    } else if (mlSum < mSum && mSum <= mrSum){
        return findMin(j, k, left, medium - 1, A, S);
    } else if (mlSum > mSum && mSum < mrSum){
        return mSum;
    } else {
        int lSum = findMin(j, k, left, medium - 1, A, S);
        int rSum = findMin(j, k, medium + 1, right, A, S);

        return std::min(std::min(lSum, rSum), mSum);
    }
}

int* alg2(int* input, int n, int k){
    Pair **arr = new Pair *[n];

    for (int i = 0; i < n; i++) {
        arr[i] = createPair(input[i], i);
    }

    mergeSort(arr, n);
    int *results = new int[n];
    int *A = new int[n];
    int *S = new int[n];

    for (int i = 0; i < n; i++) {
        A[i] = arr[i]->value - arr[0]->value;
    }

    S[0] = 0;
    for (int i = 1; i < n; i++) {
        S[i] = S[i - 1] + A[i];
    }

    for (int i = 0; i < n; i++) {
        int left = i - k >= 0 ? i - k : 0;
        int right = i + k < n ? i : n - 1 - k;
        results[arr[i]->index] = findMin(i, k, left, right, A, S);
    }

    free(A);
    free(S);
    for (int i = 0; i < n; i++) {
        free(arr[i]);
    }

    return results;
}

#if TEST == 0
int main() {
    int n, k;
    std::cin >> n;
    std::cin >> k;
    int* arr = new int[n];

    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    int* answer = alg2(arr, n, k);

    for (int i = 0; i < n; i++) {
        printf("%d ", answer[i]);
    }
}
#else
int* alg1(int* input, int n, int k){
    Pair** arr = new Pair*[n];

    for (int i = 0; i < n; i++) {
        arr[i] = createPair(input[i], i);
    }

    mergeSort(arr, n);
    int* results = new int[n];

    for (int i = 0; i < n; i++) {
        int l = i - 1, r = i + 1;
        int res = 0;

        for (int j = 0; j < k; j++) {
            int new_l_res = 0;
            int new_r_res = 0;

            if (l > -1){
                new_l_res += std::abs(arr[l]->value - arr[i]->value);
            } else{
                new_l_res = INT32_MAX;
            }

            if (r < n){
                new_r_res += std::abs(arr[r]->value - arr[i]->value);
            } else{
                new_r_res = INT32_MAX;
            }

            if (new_l_res < new_r_res){
                l--;
                res += new_l_res;
            } else{
                r++;
                res += new_r_res;
            }
        }

        results[arr[i]->index] = res;
    }

    for (int i = 0; i < n; i++) {
        free(arr[i]);
    }

    return results;
}

int* randArray(int n, int min_, int max_){
    int* out = new int[n];
    for (int i = 0; i < n; i++) {
        out[i] = rand() % (max_ - min_ + 1) + min_;
    }
    return out;
}

int main() {
    srand(time(nullptr));
    const int NUMBER_OF_TEST = 100;
    const int MAX_SIZE = 5000;
    const int MIN_SIZE = 5;
    const int MAX_VALUE = 1000000;
    const int MIN_VALUE = 1;

    for (int i = 0; i < NUMBER_OF_TEST; i++) {
        int size = rand() % (MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE;
        int k = rand() % (size - 1) + 1;
        int* input = randArray(size, MIN_VALUE, MAX_VALUE);

        int* ans1 = alg1(input, size, k);
        int* ans2 = alg2(input, size, k);

        bool success = true;

        for (int j = 0; j < size; j++) {
            if (ans1[j] != ans2[j]){
                printf("Error! Iteration = %d, Size = %d, K = %d\n", i, size, k);

                for (int l = 0; l < size; l++) {
                    printf("%d ", input[l]);
                }
                printf("\n");

                for (int l = 0; l < size; l++) {
                    printf("%d ", ans1[l]);
                }
                printf("\n");

                for (int l = 0; l < size; l++) {
                    printf("%d ", ans2[l]);
                }
                printf("\n");

                success = false;
                break;
            }
        }

        if (success){
            printf("Success! Iteration = %d\n", i);
        }

        free(input);
        free(ans1);
        free(ans2);
    }

    const int ABSOLUTE_MAX_SIZE = 300000;
    const int ABSOLUTE_MAX_K = 299999;
    const int ABSOLUTE_MAX_VALUE = 100000000;

    int* input = randArray(ABSOLUTE_MAX_SIZE, 0, ABSOLUTE_MAX_VALUE);

    clock_t tStart = clock();
    alg2(input, ABSOLUTE_MAX_SIZE, ABSOLUTE_MAX_K);
    printf("Algorithm 2 taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    tStart = clock();
    alg1(input, ABSOLUTE_MAX_SIZE, ABSOLUTE_MAX_K);
    printf("Algorithm 1 taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    return 0;
}
#endif