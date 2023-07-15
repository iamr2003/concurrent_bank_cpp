#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>
#include <vector>

struct DummyType {
  int a;
  double b;
  double c;
};

// let's remake a data structure that would be heap allocated using unique
// note this is not something you should actually write
// smart_ptrs Linked List
template <class T> class linked_list {
  // singly linked for now
  struct Node {
    T val;
    std::unique_ptr<Node> next;
    Node(T _val) : val(_val), next(nullptr){};
  };

private:
  // No explicit destructor needed!
  std::unique_ptr<Node> head;

public:
  linked_list() : head(nullptr){};

  linked_list(linked_list<T> &other) {
    auto theirs = other.head.get();
    head = std::make_unique<Node>(other.head->val);
    auto ours = head.get();
    if (!theirs) {
      return;
    }
    while (theirs->next) {
      ours->next = std::make_unique<Node>(theirs->next->val);
      ours = ours->next.get();
      theirs = theirs->next.get();
    }
  }

  void push_back(T val) {
    // bc we will be breaking the managed ptr behavior, have to grab the regular
    // ptr for iterating
    auto curr = head.get();
    if (!curr) {
      head = std::make_unique<Node>(val);
      return;
    }
    while (curr->next) {
      curr = curr->next.get();
    }
    curr->next = std::make_unique<Node>(val);
  }

  T pop_back() {
    auto curr = head.get();
    // setting a unique ptr to nullptr auto deletes content
    if (!curr) {
      return T();
    }
    if (!curr->next) {
      T temp = head->val;
      head = nullptr;
      return temp;
    }
    while (curr->next->next) {
      curr = curr->next.get();
    }

    T temp = curr->next->val;
    curr->next = nullptr;
    return temp;
  }

  void print() {
    if(!head){
      std::cout<<std::endl;
      return;
    }
    auto curr = head.get();
    while (curr->next) {
      std::cout << curr->val << " ";
      curr = curr->next.get();
    }
    std::cout << curr->val << " ";
    std::cout << std::endl;
  }
};

std::unique_ptr<DummyType> mover(std::unique_ptr<DummyType> ptr_in) {
  auto here = std::make_unique<DummyType>(*ptr_in);
  // implicit move on return, no copies occur
  return here;
}

int main(int argc, char *argv[]) {
  // basic testing
  std::unique_ptr<DummyType> ptr = std::make_unique<DummyType>();
  ptr->a = -1;
  ptr->b = 3.14;
  ptr->c = 6.28;

  std::unique_ptr<DummyType> passed_around = mover(std::move(ptr));

  std::cout << passed_around->a << std::endl;
  // invalidated after move
  std::cout << (ptr == nullptr) << std::endl;

  // linked list testing
  std::vector<int> truth = {1, 2, 3, 4, 5, 6};
  linked_list<int> lst;

  for (auto elem : truth) {
    lst.push_back(elem);
  }
  lst.print();
  linked_list<int> cpy(lst);
  cpy.print();
  for (auto elem : truth) {
    std::cout << lst.pop_back() << " ";
  }
  std::cout << std::endl;
  lst.print();
  cpy.print();
  return 0;
}
