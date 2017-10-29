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

#include "postprocessor.hpp"

#include <cmath>
#include <vector>

#include <gsl/gsl_fit.h>

#include "approximation.hpp"
#include "field.hpp"
#include "interpolant.hpp"

void PureMetal::PostProcessor::process ( const Approximation * approximation, const unsigned & ts, const Field * psi, const double & delt )
{
    switch ( approximation->type() ) {
    case ApproximationType::QuarterDomain: {
        const unsigned & Nx = approximation->size ( 0u );
        unsigned i0, cell0;
        cell0 = i0 = approximation->i ( _x0 );
        while ( psi->at ( i0, 0u ) > 0. ) {
            ++i0;
            if ( i0 == Nx ) {
                --i0;
                return;
            }
        } //i0 is te first non positive index

//         if ( - ( *psi ) ( i0, 0 ) > ( *psi ) ( i0 - 1, 0 ) )
        i0 += 1;
//         else
//             i0 += 2;
        if ( i0 >= Nx ) {
            i0 = Nx - 1u;
        }
        i0 -= 4;

        double xi[5], y2j[3];
        for ( unsigned i = 0u; i < 5u; ++i ) {
            xi[i] = approximation->x ( i0 + i );
        }
        for ( unsigned j = 0u; j < 3u; ++j ) {
            y2j[j] = approximation->y ( j );
            y2j[j] *= y2j[j];
        }

        double x0j[3], dxpsij[3];
        for ( unsigned j = 0; j < 3; ++j ) {
            double _psi[5];
            for ( unsigned i = 0; i < 5; ++i ) {
                _psi[i] = psi->at ( i0 + i, j );
            }
            Interpolant * ppsij = this->create_interpolant ( xi, _psi, 5 );
            x0j[j] = ppsij->root ( xi[0], xi[4] );
            dxpsij[j] = ppsij->derivative ( x0j[0] );
            x0j[j] = ppsij->root ( xi[0], xi[4] );
            delete ppsij;
        }

        _x = x0j[0];

        _v = ( _x - _x0 ) / delt;

        Interpolant * px0 = this->create_interpolant ( y2j, x0j, 3 );
        _k2 = 2 * px0->derivative0();

        double d2ypsii[5];
        for ( unsigned i = 0; i < 5; ++i ) {
            double _psi[3];
            for ( unsigned j = 0; j < 3; ++j ) {
                _psi[j] = psi->at( i0 + i, j );
            }
            Interpolant * ppsii = this->create_interpolant ( y2j, _psi, 3 );
            d2ypsii[i] = 2 * ppsii->derivative0 ();
            delete ppsii;
        }

        Interpolant * pd2ypsi = this->create_interpolant ( xi, d2ypsii, 5 );
        Interpolant * pdxpsi = this->create_interpolant ( y2j, dxpsij, 3 );

        _k1 = ( *pd2ypsi ) ( _x ) / std::abs ( pdxpsi->val0 () );

        delete px0;
        delete pd2ypsi;
        delete pdxpsi;

        std::vector<double> xc, y2c; //contour lines
        for ( unsigned i = 0.; i < approximation->i ( _x ); ++i ) {
            unsigned j = 0;
            while ( ( *psi ) ( i, j ) > 0 ) {
                j++;
            }
            double y0 = approximation->y ( j - 1 );
            const double & dy = approximation->spacing(1);
            const double & z0 = ( *psi ) ( i, j - 1 );
            const double & z1 = ( *psi ) ( i, j );
            double cx = approximation->x ( i );
            double cy = y0 - ( dy * z0 ) / ( z1 - z0 );
            if ( cy > .1 * cx ) {
                xc.push_back ( cx );
                y2c.push_back ( cy * cy );
                if ( 2 * cy < cx ) {
                    break;
                }
            }
        }
        double c0, c1, cov00, cov01, cov11, sumsq;
        if ( xc.size() > 2 ) {
            gsl_fit_linear ( y2c.data(), 1, xc.data(), 1, xc.size(), &c0, &c1, &cov00, &cov01, &cov11, &sumsq );
            _kpar = 2 * c1;
        } else {
            _kpar = NAN;
        }
        
        y2c.clear();
        xc.clear();

        _out_dat->add ( ts, ts * delt, _x, _v, _k1, _k2, _kpar );
        _x0 = _x;
        _v0 = _v;
    }
    default:
        break;
    }
}
