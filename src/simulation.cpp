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

#include "simulation.hpp"

#include <cmath>

#include "approximation.hpp"
#include "field.hpp"
#include "specifications.hpp"
#include "postprocessor.hpp"
#include "messages.hpp"
#include "visitfile.hpp"
#include "vtkfile.hpp"
#include "polynomialpostprocessor.hpp"
#include "csplinepostprocessor.hpp"

PureMetal::Simulation::Simulation ( const PureMetal::Specifications * specs ) :
    _alpha ( specs->alpha() ),
    _lambda ( specs->alpha() / 0.6267 ),
    _epsilon ( specs->epsilon() ),
    _delta ( specs->delta() ),
    _r0 ( specs->r0() ),
    _gamma_psi ( specs->gamma_psi() ),
    _gamma_u ( specs->gamma_u() ),
    _tolerance ( specs->tolerance() ),
    _delt ( 0. ),
    _maxts ( 0u ),
    _ts ( 0u ),
    _threshold ( 0. ),
    _approximation ( nullptr ),
    _psi ( nullptr ),
    _u ( nullptr ),
    _psi0 ( nullptr ),
    _u0 ( nullptr ),
    _psix ( nullptr ),
    _psiy ( nullptr ),
    _n2 ( nullptr ),
    _a ( nullptr ),
    _a2 ( nullptr ),
    _bxy ( nullptr ),
    _out_path ( specs->out_path() ),
    _out_interval ( specs->out_interval() ),
    _out_map (),
    _out_visit ( nullptr ),
    _post_polynomial ( false ),
    _post_cspline ( false ),
    _post_processors ( )
{
    _approximation = Approximation::New ( specs->simulation_type(), specs->upper(), specs->lower(), specs->spacing() );

    _psi = _approximation->create_field ( 0. );
    _u = _approximation->create_field ( 0. );
    _psi0 = _approximation->create_field ( 0. );
    _u0 = _approximation->create_field ( 0. );
    _psix = _approximation->create_field ( 0. );
    _psiy = _approximation->create_field ( 0. );
    _n2 = _approximation->create_field ( 0. );
    _a = _approximation->create_field ( 0. );
    _a2 = _approximation->create_field ( 0. );
    _bxy = _approximation->create_field ( 0. );

    for ( const auto & label : specs->out_labels() ) {
        if ( label == "psi" ) {
            _out_map[label] = _psi;
        } else if ( label == "u" ) {
            _out_map[label] = _u;
        } else if ( label == "psi_x" ) {
            _out_map[label] = _psix;
        } else if ( label == "psi_y" ) {
            _out_map[label] = _psiy;
        } else if ( label == "grad_psi_norm2" ) {
            _out_map[label] = _n2;
        } else if ( label == "A" ) {
            _out_map[label] = _a;
        } else if ( label == "A2" ) {
            _out_map[label] = _a2;
        } else if ( label == "Bxy" ) {
            _out_map[label] = _bxy;
        }
    }

    _post_polynomial = specs->postprocess_polynomial();
    _post_cspline = specs->postprocess_cspline();
}

PureMetal::Simulation::~Simulation()
{
    for ( PostProcessor * post_processor : _post_processors ) {
        delete post_processor;
    }
    _post_processors.clear();
    delete _out_visit;
    _out_map.clear();
    delete _approximation;
    delete _bxy;
    delete _a2;
    delete _a;
    delete _n2;
    delete _psiy;
    delete _psix;
    delete _u0;
    delete _psi0;
    delete _u;
    delete _psi;
}

void PureMetal::Simulation::start()
{
    _psi->update ( [ = ] ( unsigned i, unsigned j )-> double {
        double x = _approximation->x ( i ), y = _approximation->y ( j );
        return -std::tanh ( _gamma_psi * ( x * x + y * y - _r0 * _r0 ) );
    } );
    _u->update ( [ = ] ( unsigned i, unsigned j )-> double {
        double x = _approximation->x ( i ), y = _approximation->y ( j );
        return -_delta * ( 1. + std::tanh ( _gamma_u * ( x * x + y * y - _r0 * _r0 ) ) ) / 2.;
    } );

    if ( _out_interval && _out_map.size() ) {
        std::string cmd = "mkdir -p " + _out_path + "; mkdir -p " + _out_path + "/data";
        if ( std::system ( cmd.c_str() ) == -1 ) {
            throw std::runtime_error ( output_dir_error_msg );
        }
        _out_visit = new VisitFile ( _out_path, "index", false );
    }

    if ( _post_polynomial ) {
        _post_processors.push_back ( new PolynomialPostProcessor ( _r0, _out_path, "tip_polynomial", false ) );
    }
    if ( _post_cspline ) {
        _post_processors.push_back ( new CSPLinePostProcessor ( _r0, _out_path, "tip_cspline", false ) );
    }
}

void PureMetal::Simulation::start ( const double & delt, const unsigned & timesteps )
{
    _delt = delt;
    _maxts = timesteps;
    start();
}

void PureMetal::Simulation::start ( const double & delt, const double & steady_state_threshold )
{
    _delt = delt;
    _threshold = steady_state_threshold;
    start();
}

void PureMetal::Simulation::restart()
{
    _ts = -_out_interval;
    VtkFile * in_vtk = nullptr;
    do {
        delete in_vtk;
        _ts += _out_interval;
        in_vtk = new VtkFile ( _out_path, _ts );
    } while ( in_vtk->exists() );
    delete in_vtk;
    if ( _ts > 0 ) {
        _out_visit = new VisitFile ( _out_path, "index", true );
        _ts -= _out_interval;
        in_vtk = new VtkFile ( _out_path, _ts );
        in_vtk->read ( _psi, _u );

        if ( _post_polynomial ) {
            _post_processors.push_back ( new PolynomialPostProcessor ( _r0, _out_path, "tip_polynomial", false ) );
        }
        if ( _post_cspline ) {
            _post_processors.push_back ( new CSPLinePostProcessor ( _r0, _out_path, "tip_cspline", false ) );
        }
        delete in_vtk;
    } else {
        start ();
        save();
    }
}

void PureMetal::Simulation::restart ( const double & delt, const unsigned & timesteps )
{
    _delt = delt;
    _maxts = timesteps;
    restart();
}

void PureMetal::Simulation::restart ( const double & delt, const double & steady_state_threshold )
{
    _delt = delt;
    _threshold = steady_state_threshold;
    restart();
}

void PureMetal::Simulation::next_ts()
{
    _psi0->copy_field ( _psi );
    _u0->copy_field ( _u );
    _psix->update ( [ = ] ( unsigned i, unsigned j )-> double {
        return _psi0->x ( i, j );
    } );
    _psiy->update ( [ = ] ( unsigned i, unsigned j )-> double {
        return _psi0->y ( i, j );
    } );
    _n2->update ( [ = ] ( unsigned i, unsigned j )-> double {
        const double & psix = ( *_psix ) ( i, j );
        const double & psiy = ( *_psiy ) ( i, j );
        return psix * psix + psiy * psiy;
    } );
    _a->update ( [ = ] ( unsigned i, unsigned j )-> double {
        const double & psix = ( *_psix ) ( i, j );
        const double & psiy = ( *_psiy ) ( i, j );
        const double & n2 = ( *_n2 ) ( i, j );
        return ( n2 > _tolerance ) ? 1 + _epsilon * ( 4. * ( psix * psix * psix * psix + psiy * psiy * psiy * psiy ) / ( n2 * n2 ) - 3 ) : 1;
    } );
    _a2->update ( [ = ] ( unsigned i, unsigned j )-> double {
        const double & a = ( *_a ) ( i, j );
        return a * a;
    } );
    _bxy->update ( [ = ] ( unsigned i, unsigned j )-> double {
        const double & psix = ( *_psix ) ( i, j );
        const double & psiy = ( *_psiy ) ( i, j );
        const double & n2 = ( *_n2 ) ( i, j );
        const double & a = ( *_a ) ( i, j );
        return ( n2 > _tolerance ) ? ( 16.*_epsilon * a * psix * psiy * ( psix * psix - psiy * psiy ) ) / ( n2 * n2 ) : 0;
    } );
    Field * Dpsi = _approximation->create_field ( [ = ] ( unsigned i, unsigned j )-> double {
        const double & psi = ( *_psi0 ) ( i, j );
        const double & u = ( *_u0 ) ( i, j );
        const double & a = ( *_a ) ( i, j );
        const double & a2 = ( *_a2 ) ( i, j );
        double source = 1. - psi * psi;
        source *= ( psi - _lambda * u * source );
        return _delt * (
            _psi0->laplacian ( i, j ) * a2
            + ( _a2->x ( i, j ) - _bxy->y ( i, j ) ) * _psi0->x ( i, j )
            + ( _bxy->x ( i, j ) + _a2->y ( i, j ) ) * _psi0->y ( i, j )
            + source
        ) / a;
    } );
    Field * Du = _approximation->create_field ( [ = ] ( unsigned i, unsigned j )-> double {
        const double & dpsi = ( *Dpsi ) ( i, j );
        return _delt * _u0->laplacian ( i, j ) * _alpha + dpsi / 2.;
    } );
    _psi->add_field ( Dpsi );
    _u->add_field ( Du );
    delete Dpsi;
    delete Du;

    ++_ts;
}

bool PureMetal::Simulation::next()
{
    next_ts();
    for ( auto & post_processor : _post_processors ) {
        post_processor->process ( _approximation, _ts, _psi, _delt );
    }
    if ( _maxts ) {
        return _ts <= _maxts;
    }
    if ( _threshold > 0 && post_processor()->next_cell() ) {
        const double & v = post_processor()->cell_velocity();
        const double & v0 = post_processor()->cell_velocity0();
        double x = post_processor()->tip_position(), xm = _approximation->x ( _approximation->size ( 0 ) - 2 );
        if ( x > xm ) {
            return false;
        }
        return std::abs ( ( v - v0 ) / v ) > _threshold;
    }
    return true;
}

void PureMetal::Simulation::save()
{
    VtkFile * out_vtk = new VtkFile ( _out_path,  _ts );
    out_vtk->set_grid ( _approximation->spacing ( 0 ), _approximation->spacing ( 1 ), _approximation->size ( 0 ), _approximation->size ( 1 ), _approximation->x ( 0 ), _approximation->x ( 1 ) );
    out_vtk->add_time ( _delt * _ts );
    for ( auto & pair : _out_map ) {
        out_vtk->add_scalar ( pair.first, pair.second->data() );
    }
    out_vtk->save();
    _out_visit->add ( out_vtk->rel_path() );
    delete out_vtk;
}

bool PureMetal::Simulation::stable()
{
    for ( unsigned i = 0u; i < _approximation->size ( 0 ); ++i ) {
        for ( unsigned j = 0u; j < _approximation->size ( 1 ); ++j ) {
            const double & u = ( *_u ) ( i, j );
            if ( u + _delta < -_tolerance || u > .5 * _delta ) {
                return false;
            }
        }
    }
    return true;
}


