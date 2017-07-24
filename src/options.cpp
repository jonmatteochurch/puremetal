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

#include "options.hpp"

#include<stdexcept>

#include "messages.hpp"

PureMetal::Options::Options ( int argc, char ** argv ) :
    _input_file ( "" ),
    _restart ( false )
{
    if ( argc < 2 || argc > 3 ) {
        throw std::runtime_error ( options_number_error_msg );
    }
    _restart = argc == 3;
    if ( _restart ) {
        std::string option = std::string ( argv[1] );
        if ( option != "--restart" ) {
            throw std::runtime_error ( unknown_option_msg + option );
        }
    }
    _input_file = std::string ( argv[argc - 1] );
}
