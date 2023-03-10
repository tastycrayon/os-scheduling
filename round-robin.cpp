#include <algorithm>
#include <iostream>
#include <queue>

using namespace std;

struct Process {
  int index, arrivalTime, burstTime, startTime = -1, completionTime;
  int remainingBurstTime = this->burstTime;
  int turnArroundTime() { return this->completionTime - this->arrivalTime; };
  int waitingTime() { return this->turnArroundTime() - this->burstTime; };
};

class Scheduler {
 private:
  queue<Process> processQueue;
  queue<Process> readyQueue;
  queue<Process> terminatedQueue;
  vector<int> timestamps;  // extra
  vector<int> history;     // extra
  unsigned int timeQuantum;
  unsigned int totalCT, totalTAT, totalWT, totalIdle, counter, duration;

 public:
  Scheduler *enqueue(Process prc) {
    this->processQueue.push(prc);
    return this;
  }
  Scheduler *init() {
    int prevCompletionTime = 0;
    this->duration = 0;
    while (!processQueue.empty() || !readyQueue.empty()) {
      this->checkProcessQueue();
      if (!readyQueue.empty()) {
        Process *prc = &readyQueue.front();
        /*  calc start */
        if (prc->startTime == -1)
          prc->startTime = this->duration;  // set start time
        /* calc end */

        this->duration += this->min(this->timeQuantum, prc->remainingBurstTime);
        readyQueue.pop();
        prc->remainingBurstTime = prc->remainingBurstTime - this->timeQuantum;

        if (prc->remainingBurstTime <= 0) {
          prc->completionTime = this->duration;
          this->calculateAverage(prc);
          this->terminatedQueue.push(*prc);
        } else {
          this->checkProcessQueue();
          this->readyQueue.push(*prc);
        }

        // extra
        this->setTimestamp(prc);  // gantt chart
      }
    }

    return this;
  }

  Scheduler *checkProcessQueue() {
    while (!processQueue.empty() &&
           processQueue.front().arrivalTime <= this->duration) {
      Process *prc = &processQueue.front();
      processQueue.pop();
      this->readyQueue.push(*prc);
    }
    return this;
  }
  // extra
  void calculateAverage(Process *prc) {
    this->totalCT += prc->completionTime;
    this->totalTAT += prc->turnArroundTime();
    this->totalWT += prc->waitingTime();
    this->counter++;
  }

  void setTimestamp(Process *prc) {
    this->history.push_back(prc->index);
    this->timestamps.push_back(this->duration);
  }

  int min(int a, int b) { return a > b ? b : a; }
  void print() {
    cout << "Gantt Chart: " << endl;
    for (int index : this->history) cout << "|  " << 'P' << index << "   ";
    cout << '|' << endl;
    for (int timestamp : this->timestamps) cout << timestamp << '\t';
    cout << endl << endl;
    cout << "ID\tArrival\tBurst\tStart\tCompletion\tTAT\tWaiting" << endl;
    while (!this->terminatedQueue.empty()) {
      Process *prc = &terminatedQueue.front();

      cout << "P" << prc->index << '\t' << prc->arrivalTime << '\t'
           << prc->burstTime << '\t' << prc->startTime << '\t'
           << prc->completionTime << "\t\t" << prc->turnArroundTime() << '\t'
           << prc->waitingTime();
      terminatedQueue.pop();
      cout << endl;
    }
    cout << "Total Completion Time: " << this->totalCT << endl;
    cout << "Total Turn Arround Time: " << this->totalTAT << endl;
    cout << "Total Waiting Time: " << this->totalWT << endl;
    cout << "Average Completion Time: " << (float)this->totalCT / this->counter
         << endl;
    cout << "Average Turn Arround Time: "
         << (float)this->totalTAT / this->counter << endl;
    cout << "Average Waiting Time: " << (float)this->totalWT / this->counter
         << endl;
  }
  Scheduler(int tq = 2) {
    this->timeQuantum = tq;
    this->totalCT = 0, this->totalTAT = 0, this->totalWT = 0, this->counter = 0;
    timestamps.push_back(0);  // intial 0
  }
  ~Scheduler() {}
};

int main(int argc, char const *argv[]) {
  Scheduler *ps = new Scheduler(2);
  ps->enqueue({1, 0, 5})->enqueue({2, 2, 4});
  ps->enqueue({3, 4, 1})->enqueue({4, 5, 6});

  // ps->enqueue({1, 0, 8})->enqueue({2, 1, 6});
  // ps->enqueue({3, 3, 3})->enqueue({4, 5, 2});
  // ps->enqueue({5, 6, 4});
  ps->init();
  ps->print();
  return 0;
}
