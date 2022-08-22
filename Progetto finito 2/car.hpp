#ifndef CAR_H
#define CAR_H

class car {
 private:
  double theta_;
  double t_;
  int exit_;
  bool can_I_enter_;

 public:
  car(double theta = 0., double t = 0., int exit = 0., bool can_I_enter = false)
      : theta_{theta}, t_{t}, exit_{exit}, can_I_enter_{can_I_enter} {}   //rivedi costruttore
  double theta();
  double& t();
  double exit();
  void evolve_tplus();
  void evolve_tminus();
  void evolve_ang(double);
};

#endif