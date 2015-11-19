#include <exception>
#include <string>

class SimulationError: public std::exception
{   private:
        std::string cause;

    public:
        SimulationError(std::string);
        virtual const char* what() const throw();
};
