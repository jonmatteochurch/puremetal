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

#ifndef PUREMETAL_APPROXIMATION_HPP
#define PUREMETAL_APPROXIMATION_HPP

#include <functional>

namespace PureMetal
{

enum class SimulationType;

enum class ApproximationType
{
    Full, QuarterDomain
};

class Field;

class Approximation
{
protected:
    unsigned _size[2];
    double _spacing[2];

    inline Approximation();
    Approximation ( const Approximation & other ) = delete;
    Approximation & operator= ( const Approximation & other ) = delete;
    bool operator== ( const Approximation & other ) const = delete;

public:
    virtual ~Approximation() = default;

    static Approximation * New ( const SimulationType & simulation_type, const double * upper, const double * lower, const double * spacing );
    virtual Field * create_field ( const double & ) const = 0;
    virtual Field * create_field ( const std::function<double ( unsigned, unsigned ) > & function ) const = 0;

    virtual ApproximationType type() const = 0;

    inline const unsigned & size ( const unsigned & d ) const;
    inline const double & spacing ( const unsigned & d ) const;

    virtual unsigned i ( const double & x ) const = 0;
    virtual unsigned j ( const double & x ) const = 0;

    virtual double x ( const unsigned & i ) const = 0;
    virtual double y ( const unsigned & j ) const = 0;
};

}

PureMetal::Approximation::Approximation( )
{
    _size[0] = _size[1] = 0u;
    _spacing[0] = _spacing[1] = 0.;
}

const unsigned & PureMetal::Approximation::size ( const unsigned & d ) const
{
    return _size[d];
}

const double & PureMetal::Approximation::spacing ( const unsigned & d ) const
{
    return _spacing[d];
}

#endif // PUREMETAL_APPROXIMATION_HPP
