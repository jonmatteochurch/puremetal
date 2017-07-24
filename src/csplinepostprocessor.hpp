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

#ifndef PUREMETAL_CSPLINEPOSTPROCESSOR_HPP
#define PUREMETAL_CSPLINEPOSTPROCESSOR_HPP

#include <string>

#include "postprocessor.hpp"
#include "csplineinterpolant.hpp"

namespace PureMetal
{

class CSPLinePostProcessor : public PostProcessor
{
    CSPLinePostProcessor ( const CSPLinePostProcessor & other ) = delete;
    CSPLinePostProcessor & operator= ( const CSPLinePostProcessor & other ) = delete;
    bool operator== ( const CSPLinePostProcessor & other ) const = delete;

public:
    inline CSPLinePostProcessor ( const double & r0, const std::string & path, const std::string & name, const bool & restart );
    ~CSPLinePostProcessor() = default;

    inline Interpolant * create_interpolant ( const double * x, const double * y, unsigned n ) override;
};

}

PureMetal::CSPLinePostProcessor::CSPLinePostProcessor ( const double & r0, const std::string & path, const std::string & name, const bool & restart )
    : PostProcessor ( r0, path, name, restart ) {}

PureMetal::Interpolant * PureMetal::CSPLinePostProcessor::create_interpolant ( const double * x, const double * y, unsigned int n )
{
    return new CSPLineInterpolant ( x, y, n );
}


#endif // PUREMETAL_CSPLINEPOSTPROCESSOR_HPP

