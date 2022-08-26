#ifndef ROUNDABOUT_H
#define ROUNDABOUT_H

#include "rbout.hpp"

class road {
 private:
  double angle_;
  std::vector<car> car_in;
  std::vector<car> car_out;

 public:
  road(double angle = 0.) : angle_{angle} {}
  double angle();
  std::vector<car>& carin();
  std::vector<car>& carout();
  std::size_t size_in();
  std::size_t size_out();
  bool empty_in();
  bool empty_out();
  void newcar_rd(bool const, int);
  void evolve_rd(bool const, rbout&, double const);
  bool transfer_rd();
  void erase_rd();
};

#endif