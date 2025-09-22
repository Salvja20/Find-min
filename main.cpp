
#include <iostream>
#include <unistd.h>     
#include <sys/wait.h>   
#include <cstdlib>      
#include <ctime>        

using namespace std;

const int SIZE = 20;

int findMin(int arr[], int start, int end) {
    int min = arr[start];
    for (int i = start + 1; i <= end; ++i) {
        if (arr[i] < min)
            min = arr[i];
    }
    return min;
}

int main() {
    int arr[SIZE];
    int pipefd[2];  

    
    srand(time(0));

    
    cout << "Array elements: ";
    for (int i = 0; i < SIZE; ++i) {
        arr[i] = rand() % 100;
        cout << arr[i] << " ";
    }
    cout << endl;

    
    if (pipe(pipefd) == -1) {
        cerr << "Pipe creation failed." << endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed." << endl;
        return 1;
    }

    else if (pid == 0) {
       
        close(pipefd[0]); 

        int childMin = findMin(arr, SIZE/2, SIZE - 1);
        cout << "[Child] PID: " << getpid() << ", Min in second half: " << childMin << endl;

       
        write(pipefd[1], &childMin, sizeof(childMin));
        close(pipefd[1]);
    }

    else {
        
        close(pipefd[1]);  

        int parentMin = findMin(arr, 0, SIZE/2 - 1);
        int childMin;

       
        read(pipefd[0], &childMin, sizeof(childMin));
        close(pipefd[0]); 

        cout << "[Parent] PID: " << getpid() << ", Min in first half: " << parentMin << endl;

        
        wait(NULL);

        int finalMin = (parentMin < childMin) ? parentMin : childMin;
        cout << "[Parent] Final minimum in array: " << finalMin << endl;
    }

    return 0;
}