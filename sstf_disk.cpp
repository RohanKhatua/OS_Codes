#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <climits>

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

    // SSTF algorithm
    vector<int> visited(numRequests, 0);
    int totalSeekTime = 0;
    int currentHeadPosition = headPosition;

    for (int i = 0; i < numRequests; i++) {
        int minSeek = INT_MAX;
        int minIndex = -1;

        for (int j = 0; j < numRequests; j++) {
            if (!visited[j]) {
                int seekDistance = abs(currentHeadPosition - requests[j]);
                if (seekDistance < minSeek) {
                    minSeek = seekDistance;
                    minIndex = j;
                }
            }
        }

        visited[minIndex] = 1;
        totalSeekTime += minSeek;
        currentHeadPosition = requests[minIndex];
    }

    cout << "Total seek time using SSTF: " << totalSeekTime << endl;

    return 0;
}
