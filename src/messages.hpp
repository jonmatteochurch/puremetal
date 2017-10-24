/*
 * PureMetal - A simple program for pure metal phase field simulations.
 * Copyright (C) 2017  Jon Matteo Church scjmc@leeds.ac.uk
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef PUREMETAL_MESSAGES_HPP
#define PUREMETAL_MESSAGES_HPP

#include <string>
#include <ostream>
#include <iomanip>
#include <iostream>
#include <cmath>

namespace PureMetal
{

const std::string options_number_error_msg = "Wrong number of options in input ";
const std::string unknown_option_msg = "Unknown option: ";
const std::string unknown_symulation_type_msg = "Unknown SimulationComponent type: ";
const std::string negative_upper_grid_msg = "Negative upper bound in grid: ";
const std::string unknown_time_type_msg = "Unknown Time type: ";
const std::string unknown_save_label_msg = "Unknown save label: ";
const std::string output_dir_error_msg = "Cannot create output directory " ;

void usage ( std::ostream & os );
void parse_error ( std::ostream & os, const std::string & file );
void restart_error ( std::ostream & os );
void stability_error ( std::ostream & os );
inline void fixed_progress_info ( std::ostream & os, const double & progress );
inline void stable_progress_info ( std::ostream & os, const double & delt );
inline void steady_state_progress_info ( std::ostream & os, const bool & next_cell, const double & t, const double & v0, const double & v );
    
}

inline void PureMetal::fixed_progress_info ( std::ostream & os, const double & progress )
{
    os << std::setprecision ( 2 ) << std::fixed << "Progress: ";
    os.width ( 6 );
    os << progress * 100. << "%" << std::endl;
}

inline void PureMetal::stable_progress_info ( std::ostream & os, const double & delt )
{
    os << "Stability check for delt: ";
    os << std::scientific << std::setprecision ( 5 ) << delt << std::endl;
}

inline void PureMetal::steady_state_progress_info ( std::ostream & os, const bool & next_cell, const double & t, const double & v0, const double & v )
{
    if ( next_cell ) {
        os.width ( 10 );
        os << "Time: " ;
        os.width ( 10 );
        os << t;
        os << "; Cell velocity: " ;
        os.width ( 10 );
        os << v;
        os << "; Delta: ";
        os.width ( 10 );
        os << std::abs ( ( v - v0 ) / v );
        os << std::endl;
    }
}

#endif // PUREMETAL_MESSAGES_HPP
