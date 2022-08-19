#ifndef CAR_H
#define CAR_H

#include<vector>

class car {
 private:
  double r_;
  double theta_;
  double t_;
  int exit_;
  bool can_I_enter_;

 public:
  car(double r = 0., double theta = 0., double t = 0., int exit = 0, bool can_I_enter= false)
      : r_{r}, theta_{theta}, t_{t}, exit_{exit}, can_I_enter_{can_I_enter} {}
  double r();
  double theta();
  double t();
  double exit();
  bool can_I_enter();
  void can_I_enter_Y(bool);
  void evolve_tplus();
  void evolve_tminus();
  void evolve_ang(std::vector<car>,int);
};

#endif