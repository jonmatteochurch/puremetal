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

#ifndef PUREMETAL_POLYNOMIALINTERPOLANT_HPP
#define PUREMETAL_POLYNOMIALINTERPOLANT_HPP

#include "interpolant.hpp"

namespace PureMetal
{

class PolynomialInterpolant : public Interpolant
{
    PolynomialInterpolant(const PolynomialInterpolant& other) = delete;
    PolynomialInterpolant& operator=(const PolynomialInterpolant& other)= delete;
    bool operator==(const PolynomialInterpolant& other) const= delete;

    unsigned _n;
    double * _coefficients;

public:
    PolynomialInterpolant(const double * x, const double * y, int n);
    inline ~PolynomialInterpolant();

    double root ( const double & min, const double & max ) override;
    double operator () ( const double & x ) override;
    inline double val0 () override;
    double derivative ( const double & x ) override;
    inline double derivative0 () override;
};

}

PureMetal::PolynomialInterpolant::~PolynomialInterpolant()
{
    delete [] _coefficients;
}

double PureMetal::PolynomialInterpolant::val0()
{
    return _coefficients[0];
}

double PureMetal::PolynomialInterpolant::derivative0()
{
    return _coefficients[1];
}

#endif // PUREMETAL_POLYNOMIALINTERPOLANT_HPP
