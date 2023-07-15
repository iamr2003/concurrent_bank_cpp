#include <iostream>
#include <memory>
#include <ostream>
#include <random>
#include <thread>
#include <vector>

//for number of entries, and number of threads
const int n = 10000000;
const int n_threads = 8;
constexpr int cutoff = n/n_threads;

void print_vec(const std::vector<int> &vec) {
  for (auto n : vec) {
    std::cout << n << " ";
  }
  std::cout << std::endl;
}

// each side is sorted
void merge(std::vector<int> &vec, int s1, int e1, int e2) {
  // not preallocating for rn, using slow insertions
  std::vector<int> out;
  int a1 = s1;
  int a2 = e1;

  while (a1 < e1 and a2 < e2) {
    if (vec[a1] < vec[a2]) {
      out.push_back(vec[a1]);
      a1++;
    } else {
      out.push_back(vec[a2]);
      a2++;
    }
  }

  while (a1 < e1) {
    out.push_back(vec[a1]);
    a1++;
  }
  while (a2 < e2) {
    out.push_back(vec[a2]);
    a2++;
  }

  // copy into original
  for (int i = 0; i < out.size(); ++i) {
    vec[s1 + i] = out[i];
  }
}

// as is convention, end is not included
void merge_helper(std::vector<int> &vec, int s, int e) {
  if (s == e or e == s + 1) {
    return;
  }
  int mid = (s + e) / 2;
  merge_helper(vec, s, mid);
  merge_helper(vec, mid, e);
  merge(vec, s, mid, e);
}

void merge_helper_multi(std::shared_ptr<std::vector<int>> vec_ptr, int s,
                        int e) {
  auto vec = *vec_ptr;
  if (e - s <= cutoff) {
    merge_helper(vec, s, e);
    return;
  }
  int mid = (s + e) / 2;
  // shared_ptr not needed?
  std::thread t1(merge_helper_multi, vec_ptr, s, mid);
  std::thread t2(merge_helper_multi, vec_ptr, mid, e);

  t1.join();
  t2.join();
  merge(vec, s, mid, e);
}
// will be inplace sorting
void merge_sort_single(std::vector<int> &vec) {
  merge_helper(vec, 0, vec.size());
}

void merge_sort_multi(std::shared_ptr<std::vector<int>> vec_ptr) {
  merge_helper_multi(vec_ptr, 0, vec_ptr->size());
}

// shared ptr is used as a method to manage things where multiple people need to
// hold it
int main(int argc, char *argv[]) {
  // let's try to write a multithreaded merge sort
  std::shared_ptr<std::vector<int>> array_for_multi =
      std::make_shared<std::vector<int>>();
  std::vector<int> array_for_single;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 2 * n);

  for (int i = 0; i < n; ++i) {
    array_for_multi->push_back(distrib(gen));
    array_for_single.push_back(distrib(gen));
  }
  // print_vec(*array);
  //
  std::cout << "started sorting single" << std::endl;
  merge_sort_single(array_for_single);
  std::cout << "finished sorting single" << std::endl;

  std::cout << "started sorting multi" << std::endl;
  merge_sort_multi(array_for_multi);
  std::cout << "finished sorting multi" << std::endl;
  // print_vec(*array);

  return 0;
}
