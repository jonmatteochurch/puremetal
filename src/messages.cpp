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

#include "messages.hpp"

void PureMetal::usage ( std::ostream & os )
{
    os << "Usage: pure_metal [--restart] <input>.xml" << std::endl;
}

void PureMetal::parse_error ( std::ostream & os, const std::string & file )
{
    os << "Unable to parse input file : " << file << std::endl;
}

void PureMetal::restart_error ( std::ostream & os )
{
    os << "Unable to restart simulation" << std::endl;
}

void PureMetal::stability_error ( std::ostream & os )
{
    os << "Simulation is unstable" << std::endl;
}
