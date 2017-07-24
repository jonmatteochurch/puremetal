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

#ifndef PUREMETAL_FULLDOMAINFIELD_HPP
#define PUREMETAL_FULLDOMAINFIELD_HPP

#include "field.hpp"

namespace PureMetal
{

class FullDomainField : public Field
{
    inline FullDomainField ( const FullDomainField & other ) = delete;
    FullDomainField & operator= ( const FullDomainField & other ) = delete;
    bool operator== ( const FullDomainField & other ) const = delete;

public:
    inline FullDomainField ( const Approximation * approximation, const double & value );
    inline FullDomainField ( const Approximation * approximation, const std::function<double ( unsigned, unsigned ) > & function );
    ~FullDomainField() = default;
};

}

PureMetal::FullDomainField::FullDomainField ( const Approximation * approximation, const double & value )
    : Field ( approximation, value ) {}

PureMetal::FullDomainField::FullDomainField ( const PureMetal::Approximation * approximation, const std::function< double ( unsigned, unsigned ) > & function )
    : Field ( approximation, function ) {}


#endif // PUREMETAL_FULLDOMAINFIELD_HPP
