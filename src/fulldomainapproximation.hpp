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

#ifndef PUREMETAL_FULLDOMAINAPPROXIMATION_HPP
#define PUREMETAL_FULLDOMAINAPPROXIMATION_HPP

#include <cmath>

#include "approximation.hpp"
#include "fulldomainfield.hpp"

namespace PureMetal
{

class FullDomainApproximation : public Approximation
{
    FullDomainApproximation ( const FullDomainApproximation & other ) = delete;
    FullDomainApproximation & operator= ( const FullDomainApproximation & other ) = delete;
    bool operator== ( const FullDomainApproximation & other ) const = delete;

public:
    inline FullDomainApproximation ( const double * upper, const double * lower, const double * spacing );
    ~FullDomainApproximation() = default;

    inline Field * create_field ( const double & value ) const override;
    inline Field * create_field ( const std::function<double ( unsigned, unsigned ) > & function ) const override;

    inline ApproximationType type() const override;

    inline unsigned i ( const double & x ) const override;
    inline unsigned j ( const double & y ) const override;

    inline double x ( const unsigned & i ) const override;
    inline double y ( const unsigned & j ) const override;
};

}

PureMetal::FullDomainApproximation::FullDomainApproximation ( const double * upper, const double * lower, const double * spacing )
    : Approximation()
{
    _size[0] = 1 + std::ceil ( upper[0] - lower[0] ) / spacing[0];
    _size[1] = 1 + std::ceil ( upper[1] - lower[1] ) / spacing[1];
    _spacing[0] = spacing[0];
    _spacing[1] = spacing[1];
}

PureMetal::Field * PureMetal::FullDomainApproximation::create_field ( const double & value ) const
{
    return new FullDomainField ( this, value );
}

PureMetal::Field * PureMetal::FullDomainApproximation::create_field ( const std::function< double ( unsigned, unsigned ) > & function ) const
{
    return new FullDomainField ( this, function );
}

PureMetal::ApproximationType PureMetal::FullDomainApproximation::type() const
{
    return ApproximationType::QuarterDomain;
}

unsigned PureMetal::FullDomainApproximation::i ( const double & x ) const
{
    return std::floor ( x / _spacing[0] + _size[0] / 2. );
}

unsigned PureMetal::FullDomainApproximation::j ( const double & y ) const
{
    return std::floor ( y / _spacing[0] + _size[1] / 2. );
}

double PureMetal::FullDomainApproximation::x ( const unsigned & i ) const
{
    return _spacing[0] * ( i - std::floor ( _size[0] / 2. ) );
}

double PureMetal::FullDomainApproximation::y ( const unsigned & j ) const
{
    return _spacing[1] * ( j - std::floor ( _size[1] / 2. ) );
}


#endif // PUREMETAL_FULLDOMAINAPPROXIMATION_HPP
