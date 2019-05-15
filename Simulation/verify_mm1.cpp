//----- Include files ----------------------------------------------------------
#include <iostream>				// Needed for cout
#include <stdlib.h>             // Needed for exit() and rand()
#include <math.h>               // Needed for log()
#include <fstream>				// Needed for I/O file
#include <algorithm>			// Needed for max

using namespace std;

//----- Constants --------------------------------------------------------------
#define SIM_TIME   8.64e4       // Simulation time
#define GREEN	   8.64e4		// green light interval time
#define RED	   0.00		// red light interval time
#define ARR_TIME   3.00 	    // Mean time between arrivals in seconds
#define SERV_TIME  4.00	    // Mean service time in seconds

//----- Function prototypes ----------------------------------------------------
double rand_val(int seed);      // RNG for unif(0,1)
double exponential(double x);   // Generate exponential RV with mean x

//===== Main program ===========================================================
int main(void)
{
  ofstream output;
  ofstream output3;
  output.open ("output_verify_debug.txt");
  output3.open ("output_verify.txt");
  
  /*cout << "Simulation time = " << SIM_TIME << endl;
  cout << "Mean time between arrivals = " << ARR_TIME << endl;
  cout << "Mean service time = " << SERV_TIME << endl;
  cout << "Counter time for green = " << GREEN/100 << endl;
  cout << "Counter time for red = " << RED/100 << endl;
  output3 << " Simulation time = " << SIM_TIME << endl;
  output3 << " Mean time = " << ARR_TIME << endl;
  output3 << " Mean service time = " << SERV_TIME << endl;
  output3 << "Counter time for green = " << GREEN/100 << endl;
  output3 << "Counter time for red = " << RED/100 << endl;*/
   
  double w_tot = 0.0, l_tot = 0.0;
  int c_tot = 0;
  unsigned int Max = 0;
  
  double   end_time = SIM_TIME; // Total time to simulate
  double   Ta = ARR_TIME;       // Mean time between arrivals
  double   Ts = SERV_TIME;      // Mean service time
  double   sim_time = 0.0;          // Simulation time
  double   t1 = 0.0;            // Time for next event #1 (arrival)
  double   t2 = SIM_TIME;       // Time for next event #2 (departure)
  unsigned int n = 0;           // Number of customers in the system
  unsigned int c = 0;           // Number of service completions
  double   s = 0.0;             // Area of number of customers in system
  double   tn = sim_time;           // Variable for "last event time"
  double   l;                   // Mean number in the query server
  double   w;                   // Mean waiting time
  double   TL_red = RED;				// Traffic Light interval time for red light
  double   TL_green = GREEN;			// Traffic Light interval time for green light
  bool	   green = true;		// light condition

  // Seed the RNG
  int seed = (rand()%100 + time(NULL))%100;
  rand_val(seed);
  
  //initialize Traffic Light Timing
/*
  srand(seed);
  int roll = rand()%10;
  cout << "Roll = " << roll << endl;
  if (roll >5) 
  {
	green = false;
	TL_red 	= double(rand()%RED)/1000;
	TL_green = 20.00;
	cout << endl << "pertama RED dengan waktu sisa " << TL_red << " s" << endl;
  }
  else
  {
	TL_green = double(rand()%GREEN)/1000;
	TL_red = 60.00;
	cout << endl << "pertama GREEN dengan waktu sisa " << TL_green << " s" << endl;
  }*/
  
  output << "===================================================================" << endl;
  output << "debugging: " << endl << endl;
  // Main simulation loop
  while (sim_time < end_time)
  {
	if (green)
	{
		if (t1 < t2)                	// *** Event #1 (arrival) ***
		{
			sim_time = t1;
			TL_green -= (sim_time - tn);		// Update green time counter
			s = s + n * (sim_time - tn);  	// Update area under "s" curve
			n++;
			Max = max(n, Max);
			tn = sim_time;                	// tn = "last event time" for next event
			output << "t1 = " << t1 << ", TL_green = " << TL_green << \
			", n = " << n << ", sim_time = " << sim_time << endl;
			t1 = sim_time + exponential(1/Ta);
			if (n == 1)
				t2 = sim_time + exponential(1/Ts);
			if (TL_green < 0)
			{
				green = false;
				TL_red += TL_green;
				TL_green = 20.00;
			}
		}
		else                        // *** Event #2 (departure) ***
		{
			sim_time = t2;
			TL_green -= (sim_time - tn);		// update red time counter
			s = s + n * (sim_time - tn);  // Update area under "s" curve
			n--;
			tn = sim_time;                // tn = "last event time" for next event
			c++;                      // Increment number of completions
			output << "t2 = " << t2 << ", TL_green = " << TL_green << \
			", n = " << n << ", C = " << c << ", sim_time = " << sim_time << endl;
			if (n > 0)
				t2 = sim_time + exponential(1/Ts);
			else
				t2 = SIM_TIME;
			if (TL_green < (t2 - sim_time) && t2 != SIM_TIME)
			{
				if( t1 > (sim_time + TL_green))
				{
					green = false;
					TL_red += TL_green -(min(t1,t2)-sim_time);
					TL_green = 20.00;
				}
			}				
		}
	}
	else
	{	//only arrival event occurs
		sim_time = t1;
		TL_red -= (sim_time - tn);		// Update green time counter
		s = s + n * (sim_time - tn);  	// Update area under "s" curve
		n++;
		tn = sim_time;                	// tn = "last event time" for next event
		output << "t1 = " << t1 << ", TL_red = " << TL_red << \
		", n = " << n << ", sim_time = " << sim_time << endl;
		t1 = sim_time + exponential(1/Ta);
		if (TL_red < (t1-sim_time))
		{
			green = true;
			t2=sim_time+TL_red;
			TL_red = 60.00;
		}
	}
  }
		// End of simulation
	
  // Compute outputs
  l = s / sim_time;   // Compute mean number in system
  w = s / c;      // Compute mean time spent in the system
  /*w_tot += w; I'm gonna use this later
  l_tot += l;
  c_tot += c;*/
  cout << " Numb. of Completion		       Mean Numb. in System	      Mean System Time" << endl;
  output << "Numb. of Completion	  Mean Numb. in System	Mean System Time" << endl;
  // Output results
  cout << "Numb. of completion = " << c << endl << "Mean Numb. in system = " << l << endl << \
  "Mean Response Time = " << w << endl;
  output3 << "Numb. of completion = " << c << endl << "Mean Numb. in system = " << l << endl << \
  "Mean Response Time = " << w << endl;
  
  cout << "Max number in queue= " << Max << endl;
  output3 << "Max number in queue= " << Max <<  endl;
  output.close();
  output3.close();
  return(0);
}

//=========================================================================
//= Multiplicative LCG for generating uniform(0.0, 1.0) random numbers    =
//=   - x_n = 7^5*x_(n-1)mod(2^31 - 1)                                    =
//=   - With x seeded to 1 the 10000th x value should be 1043618065       =
//=   - From R. Jain, "The Art of Computer Systems Performance Analysis," =
//=     John Wiley & Sons, 1991. (Page 443, Figure 26.2)                  =
//=   - Seed the RNG if seed > 0, return a unif(0,1) if seed == 0         =
//=========================================================================
double rand_val(int seed)
{
  const long  a =      16807;  // Multiplier
  const long  m = 2147483647;  // Modulus
  const long  q =     127773;  // m div a
  const long  r =       2836;  // m mod a
  static long x;               // Random int value (seed is set to 1)
  long        x_div_q;         // x divided by q
  long        x_mod_q;         // x modulo q
  long        x_new;           // New x value

  // Seed the RNG
  if (seed != 0) x = seed;

  // RNG using integer arithmetic
  x_div_q = x / q;
  x_mod_q = x % q;
  x_new = (a * x_mod_q) - (r * x_div_q);
  if (x_new > 0)
    x = x_new;
  else
    x = x_new + m;

  // Return a random value between 0.0 and 1.0
  return((double) x / m);
}

//==============================================================================
//=  Function to generate exponentially distributed RVs using inverse method   =
//=    - Input:  x (mean value of distribution)                                =
//=    - Output: Returns with exponential RV                                   =
//==============================================================================
double exponential(double x)
{
  double z;                     // Uniform random number from 0 to 1

  // Pull a uniform RV (0 < z < 1)
  do
  {
    z = rand_val(0);
  }
  while ((z == 0) || (z == 1));

  return(-x * log(z));
}
