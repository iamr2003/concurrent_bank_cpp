#include <thread>
#include <vector>

// have a lock then allow things to access it
//
struct Bank {
  int val;
  bool lock;
  Bank() : val{0} {}
};

void withdraw() {}

void deposit() {}
// Start with one account and value, and multiple accessors
int main() {
  int num_transactions = 10;
  std::vector<std::thread> threads;
  // use constant transaction

  for(int i=0;i<num_transactions;++i){
    std::thread t(withdraw);
    threads.push_back(std::move(t));
  }

  for(int i=0;i<num_transactions;++i){
    std::thread t(deposit);
    threads.push_back(std::move(t));
  }
  
  for(auto &t:threads){
    t.join();
  }
}
