#include <cmath>
#include <random>

#include "functions.hpp"

////////////////////////////////////////////////////
/////            funzioni libere              //////
////////////////////////////////////////////////////

std::random_device rd;
std::default_random_engine engine(rd());

bool can_generate(int const rate) {
  std::uniform_int_distribution<int> unif(1, 100);
  int const test = unif(engine);
  if (test <= rate) {
    return true;
  }
  return false;
}

int random_call(int const max_exit) {
  std::uniform_int_distribution<int> unif(1, max_exit);
  int const assigned_exit = unif(engine);
  return assigned_exit;
}

double module360(double angle) {
  while (angle > 0.) {
    angle = angle - 2 * M_PI;
  }
  return angle + 2 * M_PI;
}

bool is_free(double const street_angle, rbout roundabout) {
  for (int i{0}; i < static_cast<int>(roundabout.size_rbout()); i++) {
    double const angle_difference =
        std::abs(street_angle + 0.0665682 -
                 module360(((roundabout.carrbout())[i]).theta()));
    double const a_d_degree = angle_difference * 180 / M_PI;
    if ((std::abs(a_d_degree) < 10.) || (std::abs(a_d_degree) > 350.)) {
      return false;
    }
  }
  return true;
}

/////////////////////////////////////////
///////      funzioni car        ////////
/////////////////////////////////////////

double car::theta() { return theta_; }
double car::t() { return t_; }
double car::exit() { return exit_; }
bool car::can_I_enter() { return can_I_enter_; }
void car::can_I_enter_Y(bool const yes) { can_I_enter_ = yes; }
void car::evolve_tplus() { t_ += 0.01; }   // 0.03
void car::evolve_tminus() { t_ -= 0.01; }  // 0.03
void car::evolve_ang() { theta_ += 0.0075; }  // 0.02

////////////////////////////////////////
//////    funzioni rotonda   ///////////
////////////////////////////////////////

int rbout::n_roads() { return n_roads_; }
double rbout::rad() { return radius_; }
std::vector<car>& rbout::carrbout() { return car_rbout; }
std::size_t rbout::size_rbout() { return car_rbout.size(); }
bool rbout::empty_rbout() { return car_rbout.empty(); }

void rbout::newcar_rbt(double const street_angle) {
  car C = car(street_angle + 0.0665682, 0., random_call(n_roads()), true);
  car_rbout.push_back(C);
}

void rbout::erase_rbt() {
  if (!empty_rbout()) {
    for (int i{0}; i < static_cast<int>(size_rbout()); i++) {
      auto car_position = car_rbout.begin() + i;
      double const angle_difference = std::abs(
          car_rbout[i].theta() -
          2 * M_PI / n_roads() * (car_rbout[i].exit() - 1) + 0.0665682);
      if (std::abs(module360(angle_difference)) < 0.02) {  //0.025
        car_rbout.erase(car_position);
        i--;
      }
    }
  }
}

int rbout::transfer_rbt() {
  if (!empty_rbout()) {
    for (int i{0}; i < static_cast<int>(size_rbout()); i++) {
      double const angle_difference = std::abs(
          car_rbout[i].theta() -
          2 * M_PI / n_roads() * (car_rbout[i].exit() - 1) + 0.0665682);
      if (std::abs(module360(angle_difference)) < 0.02) {
        return car_rbout[i].exit();
      }
    }
    return 0;
  } else {
    return 0;
  }
}

void rbout::evolve_rbt() {
  if (!empty_rbout()) {
    for (int i{0}; i < static_cast<int>(size_rbout()); i++) {
      double const angle_difference = std::abs(
          car_rbout[i].theta() -
          2 * M_PI / n_roads() * (car_rbout[i].exit() - 1) + 0.0665682);
      if (std::abs(module360(angle_difference)) > 0.02) {
        car_rbout[i].evolve_ang();
      }
    }
  }
}

///////////////////////////////////////
//////      funzioni road      ////////
///////////////////////////////////////

double road::angle() { return angle_; }
std::vector<car>& road::carin() { return car_in; }
std::vector<car>& road::carout() { return car_out; }
std::size_t road::size_in() { return car_in.size(); }
std::size_t road::size_out() { return car_out.size(); }
bool road::empty_in() { return car_in.empty(); }
bool road::empty_out() { return car_out.empty(); }

void road::newcar_rd(bool const input, int rate) {
  if (input) {
    if ((static_cast<int>(size_in()) < 10) && (can_generate(rate))) {
      car C = car(0., 0., 0, false);
      car_in.push_back(C);
    }
  } else {
    car C_ = car(angle() - 0.0665682, 1., 0, true);

    car_out.push_back(C_);
  }
}

void road::evolve_rd(bool const input, rbout& roundabout) {
  if (input) {  // input == true
    for (int i = 0; i < static_cast<int>(size_in()); i++) {
      if ((i == 0) && ((car_in[i]).t() < 0.95)) {
        car_in[i].evolve_tplus();
      }
      if (car_in[i].can_I_enter()) {  // car_in[i].can_I_enter == true
        car_in[i].evolve_tplus();
      }
      if ((i == 0) && ((car_in[i]).t() >= 0.95) &&
          (is_free(angle(), roundabout)) && (!car_in[i].can_I_enter())) {
        car_in[i].evolve_tplus();
        car_in[i].can_I_enter_Y(true);
      }

      if ((i != 0) && ((car_in[i]).t() < 0.95) &&
          (std::abs((car_in[i]).t() - (car_in[i - 1]).t()) > 0.15)) {
        car_in[i].evolve_tplus();
      }
    }
  } else {
    for (int i = 0; i < static_cast<int>(size_out()); i++) {
      if ((car_out[i]).t() > 0.) {
        car_out[i].evolve_tminus();
      }
    }
  }
}

bool road::transfer_rd() {
  if (!empty_in()) {
    if ((*(car_in.begin())).t() >= 1.) {
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
  if (!empty_out()) {
    for (int i{0}; i < static_cast<int>(size_out()); i++) {
      if ((*(car_out.begin())).t() <= 0.) {
        car_out.erase(car_out.begin());
      }
    }
  }
}