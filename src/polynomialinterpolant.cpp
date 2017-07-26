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

#include "polynomialinterpolant.hpp"

#include <cmath>
#include <gsl/gsl_poly.h>

PureMetal::PolynomialInterpolant::PolynomialInterpolant ( const double * x, const double * y, int n )
    : _n ( n )
    , _coefficients ( new double[n] )
{
    double phi, ff, b, *s;
    s = new double[_n];
    for ( unsigned i = 0; i < _n; ++i ) {
        s[i] = _coefficients[i] = 0.;
    }
    s[_n - 1] = -x[0];
    for ( unsigned i = 1; i < _n; ++i ) {
        for ( unsigned j = _n - 1 - i; j < _n - 1; ++j ) {
            s[j] -= x[i] * s[j + 1];
        }
        s[_n - 1] -= x[i];
    }
    for ( unsigned j = 0; j < _n; ++j ) {
        phi = _n;
        for ( unsigned k = _n - 1; k >= 1; --k ) {
            phi = k * s[k] + x[j] * phi;
        }
        ff = y[j] / phi;
        b = 1.;
        for ( unsigned k = _n; k -- > 0 ; ) {
            _coefficients[k] += b * ff;
            b = s[k] + x[j] * b;
        }
    }
    delete [] s;
}

double PureMetal::PolynomialInterpolant::root ( const double & min, const double & max )
{
    double * z = new double[2 * ( _n - 1 )];
    gsl_poly_complex_workspace * w = gsl_poly_complex_workspace_alloc ( _n );
    gsl_poly_complex_solve ( _coefficients, _n, w, z );
    gsl_poly_complex_workspace_free ( w );
    double res = NAN;
    for ( unsigned i = 0; i < _n - 1; ++i ) {
        const double & real = z[2 * i], & imag = z[2 * i + 1];
        if ( imag == 0 && min <= real && real <= max ) {
            res = real;
            break;
        }
    }
    delete[] z;
    return res;
}

double PureMetal::PolynomialInterpolant::operator() ( const double & x )
{
    return gsl_poly_eval ( _coefficients, _n, x );
}

double PureMetal::PolynomialInterpolant::derivative ( const double & x )
{
    double res[2];
    gsl_poly_eval_derivs ( _coefficients, _n, x, res, 2 );
    double der = res[1];
    return der;
}


