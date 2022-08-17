
#include"roundabout.hpp"
#include <SFML/Graphics.hpp>

void simulate(std::vector<road> roads, rbout rotonda, sf::RenderWindow& window){
    unsigned const display_height = .8 * sf::VideoMode::getDesktopMode().height;
    float l = 1 * rotonda.rad();
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
        sf::CircleShape pallino(0.025 * rotonda.rad());
        pallino.setFillColor(sf::Color::Blue);
        pallino.setOrigin(0.025 * rotonda.rad(), 0.025 * rotonda.rad());
        pallino.setPosition(0.5 * display_height +
                                rotonda.rad() * std::cos((*it).angle() + 0.0665682) -
                                l * std::cos(M_PI - (*it).angle()) +
                                c.t() * l * std::cos(M_PI - (*it).angle()),
                            0.5 * display_height -
                                rotonda.rad() * std::sin((*it).angle() + 0.0665682) -
                                l * std::sin(M_PI - (*it).angle()) +
                                c.t() * l * std::sin(M_PI - (*it).angle()));
        window.draw(pallino);
      }
      for (car& c_ : (*it).carout()) {
        sf::CircleShape pallino_(0.025 * rotonda.rad());
        pallino_.setFillColor(sf::Color::Blue);
        pallino_.setOrigin(0.025 * rotonda.rad(), 0.025 * rotonda.rad());
        pallino_.setPosition(0.5 * display_height +
                                 rotonda.rad() * std::cos((*it).angle() - 0.0665682) -
                                 l * std::cos(M_PI - (*it).angle()) +
                                 c_.t() * l * std::cos(M_PI - (*it).angle()),
                             0.5 * display_height -
                                 rotonda.rad() * std::sin((*it).angle() - 0.0665682) -
                                 l * std::sin(M_PI - (*it).angle()) +
                                 c_.t() * l * std::sin(M_PI - (*it).angle()));
        window.draw(pallino_);
      }
      for (car& d : rotonda.carrbout()) {
        sf::CircleShape pallinos(0.025 * rotonda.rad());
        pallinos.setFillColor(sf::Color::Red);
        pallinos.setOrigin(0.025 * rotonda.rad(), 0.025 * rotonda.rad());
        pallinos.setPosition(
            0.5 * display_height + rotonda.rad() * std::cos(d.theta()),
            0.5 * display_height - rotonda.rad() * std::sin(d.theta()));
        window.draw(pallinos);
      }
    }
}