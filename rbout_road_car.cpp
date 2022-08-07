#include <random>
#include <vector>
#include <cmath>

int n_roads; //saranno quattro poi di base, Simo è rompiballe (se lo dice da solo)
double v_max_rbout;
double v_max_road;

double exit_angle() //funzione che mi dà l'uscita espressa come angolo
{
  std::random_device seed_1; //è il seed siamo felici
  std::default_random_engine random_1(seed_1());
  std::uniform_int_distribution<int> numbers(1, n_roads);
  int x = numbers(random_1);
  double y = (2 * M_PI / n_roads) * x;
  return y;
}


class car
{
  private:
    double r_;
    double theta_;
    double v_; //nel costruttire poi andrebbe chiamata la tangente nel caso si volesse fare
    double exit_; //vedi dopo
  public:
    car(double r = 0., double theta = 0., double v = 0., double exit = exit_angle()) : r_{r}, theta_{theta}, v_{v}, exit_{exit} {}

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
    double angle = (2 * M_PI)/n_roads;
    double entrance_par_;
    std::vector<car> car_in;
    std::vector<car> car_out;
  public:
    road(double len = 0., double en_par = 0.) : lenght_{len}, entrance_par_{en_par} {}

    void newcar(car const& C)
    {
      std::random_device seed_2;
      std::default_random_engine random_2(seed_2);
      std::uniform_real_distribution<double> parameters (0., 1.);
      double par = parameters(random_2);
      if (par < road.en_par) //non prende la variabile e non so come fare
      {
        car_in.push_back(C); //in fase di progettanzione ma ci siamo (direi)

      }
      
    }
    /*qui ci saranno solo le dichiarazioni delle funzioni membro 
    (se non erro) mentre le definizioni saranno da un'altra parte
    in un altro file cpp (mi sono basata su vaghi e lontani ricordi
    del lab dei ppses)*/

};
