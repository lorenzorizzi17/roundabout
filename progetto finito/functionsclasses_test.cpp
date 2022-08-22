#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "road.hpp"

TEST_CASE("Ingresso corretto delle macchine nelle strade")  // non funzionano
{
  road road_{0.0};
  REQUIRE(road_.carin().empty());
  road_.carin().push_back({0., 0., 0, false});
  std::cout << "per quale cazzo di motivo la dimensione è ancora "
            << static_cast<int>(road_.size_in());
  road_.carin().push_back({0., 0., 0, false});
  CHECK(road_.size_in() == 2);
}

TEST_CASE("Ingresso corretto delle macchine nelle strade 2")  // funziona ma non
                                                              // so perchè
{
  std::vector<car> uff;
  car C = car(0., 0., 0, false);
  car C_ = car(0., 0., 0, false);
  uff.push_back(C);
  std::cout << "per quale cazzo di motivo la dimensione è ancora "
            << uff.size();
  uff.push_back(C_);
  CHECK(uff.size() == 2);
}

TEST_CASE("Ingresso corretto della macchina nella rotonda") {
  bool parameter_true = true;
  rbout roundabout = rbout(4, 50.);
  road road_ = road(2 * M_PI / roundabout.n_roads());
  car C = car(0., 0., 0, false);
  road_.carin().push_back(C);

  do {
    road_.evolve_rd(parameter_true, roundabout);
  } while (static_cast<int>(road_.carin().size()) != 0);

  if (road_.transfer_rd()) {
    roundabout.newcar_rbt(0.);
  }

  CHECK(static_cast<int>(roundabout.size_rbout()) == 1);
}

/*
TEST_CASE("Hooke's law")
{
  SUBCASE("Negative k throws")
  {
    CHECK_THROWS(Hooke{-1.0, 1.0});
  }
  SUBCASE("Negative l0 throws")
  {
    CHECK_THROWS(Hooke{1.0, -1.0});
  }

  SUBCASE("k=0 throws")
  {
    CHECK_THROWS(Hooke{0.0, 1.0});
  }

  Hooke const hooke{2.0, 10.0};

  SUBCASE("Nominal use, zero F")
  {
    PPState p1{1.0, 0.0, 0.0};
    PPState p2{1.0, 10.0, 0.0};
    CHECK(hooke(p1, p2) == doctest::Approx(0.0));
  }

  SUBCASE("Nominal use, F > 0")
  {
    PPState p1{1.0, 0.0, 0.0};
    PPState p2{1.0, 12.0, 0.0};
    CHECK(hooke(p1, p2) == doctest::Approx(4.0));
  }

  SUBCASE("Nominal use, F < 0")
  {
    PPState p1{1.0, 0.0, 0.0};
    PPState p2{1.0, 8.0, 0.0};
    CHECK(hooke(p2, p1) == doctest::Approx(-4.0));
  }
}
*/