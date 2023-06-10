#include <chrono>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
// have a lock then allow things to access it
//
struct Bank {
  int val;
  Bank() : val{0} {}
};

Bank b;
std::mutex b_mutex;

void withdraw(int val) {
  const std::lock_guard<std::mutex> lock(b_mutex);
  b.val -= val;
}

void deposit(int val) {
  const std::lock_guard<std::mutex> lock(b_mutex);
  b.val += val;
}
// Start with one account and value, and multiple accessors
int main() {
  int num_transactions = 10000;
  std::vector<std::thread> threads;
  // use constant transaction
  // I don't think this is a good test, needs to mix them more

  for (int i = 0; i < num_transactions; ++i) {
    std::thread t(withdraw,10);
    threads.push_back(std::move(t));
  }

  for (int i = 0; i < num_transactions; ++i) {
    std::thread t(deposit,11);
    threads.push_back(std::move(t));
  }

  for (auto &t : threads) {
    t.join();
  }
  std::cout << b.val; 
}
