#include <bits/stdc++.h>
using namespace std;

int num_resource_type;

class Process {
    public:
    int id;
    bool done;
    vector<int> allocation;
    vector<int> need;

    Process (int id, vector<int> allocation, vector<int> need) {
        this->id = id;
        this->allocation = allocation;
        this->need = need;
        done = false;
    }

    bool canExecute (vector<int> available) {
        for (int i=0;i<num_resource_type;i++) {
            if (need[i]>available[i]) return false;
        }
        return true;
    }
};

bool processesLeft(vector<Process>* processes) {
    for (int i=0;i<num_resource_type;i++) {
        if (!processes->at(i).done) {
            return true;
        }
    }
    return false;
}

int main() {
    int num_process;
    cout<<"Enter the number of processes : ";
    cin>>num_process;

    vector<Process> processes;
    processes.reserve(num_process);

    cout<<"Enter the number of types of Resources : ";
    cin>>num_resource_type;

    vector<int> resources_available;
    resources_available.reserve(num_resource_type);

    cout<<"Enter the number of resources of each type : ";
    for (int i=0;i<num_resource_type;i++) {
        int x;
        cin>>x;
        resources_available.push_back(x);
    }

    vector<int> initial_usage(num_resource_type, 0);

    for (int i=0;i<num_process;i++) {
        cout<<"Process "<<i+1<<endl;
        cout<<"Enter allocation : "<<endl;
        vector<int> allocation;
        for (int j=0;j<num_resource_type;j++) {
            int x;
            cin>>x;
            allocation.push_back(x);
            initial_usage[j] += x;
        }

        cout<<"Enter Need : ";
        vector<int> need;
        for (int j=0;j<num_resource_type;j++) {
            int x;
            cin>>x;
            need.push_back(x);
        }

        processes.emplace_back(i+1, allocation, need);
    }

    vector<int> current_available_resources(num_resource_type, 0);

    for (int i=0;i<num_resource_type;i++) {
        current_available_resources[i] = resources_available[i] - initial_usage[i];
    }

    string exec_order;
    int passes = 0;
    while (passes<num_process) {
        for (Process& process:processes) {
            if (process.canExecute(current_available_resources) && !process.done) {
                process.done = true;
                for (int i=0;i<num_resource_type;i++){
                    current_available_resources[i] += process.need[i];
                }
                exec_order+=(to_string(process.id)+"\t");
            }
        }
        if (processesLeft(&processes)) {
            passes++;
            continue;
        }
        break;
    }
    if (processesLeft(&processes)) {
        cout<<"Deadlock occurred"<<endl;
    }
    cout<<"Execution Order : "<<exec_order;
}