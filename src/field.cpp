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

#include "field.hpp"

PureMetal::Field::Field ( const PureMetal::Approximation * approximation, const double & value )
    :    _approximation ( approximation ),
         _values ( nullptr )
{
    unsigned size = _approximation->size ( 0 ) * _approximation->size ( 1 );
    _values = new double[size];
    for ( unsigned i = 0u; i < size; ++i ) {
        _values[i] = value;
    }
}

void PureMetal::Field::update ( const std::function< double ( const unsigned &, const unsigned & ) > & function )
{
    const unsigned & Nx = _approximation->size ( 0 );
    const unsigned & Ny = _approximation->size ( 1 );
    for ( unsigned j = 0u; j < Ny; ++j )
        for ( unsigned i = 0u; i < Nx; ++i ) {
            _values[ j * Nx + i] =  function ( i, j );
        }
}

void PureMetal::Field::copy_field ( const PureMetal::Field * field )
{
    const unsigned & Nx = _approximation->size ( 0 );
    const unsigned & Ny = _approximation->size ( 1 );
    for ( unsigned k = 0u; k < Nx * Ny; ++k ) {
        _values[ k ] =  field->_values[k];
    }
}

void PureMetal::Field::add_field ( const PureMetal::Field * field )
{
    const unsigned & Nx = _approximation->size ( 0 );
    const unsigned & Ny = _approximation->size ( 1 );
    for ( unsigned k = 0u; k < Nx * Ny; ++k ) {
        _values[ k ] +=  field->_values[k];
    }
}

const double & PureMetal::Field::at ( int i, int j ) const
{
    const int & Nx = _approximation->size(0);
    const int & Ny = _approximation->size(1);

    if ( i < 0 ) i = -i;
    if ( j < 0 ) j = -j;
    if ( i >= Nx ) i = 2 * Nx - i - 2;
    if ( j >= Ny ) j = 2 * Ny - j - 2;
    return ( *this ) ( i, j );
}
