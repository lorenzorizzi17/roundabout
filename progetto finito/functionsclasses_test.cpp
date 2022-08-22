#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "road.hpp"
#include "doctest.h"


TEST_CASE("Corretto funzionamento di .size_in() e .carin()") 
{
  road road_{0};
  car C = car(0., 0., 0, false);
  ((road_).carin()).push_back(C);
  CHECK((road_).size_in()== 1);
}

TEST_CASE("Corretto funzionamento di .newcar_rbt()")  
{
  rbout rotonda(2,1);
  road road_(M_PI/2);
  rotonda.newcar_rbt((road_).angle());
  CHECK((rotonda.size_rbout())== 1);
  CHECK(((rotonda.carrbout())[0]).theta() == (M_PI/2+0.0665682));
}

TEST_CASE("Corretto avanzamento e stop delle macchine nella strada per dare la precedenza")
{
  rbout rotonda(2,1);
  road road_{0};
  car C = car(0., 0., 0, false);
  rotonda.newcar_rbt(0.1);   // praticamente ho inserito a mano una macchina nella rotonda vicino all'imbocco della strada. Se tutto va bene, la macchina nella strada si ferma a 0.95 per darle la precendenza
  ((road_).carin()).push_back(C);
  for (int i = 0; i < 50; i++)
  {
    (road_).evolve_rd(true, rotonda);
  }
  SUBCASE("La macchina rimane in strada se supera i 0.95"){ CHECK((road_).size_in()== 1);}
  SUBCASE("La macchina ha t = 0.95"){CHECK((((road_).carin())[0]).t() == doctest::Approx(0.95).epsilon(0.1));}

}

