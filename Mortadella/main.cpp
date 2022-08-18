#include"road.hpp"

#include<iostream>
#include<cmath>
#include<SFML/Graphics.hpp>



int main() {
  unsigned const display_height = .8 * sf::VideoMode::getDesktopMode().height;
  const int radius = .25 * display_height;
 
  if (n_street() <1) {
     std::runtime_error {"Il numero di strade deve essere maggiore di uno"}; //non funziona
  }
  int const fps = 150;
  //std::srand(time(NULL));

  std::vector<road> roads;
  for (int k = 0; k < n_street(); k++) {
    road strada_test(2 * k * M_PI / n_street(), 0);
    roads.push_back(strada_test);
  }

  rbout rotonda(radius);

  sf::RenderWindow window(sf::VideoMode(display_height, display_height),
                          "Roundabout", sf::Style::Default);
  window.setFramerateLimit(fps);
  sf::Texture texture;
  if (!texture.loadFromFile(
          "texture.jpg", sf::IntRect(0, 0, display_height, display_height))) {
    throw std::runtime_error {"Attenzione! L'immagine non è stata caricata correttamente. "
                 "Controlla che il file esista e sia nella giusta working "
                 "directory"};
  }
  texture.setSmooth(true);

  sf::CircleShape cerchio1(1.1 * radius);
  sf::CircleShape cerchio2(0.9 * radius);
  cerchio1.setTexture(&texture);
  cerchio1.setPosition(.5 * display_height - 1.1 * radius,
                       .5 * display_height - 1.1 * radius);
  cerchio2.setPosition(.5 * display_height - .9 * radius,
                       .5 * display_height - .9 * radius);
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    window.clear(sf::Color::White);
    window.draw(cerchio1);
    window.draw(cerchio2);
    float l = 1 * radius;
    for (int i = 1; i <= n_street(); i++) {
      float sfasorad = ((2 * M_PI) / n_street()) * i;
      sf::RectangleShape strada(sf::Vector2f(l, 0.2 * radius));
      strada.setOrigin(0.5 * l, 0.5 * 0.2 * radius);
      strada.setTexture(&texture);
      strada.setPosition(
          .5 * display_height + (std::cos(sfasorad)) * (radius + 0.5 * l),
          0.5 * display_height - (std::sin(sfasorad)) * (radius + 0.5 * l));
      strada.rotate(-sfasorad * (180 / M_PI));
      window.draw(strada);
    }
    for (auto it = roads.begin(); it != roads.end(); ++it) {
      (*it).newcar_rd(true);
      (*it).evolve_rd(true);
      if ((*it).transfer_rd(rotonda)) {
        rotonda.newcar_rbt((*it).angle());
      }
      (*it).erase_rd();
      rotonda.evolve_rbt();
      if (rotonda.transfer_rbt() > 0) {
        auto a = rotonda.transfer_rbt();
        (roads[a - 1]).newcar_rd(false);
      }
      rotonda.erase_rbt();
      (*it).evolve_rd(false);

      for (car& c : (*it).carin()) {
        sf::CircleShape pallino(0.025 * radius);
        pallino.setFillColor(sf::Color::Blue);
        pallino.setOrigin(0.025 * radius, 0.025 * radius);
        pallino.setPosition(0.5 * display_height +
                                radius * std::cos((*it).angle() + 0.0665682) -
                                l * std::cos(M_PI - (*it).angle()) +
                                c.t() * l * std::cos(M_PI - (*it).angle()),
                            0.5 * display_height -
                                radius * std::sin((*it).angle() + 0.0665682) -
                                l * std::sin(M_PI - (*it).angle()) +
                                c.t() * l * std::sin(M_PI - (*it).angle()));
        window.draw(pallino);
      }
      for (car& c_ : (*it).carout()) {
        sf::CircleShape pallino_(0.025 * radius);
        pallino_.setFillColor(sf::Color::Blue);
        pallino_.setOrigin(0.025 * radius, 0.025 * radius);
        pallino_.setPosition(0.5 * display_height +
                                 radius * std::cos((*it).angle() - 0.0665682) -
                                 l * std::cos(M_PI - (*it).angle()) +
                                 c_.t() * l * std::cos(M_PI - (*it).angle()),
                             0.5 * display_height -
                                 radius * std::sin((*it).angle() - 0.0665682) -
                                 l * std::sin(M_PI - (*it).angle()) +
                                 c_.t() * l * std::sin(M_PI - (*it).angle()));
        window.draw(pallino_);
      }
      for (car& d : rotonda.carrbout()) {
        sf::CircleShape pallinos(0.025 * radius);
        pallinos.setFillColor(sf::Color::Red);
        pallinos.setOrigin(0.025 * radius, 0.025 * radius);
        pallinos.setPosition(
            0.5 * display_height + radius * std::cos(d.theta()),
            0.5 * display_height - radius * std::sin(d.theta()));
        window.draw(pallinos);
      }
    }
    // questo comando termina il game loop.
    window.display();
    //std::cout << rotonda.size_rbout();
  }
}