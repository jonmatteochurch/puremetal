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

#include "approximation.hpp"

#include "specifications.hpp"
#include "fulldomainapproximation.hpp"
#include "quarterdomainapproximation.hpp"

PureMetal::Approximation * PureMetal::Approximation::New ( const SimulationType & simulation_type, const double * upper, const double * lower, const double * spacing )
{
    switch ( simulation_type ) {
    case SimulationType::full :
        return new FullDomainApproximation ( upper, lower, spacing );
    case SimulationType::quadrant :
        return new QuarterDomainApproximation ( upper, spacing );
    default :
        return nullptr;
    };
}

