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

#ifndef PUREMETAL_FIELD_HPP
#define PUREMETAL_FIELD_HPP

#include <functional>

#include "approximation.hpp"

namespace PureMetal
{

class Field
{
    Field ( const Field & other ) = delete;
    Field & operator= ( const Field & other ) = delete;
    bool operator== ( const Field & other ) const = delete;

protected:
    const Approximation * _approximation;
    double * _values;

    Field ( const Approximation * approximation, const double & value );
    inline Field ( const Approximation * approximation, const std::function<double ( const unsigned &, const unsigned & ) > & function );

public:
    virtual inline ~Field();

    inline double * data() const;

    void update ( const std::function<double ( const unsigned &, const unsigned & ) > & function );
    void copy_field ( const Field * field );
    void add_field ( const Field * field );

    inline const double & operator () ( const unsigned & i, const unsigned & j ) const;
    const double & at ( int i, int j ) const;

    inline double x ( const unsigned & i, const unsigned & j ) const;
    inline double y ( const unsigned & i, const unsigned & j ) const;
    inline double laplacian ( const unsigned & i, const unsigned & j ) const;
};

}

PureMetal::Field::Field ( const PureMetal::Approximation * approximation, const std::function< double ( const unsigned &, const unsigned & ) > & function )
    : _approximation ( approximation ),
      _values ( nullptr )
{
    unsigned size = _approximation->size ( 0 ) * _approximation->size ( 1 );
    _values = new double[size];
    update ( function );
}

PureMetal::Field::~Field()
{
    delete [] _values;
    _values = nullptr;
}

double * PureMetal::Field::data() const
{
    return _values;
}

const double & PureMetal::Field::operator() ( const unsigned & i, const unsigned & j ) const
{
    const unsigned & Nx = _approximation->size ( 0 );
    return _values[ j * Nx + i];
}

double PureMetal::Field::x ( const unsigned & i, const unsigned & j ) const
{
    const double & hx = _approximation->spacing ( 0 );
    return ( at ( i + 1, j ) - at ( i - 1, j ) ) / ( 2.*hx );
}

double PureMetal::Field::y ( const unsigned & i, const unsigned & j ) const
{
    const double & hy = _approximation->spacing ( 1 );
    return ( at ( i, j + 1 ) - at ( i, j - 1 ) ) / ( 2.*hy );
}

double PureMetal::Field::laplacian ( const unsigned & i, const unsigned & j ) const
{
    const double & hx = _approximation->spacing ( 0 );
    const double & hy = _approximation->spacing ( 1 );
    return ( at ( i + 1, j ) + at ( i - 1, j ) - 2 * at ( i, j ) ) / hx +
           ( at ( i, j + 1 ) + at ( i, j - 1 ) - 2 * at ( i, j ) ) / hy;
}

#endif // PUREMETAL_FIELD_HPP


