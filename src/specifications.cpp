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

#include "specifications.hpp"

#include <boost/property_tree/xml_parser.hpp>

#include "messages.hpp"

PureMetal::Specifications::Specifications ( std::string input_file ) :
    _time_type ( TimeType::undefined ),
    _simulation_type ( SimulationType::undefined ),
    _alpha ( 0. ),
    _epsilon ( 0. ),
    _delta ( 0. ),
    _r0 ( 0. ),
    _gamma_psi ( 0. ),
    _gamma_u ( 0. ),
    _tolerance ( 0. ),
    _stability_check ( false ),
    _postprocess_polynomial ( false ),
    _postprocess_cspline ( false ),
    _delt ( 0. ),
    _max_time ( 0. ),
    _delt_max ( 0. ),
    _delt_min ( 0. ),
    _delt_multiplier ( 0. ),
    _delt_step ( 0. ),
    _max_timestep ( 0 ),
    _steady_state_threshold ( 0. )
{
    boost::property_tree::ptree tree, subtree;
    boost::property_tree::read_xml ( input_file, tree );
    tree = tree.get_child ( "PureMetal_specification" );

    // SimulationComponent
    std::string simualtion_type_str = tree.get<std::string> ( "SimulationComponent.<xmlattr>.type" );
    if ( simualtion_type_str == "full" ) {
        _simulation_type = SimulationType::full;
    } else if ( simualtion_type_str == "quadrant" ) {
        _simulation_type = SimulationType::quadrant;
    } else {
        throw std::runtime_error ( unknown_symulation_type_msg + simualtion_type_str );
    }

    // PhaseField
    subtree = tree.get_child ( "PhaseField" );
    _alpha = subtree.get<double> ( "alpha" );
    _epsilon = subtree.get<double> ( "epsilon" );
    _delta = subtree.get<double> ( "Delta" );
    _r0 = subtree.get<double> ( "R0" );
    _gamma_psi = subtree.get ( "gamma_psi", 0.707106781187 ); // 2^(-.5)
    _gamma_u = subtree.get ( "gamma_u", 0.1 );
    _tolerance = subtree.get ( "tolerance", 1e-6 );

    _stability_check = subtree.get ( "stability_check", true );
    _postprocess_polynomial = subtree.get ( "postprocess_polynomial", false );
    _postprocess_cspline = subtree.get ( "postprocess_cspline", false );

    // Grid
    subtree = tree.get_child ( "Grid" );
    std::string upper_str = subtree.get<std::string> ( "upper" );
    std::sscanf ( upper_str.c_str(), "[%lf, %lf]", _upper, _upper + 1 );
    if ( _upper[0] <= 0 || _upper[1] <= 0 ) {
        throw std::runtime_error ( negative_upper_grid_msg + upper_str );
    }

    if ( _simulation_type == SimulationType::full ) {
        std::string lower_str = subtree.get<std::string> ( "lower" );
        std::sscanf ( lower_str.c_str(), "[%lf, %lf]", _lower, _lower + 1 );
    } else {
        _lower[0] = 0.;
        _lower[1] = 0.;
    }
    std::string spacing_str = subtree.get<std::string> ( "spacing" );
    std::sscanf ( spacing_str.c_str(), "[%lf, %lf]", _spacing, _spacing + 1 );

    // Time
    subtree = tree.get_child ( "Time" );
    std::string time_type_str = tree.get<std::string> ( "Time.<xmlattr>.type" );
    if ( time_type_str == "fixed" ) {
        _time_type = TimeType::fixed;
        _delt = subtree.get<double> ( "delt" );
        _max_timestep = 0u;
        _max_time = subtree.get<double> ( "maxTime" );
    } else if ( time_type_str == "steady_state" ) {
        _time_type = TimeType::steady_state;
        _delt = subtree.get<double> ( "delt" );
        _steady_state_threshold = subtree.get<double> ( "steady_state_threshold" );
        _window_size = subtree.get<double> ( "window_size" );
    } else if ( time_type_str == "stable" ) {
        _time_type = TimeType::stable;
        _delt_max = subtree.get<double> ( "delt_max" );
        _delt_min = subtree.get<double> ( "delt_min" );
        _delt_multiplier = subtree.get ( "delt_multiplier", 0. );
        if ( !_delt_multiplier )
            _delt_step = subtree.get<double> ( "delt_step" );
        _max_timestep = subtree.get<unsigned> ( "max_Timesteps" );
    } else {
        throw std::runtime_error ( unknown_time_type_msg + time_type_str );
    }

    // DataArchiver
    subtree = tree.get_child ( "DataArchiver" );
    _out_path = subtree.get<std::string> ( "filebase" );
    _out_interval = subtree.get<unsigned> ( "outputTimestepInterval" );
    auto save_range = subtree.equal_range ( "save" );
    for ( auto it = save_range.first; it != save_range.second; ++it ) {
        std::string label = it->second.get<std::string> ( "<xmlattr>.label" );
        if ( label == "psi" || label == "u" || label == "psi_x" || label == "psi_y" ||
                label == "grad_psi_norm2" || label == "A" || label == "A2" || label == "Bxy" ) {
            _out_labels.push_back ( label );
        } else {
            throw std::runtime_error ( unknown_save_label_msg + label );
        }
    }
}

PureMetal::Specifications::~Specifications()
{
    _out_labels.clear();
}

