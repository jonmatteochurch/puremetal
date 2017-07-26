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

#ifndef PSPECIFICATIONS_HPP
#define PUREMETAL_SPECIFICATIONS_HPP

#include <string>
#include <list>

namespace PureMetal
{

enum class TimeType
{
    undefined, fixed, steady_state, stable
};

enum class SimulationType
{
    undefined, full, quadrant
};

class Specifications
{
    TimeType _time_type;
    SimulationType _simulation_type;

    double _alpha;
    double _epsilon;
    double _delta;
    double _r0;
    double _gamma_psi;
    double _gamma_u;
    double _tolerance;

    bool _stability_check;
    bool _postprocess_polynomial;
    bool _postprocess_cspline;

    double _upper[2];
    double _lower [2];
    double _spacing [2];

    double _delt;

    // fixed
    double _max_time;

    // stable
    double _delt_max;
    double _delt_min;
    double _delt_multiplier;
    double _delt_step;

    // steady_state
    unsigned _max_timestep;
    double _steady_state_threshold;

    std::string _out_path;
    unsigned _out_interval;
    std::list<std::string> _out_labels;

    Specifications ( const Specifications & other ) = delete;
    Specifications & operator= ( const Specifications & other ) = delete;
    bool operator== ( const Specifications & other ) const = delete;

public:
    Specifications ( std::string input_file );
    ~Specifications();

    inline const TimeType & time_type() const;
    inline const SimulationType & simulation_type() const;

    inline const double & alpha() const;
    inline const double & epsilon() const;
    inline const double & delta() const;
    inline const double & r0() const;
    inline const double & gamma_psi() const;
    inline const double & gamma_u() const;
    inline const double & tolerance() const;

    inline const bool & stability_check() const;
    inline const bool & postprocess_polynomial() const;
    inline const bool & postprocess_cspline() const;

    inline const double * upper() const;
    inline const double * lower() const;
    inline const double * spacing() const;

    inline const double & delt() const;

    inline const double & max_time() const;

    inline const double & delt_max() const;
    inline const double & delt_min() const;
    inline const double & delt_muptiplier() const;
    inline const double & delt_step() const;

    inline const unsigned & max_timestep() const;
    inline const double & steady_state_threshold() const;

    inline const std::string & out_path() const;
    inline const unsigned & out_interval() const;
    inline const std::list<std::string> & out_labels() const;
};

}

const PureMetal::TimeType & PureMetal::Specifications::time_type() const
{
    return _time_type;
}

const PureMetal::SimulationType & PureMetal::Specifications::simulation_type() const
{
    return _simulation_type;
}

const double & PureMetal::Specifications::alpha() const
{
    return _alpha;
}

const double & PureMetal::Specifications::epsilon() const
{
    return _epsilon;
}

const double & PureMetal::Specifications::delta() const
{
    return _delta;
}

const double & PureMetal::Specifications::r0() const
{
    return _r0;
}

const double & PureMetal::Specifications::gamma_psi() const
{
    return _gamma_psi;
}

const double & PureMetal::Specifications::gamma_u() const
{
    return _gamma_u;
}

const double & PureMetal::Specifications::tolerance() const
{
    return _tolerance;
}

const double * PureMetal::Specifications::upper() const
{
    return _upper;
}

const double * PureMetal::Specifications::lower() const
{
    return _lower;
}

const double * PureMetal::Specifications::spacing() const
{
    return _spacing;
}

const double & PureMetal::Specifications::delt() const
{
    return _delt;
}

const double & PureMetal::Specifications::max_time() const
{
    return _max_time;
}

const double & PureMetal::Specifications::delt_max() const
{
    return _delt_max;
}

const double & PureMetal::Specifications::delt_min() const
{
    return _delt_min;
}

const double & PureMetal::Specifications::delt_muptiplier() const
{
    return _delt_multiplier;
}

const double & PureMetal::Specifications::delt_step() const
{
    return _delt_step;
}

const unsigned & PureMetal::Specifications::max_timestep() const
{
    return _max_timestep;
}

const double & PureMetal::Specifications::steady_state_threshold() const
{
    return _steady_state_threshold;
}

const bool & PureMetal::Specifications::stability_check() const
{
    return _stability_check;
}

const bool & PureMetal::Specifications::postprocess_polynomial() const
{
    return _postprocess_polynomial;
}

const bool & PureMetal::Specifications::postprocess_cspline() const
{
    return _postprocess_cspline;
}

const std::string & PureMetal::Specifications::out_path() const
{
    return _out_path;
}

const unsigned int & PureMetal::Specifications::out_interval() const
{
    return _out_interval;
}

const std::list< std::string > & PureMetal::Specifications::out_labels() const
{
    return _out_labels;
}

#endif // PUREMETAL_SPECIFICATIONS_HPP
