#include "roundabout.hpp"

////////////////////////////////////////////////////
////          funzioni preliminari           ///////
////////////////////////////////////////////////////
std::random_device rd;
std::default_random_engine eng(rd());

bool can_generate(int r) {
  std::uniform_int_distribution<int> unif(1,r);
  int a = unif(eng);
  //int a = std::rand()%r +1;
  if (a <= 5) {
    return true;
  }
  return false;
}

int random_call(int n) {
  std::uniform_int_distribution<int> unif(1,n);
  int a = unif(eng);
  //int a = std::rand() % (n) + 1;
  return a;
}

double module360(double n) {
  double a = n;
  while (a > 0) {
    a = a - 2 * M_PI;
  }
  return a + 2 * M_PI;
}

bool is_free(double ang, rbout rotonda) {
  for (int i{0}; i < static_cast<int>(rotonda.size_rbout()); i++) {
    double a = std::abs(ang + 0.0665682 -
                        module360(((rotonda.carrbout())[i]).theta()));
    double b = a * 180 / M_PI;
    if ((std::abs(b) < 10) || (std::abs(b) > 350)) {
      return false;
    }
  }
  return true;
}

/////////////////////////////////////////
///////      funzioni car        ////////
/////////////////////////////////////////

double car::r() { return r_; }
double car::theta() { return theta_; }
double car::t() { return t_; }
double car::exit() { return exit_; }
void car::evolve_tplus() { t_ += 0.01; }
void car::evolve_tminus() { t_ -= 0.01; }
void car::evolve_ang() { theta_ += 0.01 / n; }

////////////////////////////////////////
//////    funzioni rotonda   ///////////
////////////////////////////////////////

double rbout::rad() { return radius_; }
std::vector<car> rbout::carrbout() { return car_rbout; }
std::size_t rbout::size_rbout() { return car_rbout.size(); }
bool rbout::empty_rbout() { return car_rbout.empty(); }
void rbout::newcar_rbt(double ang) {
  car C = car(rad(), ang + 0.0665682, 0, random_call(n));
  car_rbout.push_back(C);
};
void rbout::erase_rbt() {
  if (!car_rbout.empty()) {
    for (int i{0}; i < static_cast<int>(car_rbout.size()); i++) {
      auto foo = car_rbout.begin() + i;
      double a = std::abs(car_rbout[i].theta() -
                          2 * M_PI / n * (car_rbout[i].exit() - 1) + 0.0665682);
      if (std::abs(module360(a)) < 0.005) {
        car_rbout.erase(foo);
        i--;
      }
    }
  }
}
int rbout::transfer_rbt() {
  if (!car_rbout.empty()) {
    for (int i{0}; i < static_cast<int>(car_rbout.size()); i++) {
      double a = std::abs(car_rbout[i].theta() -
                          2 * M_PI / n * (car_rbout[i].exit() - 1) + 0.0665682);
      if (std::abs(module360(a)) < 0.005) {
        return car_rbout[i].exit();
      }
    }
    return -7;
  } else {
    return -7;
  }
}
void rbout::evolve_rbt() {
  if (!car_rbout.empty()) {
    for (int i{0}; i < static_cast<int>(car_rbout.size()); i++) {
      double a = std::abs(car_rbout[i].theta() -
                          2 * M_PI / n * (car_rbout[i].exit() - 1));
      if (!(std::abs(module360(a)) < 0.001)) {
        car_rbout[i].evolve_ang();
      }
    }
  }
}

///////////////////////////////////////
//////      funzioni road      ////////
///////////////////////////////////////

double road::angle() { return angle_; }
int road::en_par() { return entrance_par_; }
std::vector<car> road::carin() { return car_in; }
std::vector<car> road::carout() { return car_out; }

std::size_t road::size_in() { return car_in.size(); }

std::size_t road::size_out() { return car_out.size(); }

bool road::empty_in() { return car_in.empty(); }

bool road::empty_out() { return car_out.empty(); }

void road::newcar_rd(bool input) {
  if (input) {
    if ((car_in.size() < 10) && (can_generate(500))) {
      car C = car(0., 0., 0., 0);
      car_in.push_back(C);
    }
  } else {
    car C_ = car(0, angle() - 0.0665682, 1, 2);
    car_out.push_back(C_);
  }
}

void road::evolve_rd(bool parameter) {
  if (parameter) {
    for (int i = 0; i < static_cast<int>(size_in()); i++) {
      if ((i == 0) && (car_in[i]).t() < 0.95) {
        car_in[i].evolve_tplus();
      }
      if ((i != 0) && ((car_in[i]).t() < 0.95) &&
          ((car_in[i]).t() - (car_in[i - 1]).t() < -0.1)) {
        car_in[i].evolve_tplus();
      }
    }
  } else {
    for (int i = 0; i < static_cast<int>(size_out()); i++) {
      if ((car_out[i]).t() > 0) {
        car_out[i].evolve_tminus();
      }
    }
  }
}

bool road::transfer_rd(rbout rotonda) {
  if (!car_in.empty()) {
    if ((*(car_in.begin())).t() >= 0.95 && is_free(angle(), rotonda)) {
      car_in.erase(car_in.begin());
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

void road::erase_rd() {
  if (!car_out.empty()) {
    for (int i{0}; i < static_cast<int>(car_out.size()); i++) {
      if ((*(car_out.begin())).t() <= 0) {
        car_out.erase(car_out.begin());
      }
    }
  }
}