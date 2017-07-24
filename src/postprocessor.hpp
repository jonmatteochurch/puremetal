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

#ifndef PUREMETAL_POSTPROCESSOR_HPP
#define PUREMETAL_POSTPROCESSOR_HPP

#include "datfile.hpp"

namespace PureMetal
{

class Approximation;
class Field;
class Interpolant;

class PostProcessor
{
    PostProcessor ( const PostProcessor & other ) = delete;
    PostProcessor & operator= ( const PostProcessor & other ) = delete;
    bool operator== ( const PostProcessor & other ) const = delete;

protected:
    double _x;
    double _x0;
    double _v;
    double _v0;
    double _k1;
    double _k2;
    double _kpar;
    double _t;
    double _t0;
    double _dt;
    double _dt0;
    bool _next_cell;
    double _cell_velocity0;
    double _cell_velocity;

    DatFile * _out_dat;

    inline PostProcessor ( const double & r0, const std::string & path, const std::string & name, const bool & restart );

    virtual Interpolant * create_interpolant ( const double * x, const double * y, unsigned n ) = 0;

public:
    inline virtual ~PostProcessor();

    inline const double & tip_position() const;
    inline const bool & next_cell() const;
    inline const double & cell_velocity0() const;
    inline const double & cell_velocity() const;

    void process ( const Approximation * approximation, const unsigned & ts, const Field * psi, const double & delt );
};

}

PureMetal::PostProcessor::PostProcessor ( const double & r0, const std::string & path, const std::string & name, const bool & restart )
    : _x ( r0 ),
      _x0 ( _x ),
      _v ( 0. ),
      _v0 ( 0. ),
      _k1 ( 0. ),
      _k2 ( 0. ),
      _kpar ( 0. ),
      _t ( 0. ),
      _t0 ( 0. ),
      _dt ( 0. ),
      _dt0 ( 0. ),
      _next_cell ( false ),
      _cell_velocity0 ( 0. ),
      _cell_velocity ( 0. ),
      _out_dat ( new DatFile ( path, name, restart ) )
{}

PureMetal::PostProcessor::~PostProcessor()
{
    delete _out_dat;
}


const double & PureMetal::PostProcessor::tip_position() const
{
    return _x;
}

const bool & PureMetal::PostProcessor::next_cell() const
{
    return _next_cell;
}

const double & PureMetal::PostProcessor::cell_velocity0() const
{
    return _cell_velocity0;
}

const double & PureMetal::PostProcessor::cell_velocity() const
{
    return _cell_velocity;
}


#endif // PUREMETAL_POSTPROCESSOR_HPP
