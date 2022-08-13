#include "roundabout.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

const int n{3};
unsigned const display_height = .8 * sf::VideoMode::getDesktopMode().height;
const int radius = .25 * display_height;

//funzioni preliminari

int random_call(int n, int roadno) {
  int a = std::rand()%(n)+1;
  while (a == roadno) {
    a = std::rand() % (n)+1;
  }
  return a;
}



double approx(double n) {
  double a = n;
  while (a > 0) {
    a = a - 2 * M_PI;
  }
  return a + 2 * M_PI;
}

void is_free() {

}

// funzioni car
car::car(double r = 0., double theta = 0., double t = 0., int exit = 0)
    : r_{r}, theta_{theta}, t_{t}, exit_{exit} {}
double car::r() { return r_; }
double car::theta() { return theta_; }
double car::t() { return t_; }
double car::exit() { return exit_; }
void car::evolve_tplus() { t_ += 0.01; }
void car::evolve_tminus() { t_ -= 0.01; }
void car::evolve_ang() { theta_ += 0.01; }

// classe rotonda
class rbout {
 private:
  double radius_;
  std::vector<car> car_rbout;

 public:
  rbout(double rad = 0.) : radius_{rad} {}
  double rad() { return radius_; }
  std::vector<car> carrbout() { return car_rbout; }

  std::size_t size_rbout() { return car_rbout.size(); }
  bool empty_rbout() { return car_rbout.empty(); }

  void newcar_rbt(double ang) {
    car C = car(rad(), ang + 0.0665682, 0, random_call(3,static_cast<int>(1+ang*n/(2*M_PI))));
    car_rbout.push_back(C);
  };

  void erase_rbt() {
    if (!car_rbout.empty()) {
      for (int i{0}; i < car_rbout.size(); i++) {
        auto foo = car_rbout.begin() + i;
        double a = std::abs(car_rbout[i].theta() -
                            2 * M_PI / n * (car_rbout[i].exit() - 1)+0.0665682);
        if (std::abs(approx(a)) < 0.005) {
          car_rbout.erase(foo);
          i--;
        }
      }
    }
  }

  int transfer_rbt() {
    if (!car_rbout.empty()) {
      for (int i{0}; i < car_rbout.size(); i++) {
        double a = std::abs(car_rbout[i].theta() -
                            2 * M_PI / n * (car_rbout[i].exit() - 1)+0.0665682);
        if (std::abs(approx(a)) < 0.005) {
          return car_rbout[i].exit();
        } //qui qualcosa
      }
      return -7;
    } else {
      return -7;
    }
  }

  void evolve_rbt() {
    if (!car_rbout.empty()) {
      for (int i = 0; i < car_rbout.size(); i++) {
        double a =std::abs(car_rbout[i].theta() -
                            2 * M_PI / n * (car_rbout[i].exit() - 1));
        if (!std::abs(approx(a)) < 0.02) {
           car_rbout[i].evolve_ang();
        }
      }
    }
  }
};

// funzioni road
road::road(double angle = 0., double en_par = 0.)
    : angle_{angle}, entrance_par_{en_par} {}

double road::angle() { return angle_; }
double road::en_par() { return entrance_par_; }
std::vector<car> road::carin() { return car_in; }
std::vector<car> road::carout() { return car_out; }

std::size_t road::size_in() { return car_in.size(); }

std::size_t road::size_out() { return car_out.size(); }

bool road::empty_in() { return car_in.empty(); }

bool road::empty_out() { return car_out.empty(); }

void road::newcar_rd(bool input, int i) {
  if (input) {
    if ((car_in.size() < 10) && (i % 50 == 0)) {
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
    for (int i = 0; i < car_in.size(); i++) {
      if ((i == 0) && (car_in[i]).t() < 1) {
        car_in[i].evolve_tplus();
      }
      if ((i != 0) && ((car_in[i]).t() < 1) &&
          ((car_in[i]).t() - (car_in[i - 1]).t() < -0.1)) {
        car_in[i].evolve_tplus();
      }
    }
  } else {
    for (int i = 0; i < car_out.size(); i++) {
      if ((car_out[i]).t() > 0) {
        car_out[i].evolve_tminus();
      }
    }
  }
}

bool road::transfer_rd() {
  if (!car_in.empty()) {
    if ((*(car_in.begin())).t() >= 1) {
      car_in.erase(car_in.begin());  // sistemo membri privati
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

int main() {
  int const fps = 60;

  std::srand(time(NULL));
  road strada1(0, 0);
  road strada2(2 * M_PI / n, 0);
  road strada3(4 * M_PI / n, 0);

  // road roads[3] = {strada1, strada2, strada3};

  rbout rotonda(radius);
  // qui creo l'oggetto finestra e l'oggetto texture
  sf::RenderWindow window(sf::VideoMode(display_height, display_height),
                          "Roundabout", sf::Style::Default);
  window.setFramerateLimit(fps);
  sf::Texture texture;
  if (!texture.loadFromFile(
          "texture.jpg", sf::IntRect(0, 0, display_height, display_height))) {
    std::cout << "Attenzione! L'immagine non è stata caricata correttamente. "
                 "Controlla che il file esista e sia nella giusta working "
                 "directory";
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

  // qui comincia il "game loop". Finché l'utente tiene aperta la finestra,
  // il programma va
  int i{0};
  while (window.isOpen()) {
    // questo while rende la finestra sensibile (altrimenti non si
    // chiuderebbe)
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    //
    // qui comincio a disegnare. In primis refresho la finestra (clear) ...
    window.clear(sf::Color::White);
    // disegno i due cerchi concentrici ...
    window.draw(cerchio1);
    window.draw(cerchio2);

    // disegno le strade. Siccome devo farne n, realizzo un ciclo for con i
    // da 1 a n, variando di volta in volta l'angolo
    for (int i = 1; i <= n; i++) {
      // sfasorad è semplicemente 360/n (in radianti). Invece l è la
      // lunghezza della strada. Per ragioni estetiche, questa è vincolata
      // dalla lunghezza della rotonda e dalle dimensioni del monitor dalla
      // formula che segue (mi sembrava la più sensata...)
      float sfasorad = ((2 * M_PI) / n) * i;
      float l = display_height / 2 - 1.1 * radius;
      sf::RectangleShape strada(sf::Vector2f(l, 0.2 * radius));
      strada.setOrigin(0.5 * l, 0.5 * 0.2 * radius);
      strada.setTexture(&texture);
      // per posizionare bene le strade, ci ho perso 5 diottrie. Purtroppo
      // sfml sembra non accettare le polari, quindi ho fatto un po' di
      // calcoli a mano per ottenere le coordinate cartesiane giuste. Le
      // formulone brutte che seguono sono frutto di ciò
      strada.setPosition(
          .5 * display_height + (std::cos(sfasorad)) * (radius + 0.5 * l),
          0.5 * display_height - (std::sin(sfasorad)) * (radius + 0.5 * l));
      strada.rotate(-sfasorad * (180 / M_PI));
      window.draw(strada);
    }

    // qui va il codice non grafico dell'int main dove disegneremo le
    // macchine
    // ...
    //...
    //...
    //...
    float l = display_height / 2 - 1.1 * radius;

    strada1.newcar_rd(true, i);   //serve per sfasare l'ingresso
    strada2.newcar_rd(true, i + 6);
    strada3.newcar_rd(true,i+2);

    strada1.evolve_rd(true);
    strada2.evolve_rd(true);
    strada3.evolve_rd(true);

    if (strada1.transfer_rd()) {
      rotonda.newcar_rbt(strada1.angle());
    };
    if (strada2.transfer_rd()) {
      rotonda.newcar_rbt(strada2.angle());
    };
    if (strada3.transfer_rd()) {
      rotonda.newcar_rbt(strada3.angle());
    };
    /*if (strada3.transfer_rd()) {
      rotonda.newcar_rbt(strada3.angle());
    }; */

    rotonda.evolve_rbt();

    if (rotonda.transfer_rbt() == 2) {
      strada2.newcar_rd(false, i);
    }
    if (rotonda.transfer_rbt() == 3) {
      strada3.newcar_rd(false, i);
    }
    if (rotonda.transfer_rbt() == 1) {
      strada1.newcar_rd(false, i);
    }
    rotonda.erase_rbt();

    strada1.evolve_rd(false);
    strada2.evolve_rd(false);
    strada3.evolve_rd(false);
    for (car& c : strada1.carin()) {
      sf::CircleShape pallino(0.025 * radius);
      pallino.setFillColor(sf::Color::Blue);
      pallino.setOrigin(0.025 * radius, 0.025 * radius);
      pallino.setPosition(0.5 * display_height +
                              radius * std::cos(strada1.angle() + 0.0665682) -
                              l * std::cos(M_PI - strada1.angle()) +
                              c.t() * l * std::cos(M_PI - strada1.angle()),
                          0.5 * display_height -
                              radius * std::sin(strada1.angle() + 0.0665682) -
                              l * std::sin(M_PI - strada1.angle()) +
                              c.t() * l * std::sin(M_PI - strada1.angle()));
      window.draw(pallino);
    }

    for (car& c : strada2.carin()) {
      sf::CircleShape pallino(0.025 * radius);
      pallino.setFillColor(sf::Color::Blue);
      pallino.setOrigin(0.025 * radius, 0.025 * radius);
      pallino.setPosition(0.5 * display_height +
                              radius * std::cos(strada2.angle() + 0.0665682) -
                              l * std::cos(M_PI - strada2.angle()) +
                              c.t() * l * std::cos(M_PI - strada2.angle()),
                          0.5 * display_height -
                              radius * std::sin(strada2.angle() + 0.0665682) -
                              l * std::sin(M_PI - strada2.angle()) +
                              c.t() * l * std::sin(M_PI - strada2.angle()));
      window.draw(pallino);
    }
    for (car& c : strada3.carin()) {
      sf::CircleShape pallino(0.025 * radius);
      pallino.setFillColor(sf::Color::Blue);
      pallino.setOrigin(0.025 * radius, 0.025 * radius);
      pallino.setPosition(0.5 * display_height +
                              radius * std::cos(strada3.angle() + 0.0665682) -
                              l * std::cos(M_PI - strada3.angle()) +
                              c.t() * l * std::cos(M_PI - strada3.angle()),
                          0.5 * display_height -
                              radius * std::sin(strada3.angle() + 0.0665682) -
                              l * std::sin(M_PI - strada3.angle()) +
                              c.t() * l * std::sin(M_PI - strada3.angle()));
      window.draw(pallino);
    }

    for (car& c_ : strada1.carout()) {
      sf::CircleShape pallino_(0.025 * radius);
      pallino_.setFillColor(sf::Color::Blue);
      pallino_.setOrigin(0.025 * radius, 0.025 * radius);
      pallino_.setPosition(
          0.5 * display_height + radius * std::cos(strada1.angle()- 0.0665682) -
              l * std::cos(M_PI - strada1.angle()) +
              c_.t() * l * std::cos(M_PI - strada1.angle()),
          0.5 * display_height - radius * std::sin(strada1.angle()-0.0665682) -
              l * std::sin(M_PI - strada1.angle()) +
              c_.t() * l * std::sin(M_PI - strada1.angle()));
      window.draw(pallino_);
    }

    for (car& c_ : strada2.carout()) {
      sf::CircleShape pallino_(0.025 * radius);
      pallino_.setFillColor(sf::Color::Blue);
      pallino_.setOrigin(0.025 * radius, 0.025 * radius);
      pallino_.setPosition(
          0.5 * display_height + radius * std::cos(strada2.angle()- 0.0665682) -
              l * std::cos(M_PI - strada2.angle()) +
              c_.t() * l * std::cos(M_PI - strada2.angle()),
          0.5 * display_height - radius * std::sin(strada2.angle()- 0.0665682) -
              l * std::sin(M_PI - strada2.angle()) +
              c_.t() * l * std::sin(M_PI - strada2.angle()));
      window.draw(pallino_);
    }

    for (car& c_ : strada3.carout()) {
      sf::CircleShape pallino_(0.025 * radius);
      pallino_.setFillColor(sf::Color::Blue);
      pallino_.setOrigin(0.025 * radius, 0.025 * radius);
      pallino_.setPosition(
          0.5 * display_height + radius * std::cos(strada3.angle()- 0.0665682) -
              l * std::cos(M_PI - strada3.angle()) +
              c_.t() * l * std::cos(M_PI - strada3.angle()),
          0.5 * display_height - radius * std::sin(strada3.angle()- 0.0665682) -
              l * std::sin(M_PI - strada3.angle()) +
              c_.t() * l * std::sin(M_PI - strada3.angle()));
      window.draw(pallino_);
    }

    for (car& d : rotonda.carrbout()) {
      sf::CircleShape pallinos(
          0.025 * radius);  // controllo sovrapposizione pallini eliminati
      pallinos.setFillColor(sf::Color::Red);
      pallinos.setOrigin(0.025 * radius, 0.025 * radius);
      pallinos.setPosition(0.5 * display_height + radius * std::cos(d.theta()),
                           0.5 * display_height - radius * std::sin(d.theta()));
      window.draw(pallinos);
    }

    // questo comando termina il game loop.
    i++;
    window.display();
  }
}
