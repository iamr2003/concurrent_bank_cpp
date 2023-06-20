// write a multithreaded version of this basic kinematic swarm sims
#include <cmath>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
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
  Vec(const Vec &other) : coords(other.coords){};

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

  // some weird behavior here, honestly I should just use eigen if I want to do
  // more
  Vec &operator*=(const double scalar) {
    for (int i = 0; i < coords.size(); ++i) {
      coords[i] = scalar * coords[i];
    }
    return *this;
  }

  double mag() const{
    double sq_sum =0;
    for(double n:coords)sq_sum+=n*n;
    return std::sqrt(sq_sum);
  }

  // Vec operator*(const double scalar)const{
  //   Vec lhs(*this);
  //   lhs*=scalar;
  //   return lhs;
  // }
};

// leaving the controller outside for now
struct Agent {
  Vec loc;
  Vec vel;
};

int main(int argc, char *argv[]) {
  // figure out how to do full single threaded sim

  // hyper params, could maybe try using defines
  constexpr double dt = 0.01;       // seconds
  constexpr double total_time = 10; // seconds
  constexpr double neighbor_radius = 5;
  constexpr double enc_size = 20; // m, square enc for all this
  constexpr int n_agents = 20;

  constexpr int timesteps = (int)(total_time/dt);
  // can eventually translate to other matrix form, but this is easier to think
  // about initially
  std::vector<Agent> agents(n_agents);
  std::vector<std::vector<Vec>> pos_history;
  std::vector<std::vector<Vec>> vel_history;

  std::random_device rd;
  std::mt19937 g(rd());
  std::uniform_real_distribution<> loc_dis(-enc_size, enc_size);
  std::uniform_real_distribution<> vel_dis(-1, 1);

  // initial agents to random velocities and positions inside enclosure
  for (Agent &agent : agents) {
    agent.loc.coords = {loc_dis(g), loc_dis(g)};
    agent.vel.coords = {vel_dis(g), vel_dis(g)};
  }
  
  for(int t=0;t<timesteps;++t){
    // compute things for each agent
    std::vector<Agent> next(n_agents);
    for(int i=0;i<agents.size();++i){
      //calculate neighbors at every iteration
      std::vector<Agent> neighbors;
      for(int j=0;j<agents.size();++j){
        if(i != j){
          double dist = (agents[i].loc-agents[j].loc).mag();
          if(dist < neighbor_radius){
            neighbors.push_back(agents[j]);
          }
        }
      }
    }
  }

}
