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
  rotonda.newcar_rbt((road_).angle());  // creo una car nella rotonda all'angolo
                                        // della strada road_
  CHECK((rotonda.size_rbout()) == 1);
  CHECK(
      ((rotonda.carrbout())[0]).theta() ==
      (M_PI / 2 +
       0.0665682));  // controllo che la car venga istanziata nel punto corretto
}

TEST_CASE("Corretto avanzamento nella rotonda") {
  rbout roundabout(2, 1);
  roundabout.newcar_rbt(M_PI / 2);  // istanzio una car a 90°
  for (int i = 0; i < 100; i++) {   // la faccio muovere nella rotonda
    (roundabout).evolve_rbt();
  }
  CHECK(((roundabout.carrbout())[0]).theta() ==
        doctest::Approx((M_PI / 2 + 0.0665682 +
                         100 * 0.0075)));  // 0.0075 è il delta_theta della
                                           // funzione car::evolve_ang()
}

TEST_CASE(
    "Corretto funzionamento della funzione is_free(): ci sono macchine a cui "
    "dare precedenza") {
  rbout roundabout(4, 1);
  road road(M_PI / 2);
  roundabout.newcar_rbt(
      M_PI / 2 -
      0.1644);  // Creo una macchina in rotonda che si trovi ad un angolo pari a
                // 0.1644(= 9 gradi) di distanza dall'angolo della strada road.
                // Così facendo, un'eventuale macchina che si trovi a voler
                // entrare in rotonda, dovrà fermarsi per dare precedenza (nel
                // main minimum_angle è pari a 10 )
  CHECK(is_free(M_PI / 2, roundabout, 10.) ==
        false);  //  la funzione is_free dovrebbe ritornare false
}

TEST_CASE(
    "Corretto funzionamento della funzione is_free(): non ci sono macchine a "
    "cui dare precedenza") {
  rbout roundabout{4, 1};
  roundabout.newcar_rbt(
      M_PI / 2 +
      0.1844);  // come sopra, ma stavolta l'angolo è superiore a minimum_angle
                // e la macchina non dovrebbe dare precedenza
  CHECK(is_free(M_PI / 2, roundabout, 10.) ==
        true);  // la funzione is_free dovrebbe ritornare true
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
        0.);  // creo volutamente una macchina in rotonda a theta=0
    ((road_).carin()).push_back(C);  // inserisco la macchina C in strada a t=0
    for (int i = 0; i < 100;
         i++) {  // faccio avanzare la macchina nella strada,
                 // tengo ferma quella nella rotonda, in questo caso
                 // non mi serve che si muova
      (road_).evolve_rd(true, roundabout, 10.);
    }
    CHECK((road_).size_in() ==
          1);  // controllo che effettivamente la macchina sia rimasta nella
               // strada (non ha la precedenza)
    CHECK((((road_).carin())[0]).t() ==
          doctest::Approx(0.95).epsilon(0.01));  // ne verifico la posizione
  }

  SUBCASE(
      "La macchina entra in rotonda se non ci sono macchine in prossimità "
      "dell'uscita") {
    roundabout.newcar_rbt(2.);  // stavolta la macchina in rotonda è distante e
                                // non c'è bisogno di darle la precedenza
    ((road_).carin()).push_back(C);
    for (int i = 0; i < 100;
         i++) {  // faccio avanzare la car nella strada verso la rotonda
      (road_).evolve_rd(true, roundabout, 10.);
    }
    if (road_.transfer_rd()) {  // quando è il caso, trasferisco la macchina in
                                // strada in rotonda
      roundabout.newcar_rbt(road_.angle());
    }
    CHECK((road_).size_in() ==
          0);  // controllo che non ci sia più la car in strada
    CHECK(roundabout.size_rbout() ==
          2);  // controllo che entrambe le car siano nella rotonda
    CHECK((((road_).carin())[0]).t() ==
          doctest::Approx(1.).epsilon(
              0.01));  // verifico il parametro t della car appena entrata
  }
}

TEST_CASE("Corretta uscita delle macchine dalla rotonda alla strada corretta") {
  rbout roundabout(4, 10);

  std::vector<road> roads;
  roads.push_back(road{0});             // strada 1
  roads.push_back(road{M_PI / 2});      // strada 2
  roads.push_back(road{M_PI});          // strada 3
  roads.push_back(road{3 * M_PI / 2});  // strada 4

  car C{0., 0., 2, true};  // voglio che esca alla strada 2

  roundabout.carrbout().push_back(
      C);  // istanzio la car direttamente in rotonda e la faccio avanzare
  for (int i = 0; i < 10000; i++) {
    roundabout.evolve_rbt();
    if (roundabout.transfer_rbt() >
        0) {  // la faccio evolvere in rotonda finché non arriva alla sua uscita
      break;
    }
  }
  int a = roundabout.transfer_rbt();
  if (a > 0) {
    roads[a - 1].newcar_rd(false, 5);  // chiamo road::newcar_rd(false), quindi
                                       // in uscita, sulla strada di uscita
    roundabout.erase_rbt();
  }
  for (int i = 0; i < 100; i++) {
    roads[a - 1].evolve_rd(
        false, roundabout,
        10.);  // faccio proseguire la car lungo la strada di uscita
  }
  CHECK(
      (roundabout).size_rbout() ==
      0);  // verifico che la car non sia più in rotonda dopo rbout::erase_rbt()
  CHECK(roads[1].size_out() ==
        1);  // verifico che la car sia nella strada in uscita, cioe la seconda
  CHECK((((roads[1]).carout())[0]).t() ==
        doctest::Approx(0.).epsilon(
            0.01));  // verifico che la car sia alla fine della strada
}
