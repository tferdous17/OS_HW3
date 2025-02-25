#include <iostream>
#include <unistd.h>

int findMin(int arr[], int start, int end) {
    int minVal = INT_MAX;
    for (int i = start; i < end; i++) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }
    return minVal;
}

int main() {
    int arr[20];
    int pipefd[2];

    // fill array with random nums
    for (int i = 0; i < 20; i++) {
        arr[i] = rand() % 100; // Random numbers between 0 and 99
        std::cout << arr[i] << " ";
    }

    if (pipe(pipefd) == -1) {
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cout << "fork failed\n";
    } else if (pid == 0) {
        close(pipefd[0]);
        // 10 == halfway thru the array, 20 is end
        int minChild = findMin(arr, 10, 20);
        write(pipefd[1], &minChild, sizeof(minChild)); // Send min to parent
        close(pipefd[1]);
    } else {
        close(pipefd[1]);
        int minParent = findMin(arr, 0, 10);

        int minChild;
        read(pipefd[0], &minChild, sizeof(minChild));
        close(pipefd[0]);
        wait(NULL);

        int result = std::min(minParent, minChild);
        std::cout << "OVERALL MIN: " << result;
    }

    return 0;
}

