#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
int main() {
  const int n{3};
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
    carprova1.setPosition(
        0.5 * display_height + radius * std::cos(2 * M_PI / n - 0.0665682) + l*t*std::cos(M_PI - (2*M_PI)/n),
        0.5 * display_height - radius * std::sin(2 * M_PI / n - 0.0665682)+ l*t*std::sin(M_PI - (2*M_PI)/n ));
    carprova2.setPosition(
        0.5 * display_height + radius * std::cos(2 * M_PI / n + 0.0665682)- l*std::cos(M_PI - (2*M_PI)/n) -t*l*std::cos(M_PI - (2*M_PI)/n),
        0.5 * display_height - radius * std::sin(2 * M_PI / n + 0.0665682)- l*std::sin(M_PI - (2*M_PI)/n) -t*l*std::sin(M_PI - (2*M_PI)/n));
    t = t - 0.01;
    window.draw(carprova1);
    window.draw(carprova2);
    // questo comando termina il game loop.
    window.display();
  }
  std::cout << t;
}
