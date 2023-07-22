#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

int main() {
    int numRequests, headPosition;

    cout << "Enter the number of disk requests: ";
    cin >> numRequests;

    vector<int> requests(numRequests);

    cout << "Enter the disk requests in order (separated by spaces):\n";
    for (int i = 0; i < numRequests; i++) {
        cin >> requests[i];
    }

    cout << "Enter the initial head position: ";
    cin >> headPosition;

    // FCFS algorithm
    int totalSeekTime = 0;
    int currentHeadPosition = headPosition;
    for (int i = 0; i < numRequests; i++) {
        int seekDistance = abs(currentHeadPosition - requests[i]);
        totalSeekTime += seekDistance;
        currentHeadPosition = requests[i];
    }

    cout << "Total seek time using FCFS: " << totalSeekTime << endl;

    return 0;
}
