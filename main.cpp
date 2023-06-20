#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

struct Bank {
  std::unordered_map<std::string, std::pair<int, std::mutex>> accts;
  Bank(std::vector<std::string> names) {
    for (std::string name : names) {
      // implicitly calls default constructor
      accts[name].first = 0;
    }
  }
};

// preallocating names rn, next step is to allow new accts to open by operations
const std::vector<std::string> names = {"Michael", "Ryan", "Ibrahim"};
Bank b(names);

void withdraw(int val, std::string name) {
  if (b.accts.find(name) != b.accts.end()) {
    const std::lock_guard<std::mutex> lock(b.accts[name].second);
    b.accts[name].first -= val;
  }
}

void deposit(int val, std::string name) {
  if (b.accts.find(name) != b.accts.end()) {
    const std::lock_guard<std::mutex> lock(b.accts[name].second);
    b.accts[name].first += val;
  }
}

// Start with one account and value, and multiple accessors
int main() {
  int num_transactions = 1000;
  // random number generator for shuffling(note, not security grade)
  std::random_device rd;
  std::mt19937 g(rd());

  struct ops {
    int val;
    std::string name;
    int op; // -1 or 1 for dep or withdraw
  };

  // a light scheduler to randomize operations more for better tests
  std::vector<ops> tasks;
  for (std::string name : names) {
    for(int i=0;i<num_transactions;++i){
    ops task;
    task.val=10;
    task.name = name;
    task.op = 1;
    tasks.push_back(task);
    task.val=9;
    task.op = -1;
    tasks.push_back(task);
    }
  }

  std::shuffle(tasks.begin(), tasks.end(), g);

  // allocate threads by defined order
  std::vector<std::thread> threads;
  for (ops &task : tasks) {
    if (task.op == -1) {
      std::thread t(withdraw, task.val, task.name);
      threads.push_back(std::move(t));
    } else {
      std::thread t(deposit, task.val, task.name);
      threads.push_back(std::move(t));
    }
  }

  // for (int i = 0; i < num_transactions; ++i) {
  //   std::thread t(withdraw, 10);
  //   threads.push_back(std::move(t));
  // }
  //
  // for (int i = 0; i < num_transactions; ++i) {
  //   std::thread t(deposit, 11);
  //   threads.push_back(std::move(t));
  // }
  //
  for (auto &t : threads) {
    t.join();
  }

  // pairs of string name , {int val, mutex}
  for (auto &acct : b.accts) {
    if (acct.second.first != 0) {
      std::cout << "NOT ZEROED acct: " << acct.first
                << " val: " << acct.second.first<<"\n";
    }
  }

  std::cout<<"No more zeroed accts!";
}
