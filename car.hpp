#ifndef CAR_H
#define CAR_H

class car {
 private:
  double r_;
  double theta_;
  double t_;
  int exit_;

 public:
  car(double r = 0., double theta = 0., double t = 0., int exit = 0)
      : r_{r}, theta_{theta}, t_{t}, exit_{exit} {}
  double r();
  double theta();
  double t();
  double exit();
  void evolve_tplus();
  void evolve_tminus();
  void evolve_ang();
};

#endif