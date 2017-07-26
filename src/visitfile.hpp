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

#ifndef PUREMETAL_VISITFILE_HPP
#define PUREMETAL_VISITFILE_HPP

#include <string>
#include <fstream>

namespace PureMetal
{

class VisitFile
{
    const std::string _path;
    const std::string _name;

    VisitFile ( const VisitFile & other ) = delete;
    VisitFile & operator= ( const VisitFile & other ) = delete;
    bool operator== ( const VisitFile & other ) const = delete;

public:
    inline VisitFile ( const std::string & path, const std::string & name, const bool & append );
    ~VisitFile() = default;

    inline void add ( const std::string & filename );

    inline std::string abs_path();
};

}

PureMetal::VisitFile::VisitFile ( const std::string & path, const std::string & name, const bool & append )
    : _path ( path ),
      _name ( name )
{
    std::ofstream out;
    out.open ( abs_path(), append ? std::ios_base::app : std::ios_base::trunc );
    out.close();
}

void PureMetal::VisitFile::add ( const std::string & filename )
{
    std::ofstream out;
    out.open ( abs_path(), std::ios_base::app );
    out << filename << std::endl;
    out.close();
}

std::string PureMetal::VisitFile::abs_path()
{
    return _path + "/" + _name + ".visit";
}

#endif // PUREMETAL_VISITFILE_HPP
