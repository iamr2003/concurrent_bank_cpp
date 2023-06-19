// write a multithreaded version of this basic kinematic swarm sims
#include <algorithm>
#include <math.h>
#include <vector>

// graph of interacting agents

// get eigen at a certain point if I really want
// there's apparently some slightly more efficient patterns:
// https://stackoverflow.com/questions/52376716/c-overloading-of-the-plus-operator
//
// should templatize at some point
struct Vec {
  std::vector<double> coords;
  Vec(int n) : coords(std::vector<double>(n, 0)){};
  Vec(const Vec & other):coords(other.coords){};

  Vec operator+(const Vec &first) const {
    // scales down to the weaker dim
    int out_dim = std::min(first.coords.size(), coords.size());
    Vec out(out_dim);
    for (int i; i < out_dim; ++i) {
      out.coords[i] = first.coords[i] + coords[i];
    }
    return out;
  }

  Vec operator-(const Vec &first) const {
    // scales down to the weaker dim
    int out_dim = std::min(first.coords.size(), coords.size());
    Vec out(out_dim);
    for (int i; i < out_dim; ++i) {
      out.coords[i] = first.coords[i] - coords[i];
    }
    return out;
  }

  // some weird behavior here, honestly I should just use eigen if I want to do more
  Vec& operator*=(const double scalar) {
    for(int i=0;i<coords.size();++i){
      coords[i]= scalar*coords[i];
    }
    return *this;
  }

  // Vec operator*(const double scalar)const{
  //   Vec lhs(*this);
  //   lhs*=scalar;
  //   return lhs;
  // }
};

//leaving the controller outside for now
struct Agent {
  Vec loc;
  Vec vel;
};

int main(int argc, char *argv[]) { 
  std::vector<Agent> agents;
  

}
