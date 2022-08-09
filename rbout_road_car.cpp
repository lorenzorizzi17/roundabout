#include <random>
#include <vector>
#include <cmath>

const int n_roads; //saranno quattro poi di base
double v_max_rbout;
double v_max_road;


double exit_angle() //funzione che mi dà l'uscita espressa come angolo
{
  std::random_device seed; //è il seed siamo felici
  std::default_random_engine random(seed());
  std::uniform_int_distribution<int> numbers(1, n_roads);
  int x = numbers(random);
  double y = (2 * M_PI / n_roads) * x;
  return y;
}

class car
{
  private:
    double r_;
    double theta_;
    double t_;
    double exit_;
  public:
    car(double r = 0., double theta = 0., double t = 0., double exit = 0.) : r_{r}, theta_{theta}, t_{t}, exit_{exit} {} //car(double r = 0., double theta = 0., double v = 0., double exit) : r_{r}, theta_{theta}, v_{v}, exit_{exit} {}
    double r()
    {
      return r_;
    }
    double t()
    { 
      return t_;
    }
    double theta()
    {
      return theta_;
    }
    double exit()
    {
      return exit_;
    }
    void evolve_rd()
    {
      t_ += 0.01;
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
    
    void newcar_rbt(rbout RBT, road RD)
    {
        car C(RBT.rad(), RD.angle(), 0., exit_angle());
        car_rbout.push_back(C);
    }

    void evolve_rbt()
    {
        //da implementare
    }

    void transfer_rbt()
    {
        //da implementare
    }
};

class road 
{
  private:
    double angle_;
    double entrance_par_;
    std::vector<car> car_in;
    std::vector<car> car_out;
  public:
    road(double angle = 0., double en_par = 0.) : angle_{angle}, entrance_par_{en_par} {}
    double angle()
    {
      return angle_;
    }
    double en_par()
    {
      return entrance_par_;
    }
    std::vector<car> carin() 
    {
      return car_in; 
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

    void newcar_rd(bool input) //Lowro non ho capito cosa fa se false, cioè concettaualmente ho capito ma mi sto impallando e tra 7 min devo essere a lavoro, ho impostato solo il "true" anche se non è ancora un bool
    {
      if (input) {
        std::random_device seed__;
        std::default_random_engine random__(seed__);
        std::uniform_real_distribution<double> parameters (0., 1.);
        double par = parameters(random__);
      
        if (par < RD.en_par() && car_in.size() < 10 )
        {
          car C(0., 0., 0., exit_angle());  
          car_in.push_back(C);
         }
      } else {
        //qui ci va la parte quando è false, la faccio poi
      }
    }

    void evolve_rd() {
    for (car& c : car_in) {
      if (c.t() < 1) {
        c.evolve_rd();
      }
    }
  }

    void transfer_rd()
    {
        //da implementare
    }
};

double define_road_angle()
{
    for (int i = 0.; i <= n_roads; i++)
    {
      std::random_device seed_;
      std::default_random_engine random_(seed_);
      std::uniform_real_distribution<double> entr_par (0., 1.);
      double n =entr_par(random_);
      road RD(i * ((2 * M_PI)/n_roads), n);
    }
}

bool is_free(road RD)
{
    //da implementare
}

