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

#ifndef PUREMETAL_VTKFILE_HPP
#define PUREMETAL_VTKFILE_HPP

#include <iomanip>
#include <fstream>
#include <sstream>

#include <vtkSmartPointer.h>

#define PUREMETAL_TIME_WIDTH 7

class vtkImageData;

namespace PureMetal
{

class Field;

class VtkFile
{
    const std::string _path;
    const std::string _data;
    std::string _name;
    vtkSmartPointer<vtkImageData> _grid;

    VtkFile ( const VtkFile & other ) = delete;
    VtkFile & operator= ( const VtkFile & other ) = delete;
    bool operator== ( const VtkFile & other ) const = delete;

public:
    inline VtkFile ( const std::string & path, const unsigned & timestep );
    ~VtkFile();

    inline bool exists();

    inline std::string abs_path();
    inline std::string rel_path();

    void read ( Field * psi, Field * u );
    void set_grid ( const double & hx, const double & hy, const int & Nx, const int & Ny, const double & x0, const double & y0 );
    void add_time ( const double & time );
    void add_scalar ( const std::string & name, const double * data );
    void save();
};

}

PureMetal::VtkFile::VtkFile ( const std::string & path, const unsigned & timestep )
    : _path ( path ),
      _data ( "data" ),
      _grid ( )
{
    std::stringstream stream;
    stream << "t" << std::setw ( PUREMETAL_TIME_WIDTH ) << std::setfill ( '0' ) << timestep << ".vti";
    _name = stream.str();
}

bool PureMetal::VtkFile::exists()
{
    std::ifstream file ( abs_path() );
    return file.good();
}

std::string PureMetal::VtkFile::abs_path()
{
    return _path + "/" + _data + "/" + _name;
}

std::string PureMetal::VtkFile::rel_path()
{
    return _data + "/" + _name;
}

#endif // PUREMETAL_VTKFILE_HPP

