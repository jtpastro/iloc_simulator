#include "Program.hpp"

/** \brief Classe que gerencia a execução do programa
 *
 * Machine é a classe que executa de fato o programa.
 * Ela é responsável pela gerência de memória, registradores e do fluxo de execução.
 * O programa pode ser executado continuamente ou passo a passo.
 * O estado do programa e da máquina simulada pode ser verificado por funções que retornam strings formatadas.
 *    \see prog_state
 *    \see mem_state
 *    \see reg_state
 */
class Machine {
        size_t PC;                                    //!< Registrador contador de programa
        int bss;                                    //!< Registrador de segmento de dados
        int fp;                                     //!< Registrador de ponteiro de quadro
        int sp;                                     //!< Registrador de ponteiro de pilha
        std::vector<char> memory;                   //!< Vetor contendo o setor de dados da memória 
        std::map<std::string,int> registers;        //!< Estrutura contendo os registradores
        size_t cycles=0;                              //!< Contador de ciclos
        size_t op_count=0;                            //!< Contador de operações
        Program program;                            //!< Estrutura contendo o programa 
        
        /** \brief Armazena um inteiro de 32bits na memória
         *
         *  \see set_memory
         *  \param location Posição de memória na qual o valor será armazenado
         *  \param value Valor inteiro de 32bits 
         */
        void set_word(size_t,int);
        
        /** \brief Armazena um inteiro de 32bits em um registrador
         *  
         *  \param reg Nome do registrador
         *  \param value Valor inteiro de 32bits 
         */
        void set_register(std::string,int);
        
        /** \brief Armazena um byte na memória
         *
         *  \param location Posição de memória na qual o valor será armazenado
         *  \param value Valor a ser gravado (byte) 
         */
        void set_memory(size_t,char);
        
        /** \brief Verifica se acesso de memória está dentro dos limites
         *
         *  \param location Posição de memória a verificar
         */
        void check_access(size_t);
        
        /** \brief Retorna o endereço de memória na qual está localizado um rótulo
         *
         *  \param lbl um rótulo
         *  \return Endereço de memória
         */
        size_t get_branch_destination(std::string);
        
        /** \brief Retorna string formatada de posição de memória
         *
         * \param addr Endereço inicial
         * \return String com a posição de memória
         */
        std::string print_address(size_t);
    public:

        /** \brief Construtor
         *
         * \param location Endereço de memória
         * \return Conteúdo do endereço no qual iniciará o quadro

         */
        Machine(Program,size_t,size_t);

        /** \brief Acessa um byte na memória
         *
         * \param location Endereço de memória
         * \return Conteúdo da memória no endereço (byte)
         */
        char get_memory(size_t);

        /** \brief Acessa um inteiro de 32bits na memória
         *
         * \param location Endereço de memória
         * \return Conteúdo da memória no endereço (32bits)
         */
        int get_word(size_t);

        /** \brief Acessa um registrador
         *
         * \param reg nome do registrador
         * \return Conteúdo do registrador no endereço (32bits)
         */
        int get_register(std::string);

        /** \brief Retorna tamanho da memória
         *
         * \return  Tamanho da memória em bytes
         */
        int mem_size();
        
        /** \brief Executa uma operação
         *
         * \return Estado da execução (false = encerrada)
         */
        bool execute_operation();

        /** \brief Executa o programa
         *
         * Executa o programa até a operação de HALT ou até encontrar um erro
         */
        void run();
        
        /** \brief Retorna estado dos registradores
         *
         * \return String com o estado dos registradores
         */
        std::string reg_state();
        
        /** \brief Retorna estado da memória
         *
         * \return String com o estado da área de dados da memória
         */
        std::string mem_state();
         
        /** \brief Retorna estado de um endereço de memória
         *
         * \param pos Endereço de memória
         * \return String com o dado contido no endereço
         */
        std::string mem_state(size_t);       

        /** \brief Retorna o programa
         *
         * \return String com o programa
         */

        std::string prog_state();
        
        /** \brief Retorna uma instrução do programa
         *
         * \param pos Endereço de memória
         * \return String com a instrução contida no endereço
         */
        std::string prog_state(size_t);
        
        /** \brief Retorna estatísticas de execução
         *
         * \return String com as estatísticas de execução
         */
        std::string exec_state();
};

