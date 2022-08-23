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
    "Corretto avanzamento e stop delle macchine nella strada per dare la "
    "precedenza") {
  rbout rotonda{2, 1};
  road road_{0};
  car C = car(0., 0., 0, false);
  rotonda.newcar_rbt(
      0.1);  // praticamente ho inserito a mano una macchina nella rotonda
             // vicino all'imbocco della strada. Se tutto va bene, la macchina
             // nella strada si ferma a 0.95 per darle la precendenza
  ((road_).carin()).push_back(C);
  for (int i = 0; i < 100; i++) {
    (road_).evolve_rd(true, rotonda);
  }
  SUBCASE("La macchina rimane in strada se supera i 0.95") {
    CHECK((road_).size_in() == 1);
  }
  SUBCASE("La macchina ha t = 0.95") {
    CHECK((((road_).carin())[0]).t() == doctest::Approx(0.95).epsilon(0.1));
  }
}

TEST_CASE(
    "Corretto funzionamento della funzione is_free(): ci sono macchine a cui "
    "dare precedenza") {
  rbout roundabout{4, 1};
  roundabout.newcar_rbt(M_PI / 2 - 0.1644);
  CHECK(is_free(M_PI / 2, roundabout) == false);
}

TEST_CASE(
    "Corretto funzionamento della funzione is_free(): non ci sono macchine a "
    "cui dare precedenza") {
  rbout roundabout{4, 1};
  roundabout.newcar_rbt(M_PI / 2 + 0.1844);
  CHECK(is_free(M_PI / 2, roundabout) == true);
}

TEST_CASE("Corretta uscita delle macchine nella strada giusta pt.1") {
  rbout roundabout{4, 1};
  road road{M_PI / 2};
  car C{0.066, 0, 2, false};
  (roundabout.carrbout()).push_back(C);
  int i{0};
  while (i < 1000) {
    roundabout.evolve_rbt();
    if (roundabout.transfer_rbt() > 0) {
      i = 1001;
      (road).newcar_rd(false);
    }
    i++;
  }
  CHECK(roundabout.transfer_rbt() == 2);
  CHECK(road.size_out() == 1);
}

TEST_CASE("Corretta uscita delle macchine nella strada giusta pt.2") {
  rbout roundabout{4, 1};
  road road{3 * M_PI / 4};
  car C{0.066, 0, 4, false};
  (roundabout.carrbout()).push_back(C);
  int i{0};
  while (i < 1000) {
    roundabout.evolve_rbt();
    if (roundabout.transfer_rbt() > 0) {
      i = 1001;
      (road).newcar_rd(false);
    }
    i++;
  }
  CHECK(roundabout.transfer_rbt() == 4);
  CHECK(road.size_out() == 1);
}

TEST_CASE("Corretta gestione delle macchine in uscita dalle strade") {
  road road{0};
  rbout roundabout{3, 1};
  road.newcar_rd(false);
  CHECK(road.size_out() == 1);
  for (int i = 0; i < 1000; i++) {
    road.evolve_rd(false, roundabout);
    road.erase_rd();
  }
  CHECK(road.size_out() == 0);
}