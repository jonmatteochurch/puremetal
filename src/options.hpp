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

#ifndef PUREMETAL_OPTIONS_HPP
#define PUREMETAL_OPTIONS_HPP

#include <string>

namespace PureMetal
{

class Options
{
    std::string _input_file;
    bool _restart;

    Options ( const Options & other ) = delete;
    Options & operator= ( const Options & other ) = delete;
    bool operator== ( const Options & other ) const = delete;

public:
    Options ( int argc, char ** argv );
    inline ~Options () = default; 

    inline const std::string & input_file () const;
    inline const bool & restart () const;
};

inline const std::string & Options::input_file () const
{
    return _input_file;
}

inline const bool & Options::restart() const
{
    return _restart;
}

}

#endif // PUREMETAL_OPTIONS_HPP
