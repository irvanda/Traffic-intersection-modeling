//----- Include files ----------------------------------------------------------
#include <iostream>				// Needed for cout
#include <stdlib.h>             // Needed for exit() and rand()
#include <math.h>               // Needed for log()
#include <fstream>				// Needed for I/O file
#include <algorithm>			// Needed for max and sort
#include <vector>				// Needed for vector


using namespace std;

bool b2s (int k,int l) { return (k>l); } 	// function for sorting vector from big to small

//----- Constants --------------------------------------------------------------
#define SIM_TIME   8.64e4       // Simulation time
#define GREEN	   20000		// green light interval time
#define RED	   	   60000		// red light interval time
#define ARR_TIME   6.00 	    // Mean time between arrivals in seconds
#define SERV_TIME  3.00	    // Mean service time in seconds

//----- Function prototypes ----------------------------------------------------
double rand_val(int seed);      // RNG for unif(0,1)
double exponential(double x);   // Generate exponential RV with mean x

//===== Main program ===========================================================
int main(void)
{
  ofstream output;
  ofstream output2;
  ofstream output3;
  output.open ("output_real_time_6_3.txt");
  output2.open ("output_debugging_6_3.txt");
  output3.open ("output_6_3.txt");
  
  cout << "Simulation time = " << SIM_TIME << endl;
  cout << "Mean time between arrivals = " << ARR_TIME << endl;
  cout << "Mean service time = " << SERV_TIME << endl;
  cout << "Counter time for green = " << GREEN/1000 << endl;
  cout << "Counter time for red = " << RED/1000 << endl;
  output3 << " Simulation time = " << SIM_TIME << endl;
  output3 << " Mean time = " << ARR_TIME << endl;
  output3 << " Mean service time = " << SERV_TIME << endl;
  output3 << "Counter time for green = " << GREEN/1000 << endl;
  output3 << "Counter time for red = " << RED/1000 << endl;
  
  vector<double> vehicles;
  
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
  double   Tidle = 0.0;			// idle time
  double   Tds;					// Time for departure scheduling
  
  //initialize time for departure become end of simulation
  vehicles.resize(0);

  // Seed the RNG
  int seed = (rand()%100 + time(NULL))%100;
  rand_val(seed);
  
  //initialize Traffic Light Timing
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
	vehicles.push_back(t2+1);
  }
  
  output << "===================================================================" << endl;
  output2 << "debugging: " << endl;
  output << "debugging: " << endl << endl;
  // Main simulation loop
  while (sim_time < end_time)
  {
	output2 << "debug 1 -- masuk while" << endl;
	if (green)
	{
		output2 << "debug 2 -- masuk green" << endl;
		if (t1 < vehicles.back())                	// *** Event #1 (arrival) ***
		{
			output2 << "debug 3 -- masuk arrival event" << endl;
			if (vehicles.back() == (SIM_TIME+1))
				vehicles.pop_back();	
			for (int i=0; i< vehicles.size();++i)
				output << vehicles[i] << "   ";
			output << endl;
			output2 << "debug 4 -- setelah resize" << endl;
			
			sim_time = t1;
			TL_green -= (sim_time - tn);		// Update green time counter
			s = s + n * (sim_time - tn);  	// Update area under "s" curve
			
			if (n==0) 
				Tidle += (sim_time - tn);
			
			n++;
			Max = max(n, Max);
			tn = sim_time;                	// tn = "last event time" for next event
			t1 = sim_time + exponential(Ta);
			output << "t1 = " << sim_time << ", TL_green = " << TL_green << ", n = " << n << \
			", next t1 = " << t1 ;
			output2 << "debug 5" << endl;
			if (n < 4)
			{
				t2 = sim_time + exponential(Ts/n);
				vehicles.push_back(t2);
				output2 << "debug 6" << endl;
				if(vehicles.size() > 1)
					sort (vehicles.begin(), vehicles.end(), b2s);
					output << ", next t2 " << vehicles.back() << endl;
			}
			else
				output << endl;
			if (TL_green < 0)
			{
				green = false;
				TL_red += TL_green;
				TL_green = 20.00;
				output2 << "debug 7 -- change green to red" << endl;
				vehicles.resize(0);
			}
		}
		else                        // *** Event #2 (departure) ***
		{
			for (int i=0; i< vehicles.size();++i)
				output << vehicles[i] << "   ";
			output << endl;
			output2 << "debug 8" << endl;
			
			sim_time = vehicles.back();
			TL_green -= (sim_time - tn);		// update red time counter
			s = s + n * (sim_time - tn);  // Update area under "s" curve
			
			n--;
			vehicles.pop_back();
			
			tn = sim_time;                // tn = "last event time" for next event
			c++;                      // Increment number of completions
			
			output << "t2 = " << sim_time << ", TL_green = " << TL_green << \
			", n = " << n << ", C = " << c ;
			output2 << "debug 9" << endl;
			if (n > 2 )
			{	
				output2 << "debug 10" << endl;
				Tds = sim_time + exponential(Ts/3);
				vehicles.push_back(Tds);
				if(vehicles.size() > 1)
					sort (vehicles.begin(), vehicles.end(), b2s);
			}
			else if (n == 0)
				vehicles.push_back(SIM_TIME+1);
			
			output2 << "debug 13 -- vector size --> "<< vehicles.size() << endl;
			output << ", next t2 " << vehicles.back() << endl;
			output2 << "debug 14" << endl;
				
			if (TL_green < (vehicles.back() - sim_time) && vehicles.back() != 86401)
			{
				output2 << "debug 15" << endl;
				if( t1 > (sim_time + TL_green))
				{
					green = false;
					TL_red += TL_green -(min(t1,vehicles.back())-sim_time);
					TL_green = 20.00;
					output2 << "debug 16 -- change green-red" << endl;
					vehicles.resize(0);
				}
			}						
		}
	}
	else
	{	//only arrival event occurs
		output2 << "debug 17" << endl;
		sim_time = t1;
		TL_red -= (sim_time - tn);		// Update green time counter
		s = s + n * (sim_time - tn);  	// Update area under "s" curve
		n++;
		tn = sim_time;                	// tn = "last event time" for next event
		t1 = sim_time + exponential(Ta);
		output << "t1 = " << sim_time << ", TL_red = " << TL_red << \
		", n = " << n << ", next t1 " << t1;
		if (TL_red < (t1-sim_time))
		{
			output2 << "debug 18 -- change red-green" << endl;
			green = true;
			if(vehicles.size()>0)
			{
				output << "wrong with vector" << endl;
				break;
			}
			t2=sim_time+TL_red;
			for(int i=0; i< 3; ++i)
				vehicles.push_back(t2);
			TL_red = 60.00;
			output << ", next t2 " << t2 << endl;
		}
		else 
			output << endl;
	}
  }
  output2 << "debug 19" << endl;
		// End of simulation
	
  // Compute outputs
  l = s / sim_time;   // Compute mean number in system
  w = s / c;      // Compute mean time spent in the system
  /*w_tot += w; I'm gonna use this later
  l_tot += l;
  c_tot += c;*/

  // Output results
  cout << "Numb. of completion = " << c << endl << "Mean Numb. in system = " << l << endl << \
  "Mean Response Time = " << w << endl;
  output3 << "Numb. of completion = " << c << endl << "Mean Numb. in system = " << l << endl << \
  "Mean Response Time = " << w << endl;
  
  cout << "Max number in queue= " << Max << ", vector size = " << vehicles.size() << "idle time " << Tidle <<  endl;
  output3 << "Max number in queue= " << Max << ", vector size = " << vehicles.size()<< "idle time " << Tidle<<   endl;
  output2 << "debug 20" << endl;
  output.close();
  output2.close();
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
