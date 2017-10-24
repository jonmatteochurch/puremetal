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

#ifndef PUREMETAL_DATFILE_HPP
#define PUREMETAL_DATFILE_HPP

#include <string>
#include <iomanip>
#include <fstream>

namespace PureMetal
{

class DatFile
{
    const std::string _path;
    const std::string _name;

    DatFile ( const DatFile & other ) = delete;
    DatFile & operator= ( const DatFile & other ) = delete;
    bool operator== ( const DatFile & other ) const = delete;

public:
    DatFile ( const std::string & path, const std::string & name, const bool & append );
    ~DatFile() = default;

    inline void add ( const unsigned & ts, const double & time, const double & x, const double & v, const double & k1, const double & k2, const double & kpar );
};

}

void PureMetal::DatFile::add ( const unsigned int & ts, const double & time, const double & x, const double & v, const double & k1, const double & k2, const double & kpar )
{
    std::ofstream out;
    out.open ( _path + "/" + _name, std::ios_base::app );
    out << std::setprecision ( 16 ) << std::scientific << ts << " " << time << " " << x << " " << v << " " << k1 << " " << k2 << " " << kpar << std::endl;
    out.close();
}

#endif // PUREMETAL_DATFILE_HPP
