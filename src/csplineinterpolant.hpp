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

#ifndef PUREMETAL_CSPLINEINTERPOLANT_HPP
#define PUREMETAL_CSPLINEINTERPOLANT_HPP

#include "interpolant.hpp"

#include <gsl/gsl_math.h>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>

namespace PureMetal
{

class CSPLineInterpolant : public Interpolant
{
    CSPLineInterpolant(const CSPLineInterpolant& other) = delete;
    CSPLineInterpolant& operator=(const CSPLineInterpolant& other) = delete;
    bool operator==(const CSPLineInterpolant& other) const = delete;

    gsl_interp_accel * acc;
    gsl_spline * spline;
    gsl_function_fdf *fun;

    static double f ( double x, void * params ) {
        CSPLineInterpolant * spl = static_cast<CSPLineInterpolant *> ( params );
        return ( *spl ) ( x );
    }
    static double df ( double x, void * params ) {
        CSPLineInterpolant * spl = static_cast<CSPLineInterpolant *> ( params );
        return spl->derivative ( x );
    }
    static void fdf ( double x, void * params, double * f, double * df )  {
        CSPLineInterpolant * spl = static_cast<CSPLineInterpolant *> ( params );
        *f = ( *spl ) ( x );
        *df = spl->derivative ( x );
    }

public:
    CSPLineInterpolant( const double * x, const double * y, int n );
    ~CSPLineInterpolant();

    double root ( const double & min, const double & max ) override;
    double operator () ( const double & x ) override;
    inline double val0 () override;
    double derivative ( const double & x ) override;
    inline double derivative0 () override;
};

}

double PureMetal::CSPLineInterpolant::val0()
{
    return ( *this ) ( 0. );
}
double PureMetal::CSPLineInterpolant::derivative0()
{
    return derivative ( 0. );
}

#endif // PUREMETAL_CSPLINEINTERPOLANT_HPP
