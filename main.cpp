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

    int medium = (left + right) / 2;

    int lSum = findMin(j, k, left, medium, A, S);
    int rSum = findMin(j, k, medium + 1, right, A, S);

    return lSum < rSum ? lSum : rSum;
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
    const int NUMBER_OF_TEST = 1000;
    const int MAX_SIZE = 1000;
    const int MIN_SIZE = 5;
    const int MAX_VALUE = 100000;
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

    return 0;
}
#endif