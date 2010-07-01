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
#include "TestSignalGenerator.hpp"

#include "test_bench.hpp"
#include "Stimulus.hpp"
#include "Recorder.hpp"

using namespace std;
using boost::lexical_cast;

int main()
{
   // constants
   const double TIME_RESOLUTION = 1.0;
   const double TOTAL_SIMULATION_TIME = 50000.0;
   const double CLOCK_PERIOD = 2.0;
   const int SAMPLE_SIZE=1024;

   // signal definition
   sc_signal< testbench::bit_type > reset_signal;
   sc_signal< testbench::data_sample_type > output_signal;
   sc_signal< bool > clock_signal;

   // set time parameters
   sc_set_time_resolution( TIME_RESOLUTION , SC_NS );
   sc_time simulation_time(TOTAL_SIMULATION_TIME,SC_NS);
   sc_time clock_time(CLOCK_PERIOD,SC_NS);

   // signal stimulus
   Stimulus stimulus( "stimulus" );
   stimulus.reset( reset_signal );

   // test signal generator
   TestSignalGenerator< testbench::BIT_WIDTH > 
      tsg( "TestSignalGenerator", SAMPLE_SIZE );
   tsg.reset( reset_signal );
   tsg.clock( stimulus.clock );
   tsg.output( output_signal );

   // output recorder
   Recorder record( "record" );
   record.input( output_signal );
   record.clock( stimulus.clock );

   // begin simulation
   sc_start( simulation_time );
}