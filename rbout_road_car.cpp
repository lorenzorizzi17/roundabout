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

double calc_angle(double m, double n) //prima la y poi la x per come è definita atan2
        {
          double alpha = atan2(m, n); //ci sono diverse arcotangenti ho visto, ne ho messa una a caso dipende il range che prendono in caso va fatta una modifica
          if (alpha < 0)
          {
            alpha += 2 * M_PI;
          }
          return alpha;
        }


class car
{
  private:
    double r_;
    double theta_;
    double v_; //nel costruttire poi andrebbe chiamata la tangente nel caso si volesse fare
    double exit_ = exit_angle(); //vedi sopra
  public:
    car(double r = 0., double theta = 0., double v = 0.) : r_{r}, theta_{theta}, v_{v} {} 
    /* in realtà potenzialmente car(double r = 0., double theta =0.) 
    e la v_/v la tratto come l'angolo relativo alla strada per evitare
    lo sbattimento di inizializzare pure la velocità quando uso la funzione 
    newcar() vedi sotto */
    double r() const
    {
      return r_;
    }
    double theta() const
    {
      return theta_;
    }
    double v()
    {
      return v_;
    }

};

class rbout 
{
  private:
    double radius_;
    std::vector<car> car_rbout;
  public:
    rbout(double rad = 0.) : radius_{rad} {}
    double rad()
    {
      return radius_;
    }

    std::size_t size_rbout() const
    {
      return car_rbout.size();
    }

    bool empty_rbout() const
    {
      return car_rbout.empty();
    }
    /*vedi sotto*/

};

class road 
{
  private:
    double lenght_; //se si volesse 
    double angle_ = (2 * M_PI)/n_roads;
    double entrance_par_;
    std::vector<car> car_in;
    std::vector<car> car_out;
  public:
    road(double len = 0., double en_par = 0.) : lenght_{len}, entrance_par_{en_par} {}
    double len()
    {
      return lenght_;
    }
    double angle()
    {
      return angle_;
    }
    double en_par()
    {
      return entrance_par_;
    }

    void newcar(rbout RB, road RD) //ho sicuro un problema con le coordinate nelle strade che non siano a 0 radianti
    {
      std::random_device seed_2;
      std::default_random_engine random_2(seed_2);
      std::uniform_real_distribution<double> parameters (0., 1.);
      double par = parameters(random_2);
      
      if (par < RD.en_par())
      {
        double x = RB.rad() * cos(0.05) + RD.len() * cos(RD.angle()); //da sistemare perché ho dei problemi
        double y = RB.rad() * sin(0.05) + RD.len() * sin(RD.angle());
        double angle = calc_angle(y, x);
        car C(sqrt(x*x + y*y), angle, v_max_road);  
        car_in.push_back(C); //in fase di progettanzione ma ci siamo (direi)
      }
    }

    std::size_t size_in() const
    {
      return car_in.size();
    }

    std::size_t size_out() const
    {
      return car_out.size();
    }

    bool empty_in() const
    {
      return car_in.empty();
    }

    bool empty_out() const
    {
      return car_out.empty();
    }

    void evolve_in(double dt); //da implementare

    void evolve_out(double dt); //da implementare


    /*qui ci saranno solo le dichiarazioni delle funzioni membro 
    (se non erro) mentre le definizioni saranno da un'altra parte
    in un altro file cpp (mi sono basata su vaghi e lontani ricordi
    del lab dei ppses)*/

};
