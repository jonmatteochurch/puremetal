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

#include "csplineinterpolant.hpp"

#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>

PureMetal::CSPLineInterpolant::CSPLineInterpolant(const double* x, const double* y, int n)
    : acc ( gsl_interp_accel_alloc() ),
      spline( gsl_spline_alloc ( gsl_interp_cspline, n ) ),
      fun ( new gsl_function_fdf() )
{
    fun->f = &CSPLineInterpolant::f;
    fun->df = &CSPLineInterpolant::df;
    fun->fdf = &CSPLineInterpolant::fdf;
    fun->params = this;
    gsl_spline_init ( spline, x, y, n );
}

PureMetal::CSPLineInterpolant::~CSPLineInterpolant()
{
    delete fun;
    gsl_spline_free ( spline );
    gsl_interp_accel_free ( acc );
}

double PureMetal::CSPLineInterpolant::root(const double& min, const double& max)
{
    int status, iter = 0, max_iter = 100;
    double x0, x = ( min + max ) / 2;
    const gsl_root_fdfsolver_type * T = gsl_root_fdfsolver_steffenson;
    gsl_root_fdfsolver * s = gsl_root_fdfsolver_alloc ( T );
    gsl_root_fdfsolver_set ( s, fun, x );
    do {
        iter++;
        status = gsl_root_fdfsolver_iterate ( s );
        x0 = x;
        x = gsl_root_fdfsolver_root ( s );
        status = gsl_root_test_delta ( x, x0, 0, 1e-6 );
    } while ( status == GSL_CONTINUE && iter < max_iter );
    gsl_root_fdfsolver_free(s);
    return x;
}

double PureMetal::CSPLineInterpolant::operator()(const double& x)
{
    return gsl_spline_eval ( spline, x, acc );
}

double PureMetal::CSPLineInterpolant::derivative(const double& x)
{
    return gsl_spline_eval_deriv ( spline, x, acc );
}

