#include <exception>
#include <string>

/** \file SimulationError.hpp */
/** \brief Classe que agrega os erros do simulador */
class SimulationError: public std::exception
{   private:
        std::string cause; //<! Mensagem de erro

    public:
        SimulationError(std::string); //<! Construtor
        virtual const char* what() const throw(); //<! Recupera causa do erro
};
