#include <time.h>

#include <iostream>
using namespace std;

enum NewsdayType { Good = 'G', Fair = 'F', Poor = 'P' };

struct NewsDay {
  unsigned int date, randomDigitForNewsday, randomDigitForDemand, demand;
  NewsdayType typesOfNewsday;
  float revenue, lostProfit, salvage, profit;
};

class Solution {
private:
  int numeberOfDays, bulkSize;
  float buyingPrice, sellingPrice, profitPerPaper, salvageSellingPrice;
  NewsDay *days;

public:
  void startSimulating() {
    unsigned int seed = time(0);
    srand(seed);
    for (int i = 0; i < this->numeberOfDays; i++) {
      NewsDay *current = &this->days[i];
      current->date = i + 1;
      current->randomDigitForNewsday = randBetween(1, 100, seed);
      assignNewsdayType(this->days[i]);
      current->randomDigitForDemand = randBetween(1, 100, seed);
      assignDemand(this->days[i]);

      current->revenue = (float)current->demand * this->sellingPrice;

      current->lostProfit =
          current->demand > this->bulkSize
              ? (float)(current->demand - this->bulkSize) * this->profitPerPaper
              : 0;

      current->salvage =
          current->demand < this->bulkSize
              ? (this->bulkSize - current->demand) * this->salvageSellingPrice
              : 0;
      current->profit = current->revenue -
                        (this->buyingPrice * this->bulkSize) -
                        current->lostProfit + current->salvage;
    }
  };

  int max(int a, int b) { return a > b ? a : b; }
  void init() {
    this->startSimulating();
    this->print();
  }
  void assignNewsdayType(NewsDay &n) {
    if (n.randomDigitForNewsday >= 0 && n.randomDigitForNewsday <= 35)
      n.typesOfNewsday = Good;
    else if (n.randomDigitForNewsday >= 36 && n.randomDigitForNewsday <= 80)
      n.typesOfNewsday = Fair;
    else if (n.randomDigitForNewsday >= 81 && n.randomDigitForNewsday <= 100)
      n.typesOfNewsday = Poor;
  }

  void assignDemand(NewsDay &n) {
    switch (n.typesOfNewsday) {
    case Good:
      if (n.randomDigitForDemand >= 0 && n.randomDigitForDemand <= 3)
        n.demand = 40;
      else if (n.randomDigitForDemand >= 4 && n.randomDigitForDemand <= 8)
        n.demand = 50;
      else if (n.randomDigitForDemand >= 9 && n.randomDigitForDemand <= 23)
        n.demand = 60;
      else if (n.randomDigitForDemand >= 24 && n.randomDigitForDemand <= 43)
        n.demand = 70;
      else if (n.randomDigitForDemand >= 44 && n.randomDigitForDemand <= 78)
        n.demand = 80;
      else if (n.randomDigitForDemand >= 79 && n.randomDigitForDemand <= 93)
        n.demand = 90;
      else if (n.randomDigitForDemand >= 94 && n.randomDigitForDemand <= 100)
        n.demand = 100;
      break;
    case Fair:
      if (n.randomDigitForDemand >= 0 && n.randomDigitForDemand <= 10)
        n.demand = 40;
      else if (n.randomDigitForDemand >= 11 && n.randomDigitForDemand <= 28)
        n.demand = 50;
      else if (n.randomDigitForDemand >= 29 && n.randomDigitForDemand <= 68)
        n.demand = 60;
      else if (n.randomDigitForDemand >= 69 && n.randomDigitForDemand <= 88)
        n.demand = 70;
      else if (n.randomDigitForDemand >= 89 && n.randomDigitForDemand <= 96)
        n.demand = 80;
      else if (n.randomDigitForDemand >= 97 && n.randomDigitForDemand <= 100)
        n.demand = 90;
      break;
    case Poor:
      if (n.randomDigitForDemand >= 0 && n.randomDigitForDemand <= 44)
        n.demand = 40;
      else if (n.randomDigitForDemand >= 45 && n.randomDigitForDemand <= 66)
        n.demand = 50;
      else if (n.randomDigitForDemand >= 67 && n.randomDigitForDemand <= 82)
        n.demand = 60;
      else if (n.randomDigitForDemand >= 83 && n.randomDigitForDemand <= 94)
        n.demand = 70;
      else if (n.randomDigitForDemand >= 94 && n.randomDigitForDemand <= 100)
        n.demand = 80;
      break;
    };
  }
  // extra
  void print() {
    cout << "Day\tRD1\tNewsday\tRD2\tDemand\tRevenue\tLostProfit\tSalvage\t"
            "Profit\t"
         << endl;
    for (int i = 0; i < this->numeberOfDays; i++) {
      cout << days[i].date << '\t' << days[i].randomDigitForNewsday << '\t'
           << (char)days[i].typesOfNewsday << '\t'
           << days[i].randomDigitForDemand << '\t' << days[i].demand << '\t'
           << '$' << days[i].revenue << '\t' << '$' << days[i].lostProfit
           << '\t' << '\t' << '$' << days[i].salvage << '\t' << days[i].profit
           << '$' << endl;
    }
  }
  int randBetween(const int start, const int end, unsigned int &seed) {
    int a = 17, c = 43, m = 100;
    int r = (17 * seed + c) % end;
    seed = r = start + r;
    return r;
  }
  // extra
  Solution() {
    cout << "Enter NewsDay Size: ";
    cin >> this->numeberOfDays;
    cout << endl;
    // this->numeberOfDays = 3;
    this->buyingPrice = 0.33;
    this->sellingPrice = 0.50;
    this->bulkSize = 70;
    this->salvageSellingPrice = 0.05;
    this->profitPerPaper = this->sellingPrice - this->buyingPrice;
    this->days = new NewsDay[this->numeberOfDays];
  }

  ~Solution() { delete[] this->days; }
};

int main(int argc, char const *argv[]) {
  Solution *simulator = new Solution();

  simulator->init();

  return 0;
}
