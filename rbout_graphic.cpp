#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
int main() {
  const int n{5};
  unsigned const display_height = .8 * sf::VideoMode::getDesktopMode().height;
  const int radius = .25 * display_height;
  // qui creo l'oggetto finestra e l'oggetto texture
  sf::RenderWindow window(sf::VideoMode(display_height, display_height),
                          "Roundabout", sf::Style::Default);
  // il prossimo comando è per caricare la texture dell'asfalto. Perché il programma non esploda, devi avere nella working directory un file texture.jpg
  sf::Texture texture;
  // questo if dà errore se la texture non è stata caricata correttamente
  if (!texture.loadFromFile(
          "texture.jpg", sf::IntRect(0, 0, display_height, display_height))) {
    std::cout << "Attenzione! L'immagine non è stata caricata correttamente. "
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
  while (window.isOpen()) {
    // questo while rende la finestra sensibile (altrimenti non si chiuderebbe)
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

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

    // qui va il codice non grafico dell'int main dove disegneremo le macchine ...
    //...
    //...
    //...

    //questo comando termina il game loop.
    window.display();
  }
}
