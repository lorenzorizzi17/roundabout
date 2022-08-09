#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

const int n{3};

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
  double t() { return t_; }
  double theta() { return theta_; }
  double exit() { return exit_; }
  void evolve_rd() { t_ += 0.01; }
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

  std::size_t size_in() const { return car_in.size(); }

  std::size_t size_out() const { return car_out.size(); }

  bool empty_in() const { return car_in.empty(); }

  bool empty_out() const { return car_out.empty(); }

  void newcar_rd(bool input) {
    if (input) {
      if (car_in.size() < 10) {
        car C(0., 0., 0, 1);
        car_in.push_back(C);
      }
    } else {
    }
  }

  void evolve_rd() {
    for (car& c : car_in) {
      if (c.t() < 1) {
        c.evolve_rd();
      }
    }
  }

  void transfer_rd() {
    // da implementare
  }
};
class rbout {
 private:
  double radius_;
  std::vector<car> car_rbout;

 public:
  rbout(double rad = 0.) : radius_{rad} {}
  double rad() { return radius_; }

  std::size_t size_rbout() const { return car_rbout.size(); }

  bool empty_rbout() const { return car_rbout.empty(); }

  void newcar_rbt(rbout RBT, road RD) {
    car C(RBT.rad(), RD.angle(), 0., 1);
    car_rbout.push_back(C);
  }

  void evolve_rbt() {
    // da implementare
  }

  void transfer_rbt() {
    // da implementare
  }
};

int main() {
  road strada1(0, 0);
  unsigned const display_height = .8 * sf::VideoMode::getDesktopMode().height;
  const int radius = .25 * display_height;
  // qui creo l'oggetto finestra e l'oggetto texture
  sf::RenderWindow window(sf::VideoMode(display_height, display_height),
                          "Roundabout", sf::Style::Default);
  sf::Texture texture;
  if (!texture.loadFromFile(
          "texture.jpg", sf::IntRect(0, 0, display_height, display_height))) {
    std::cout
        << "Attenzione! L'immagine non è stata caricata correttamente. "
           "Controlla che il file esista e sia nella giusta working directory";
  }
  texture.setSmooth(true);

  // qui istanzio due cerchi concentrici per poi creare la rotonda
  sf::CircleShape cerchio1(1.1 * radius);
  sf::CircleShape cerchio2(0.9 * radius);
  cerchio1.setTexture(&texture);
  cerchio1.setPosition(.5 * display_height - 1.1 * radius,
                       .5 * display_height - 1.1 * radius);
  cerchio2.setPosition(.5 * display_height - .9 * radius,
                       .5 * display_height - .9 * radius);

  // qui comincia il "game loop". Finché l'utente tiene aperta la finestra, il
  // programma va
  float t{0};
  while (window.isOpen()) {
    // questo while rende la finestra sensibile (altrimenti non si chiuderebbe)
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    //
    sf::CircleShape carprova1(0.025 * radius);
    carprova1.setOrigin(0.05 * radius, 0.05 * radius);
    sf::CircleShape carprova2(0.025 * radius);
    carprova1.setFillColor(sf::Color::Blue);
    carprova2.setFillColor(sf::Color::Red);

    //
    // qui comincio a disegnare. In primis refresho la finestra (clear) ...
    window.clear(sf::Color::White);
    // disegno i due cerchi concentrici ...
    window.draw(cerchio1);
    window.draw(cerchio2);

    // disegno le strade. Siccome devo farne n, realizzo un ciclo for con i da 1
    // a n, variando di volta in volta l'angolo
    for (int i = 1; i <= n; i++) {
      // sfasorad è semplicemente 360/n (in radianti). Invece l è la lunghezza
      // della strada. Per ragioni estetiche, questa è vincolata dalla lunghezza
      // della rotonda e dalle dimensioni del monitor dalla formula che segue
      // (mi sembrava la più sensata...)
      float sfasorad = ((2 * M_PI) / n) * i;
      float l = display_height / 2 - 1.1 * radius;
      sf::RectangleShape strada(sf::Vector2f(l, 0.2 * radius));
      strada.setOrigin(0.5 * l, 0.5 * 0.2 * radius);
      strada.setTexture(&texture);
      // per posizionare bene le strade, ci ho perso 5 diottrie. Purtroppo sfml
      // sembra non accettare le polari, quindi ho fatto un po' di calcoli a
      // mano per ottenere le coordinate cartesiane giuste. Le formulone brutte
      // che seguono sono frutto di ciò
      strada.setPosition(
          .5 * display_height + (std::cos(sfasorad)) * (radius + 0.5 * l),
          0.5 * display_height - (std::sin(sfasorad)) * (radius + 0.5 * l));
      strada.rotate(-sfasorad * (180 / M_PI));
      window.draw(strada);
    }

    // qui va il codice non grafico dell'int main dove disegneremo le macchine
    // ...
    //...
    //...
    //...
    float l = display_height / 2 - 1.1 * radius;
    strada1.newcar_rd(true);
    strada1.evolve_rd();
    for (car& c : strada1.carin()) {
      sf::CircleShape pallino(0.025 * radius);
      pallino.setFillColor(sf::Color::Blue);

      pallino.setPosition(
          0.5 * display_height + radius * std::cos(2 * M_PI / n + 0.0665682) -
              l * std::cos(M_PI - (2 * M_PI) / n) +
              c.t() * l * std::cos(M_PI - (2 * M_PI) / n),
          0.5 * display_height - radius * std::sin(2 * M_PI / n + 0.0665682) -
              l * std::sin(M_PI - (2 * M_PI) / n) +
              c.t() * l * std::sin(M_PI - (2 * M_PI) / n));
      window.draw(pallino);
    }
    // questo comando termina il game loop.
    window.display();
  }
  std::cout << t;
}
