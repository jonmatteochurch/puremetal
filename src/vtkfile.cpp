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

#include "vtkfile.hpp"

#include <vtkSmartPointer.h>
#include <vtkDataArray.h>
#include <vtkDoubleArray.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkXMLImageDataReader.h>
#include <vtkXMLImageDataWriter.h>

#include "field.hpp"

PureMetal::VtkFile::~VtkFile()
{
    if ( _grid ) {
        _grid->Delete();
    }
}

bool PureMetal::VtkFile::exists()
{
    std::ifstream file ( abs_path() );
    return file.good();
}

void PureMetal::VtkFile::read ( PureMetal::Field * psi, PureMetal::Field * u )
{
    vtkXMLImageDataReader * reader = vtkXMLImageDataReader::New();
    reader->SetFileName ( abs_path().c_str() );
    reader->Update();
    vtkImageData * grid = reader->GetOutput();
    grid->GetPointData()->GetArray ( "psi" )->GetTuple ( 0, psi->data() );
    grid->GetPointData()->GetArray ( "u" )->GetTuple ( 0, u->data() );
    reader->Delete();
}

void PureMetal::VtkFile::set_grid ( const double & hx, const double & hy, const int & Nx, const int & Ny, const double & x0, const double & y0 )
{
    if ( _grid ) {
        _grid->Delete();
    }
    _grid = vtkImageData::New();
    _grid->SetSpacing ( hx, hy, 1. );
    _grid->SetDimensions ( Nx, Ny, 1 );
    _grid->SetOrigin ( x0, y0, 0. );
}

void PureMetal::VtkFile::add_time ( const double & time )
{
    vtkSmartPointer<vtkDoubleArray> array = vtkSmartPointer<vtkDoubleArray>::New();
    array->SetName ( "TIME" );
    array->SetNumberOfTuples ( 1 );
    array->SetTuple1 ( 0, time );
    _grid->GetFieldData()->AddArray ( array );

}

void PureMetal::VtkFile::add_scalar ( const std::string & name, const double * data )
{
    const unsigned &n = _grid->GetNumberOfPoints();
    vtkSmartPointer< vtkDoubleArray> array = vtkSmartPointer<vtkDoubleArray>::New();
    
    double * tmp = new double[n];
    std::copy(data, data+n, tmp);
    array->SetNumberOfComponents ( 1 );
    array->SetNumberOfTuples ( _grid->GetNumberOfPoints() );
    array->SetArray ( tmp, _grid->GetNumberOfPoints(), 0, 1 );
    array->SetName ( name.c_str() );
    _grid->GetPointData()->AddArray ( array );
}

void PureMetal::VtkFile::save()
{
    vtkXMLImageDataWriter * writer = vtkXMLImageDataWriter::New();
    writer->SetFileName ( abs_path().c_str() );
    writer->SetInputData ( _grid );
    writer->Write();
    writer->Delete();
}

