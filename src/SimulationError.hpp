#include <exception>

class SimulationError: public std::exception
{   private:
        const char* cause;

    public:
        SimulationError(const char*);
        virtual const char* what() const throw();
};
