#include <memory>

struct DummyType{
  int a;
  double b;
  double c;
};

std::unique_ptr<DummyType> mover(std::unique_ptr<DummyType> ptr_in){ 
  return std::make_unique<DummyType>(*ptr_in);
}


//small project getting familiar with smart_ptrs
int main (int argc, char *argv[])
{
  std::unique_ptr<DummyType> ptr = std::make_unique<DummyType>();
  ptr->a = -1;
  ptr->b = 3.14;
  ptr->c = 6.28;

  return 0;
}
