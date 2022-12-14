#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "road.hpp"

int main() {
  unsigned const display_height = 0.8 * sf::VideoMode::getDesktopMode().height;
  const double radius = 0.25 * display_height;

  rbout roundabout(5, radius);
  int const fps = 150;

  std::vector<road> roads;
  for (int k = 0; k < roundabout.n_roads(); k++) {
    road strada_test(2 * k * M_PI / roundabout.n_roads());
    roads.push_back(strada_test);
  }

  sf::RenderWindow window(sf::VideoMode(display_height, display_height),
                          "Roundabout", sf::Style::Default);
  window.setFramerateLimit(fps);

  sf::Texture texture;
  if (!texture.loadFromFile(
          "texture.jpg", sf::IntRect(0, 0, display_height, display_height))) {
    throw std::runtime_error{
        "Attenzione! L'immagine non è stata caricata correttamente. "
        "Controlla che il file esista e sia nella giusta working "
        "directory"};
  }
  texture.setSmooth(true);

  sf::Texture texture2;
  if (!texture2.loadFromFile(
          "texture2.jpg", sf::IntRect(0, 0, 0, 0))) {
    throw std::runtime_error{
        "Attenzione! L'immagine non è stata caricata correttamente. "
        "Controlla che il file esista e sia nella giusta working "
        "directory"};
  }
  texture2.setSmooth(true);

  sf::CircleShape cerchio1(1.1 * radius);
  sf::CircleShape cerchio2(0.9 * radius);
  cerchio1.setTexture(&texture2);
  cerchio1.setPosition(0.5 * display_height - 1.1 * radius,
                       0.5 * display_height - 1.1 * radius);
  cerchio2.setPosition(0.5 * display_height - 0.9 * radius,
                       0.5 * display_height - 0.9 * radius);
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
    float l = 1*radius;
    for (int i = 1; i <= roundabout.n_roads(); i++) {
      float sfasorad = ((2 * M_PI) / roundabout.n_roads()) * i;  // radianti
      sf::RectangleShape strada(sf::Vector2f(l, 0.2 * radius));
      strada.setOrigin(0.5 * l, 0.5 * 0.2 * radius);
      strada.setTexture(&texture);
      strada.setPosition(
          0.5 * display_height + (std::cos(sfasorad)) * (radius +0.5*l),
          0.5 * display_height - (std::sin(sfasorad)) * (radius +0.5*l));
      strada.rotate(-sfasorad * (180 / M_PI));  // in gradi
      window.draw(strada);
    }

    for (auto it = roads.begin(); it != roads.end(); ++it) {
      (*it).newcar_rd(true);
      (*it).evolve_rd(true, roundabout);
      if ((*it).transfer_rd()) {
        roundabout.newcar_rbt((*it).angle());
      }
      (*it).erase_rd();
      roundabout.evolve_rbt((l)/(radius));
      if (roundabout.transfer_rbt() > 0) {
        auto a = roundabout.transfer_rbt();
        (roads[a - 1]).newcar_rd(false);
      }
      roundabout.erase_rbt();
      (*it).evolve_rd(false, roundabout);

      for (car& c : (*it).carin()) {
        sf::CircleShape pallino(0.006*display_height);
        pallino.setFillColor(sf::Color::Blue);
        pallino.setOrigin(0.006*display_height, 0.006*display_height);
        pallino.setPosition(
            0.5 * display_height +
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
        sf::CircleShape pallino_(0.006*display_height);
        pallino_.setFillColor(sf::Color::Blue);
        pallino_.setOrigin(0.006*display_height, 0.006*display_height);
        pallino_.setPosition(
            0.5 * display_height +
                radius * std::cos((*it).angle() - 0.0665682) -
                l * std::cos(M_PI - (*it).angle()) +
                c_.t() * l * std::cos(M_PI - (*it).angle()),
            0.5 * display_height -
                radius * std::sin((*it).angle() - 0.0665682) -
                l * std::sin(M_PI - (*it).angle()) +
                c_.t() 
                * l * std::sin(M_PI - (*it).angle()));
        window.draw(pallino_);
      }
      for (car& d : roundabout.carrbout()) {
        sf::CircleShape pallino__(0.025 * radius);
        pallino__.setFillColor(sf::Color::Red);
        pallino__.setOrigin(0.025 * radius, 0.025 * radius);
        pallino__.setPosition(
            0.5 * display_height + radius * std::cos(d.theta()),
            0.5 * display_height - radius * std::sin(d.theta()));
        window.draw(pallino__);
      }
    }
    window.display();
  }
}