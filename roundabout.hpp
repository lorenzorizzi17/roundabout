#include <vector>

class car {
 private:
  double r_;
  double theta_;
  double t_;
  int exit_;

 public:
  car(double, double, double, int);
  double r();
  double theta() ;
  double t();
  double exit();
  void evolve_tplus();
  void evolve_tminus();
  void evolve_ang();
};

class road {
 private:
  double angle_;
  double entrance_par_;
  std::vector<car> car_in;
  std::vector<car> car_out;

 public:
  road(double, double);

  double angle() ;
  double en_par() ;
  std::vector<car> carin();
  std::vector<car> carout();

  std::size_t size_in();

  std::size_t size_out();

  bool empty_in();

  bool empty_out();

  void newcar_rd(bool, int);

  void evolve_rd(bool);

  bool transfer_rd();
};