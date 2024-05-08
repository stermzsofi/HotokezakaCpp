#ifndef HOTOKEZAKA
#define HOTOKEZAKA

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <filesystem>
#include <algorithm>
#include <random>
#include <regex>
//#include <boost/math/distributions/laplace.hpp>
#include <boost/random/laplace_distribution.hpp>
//#include <boost/math>
#include "Trapezoidal_rule/trapezoidal.hpp"
#include "Linear_interpol/linear_interpol.hpp"
#include "constant.hpp"

bool IsDouble(const std::string& s);
//void check_argument(int argc, char* argv);

struct Read_In_Parameters;
class Calculated_Numbers_Based_on_read_in_parameters;

class Time_redshift
{
    public:
        Time_redshift();
        double time_to_z(double t);
        double z_to_time(double z);
        double time_of_universe();
        //Interpolator time_to_z_interpolator;
    private:
        Interpolator time_to_z_interpolator;
        double Time_of_the_Universe;
};

/*************************************************************/
/********DIFFERENT RATE DENSITY FUNCTIONS ********************/
/*************************************************************/
class Rate_density : public Fx
{
    public:
        //virtual double get_rate_density_at_t(double t) = 0;
        virtual double get_rate_density_at_z(double z) = 0;
        virtual double get_rate_density_at_t(double t) = 0;
        void set_r0_rate_density_with_rate_density(double r0);
        virtual void calc_normalize_factor() = 0;
        double calc_Fx(double x){ return get_rate_density_at_t(x);}
    protected:
        double r0_rate_density;
        double normalize_factor;
};

    class WandermanRate : public Rate_density
    {
        public:
            WandermanRate(Time_redshift& t_z) : time_z(t_z){};
            //WandermanRate(double r0);
            double get_rate_density_at_z(double z);
            double get_rate_density_at_t(double t);
            //double calc_Fx(double x);
            void calc_normalize_factor();
            //void set_r0_rate_density_with_rate_density(double r0);
            //void set_r0_rate_density_with_rate(double r0);
        private:
            Time_redshift& time_z;
            //double r0_rate_density;
            //double normalize_factor;    //the rate function at z=0 must return r0 
            //valószínűleg őt is át lesz érdemes vinni Rate_density functionba + ott kell egy virtual 
            // calc_normalize_factor fv is
    };

    class HopkinsRate : public Rate_density
    {
        public:
            HopkinsRate(Time_redshift& t_z);
            double get_rate_density_at_z(double z);
            double get_rate_density_at_t(double t);
            void calc_normalize_factor();
        private:
            Time_redshift& time_z;
            double a,b,c,d,h;
    };

/*************************************************************/
/********DIFFERENT NI calculation methods ********************/
/*************************************************************/

class Ni_calculation
{
    public:
        Ni_calculation(Calculated_Numbers_Based_on_read_in_parameters& p): calc_param(p){};
        virtual double calculate_Ni() = 0;
    protected:
        Calculated_Numbers_Based_on_read_in_parameters& calc_param;
        //Read_In_Parameters& param;
};

    class Ni_Read_In : public Ni_calculation
    {
        public:
            Ni_Read_In(Calculated_Numbers_Based_on_read_in_parameters& p) : Ni_calculation(p){};
            double calculate_Ni();
    };

    class Ni_Measurement : public Ni_calculation
    {
        public:
            Ni_Measurement(Calculated_Numbers_Based_on_read_in_parameters& p) : Ni_calculation(p){};
            double calculate_Ni();
            void calculate_N_Pu();
        private:
            double N_Pu;
    };

    class Ni_Hotokezaka : public Ni_calculation
    {
        public:
            Ni_Hotokezaka(Calculated_Numbers_Based_on_read_in_parameters& p) : Ni_calculation(p){};
            double calculate_Ni();
    };

/*************************************************************/
/********Read in parameters struct ***************************/
/*************************************************************/

struct Read_In_Parameters
{
    Read_In_Parameters();
    //in inputEventGenerator.in at initial code
    double h_scale;                 //ISM scale height in kpc
    double r_Sun;                   //Solar radius in kpc
    double width;                   //Width of the solar circle in kpc
    double rd;                      //Radius scale for galaxy mass distribution in kpc    
    double simulation_Time;         //Simulation time in Myr
    double r0_rate;                 //Present-time galactic event rate in number/Myr
    double sampleDt;                //set the time resolution of the result file in Myr
    //in inputHotokezakaGalaxy.in at initial code
    double alpha;                   //mixing length parameter
    double vt;                      //typical turbulent velocity in km/s
    //I think we also will need the scaleheight of the disk - > in Andres method I do not need
    double zd;
    //in tauList.in at initial code
    //later could be a vector for more than one tau values
    double tau;                     //mean life of the element in [Myr]    
    //double Ni;      //inkább a calculated classba mégis
    //in intputEventGeneratator.in at initial code
    int number_of_runs;             //the number of calculated random examples
    std::string ni_calculation_method;
    std::string read_in_rate_function;
    std::unique_ptr<Rate_density> rate_function; //unique_ptr kell valószínűleg
    //std::unique_ptr<Ni_calculation> Ni_calc;    //inkább a calculated classba mégis

    //only need it if the ni calculation method is from measurement
    double element_initial_production_ratio;

    double Pu_initial_production_ratio = 0.4;   //currently nor read from file, later can be

    //for time redshift interpolations
    Time_redshift time_z;

    //output filename
    std::string out_file = "results.dat";

    void read_parameter_file(); //Read the values based on parameter file and run init function
    void init();
        //- create rate_function based in read_in_rate_function string
        // - run the set_r0_rate_density_with_rate_density function -> 
        //       not here, in Calculated_Numbers_Based_on_read_in_parameters class
};

/*************************************************************/
/********Calculated Numbers based on read in parameters*******/
/*************************************************************/

class Calculated_Numbers_Based_on_read_in_parameters
{
    public:
        Calculated_Numbers_Based_on_read_in_parameters(Read_In_Parameters& params/*, Time_redshift& t_z*/);
        double rate_to_rate_density(double R);
        double rate_density_to_rate(double R_density);
        void calculate_number_of_events();
        //double time_to_z(double t);
        //double z_to_time(double z);
        //void init_cumulative_distribution();
        double interpolate_random_number_to_time(double myrand);
        int get_number_of_events() {return number_of_events;}
        void init();
        Read_In_Parameters& param;
        double D;       //D=alpha*(vt/7)*(H/0.2)
        //in Myr
        double taumix;  //taumix = 300 (R0/10)^-2/5 * (alpha/0.1)^-3/5*(vt/7)^-3/5*(H/0.2)^-3/5
        double Ni;
    private:
        //Read_In_Parameters& param;
        Quad_Trapezoidal integral;
        std::unique_ptr<Ni_calculation> Ni_calc;
        //Quad_Trapezoidal integrator_for_cumulative_distribution;
        //Interpolator time_to_z_interpolator;
        //Time_redshift& time_z;
        Interpolator cumulative_distribution_interpolator;
        double M_star;
        double rho_star;
        double Time_of_the_Universe;
        double number_of_events_d;
        int number_of_events;
};

class Fx_for_timeintegral : public Fx
{
    public:
        double calc_Fx(double x);
};

class create_file_for_interpolation
{
    public:
        create_file_for_interpolation();
        void save_data_to_file();
    private:
        Fx_for_timeintegral fx_time;
        Quad_Trapezoidal integral;
        double a_step = 1e-4;   //ezt lehet érdemes lenne majd nagyobbra állítani memória miatt
                                //1e-3 also enough 
};

struct randomEvent
{
    double time;
    double x;
    double y;
    double z;
    double distance;
};

class Create_events_and_calc_number_density
{
    public:
        Create_events_and_calc_number_density(Calculated_Numbers_Based_on_read_in_parameters& calc_par);
        void allEvent_number_densities();
    private:
        //Read_In_Parameters& param;
        //std::random_device rd;
        //std::mt19937 mt;
        std::uniform_real_distribution<double> rand_number_0_1;
        boost::random::laplace_distribution<double> rand_laplace;
        Calculated_Numbers_Based_on_read_in_parameters& calculated_parameters;
        double delta_t_crit; //delta_t_crit = ((8*pi*H*D)^2)/((4*pi*D)^3)
                                //if delta_t >= delta_t_crit, than 8*pi*D*deltaj will be smaller
                                //other case: (4*pi*D*deltat)^3/2 will be smaller
        std::vector<double> sampling_time_points;
        std::vector<double> number_densites;
        std::vector<double> median_number_densities;
        randomEvent create_random_event();
        void calc_number_density_for_an_event();
        double calc_Kj(double delta_time);
        double const_for_Kj_1;
        double const_for_Kj_2;
        
};

#endif