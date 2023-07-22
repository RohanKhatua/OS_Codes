#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

int main() {
    int numRequests, headPosition, direction;

    cout << "Enter the number of disk requests: ";
    cin >> numRequests;

    vector<int> requests(numRequests);

    cout << "Enter the disk requests in order (separated by spaces):\n";
    for (int i = 0; i < numRequests; i++) {
        cin >> requests[i];
    }

    cout << "Enter the initial head position: ";
    cin >> headPosition;

    cout << "Enter the direction (0 for left, 1 for right): ";
    cin >> direction;

    // SCAN algorithm
    sort(requests.begin(), requests.end());

    int totalSeekTime = 0;
    int currentHeadPosition = headPosition;
    int leftBound = 0, rightBound = numRequests - 1;

    if (direction == 1) {
        // Move towards right
        for (int i = 0; i < numRequests; i++) {
            if (requests[i] >= headPosition) {
                rightBound = i;
                break;
            }
        }

        for (int i = rightBound; i < numRequests; i++) {
            totalSeekTime += abs(currentHeadPosition - requests[i]);
            currentHeadPosition = requests[i];
        }

        totalSeekTime += abs(currentHeadPosition - requests[numRequests - 1]);
        currentHeadPosition = requests[numRequests - 1];

        for (int i = rightBound - 1; i >= 0; i--) {
            totalSeekTime += abs(currentHeadPosition - requests[i]);
            currentHeadPosition = requests[i];
        }
    } else {
        // Move towards left
        for (int i = numRequests - 1; i >= 0; i--) {
            if (requests[i] <= headPosition) {
                leftBound = i;
                break;
            }
        }

        for (int i = leftBound; i >= 0; i--) {
            totalSeekTime += abs(currentHeadPosition - requests[i]);
            currentHeadPosition = requests[i];
        }

        totalSeekTime += abs(currentHeadPosition - requests[0]);
        currentHeadPosition = requests[0];

        for (int i = leftBound + 1; i < numRequests; i++) {
            totalSeekTime += abs(currentHeadPosition - requests[i]);
            currentHeadPosition = requests[i];
        }
    }

    cout << "Total seek time using SCAN: " << totalSeekTime << endl;

    return 0;
}
