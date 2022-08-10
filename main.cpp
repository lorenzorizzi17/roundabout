#include "rbout_road_car_08_10.cpp"

#include <vector>
#include <cmath>
#include <iomanip>
#include <random>

/*std::random_device seed; //è il seed siamo felici
std::default_random_engine randomize(seed());*/
std::default_random_engine randomize;


/*double define_road_angle()
{
    for (int i = 0.; i <= n_roads; i++)
    {
      double angle = i * ((2 * M_PI)/n_roads);
      return angle;
    }
}*/

int main()
{
    n_roads = 4;
    v_max_rbout = 40./3.6; //ho fatto la conversione in m/s, se ho sbagliato rido
    v_max_road = 30./3.6;

    rbout RTD(20.);
    std::vector<road> v_roads(n_roads);
    for( int i = 0; i < n_roads; i++)
    {
        std::uniform_real_distribution<double> entr_par (0., 0.7); //perché se en_par è 1 mi spawna sempre macchine
        double n = entr_par(randomize); //parametro di entrata rispetto a cui spawna le macchine o meno
        v_roads[i] = (i * ((2 * M_PI)/n_roads), n);
    }


}