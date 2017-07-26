#include <iostream>

#include "options.hpp"
#include "specifications.hpp"
#include "messages.hpp"
#include "simulation.hpp"
#include "postprocessor.hpp"

using namespace PureMetal;

int main ( int argc, char ** argv )
{
    Options * options = nullptr;
    Specifications * specifications = nullptr;

    try {
        options = new Options ( argc, argv );
    } catch ( const std::exception & e ) {
        std::cerr << e.what() << std::endl;
        usage ( std::cout );
        delete specifications;
        delete options;
        return 1;
    }

    try {
        specifications = new Specifications ( options->input_file() );
    } catch ( const std::exception & e ) {
        std::cerr << e.what() << std::endl;
        parse_error ( std::cout, options->input_file() );
        delete specifications;
        delete options;
        return 1;
    }

    switch ( specifications->time_type() ) {
    case TimeType::fixed : {
        unsigned timesteps = specifications->max_time() / specifications->delt();
        Simulation simulation ( specifications );
        if ( options->restart() ) {
            try {
                simulation.restart ( specifications->delt(), timesteps );
            } catch ( const std::exception & e ) {
                std::cerr << e.what();
                restart_error ( std::cout );
            }
        } else {
            simulation.start ( specifications->delt(), timesteps );
            simulation.save();
        }

        while ( simulation.next() ) {
            if ( specifications->stability_check() && !simulation.stable() ) {
                stability_error ( std::cout );
                delete specifications;
                delete options;
                return 1;
            }
            fixed_progress_info ( std::cout, simulation.progress() );
            if ( simulation.save_timestep() ) {
                simulation.save();
            }
        }
    }
    break;
    case TimeType::stable: {
        double delt ( specifications->delt_max() );
        bool stable = true;
        while ( delt > specifications->delt_min() ) {
            stable_progress_info ( std::cout, delt );
            Simulation simulation ( specifications );
            simulation.start ( delt, specifications->max_timestep() );
            if ( specifications->out_interval() ) {
                simulation.save ( );
            }

            while ( simulation.next() ) {
                if ( ! ( stable = simulation.stable() ) ) {
                    stability_error ( std::cout );
                    break;
                }
                if ( simulation.save_timestep() ) {
                    simulation.save();
                }
            }
            if ( stable ) {
                break;
            }
            if ( specifications->delt_muptiplier() ) {
                delt *= specifications->delt_muptiplier();
            } else {
                delt -= specifications->delt_step();
            }
        }
    }
    break;
    case TimeType::steady_state: {
        Simulation simulation ( specifications );

        if ( options->restart() ) {
            try {
                simulation.restart ( specifications->delt(), specifications->steady_state_threshold() );
            } catch ( const std::exception & e ) {
                std::cerr << e.what();
                restart_error ( std::cout );
            }
        } else {
            simulation.start ( specifications->delt(), specifications->steady_state_threshold() );
            simulation.save();
        }

        while ( simulation.next() ) {
            if ( specifications->stability_check() && !simulation.stable() ) {
                stability_error ( std::cout );
                delete specifications;
                delete options;
                return 1;
            }
            steady_state_progress_info ( std::cout, simulation.post_processor()->next_cell(), simulation.time(), simulation.post_processor()->cell_velocity0(), simulation.post_processor()->cell_velocity() );
            if ( simulation.save_timestep() ) {
                simulation.save();
            }
        }
    }
    break;
    default:
        break;
    }


    delete specifications;
    delete options;

    return 0;
}
