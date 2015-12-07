#include <string>
#include <vector>
#include <cctype>

/** \file Operation.hpp */
typedef enum opcode_name {NOP=0, ADD, SUB, MULT, DIV,
                  ADDI, SUBI, RSUBI, MULTI, DIVI, RDIVI,
                  LSHIFT, LSHIFTI, RSHIFT, RSHIFTI,
                  AND, ANDI, OR, ORI, XOR, XORI,
			      LOADI, LOAD, LOADAI, LOADAO, CLOAD, CLOADAI, CLOADAO,
                  STORE, STOREAI, STOREAO, CSTORE, CSTOREAI, CSTOREAO,
                  I2I, C2C, C2I, I2C,
                  JUMPI, JUMP, CBR,
                  CMPLT, CMPLE, CMPEQ, CMPNE, CMPGE, CMPGT,
                  HALT,
			      INVALID_OP} Opcode_Name; //<! Enumeração dos OPCODEs, o último OPCODE é reservado para código inválido

/** \brief Classe auxiliar que guarda as características de uma instrução
 *
 * O atributo name representa a forma canonica do OPCODE da instrução
 * Os atributos regs, consts e labels indicam respectivamente quantos operandos do tipo registrador, número e rótulo a instrução possui.
 */
class Opcode {
    public:
        const std::string name;
        size_t regs, consts, labels, latency;
};

/** \brief Classe que representa uma instrução
 *
 * Operation contém todas as informações relevantes de uma instrução: o código de operação e os operandos.
 */
class Operation{
        std::vector<int> consts; //<! Vetor com os operandos numéricos constantes
        std::vector<std::string> regs; //<! Vetor com os operandos do tipo registrador
        std::vector<std::string> labels; //<! Vetor com os operandos do tipo rótulo
    public:
        Opcode_Name opcode; //<! Código de operação (OPCODE)

        /** \brief Converte uma string para um OPCODE
         *
         * \param op String com um OPCODE
         * \return Um OPCODE representando a instrução se válido, caso contrário retorna INVALID_OP
         */
        static Opcode_Name string_to_opcode(std::string);

        /** \brief Converte um OPCODE para string
         *
         * \param op String com um OPCODE
         * \return Um OPCODE representando a instrução se válido, caso contrário retorna INVALID_OP
         */
        static std::string opcode_to_string(Opcode_Name);

        /** \brief Altera o custo de uma instrução
         *
         * \param op_name OPCODE da instrução
         * \param latency Custo da instrução
         */
        static void set_latency(Opcode_Name, size_t);

        /** \brief Verifica se a instrução possui o número correto de operandos
         *
         * \return Verdadeiro se instrução bem formada
         */
        bool verify_operation();

        /** \brief Retorna a quantidade de operandos requeridos do tipo registrador
         *
         * \return Quantidade de operandos requeridos do tipo registrador
         */
        size_t num_regs();

        /** \brief Retorna a quantidade de operandos requeridos do tipo rótulo
         *
         * \return Quantidade de operandos requeridos do tipo rótulo
         */
        size_t num_lbls();        

        /** \brief Retorna a quantidade de operandos requeridos do tipo constante
         *
         * \return Quantidade de operandos requeridos do tipo constante
         */
        size_t num_consts();        

        /** \brief Retorna o custo da operação
         *
         * \return Custo da operação
         */
        size_t get_latency();               

        /** \brief Retorna a instrução em forma de string
         *
         * \return String representando a instrução
         */
        std::string toString();

        /** \brief Concatena os operandos duas instruções
         *
         * \param other Instrução cujos operandos serão concatenados à instrução representada por este objeto
         */
        void concatenate(Operation);

        /** \brief Adiciona um operando do tipo rótulo
         *
         * \param lbl String com o rótuto
         */
        void add_label(std::string);

        /** \brief Adiciona um operando do tipo registrador
         *
         * \param reg String com o registrador
         */
        void add_register(std::string);

        /** \brief Adiciona um operando do tipo constante
         *
         * \param cons Valor inteiro 
         */
        void add_constant(int);

        /** \brief Retorna o operando constante 
         *
         * \return O operando constante
         */
        int get_constant();

        /** \brief Retorna uma cópia dos registradores
         *
         * \return Vetor de registradores
         */
        const std::vector<std::string>& get_regs() const;

        /** \brief Retorna uma cópia dos rótulos
         *
         * \return Vetor de rótulos
         */
        const std::vector<std::string>& get_labels() const;
};
