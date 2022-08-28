#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "functions.hpp"

TEST_CASE("Corretto funzionamento di .size_in() e .carin()") {
  road road_{0};
  car C = car(0., 0., 0, false);
  ((road_).carin()).push_back(C);
  CHECK((road_).size_in() == 1);
}

TEST_CASE("Corretto funzionamento di .newcar_rbt()") {
  rbout rotonda(1, 1);
  road road_(M_PI / 2);
  rotonda.newcar_rbt((road_).angle());
  CHECK((rotonda.size_rbout()) == 1);
  CHECK(((rotonda.carrbout())[0]).theta() == (M_PI / 2 + 0.0665682));
}

TEST_CASE("Corretto avanzamento nella rotonda") {
  rbout roundabout(2, 1);
  roundabout.newcar_rbt(M_PI / 2);
  for (int i = 0; i < 100; i++) {
    (roundabout).evolve_rbt();
  }
  CHECK(((roundabout.carrbout())[0]).theta() ==
        doctest::Approx((M_PI / 2 + 0.0665682 + 100 * 0.0075)));
}
TEST_CASE(
    "Corretto funzionamento della funzione is_free(): ci sono macchine a cui "
    "dare precedenza") {
  rbout roundabout{4, 1};
  roundabout.newcar_rbt(M_PI / 2 - 0.1644);
  CHECK(is_free(M_PI / 2, roundabout, 10.) == false);
}

TEST_CASE(
    "Corretto funzionamento della funzione is_free(): non ci sono macchine a "
    "cui dare precedenza") {
  rbout roundabout{4, 1};
  roundabout.newcar_rbt(M_PI / 2 + 0.1844);
  CHECK(is_free(M_PI / 2, roundabout, 10.) == true);
}
TEST_CASE(
    "Corretto avanzamento e stop delle macchine nella strada per dare la "
    "precedenza") {
  rbout roundabout(2, 1);
  road road_{0};
  car C = car(0., 0., 0, false);

  SUBCASE(
      "La macchina rimane in strada se è presente un'altra vettura in "
      "rotonda") {
    roundabout.newcar_rbt(
        0.);  // praticamente ho inserito a mano una macchina nella rotonda
              // vicino all'imbocco della strada. Se tutto va bene, la macchina
              // nella strada si ferma a 0.95 per darle la precendenza
    ((road_).carin()).push_back(C);
    for (int i = 0; i < 100; i++) {
      (road_).evolve_rd(true, roundabout, 10.);
    }
    CHECK((road_).size_in() == 1);
    CHECK((((road_).carin())[0]).t() == doctest::Approx(0.95).epsilon(0.01));
  }

  SUBCASE(
      "La macchina entra in rotonda se non ci sono macchine in prossimità "
      "dell'uscita") {
    roundabout.newcar_rbt(
        2.);  // praticamente ho inserito a mano una macchina nella rotonda
              // vicino all'imbocco della strada. Se tutto va bene, la macchina
              // nella strada si ferma a 0.95 per darle la precendenza
    ((road_).carin()).push_back(C);
    for (int i = 0; i < 100; i++) {
      (road_).evolve_rd(true, roundabout, 10.);
    }
    if (road_.transfer_rd()) {
      roundabout.newcar_rbt(road_.angle());
    }
    CHECK((road_).size_in() == 0);
    CHECK(roundabout.size_rbout() == 2);
    CHECK((((road_).carin())[0]).t() == doctest::Approx(1.).epsilon(0.01));
  }
}

TEST_CASE("Corretta uscita delle macchine dalla rotonda alla strada corretta") {
  rbout roundabout(4, 10);

  std::vector<road> roads;
  roads.push_back(road{0});
  roads.push_back(road{M_PI / 2});
  roads.push_back(road{M_PI});
  roads.push_back(road{3 * M_PI / 2});

  car C{0., 0., 2, true};

  roundabout.carrbout().push_back(C);
  for (int i = 0; i < 10000; i++) {
    roundabout.evolve_rbt();
    if (roundabout.transfer_rbt() > 0) {
      break;
    }
  }
  int a = roundabout.transfer_rbt();
  if (a > 0) {
    roads[a].newcar_rd(false, 5);
    roundabout.erase_rbt();
  }
  for (int i = 0; i < 100; i++) {
    roads[a].evolve_rd(false, roundabout, 10.);
  }
  CHECK((roundabout).size_rbout() == 0);
  CHECK(roads[a].size_out() == 1);
  CHECK((((roads[a]).carout())[0]).t() == doctest::Approx(0.).epsilon(0.01));
}