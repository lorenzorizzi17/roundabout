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
  double angle() const;
  std::vector<car>&  carin() ;
  std::vector<car>&  carout() ;
  std::size_t size_in() const;
  std::size_t size_out() const;
  bool empty_in() const;
  bool empty_out() const;
  void newcar_rd(bool const, int);
  void evolve_rd(bool const, rbout&, double const);
  bool transfer_rd();
  void erase_rd();
};

#endif
