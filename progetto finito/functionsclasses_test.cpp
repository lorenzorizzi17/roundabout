#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "functionsclasses.cpp"

TEST_CASE("Ingresso corretto delle macchine nelle strade") //non funzionano
{
  road road (0.); 
  car C = car(0., 0., 0, false);
  car C_ = car(0., 0., 0, false);
  ((road).carin()).push_back(C);
  road.carin().push_back(C_);
  CHECK(road.size_in() == 2);
}

TEST_CASE("Ingresso corretto della macchina nella rotonda")
{
  bool parameter_true = true;
  rbout roundabout (4, 50.);
  road road (2 * M_PI / roundabout.n_roads()); 
  car C = car(0., 0., 0, false);
  road.carin().push_back(C);

  do
  {
    road.evolve_rd(parameter_true, roundabout);
  } while (static_cast<int>(road.carin().size()) != 0);

  if (road.transfer_rd()) {
      roundabout.newcar_rbt(0.);
    }

  CHECK( static_cast<int>(roundabout.size_rbout()) == 1);
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