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

#include <tr1/math.h>
#include <sdr_simulator/PhaseAccumulator.hpp>
#include <sdr_simulator/util/FileRecorder.hpp>
#include <sdr_simulator/util/Stimulus.hpp>
#include <sdr_simulator/sc_timer.hpp>
#include <sdr_simulator/Types.hpp>

#include "TruncatedRecorder.hpp"

#include <iostream>
#include <boost/math/constants/constants.hpp>
#include <boost/lexical_cast.hpp>

// this file is auto-generated
#include "test_bench.hpp"

using namespace std;
using namespace boost::math;
using namespace boost;

int sc_main(int argc, char* argv[]){

   //TODO: Create a local xml file to read these and add 
   // to the existing header generator code.
   const double RESET_HOLD_TIME = 5;
   const string RECORDER_FILE_NAME = "accumulator_output.dat";
   const string TRUNC_RECORDER_FILE_NAME = "accumulator_trunc_output.dat";
   const double CLOCK_PERIOD = 15.68; //NS

   sc_signal< data_output_type > out_signal;
   sc_signal< data_output_type > null_signal;

   // define system clock 
   sc_clock clock("clk",sc_time(CLOCK_PERIOD,SC_NS));

   const double STEP_SIZE = tr1::pow(2.0, BIT_WIDTH*1.0 )*TUNING_FREQUENCY/SAMPLE_RATE;
   // determine phase step size from desired frequency

   // display settings
   cout 
      << "\nsample rate = " << SAMPLE_RATE 
      << "\nfrequency   = " << TUNING_FREQUENCY 
      << "\nbit width   = " << BIT_WIDTH 
      << "\nstep size   = " << STEP_SIZE
      << "\n\n";

   // define testbench stimulus
   Stimulus< data_output_type > stimulus( "stimulus", RESET_HOLD_TIME, clock);

   // In this test bench, we're not using the output signal, only 
   // clock and reset.
   stimulus.output( null_signal );

   // DUT
   PhaseAccumulator<data_output_type,reset_type> accumulator(
         "accumulator", STEP_SIZE );
   accumulator.clock( stimulus.clock );
   accumulator.out(out_signal);
   accumulator.reset( stimulus.reset );

   // record output
   FileRecorder< data_output_type > record( "recorder", RECORDER_FILE_NAME );
   record.clock( stimulus.clock );
   record.input( out_signal );

   TruncatedRecorder< data_output_type > t_record( "trunc_recorder", TRUNC_RECORDER_FILE_NAME );
   t_record.clock( stimulus.clock );
   t_record.input( out_signal );

   //run simulations for 22 nsec
   sc_start(sc_time(5000, SC_NS));

   return EXIT_SUCCESS;
}
