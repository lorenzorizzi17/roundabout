#include <random>
#include<vector>
#include <cmath>

int n_roads;
std::default_random_engine gen;
std::binomial_distribution<int> binomial(0.5); //è stato un tentativo per mettere gli ingressi ma mi sa che devo essere un po' più specifica (devo fare come in root il set seed o una cosa analoga?)
std::uniform_int_distribution<int> out(1, n_roads); //poi la potremo cambiare con qualsiasi distribuzione preferiamo, potrei sempre aver sbagliato la sintassi obv


class car
{
  private:
    double r_;
    double theta_;
    double omega_;
    int exit_;
  public:
    car(double r = 0., double theta = 0., double omega = 0., int exit = out(gen)) : r_{r}, theta_{theta}, omega_{omega}, exit_{exit} {}

};

class rbout 
{
  private:
    double radius_;
    std::vector<car> car_rbout;
  public:
    rbout(double rad = 0.) : radius_{rad} {}

    /*vedi sotto*/

};

class road 
{
  private:
    double lenght_;
    auto angle = (2*M_PI)/n_roads;
    int entrance_flow_;
    std::vector<car> car_in;
    std::vector<car> car_out;
  public:
    road(double len = rbout::rad, int in_f = binomial(gen)) : lenght_{len}, entrance_flow_{in_f} {}

    /*qui ci saranno solo le dichiarazioni delle funzioni membro 
    (se non erro) mentre le definizioni saranno da un'altra parte
    in un altro file cpp (mi sono basata su vaghi e lontani ricordi
    del lab dei ppses)*/

};
