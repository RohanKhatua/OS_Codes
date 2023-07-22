#include <bits/stdc++.h>
using namespace std;

class Segment
{
public:
    int id;
    int size;
    bool allocated;
    int process_id;

    Segment(int id, int size)
    {
        this->id = id;
        this->size = size;
        allocated = false;
        process_id = -1 * INT_MAX;
    }
};

class Process
{
public:
    int id;
    int size;

    Process(int id, int size)
    {
        this->id = id;
        this->size = size;
    }
};

void displaySegments(vector<Segment> *segments)
{

    for (int i = 0; i < segments->size(); i++)
    {
        cout << "Segment " << segments->at(i).id << " : ";
        if (segments->at(i).allocated)
        {
            cout << "P" << segments->at(i).process_id;
        }
        else
        {
            cout << "Unallocated";
        }
        cout << endl;
    }
    cout << "------------------------------------" << endl;
}

int main()
{
    // Input the segments
    cout << "Enter the number of segments : ";
    int num_segments;
    cin >> num_segments;

    vector<Segment> segments;
    segments.reserve(num_segments);
    for (int i = 0; i < num_segments; i++)
    {
        int size;
        cout << "Enter Size of Segment " << i + 1 << " : ";
        cin >> size;
        segments.push_back(Segment(i + 1, size));
    }

    // Input the processes
    cout << "Enter the number of processes : ";
    int num_processes;
    cin >> num_processes;

    vector<Process> processes;
    processes.reserve(num_processes);
    for (int i = 0; i < num_processes; i++)
    {
        int size;
        cout << "Enter Size of Process " << i + 1 << " : ";
        cin >> size;
        processes.push_back(Process(i + 1, size));
    }

    int total_memory_available = 0;
    for (auto segment : segments)
    {
        total_memory_available += segment.size;
    }

    // For each process we try to accomodate it in the block which maximizes internal fragmentation
    vector<int> wasted_spaces;
    wasted_spaces.reserve(num_segments);
    for (int i = 0; i < num_processes; i++)
    {
        wasted_spaces.clear();
        for (int j = 0; j < num_segments; j++)
        {

            int diff = segments[j].size - processes[i].size;
            if (diff < 0 || segments[j].allocated)
            {
                // If the segment is not large enough, we do not want it to be considered.
                // Same if the segment is already allocated
                wasted_spaces.push_back(INT_MIN);
            }
            else
            {
                wasted_spaces.push_back(diff);
            }
        }
        int maximum_wasted_space = *max_element(wasted_spaces.begin(), wasted_spaces.end());
        if (maximum_wasted_space != INT_MIN)
        { // Check if a valid segment was found
            // Find the segment index at which there is least wasted space
            int segment_index = find(wasted_spaces.begin(), wasted_spaces.end(), maximum_wasted_space) - wasted_spaces.begin();

            // Allocate this segment to the process
            segments[segment_index].allocated = true;
            segments[segment_index].process_id = processes[i].id;
            total_memory_available -= processes[i].size;
            if (maximum_wasted_space != 0)
            {
                cout << "Internal Fragmentation : Segment " << segment_index + 1 << endl;
            }
        }

        else
        { // The process cannot be allocated memory
            if (processes[i].size <= total_memory_available)
            {
                cout << "External Fragementation : Process " << processes[i].id << " could not be allocated memory"<<endl;
            }
            else
            {
                cout << "Process " << processes[i].id << " was too large" << endl;
            }
            cout << "Process Size = " << processes[i].size << endl;
            cout << "Available Memory = " << total_memory_available << endl;
        }

        displaySegments(&segments);
    }
}