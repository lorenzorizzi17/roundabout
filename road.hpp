#ifndef ROUNDABOUT_H
#define ROUNDABOUT_H


#include"rbout.hpp"
#include<cmath>
#include<random>

int n;

class road {
 private:
  double angle_;
  int entrance_par_;
  std::vector<car> car_in;
  std::vector<car> car_out;

 public:
  road(double angle = 0., int en_par = 0.)
      : angle_{angle}, entrance_par_{en_par} {}
  double angle();
  int en_par();
  std::vector<car> carin();
  std::vector<car> carout();

  std::size_t size_in();

  std::size_t size_out();

  bool empty_in();

  bool empty_out();

  void newcar_rd(bool);

  void evolve_rd(bool);

  bool transfer_rd(rbout);
  void erase_rd();
};

#endif