#include "SimulationError.hpp"

SimulationError::SimulationError(std::string aCause) {
    cause = aCause;
}
        
const char* SimulationError::what() const throw(){return cause.c_str();}
