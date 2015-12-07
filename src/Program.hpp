#include <map>
#include "Operation.hpp"

/** \file Program.hpp */
/** \brief Classe encapsula as instruções do programa 
 *
 * A classe Program é composta por uma série de estruturas representando o programa.
 * Dentre elas symbol_table que mapeia os rótulos para endereços de memória, line_table que mapeia os endereços que possuem rótulos, unused_labels que é usado para verificar se as instruções referenciam rótulos válidos e 
 */
class Program{
        std::map<std::string,size_t> symbol_table; //<! Estrutura que mapeia os rótulos para endereços de memória
        std::map<size_t,std::string> line_table; //<! Estrutura que que mapeia os endereços que possuem rótulos
        std::map<std::string,size_t> unused_labels; //<! Estrutura que é usada para verificar se as instruções referenciam rótulos válidos
        std::vector<Operation> op_list; //<! Estrutura contendo as instruções do programa
    public:

        /** \brief Retorna a instrução contida na posição indicada
         *
         * \param pc Posição no programa
         * \return Instrução contida na posição indicada
         */
        Operation get_operation(size_t);

        /** \brief Adiciona uma instrução ao programa
         *
         * \param op Instrução a ser adicionada
         */
        void add_operation(Operation);

        /** \brief Adiciona uma instrução com rótulo ao programa
         *
         * \param lbl Rótulo da instrução
         * \param op Instrução a ser adicionada
         */
        void add_operation(std::string, Operation);

        /** \brief Retorna a posição do rótulo no programa
         *
         * \param lbl Posição no programa
         * \return Posição no programa do rótulo
         */
        size_t get_label(std::string);

        /** \brief Retorna o número de instruções do programa
         *
         * \return Instrução contida na posição indicada
         */
        size_t get_size();

        /** \brief Retorna a operação contida na posição indicada
         *
         * \param pc Posição no programa
         * \return Instrução contida na posição indicada
         */
        std::string get_line(size_t);

        /** \brief Retorna mensagens de erro para rótulos não declaradas
         *
         * \return Uma mensagem de erro com os rótulos não declarados, ou uma string vazia caso todos os rótulos estejam corretamente declarados.
         */
        std::string get_unused_labels();
};
