
#include "hotokezaka.hpp"


int main(int argc, char* argv[])
{
    /*if(!std::filesystem::exists("interpolate_table.dat"))
    {
        create_file_for_interpolation file;
        file = create_file_for_interpolation();
        file.save_data_to_file();
    }
    Interpolator interpolate_a_time;
    interpolate_a_time.init("interpolate_table.dat");*/
    try{
        Read_In_Parameters readInParam;
        readInParam.read_parameter_file();

        Calculated_Numbers_Based_on_read_in_parameters calcParams(readInParam);
        //calcParams.init();

        /*for(double i = 0.0; i < 6000.0; i+=100.0)
        {
            std::cout << i << "\t" << calcParams.time_to_z(i) << std::endl;
        }*/

        //redshift to time test
        /*Time_redshift time_z = Time_redshift();
        for(double z=0; z < 4.5; z+= 0.1)
        {
            std::cout << "z = " << z << "\t time = " << time_z.z_to_time(z) << std::endl;
        }*/

        Create_events_and_calc_number_density calc(calcParams);
        calc.allEvent_number_densities();
        calc.allEvent_number_densities_new();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch(const char* e)
    {
        std::cerr << e << std::endl;
    }
    return 0;
}