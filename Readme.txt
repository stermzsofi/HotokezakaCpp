*******************************************
******Compile the code*********************
*******************************************

First: compile the code
 - need to compile all files: 
 	 hotokezaka.cpp Linear_interpol/linear_interpol.cpp Trapezoidal_rule/trapezoidal.cpp hotkezakamain.cpp
 - You will need boost library (boost/random/laplace_distribution.hpp) (because of the laplace distribution random number to get the z position of the random event)
 - You will need at least c++17 version   
 - It was tested with flags: -Wall -Wextra -std=c++17 -O2, with g++ version 9.4.0
 
  The used files in the code:
  	- constant.hpp: only include some used constants (like Hubble constant, etc.)
  	- hotokezakamain.cpp: the main function of the code
  	- hotokezaka.cpp/hotokezaka.hpp: it contain all calculations, this is the base of the code
  	- Linear_interpol: the linear interpolation method (this is a directory/library)
  	- Trapezoidal_rule: trapezoidal integral method (this is a directory/library)
  	
  Other important files:
  	- parameters.txt: contain all input parameters (more details later in this document)
  	- interpolate_table.dat: binary file, contain data for linear interpolation during time and redshift exchange (if it does not exist the code will generate it)
  	
****************************************************  
************ The parameter file*********************
****************************************************

Parameter file
  - the name of the parameter and its value need to be separated with tabulator (TAB)
  List of parameters and their units:
  	- h_scale [kpc]: ISM scale height
  	- r_Sun [kpc]: Solar radius
  	- width [kpc]: Width of the solar circle
  	- simulation_Time [Myr]: Simulation time
  	- rd [kpc]: Radius scale for galaxy mass distribution
  	- r0_rate [number/Myr]: Present-time galactic event rate
  	- sampleDt [Myr]: set the time resolution of the result file
  	- alpha: mixing length parameter
  	- vt [km/s]: typical turbulent velocity
  	- tau [Myr]: mean life of the element
  	- number_of_runs: the number of calculated random examples
  	- rate_function: the used event rate function. Currently usable: Wanderman (Wanderman, D. & Piran, T., 2015) and Hopkins (Hopkins, A. M. & Beacom, 2006) and Constant (Constant rate density at all time)
  	- Ni_calc_method: set the method for calculation Ni, where Ni is the total number of atoms of isotope i ejected by each event. 
          Currently usable: Hotokezaka, Measurement, Number (for more details see Ni methods part of this readme)
  	- element_initial_prod_ratio: the initial production ratio of the element (compared to U238). only interesting if the Ni calculation method is Measurement
  	- stable_izotope: calculate the stable izotope or not (true or false)
  	- output: optional, you can set the name of the output file

*****************************************************
*****************Ni methods**************************
*****************************************************

There are 3 possible methods

- Hotokezaka
	This is N_Pu = 3.9540983606557375e+49, and it based on N_pu = (244Pu/238U)_O * N_238U from Hotokezaka, at the end of the Suppl. Mat. 
         This reproduces Hotokezaka's results for R0 = 5 Myr^-1 within a factor of 2, needs to be tested for R0 = 300 Myr^-1
	
- Based on today measurement
        here, the number of Pu (N_Pu) is extracted from the formula of the median number density <n_i>_m (eq. 3 in Hotokezaka paper):
	N_Pu = <n_i>_m/(rate_density_0 * tau_i * exp(-tau_mix/(2*tau_i)))
	using <n_i>_m = today value of number density from measurements. 

        In general for any other element/isotope the calibration above can be used as 
  	N_any = (any/238U)_0 * N_238U = (any/238U)_0 * N_pu * 1/(244Pu/238U)_0 if we derive N_238U 
	For this method we will need the initial production ratio of the element relative to U238 ((any/238U)_0) = element_initial_prod_rati

	
	(Note: this method could be useful if we did not know anything about the Ni value but the isotope come from the same type of event as Pu and U. The shape of the density evolution and the order of magnitude could be investigated by this.)
	
- Number
	In this case the N_i value is an input parameter. If you write only a number after Ni_calc_method in the parameter file, N_i will be this value.
	
*********************************************
*********** The output file******************
*********************************************

Default case, the name of the output file is results.dat
The structure of the file is the following:
	- First line: Times in Myr BP
	- Second line: The median number density calculated from eq 3. in Hotokezaka paper in kpc^-3
	- Other lines: Random examples	 
 
The file transpo.awk allows to reverse the rows and column (so that time is the first column instead of the first row) for easier plotting.
 
******************************************************************
**********Exchange between time and redshift**********************
******************************************************************

For this exchange I used the same equation as eq. 14 in Boylan-Kolchin, Michael and Weisz, Daniel R. 2021
The cosmological parameters come from the Planck 18 results.


