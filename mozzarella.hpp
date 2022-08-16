#include <cmath>
#include <vector>
#include<random>

int n;

class car {
 private:
  double r_;
  double theta_;
  double t_;
  int exit_;
  void module() {
    while (theta_ > 2 * M_PI) {
      theta_ -= 2 * M_PI;
      if (theta_ < 0) {
        theta_ += 2 * M_PI;
      }
    }
  }

 public:
  car(double r = 0., double theta = 0., double t = 0., int exit = 0)
      : r_{r}, theta_{theta}, t_{t}, exit_{exit} {
    module();
  }
  double r();
  double theta();
  double t();
  double exit();
  void evolve_tplus();
  void evolve_tminus();
  void evolve_ang();
};

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
