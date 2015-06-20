#include "SimulationError.hpp"

SimulationError::SimulationError(const char* aCause) {cause=aCause;}
        
const char* SimulationError::what() const throw(){return cause;}
