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

#ifndef PUREMETAL_SIMULATION_HPP
#define PUREMETAL_SIMULATION_HPP

#include <list>
#include <map>
#include <string>

namespace PureMetal
{

class Approximation;
class Field;
class Specifications;
class PostProcessor;
class VisitFile;

class Simulation
{
    const double _alpha;
    const double _lambda;
    const double _epsilon;
    const double _delta;
    const double _r0;
    const double _gamma_psi;
    const double _gamma_u;
    const double _tolerance;

    double _delt;
    unsigned _maxts;
    unsigned _ts;
    double _threshold;

    bool _steady_state_checkpoint;
    double _mean_v0;
    double _mean_v;
    double _mean_k10;
    double _mean_k1;
    double _mean_k20;
    double _mean_k2;
    double _mean_kpar0;
    double _mean_kpar;
    unsigned _window_size;

    Approximation * _approximation;

    Field * _psi;
    Field * _u;
    Field * _psi0;
    Field * _u0;
    Field * _psix;
    Field * _psiy;
    Field * _n2;
    Field * _a;
    Field * _a2;
    Field * _bxy;

    std::string _out_path;
    unsigned _out_interval;
    std::map<std::string, const Field *> _out_map;
    VisitFile * _out_visit;

    bool _post_polynomial;
    bool _post_cspline;
    std::list<PostProcessor *> _post_processors;

    void start();
    void restart();
    void next_ts();

public:
    Simulation ( const Specifications * specs );
    ~Simulation();
    Simulation ( const Simulation & other ) = delete;
    Simulation & operator= ( const Simulation & other ) = delete;
    bool operator== ( const Simulation & other ) const = delete;

    inline const PostProcessor * post_processor() const;

    void start ( const double & delt, const unsigned & timesteps );
    void start ( const double & delt, const double & steady_state_threshold, const unsigned & window_size );
    void restart ( const double & delt, const unsigned & timesteps );
    void restart ( const double & delt, const double & steady_state_threshold, const unsigned & window_size );
    bool next();
    void save();
    bool stable ();
    inline double time();
    inline bool save_timestep();
    inline double progress();
    
    inline const bool & steady_state_checkpoint() const;
    inline const double & mean_v0() const;
    inline const double & mean_v() const;
    inline const double & mean_k10() const;
    inline const double & mean_k1() const;
    inline const double & mean_k20() const;
    inline const double & mean_k2() const;
    inline const double & mean_kpar0() const;
    inline const double & mean_kpar() const;
};

}

const PureMetal::PostProcessor * PureMetal::Simulation::post_processor() const
{
    return _post_processors.front();
}

double PureMetal::Simulation::time()
{
    return static_cast<double> ( _ts ) * _delt;
}

double PureMetal::Simulation::progress()
{
    return static_cast<double> ( _ts ) / static_cast<double> ( _maxts );
}

bool PureMetal::Simulation::save_timestep()
{
    return _out_interval && ! ( _ts % _out_interval );
}

const bool & PureMetal::Simulation::steady_state_checkpoint() const
{
    return _steady_state_checkpoint;
}

const double & PureMetal::Simulation::mean_v0() const
{
    return _mean_v0;
}

const double & PureMetal::Simulation::mean_v() const
{
    return _mean_v;
}

const double & PureMetal::Simulation::mean_k10() const
{
    return _mean_k10;
}

const double & PureMetal::Simulation::mean_k1() const
{
    return _mean_k1;
}

const double & PureMetal::Simulation::mean_k20() const
{
    return _mean_k20;
}

const double & PureMetal::Simulation::mean_k2() const
{
    return _mean_k2;
}

const double & PureMetal::Simulation::mean_kpar0() const
{
    return _mean_kpar0;
}

const double & PureMetal::Simulation::mean_kpar() const
{
    return _mean_kpar;
}

#endif // PUREMETAL_SIMULATION_HPP
