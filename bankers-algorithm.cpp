#include <iostream>
#include <queue>
using namespace std;

struct Process {
  int index, maxNeed[3] = {0}, allocated[3] = {0}, currentNeed[3] = {0};
};

class Compare {
public:
  int max(const int &a, const int &b, const int &c) {
    const int &d = (a > b) ? a : b;
    return d > c ? d : c;
  }
  bool operator()(const Process &p1, const Process &p2) {
    return this->max(p1.currentNeed[0], p1.currentNeed[1], p1.currentNeed[2]) >
           this->max(p2.currentNeed[0], p2.currentNeed[1], p2.currentNeed[2]);
  }
};

class Scheduler {
private:
  queue<Process> processQueue;
  priority_queue<Process, vector<Process>, Compare> readyQueue;
  queue<Process> terminatedQueue;
  queue<int> history;
  queue<int> availableResources;
  int available[3] = {};
  int r1, r2, r3;

public:
  Scheduler *enqueue(Process prc) {
    this->processQueue.push(prc);
    return this;
  }
  Scheduler *init() {
    int totalAllocated[3] = {0};
    while (!processQueue.empty()) {
      Process *prc = &processQueue.front();
      prc->currentNeed[0] = prc->maxNeed[0] - prc->allocated[0];
      prc->currentNeed[1] = prc->maxNeed[1] - prc->allocated[1];
      prc->currentNeed[2] = prc->maxNeed[2] - prc->allocated[2];
      totalAllocated[0] += prc->allocated[0];
      totalAllocated[1] += prc->allocated[1];
      totalAllocated[2] += prc->allocated[2];
      processQueue.pop();
      readyQueue.push(*prc);
    }

    this->available[0] = this->r1 - totalAllocated[0];
    this->available[1] = this->r2 - totalAllocated[1];
    this->available[2] = this->r3 - totalAllocated[2];

    while (!readyQueue.empty()) {
      Process prc = readyQueue.top();
      this->history.push(prc.index);
      readyQueue.pop();
      availableResources.push(this->available[0]);
      availableResources.push(this->available[1]);
      availableResources.push(this->available[2]);
      this->available[0] += prc.allocated[0];
      this->available[1] += prc.allocated[1];
      this->available[2] += prc.allocated[2];
      terminatedQueue.push(prc);
    }
    return this;
  }
  // extra
  void print() {
    cout << "Gantt Chart: " << endl;
    while (!this->history.empty()) {
      cout << 'P' << (this->history.front()) << " | ";
      this->history.pop();
    }
    cout << endl << endl;
    cout
        << "ID\tMax Level\tAllocated Resource\tCurrent Need\tAvailable Resource"
        << endl;
    cout << "\tA  B  C\t\tA  B  C\t\t\tA  B  C\t\tA  B  C" << endl;
    while (!this->terminatedQueue.empty()) {
      Process *prc = &terminatedQueue.front();
      int a1 = availableResources.front();
      availableResources.pop();
      int a2 = availableResources.front();
      availableResources.pop();
      int a3 = availableResources.front();
      availableResources.pop();
      cout << "P" << prc->index << '\t';
      cout << prc->maxNeed[0] << ", " << prc->maxNeed[1] << ", "
           << prc->maxNeed[2] << '\t' << '\t';
      cout << prc->allocated[0] << ", " << prc->allocated[1] << ", "
           << prc->allocated[2] << '\t' << '\t' << '\t';
      cout << prc->currentNeed[0] << ", " << prc->currentNeed[1] << ", "
           << prc->currentNeed[2] << '\t' << '\t';
      cout << a1 << ", " << a2 << ", " << a3 << '\t' << endl;
      terminatedQueue.pop();
    }
    cout << "\t\t\t\t\t\t\t\t" << available[0] << ", " << available[1] << ", "
         << available[2];
    cout << endl;
  }
  Scheduler(int r1, int r2, int r3) {
    this->r1 = r1;
    this->r2 = r2;
    this->r3 = r3;
  }
  ~Scheduler() {}
};

int main(int argc, char const *argv[]) {
  Scheduler *bankers = new Scheduler(10, 5, 7);
  bankers->enqueue({0, {7, 5, 3}, {0, 1, 0}})
      ->enqueue({1, {3, 2, 2}, {2, 0, 0}})
      ->enqueue({2, {9, 0, 2}, {3, 0, 2}})
      ->enqueue({3, {2, 2, 2}, {2, 1, 1}})
      ->enqueue({4, {4, 3, 3}, {0, 0, 2}});

  bankers->init()->print();
  return 0;
}