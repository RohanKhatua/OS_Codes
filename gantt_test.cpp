#include <iostream>
#include <map>
using namespace std;

int main(int argc, char const *argv[])
{
    map<int, int> timeJobMap;
    timeJobMap.insert(make_pair(0,1));
    timeJobMap.insert(make_pair(1,2));
    timeJobMap.insert(make_pair(2,2));
    timeJobMap.insert(make_pair(3,2));
    timeJobMap.insert(make_pair(4,3));
    timeJobMap.insert(make_pair(5,2));
    timeJobMap.insert(make_pair(6,1));
    timeJobMap.insert(make_pair(7,1));
    timeJobMap.insert(make_pair(8,2));
    cout<<"Time\tJobID"<<endl;
    for (int i=0;i<timeJobMap.size();i++) {
        int start_time = i;
        int end_time = i;
        int current_job = timeJobMap[i];
        while (i+1<timeJobMap.size() && timeJobMap[i+1] == current_job) {
            i++;
            end_time++;
        }
        cout<<start_time<<"-"<<end_time<<"\t"<<current_job<<endl;
    }
    return 0;
}
