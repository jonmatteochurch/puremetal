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

#ifndef PUREMETAL_QUARTERDOMAINAPPROXIMATION_HPP
#define PUREMETAL_QUARTERDOMAINAPPROXIMATION_HPP

#include <cmath>

#include "approximation.hpp"
#include "quarterdomainfield.hpp"

namespace PureMetal
{

class QuarterDomainApproximation : public Approximation
{
    QuarterDomainApproximation ( const QuarterDomainApproximation & other ) = delete;
    QuarterDomainApproximation & operator= ( const QuarterDomainApproximation & other ) = delete;
    bool operator== ( const QuarterDomainApproximation & other ) const = delete;

public:
    QuarterDomainApproximation ( const double * upper, const double * spacing );
    ~QuarterDomainApproximation() = default;

    inline Field * create_field ( const double & value ) const override;
    inline Field * create_field ( const std::function<double ( unsigned, unsigned ) > & function ) const override;

    inline ApproximationType type() const override;

    inline unsigned i ( const double & x ) const override;
    inline unsigned j ( const double & y ) const override;
    
    inline  double x ( const unsigned & i ) const override;
    inline  double y ( const unsigned & j ) const override;
};

}

PureMetal::QuarterDomainApproximation::QuarterDomainApproximation ( const double * upper, const double * spacing )
    : Approximation ()
{
    _size[0] = 1 + std::ceil ( upper[0] ) / spacing[0];
    _size[1] = 1 + std::ceil ( upper[1] ) / spacing[1];
    _spacing[0] = spacing[0];
    _spacing[1] = spacing[1];
}

PureMetal::Field * PureMetal::QuarterDomainApproximation::create_field ( const double & value ) const
{
    return new QuarterDomainField ( this, value );
}

PureMetal::Field * PureMetal::QuarterDomainApproximation::create_field ( const std::function< double ( unsigned, unsigned ) > & function ) const
{
    return new QuarterDomainField ( this, function );
}

PureMetal::ApproximationType PureMetal::QuarterDomainApproximation::type() const
{
    return ApproximationType::QuarterDomain;
}

unsigned PureMetal::QuarterDomainApproximation::i ( const double & x ) const
{
    return std::floor ( x / _spacing[0] );
}

unsigned PureMetal::QuarterDomainApproximation::j ( const double & y ) const
{
    return std::floor ( y / _spacing[1] );
}

double PureMetal::QuarterDomainApproximation::x ( const unsigned & i ) const
{
    return _spacing[0] * static_cast<double> ( i );
}

double PureMetal::QuarterDomainApproximation::y ( const unsigned & j ) const
{
    return _spacing[1] * static_cast<double> ( j );
}


#endif // PUREMETAL_QUARTERDOMAINAPPROXIMATION_HPP
