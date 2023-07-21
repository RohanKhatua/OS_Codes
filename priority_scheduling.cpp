#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

class Job
{
public:
    int id;
    int burst_time;
    int arrival_time;
    int priority;

    Job(int id, int burst_time, int arrival_time, int priority)
    {
        this->id = id;
        this->burst_time = burst_time;
        this->arrival_time = arrival_time;
        this->priority = priority;
    }
};

// Sorts jobs first on priority, then on arrival time, then on id
// Lowest magnitude of priority refers to highest priority
bool compareJobs(Job a, Job b)
{
    if (a.priority < b.priority)
        return true;
    else if (a.priority == b.priority)
    {
        if (a.arrival_time < b.arrival_time)
            return true;
        else if (a.arrival_time == b.arrival_time)
            return a.id < b.id;
        else
            return false;
    }
    else
        return false;
}

// Removes the jobs that have already been completed from the jobs vector
vector<Job> removeCompletedJobs(vector<Job> jobs)
{
    for (int i = 0; i < jobs.size(); i++)
    {
        if (jobs[i].burst_time == 0)
        {
            jobs.erase(jobs.begin() + i);
            i--;
        }
    }

    return jobs;
}

// finds the best job to perform at any given time
// returns a job with id of -1 if none found
Job findJob(vector<Job> jobs, int current_time)
{
    sort(jobs.begin(), jobs.end(), compareJobs);

    for (int i = 0; i < jobs.size(); i++)
    {
        if (jobs[i].arrival_time <= current_time && jobs[i].burst_time != 0)
            return jobs[i];
    }
    return Job(-1, 0, 9999, 9999);
}

bool allComplete(vector<Job> jobs)
{
    for (Job job : jobs)
    {
        if (job.burst_time != 0)
            return false;
    }

    return true;
}

int firstOccurenceOfJob(map<int, int> timeJobMap, int jobID)
{
    int counter = 0;
    for (pair<int, int> timeJobPair : timeJobMap)
    {
        if (timeJobPair.second == jobID)
            return counter;
        counter++;
    }
    return -1;
}

int lastOccurenceOfJob(map<int, int> timeJobMap, int jobID)
{
    int mapsize = timeJobMap.size();
    for (int i = mapsize - 1; i >= 0; i--)
    {
        if (timeJobMap[i] == jobID)
            return i;
    }
    return -1;
}

int main(int argc, char const *argv[])
{
    // cout << "Enter the number of jobs : ";
    // int n;
    // cin >> n;
    vector<Job> jobs;
    // int burst_time, arrival_time, priority;
    // // Input jobs
    // for (int i = 0; i < n; i++)
    // {
    //     cout << "Job " << i << " :" << endl;
    //     cout << "Arrival Time : ";
    //     cin >> arrival_time;
    //     cout << "Burst Time : ";
    //     cin >> burst_time;
    //     cout << "Priority : ";
    //     cin >> priority;
    //     jobs.push_back(Job(i, burst_time, arrival_time, priority));
    // }

    jobs.push_back(Job(0, 10, 0, 3));
    jobs.push_back(Job(1, 1, 2, 1));
    jobs.push_back(Job(2, 2, 2, 4));
    jobs.push_back(Job(3, 1, 2, 5));
    jobs.push_back(Job(4, 5, 2, 2));
    int n = 5;

    // Create a copy of the jobs vector
    vector<Job> jobs_copy = jobs;

    int current_time = 0;
    /*
    timeJobMap maps between the time and the job being perfomed at that instant
    the first time a job appears is the starting time
    the last time it appears is the ending time
    the difference of these becomes the waiting time
    */
    map<int, int> timeJobMap;

    while (!allComplete(jobs))
    {
        Job current_job = findJob(jobs, current_time);
        
        if (current_job.id == -1) {
            current_time++;
            continue; // no valid job found
        }

        else
        {
            timeJobMap[current_time] = current_job.id;
            current_time++;
            for (int i = 0; i < jobs.size(); i++)
            {
                if (jobs[i].id == current_job.id)
                {
                    jobs[i].burst_time--;
                    break;
                }
            }
        }
    }

    int starting_times[n];
    int ending_times[n];
    int waiting_times[n];
    int turnaround_times[n];

    int sum_waiting_times = 0;
    int sum_turnaround_times = 0;

    for (int i = 0; i < n; i++)
    {
        starting_times[i] = firstOccurenceOfJob(timeJobMap, i);
        ending_times[i] = lastOccurenceOfJob(timeJobMap, i);
        waiting_times[i] = starting_times[i] - jobs[i].arrival_time + 1;
        sum_waiting_times += waiting_times[i];
        turnaround_times[i] = ending_times[i] + 1 - jobs[i].arrival_time;
        sum_turnaround_times += turnaround_times[i];
    }

    // Make Gantt Chart
    cout << "Gantt Chart : " << endl;
    cout << "Time\tJobID" << endl;
    for (int i = 0; i < timeJobMap.size(); i++)
    {
        int start_time = i;
        int end_time = i;
        int current_job = timeJobMap[i];
        while (i + 1 < timeJobMap.size() && timeJobMap[i + 1] == current_job)
        {
            i++;
            end_time++;
        }
        cout << start_time << "-" << end_time << "\t" << current_job << endl;
    }

    cout << "Job Schedule Table : " << endl;
    cout << "Job ID\tStarting Time\tEnding Time\tWaiting Time\tTurnaround Time" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << i << "\t\t" << starting_times[i] << "\t\t" << ending_times[i] << "\t\t" << waiting_times[i] << "\t\t" << turnaround_times[i] << endl;
    }

    cout << "Average Waiting Time : " << (float)(sum_waiting_times) / n << endl;
    cout << "Average Turnaround Time : " << (float)(sum_turnaround_times) / n << endl;

    return 0;
}
