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

#ifndef PUREMETAL_POLYNOMIALPOSTPROCESSOR_HPP
#define PUREMETAL_POLYNOMIALPOSTPROCESSOR_HPP

#include <string>

#include "postprocessor.hpp"
#include "polynomialinterpolant.hpp"

namespace PureMetal
{

class PolynomialPostProcessor : public PostProcessor
{
    PolynomialPostProcessor ( const PolynomialPostProcessor & other ) = delete;
    PolynomialPostProcessor & operator= ( const PolynomialPostProcessor & other ) = delete;
    bool operator== ( const PolynomialPostProcessor & other ) const = delete;

public:
    inline PolynomialPostProcessor ( const double & r0, const std::string & path, const std::string & name, const bool & restart );
    ~PolynomialPostProcessor() = default;

    inline Interpolant * create_interpolant ( const double * x, const double * y, unsigned n ) override;
};

PolynomialPostProcessor::PolynomialPostProcessor ( const double & r0, const std::string & path, const std::string & name, const bool & restart )
    : PostProcessor ( r0, path, name, restart ) {}

Interpolant * PolynomialPostProcessor::create_interpolant ( const double * x, const double * y, unsigned int n )
{
    return new PolynomialInterpolant ( x, y, n );
}

}

#endif // PUREMETAL_POLYNOMIALPOSTPROCESSOR_HPP
