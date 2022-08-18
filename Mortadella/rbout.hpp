#ifndef RBOUT_H
#define RBOUT_H

#include"car.hpp"
#include<vector>


class rbout {
 private:
  double radius_;
  std::vector<car> car_rbout;

 public:
  rbout(double rad = 0.) : radius_{rad} {}
  double rad();
  std::vector<car> carrbout();

  std::size_t size_rbout();
  bool empty_rbout();

  void newcar_rbt(double);

  void erase_rbt();

  int transfer_rbt();

  void evolve_rbt();
};
#endif