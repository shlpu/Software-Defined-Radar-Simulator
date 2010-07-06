// Copyright (c) 2010 Ryan Seal <rlseal -at- gmail.com>
//
// This file is part of Software Defined Radar Simulator (SDRS) Software.
//
// SDRS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//  
// SDRS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SDRS.  If not, see <http://www.gnu.org/licenses/>.
//#include <sdr_simulator/CicIntegrator.hpp>

#include <boost/lexical_cast.hpp>
#include <vector>
#include <sdr_simulator/util/SinusoidGenerator.hpp>

#include "test_bench.hpp"
#include <sdr_simulator/util/FileRecorder.hpp>
#include <sdr_simulator/util/Stimulus.hpp>

using namespace std;
using boost::lexical_cast;

int main()
{
   // constants
   const double TIME_RESOLUTION = 1.0;
   const double TOTAL_SIMULATION_TIME = 50000.0;
   const double CLOCK_PERIOD = 2.0;
   const double FREQUENCY = 14.0e6;
   const double SAMPLE_RATE = 64.0e6;
   const double NORMALIZED_FREQUENCY = FREQUENCY/SAMPLE_RATE;
   const unsigned int RESET_TIME = 10;
   const float AMPLITUDE = 0.125;
   const char* FILE_NAME = "output.dat";

   // signal definition
   sc_signal< testbench::bit_type > reset_signal;
   sc_signal< testbench::data_sample_type > output_signal;
   sc_signal< testbench::data_sample_type > null_signal;
   sc_signal< bool > clock_signal;

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_NS );
   sc_time simulation_time(TOTAL_SIMULATION_TIME,SC_NS);
   sc_time clock_time(CLOCK_PERIOD,SC_NS);
   sc_clock stimulus_clock( "stim_clock", clock_time);

   // signal stimulus
   Stimulus< testbench::data_sample_type > stimulus( "stimulus", RESET_TIME, 
         stimulus_clock 
         );
   stimulus.output( null_signal );

   // test signal generator
   SinusoidGenerator< testbench::BIT_WIDTH > 
      sg( "SinusoidGen", NORMALIZED_FREQUENCY, AMPLITUDE );
   sg.reset( stimulus.reset );
   sg.clock( stimulus.clock );
   sg.output( output_signal );

   // output recorder
   FileRecorder< testbench::data_sample_type> record( "record", FILE_NAME );
   record.input( output_signal );
   record.clock( stimulus.clock );

   // begin simulation
   sc_start( simulation_time );
}
