#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

int n_roads;

class car {
 private:
  double r_;
  double theta_;
  double t_;
  double exit_;

 public:
  car(double r = 0., double theta = 0., double t = 0., double exit = 0.)
      : r_{r}, theta_{theta}, t_{t}, exit_{exit} {}
  double r() { return r_; }
  double theta() { return theta_; }
  double t() { return t_; }
  double exit() { return exit_; }
  void evolve_tplus() { t_ += 0.01; }
  void evolve_tminus() { t_ -= 0.05; }
  void evolve_ang() { theta_ += 0.01; }
};

class rbout {
 private:
  double radius_;
  std::vector<car> car_rbout;

 public:
  rbout(double rad = 0.) : radius_{rad} {}
  double rad() { return radius_; }
  auto carrbout() { return car_rbout; }

  std::size_t size_rbout() const { return car_rbout.size(); }
  bool empty_rbout() const { return car_rbout.empty(); }

  void newcar_rbt(double ang) {
    std::cout << "Arrivato";
    car C = car(rad(), ang + 0.0665682, 0,
                1.);  // angolo deviato per l'ingresso
    (car_rbout).push_back(C);
    std::cout << (car_rbout[0]).theta();
  };

  void evolve_rbt() {
    if (!car_rbout.empty()) {
      car_rbout[0].evolve_ang();
      if (car_rbout.size() > 1) {
        for (int i = 1; i <= car_rbout.size(); i++) {
          if (((car_rbout[i]).theta() - (car_rbout[i - 1]).theta() < -0.1)) {
            car_rbout[i].evolve_ang();
          }
        }
      }
    }
  }

  void transfer_rbt() {}
};

class road {
 private:
  double angle_;
  double entrance_par_;
  std::vector<car> car_in;
  std::vector<car> car_out;

 public:
  road(double angle = 0., double en_par = 0.)
      : angle_{angle}, entrance_par_{en_par} {}
  double angle() { return angle_; }
  double en_par() { return entrance_par_; }
  std::vector<car> carin() { return car_in; }
  std::vector<car> carout() { return car_out; }

  std::size_t size_in() const { return car_in.size(); }

  std::size_t size_out() const { return car_out.size(); }

  bool empty_in() const { return car_in.empty(); }

  bool empty_out() const { return car_out.empty(); }

  void newcar_rd(bool input, int i) {
    if (input) {
      if ((car_in.size() < 8) && (i % 20 == 0)) {
        car C = car(0., 0., 0, 1);
        car_in.push_back(C);
      }
    } else {
      // macchine in uscita
    }
  }

  void evolve_rd() {
    if ((car_in[0]).t() < 1) {
      car_in[0].evolve_tplus();
    }
    for (int i = 1; i <= car_in.size(); i++) {
      if (((car_in[i]).t() < 1) &&
          ((car_in[i]).t() - (car_in[i - 1]).t() < -0.1)) {
        car_in[i].evolve_tplus();
      }
    }
  }

  bool transfer_rd(rbout rotond) {
    if ((car_in[0]).t() >= 1) {
      car_in.erase(car_in.begin());
      return true;
    }
  }
};

int main() {
    
    n_roads = 4;
    
    std::random_device rd; //è il seed siamo felici
    std::default_random_engine random(rd());

    rbout RTD(20.);
    std::vector<road> v_roads(n_roads);
    for( int i = 0; i < n_roads; i++)
    {
        std::uniform_int_distribution<int> entr_par (1, 7); //perché se en_par è 1 mi spawna sempre macchine
        double n = entr_par(random) / 10.; //parametro di entrata rispetto a cui spawna le macchine o meno
        double angle = i * ((2 * M_PI)/n_roads);
        v_roads[i] = (angle, n);

        std::cout << "Angolo strada: " << v_roads[i].angle() << "  Parametro di entrata: " << v_roads[i].en_par() << "\n"; 
    }

}