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

#ifndef PUREMETAL_INTERPOLANT_HPP
#define PUREMETAL_INTERPOLANT_HPP

namespace PureMetal
{

class Interpolant
{
    Interpolant ( const Interpolant & other ) = delete;
    Interpolant & operator= ( const Interpolant & other ) = delete;
    bool operator== ( const Interpolant & other ) const = delete;

protected:
    Interpolant() = default;

public:
    virtual ~Interpolant() = default;

    virtual double root ( const double & min, const double & max ) = 0;
    virtual double operator () ( const double & x ) = 0;
    virtual double val0 () = 0;
    virtual double derivative ( const double & x ) = 0;
    virtual double derivative0 () = 0;
};

}

#endif // PUREMETAL_INTERPOLANT_HPP
