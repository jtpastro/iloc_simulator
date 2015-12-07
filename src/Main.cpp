/**
\mainpage ILOC Simulator
\author Jonata Teixeira Pastro

Este simulador funciona com uma interface simples de linha de comando.
Os argumentos do programa são:
    --costs ou -c seguido do nome de um arquivo com a tabela de custos de instrução, redefine os custos das instruções.
    --frame_start ou -f seguido de um número inteiro, define o valor inicial do registrador rarp.
    --mem_size ou -t' seguido de um número inteiro, define o tamanho da memória em bytes.
    --memory ou -m, exibe o conteúdo da área de dados da memória ao fim da execução.
    --program ou -p, exibe o conteúdo da área de programa da memória ao fim da execução.
    --register ou -r, exibe o conteúdo dos registradores usados pelo programa ao fim da execução.
    --statistics ou -s, exibe as estatísticas de execução do programa.
    --debug ou -d, executa no modo de depuração.

Podem configurados os seguintes parametros do simulador: custos de execução, valor do registrador de ponteiro de quadro e tamanho da memória.
O simulador possui dois modos de execução: contínua e depuração.
Caso o argumento --debug não seja usado, o simulador entra no modo de execução contínua.
Nesse modo, o simulador executa as operações até encontrar HALT ou acessar uma posição de memória ilegal.
O modo de depuração oferece uma interface simples que permite a exibição de registradores, do programa, da memória e de pontos de parada.
A execução no modo depuração pode ser feita em passos, executando uma instrução a cada comando ou até encontar pontos de parados adicionados pelo usuário.
Ao final da execução poderão ser exibidos os conteúdos da memória de dados e programa, registradores e estatísticas de execução, conforme passado por argumentos de chamada.
*/

#include <stdlib.h>
#include <argp.h> //argp_parse
#include <locale.h> //setlocale
#include <libintl.h>//textdomain
#include <iostream> //cout
#include <fstream> //ifstream
#include <string> //std::string
#include <sstream> //stringstream
#include <exception> //std::out_of_range
#include "Machine.hpp" //Machine
#include "SimulationError.hpp" //SimulationError
#include "parser.hpp" //yyparse

#define DEFAULT_FRAME_ADDRESS 1024
#define DEFAULT_MEMORY_SIZE 8192

extern "C"
{
    extern int yylex(void);
    extern FILE*   yyin;
}
extern Program program;
int yyparse();

static struct argp_option options[] = {
    {"costs",		'c', "COST_FILE", 0, "Arquivo com tabela de custos por instrução."},
    {"frame_start", 'f', "SIZE", 0, "Define o valor inicial do registrador rarp."},
    {"mem_size",    't', "SIZE", 0, "Define o tamanho da memória em bytes."},
    {"memory",		'm',  0, 0, "Exibe o conteúdo da área de dados da memória."},
    {"program",     'p',  0, 0, "Exibe o conteúdo da área de programa da memória."},
    {"register",    'r',  0, 0, "Exibe o conteúdo dos registradores usados pelo programa."},
    {"statistics",  's',  0, 0, "Exibe as estatísticas de execução do programa."},
    {"debug",       'd',  0, 0, "Executa no modo de depuração."},
    { 0 }
};

struct arguments { 
    std::string iloc_file;   
    std::string costs_file;
    bool mem, reg, prog, stat,debug;
    int fp, mem_size;
};

static error_t parse_options (int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = (struct arguments*)(state->input);
    switch (key){
        case 'c': arguments->costs_file = arg; break;
        case 'm': arguments->mem = true; break;
        case 'r': arguments->reg = true; break;
        case 's': arguments->stat = true; break;    
        case 'p': arguments->prog = true; break;
        case 'd': arguments->debug = true; break;
        case 'f': arguments->fp = atoi(arg); break;
        case 't': arguments->mem_size = atoi(arg); break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 1)        //Argumentos excessivos
                argp_usage (state);

            arguments->iloc_file = arg;

            break;
        case ARGP_KEY_END:
            if (state->arg_num < 1)         // Argumentos insuficientes
                  argp_usage (state);
            break;
        default: return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

const char *argp_program_version = "ilocsim 1.0";
const char *argp_program_bug_address = "jtpastro@inf.ufrgs.br";

static char doc[] = "ilocsim - um simulador para a linguagem ILOC.\n";

static char args_doc[] = "ARQUIVO_ILOC";

static struct argp argp = { options, parse_options, args_doc, doc};

/** \brief Lê o arquivo de parametrização dos custos de instrução
    
    O arquivo possui o formato de uma dupla de CODIGO_DE_OPERACAO e VALOR por linha e separados por espaços em branco.
 
    \param filename O caminho e nome para o arquivo de custos
*/
void read_costs (std::string filename) {
    std::ifstream myfile (filename);
    if (myfile.is_open()) {
        int latency, line_count=0;
        std::string opcode, line;
        while ( getline (myfile,line) ) {
            line_count++;
            std::stringstream stream(line);
            try {
                stream >> opcode >> latency;
                Operation::set_latency(Operation::string_to_opcode(opcode), latency);
            } catch (const std::out_of_range& oor) {
                std::stringstream ss;
                ss << "Erro lendo arquivo " << filename << " na linha " << line_count << ": " << line << std::endl;
                throw SimulationError(ss.str());
            }
        }
        myfile.close();
    } else {
        std::stringstream ss;
        ss << "Não foi possível abrir o arquivo: " << filename << "." << std::endl;
        throw SimulationError(ss.str());
    }
}

/** \brief Lê o arquivo com programa ILOC
    
    Um arquivo ILOC possui o formato de uma instruções ILOC por linha.
    O arquivo é processado pela função yyparse.
    Essa função usa os analisadores léxico e sintático gerados por iloc.l e iloc.y.
 
    \param filename O caminho e nome para o arquivo ILOC
*/
void read_iloc(std::string filename) {
	FILE *myfile = fopen(filename.c_str(), "r");
	if (!myfile) {
		std::stringstream ss;
        ss << "Não foi possível abrir o arquivo: " << filename << "." << std::endl;
        throw SimulationError(ss.str());
	}
	yyin = myfile;

	do {
		yyparse();
	} while (!feof(yyin));	
    
    fclose(yyin);
}

/** \brief Lista os comandos do modo depuração */
void list_commands(){
    std::cout << "Comandos disponíveis:" << std::endl;
    std::cout << "\t?/h: lista comandos disponíveis." << std::endl;
    std::cout << "\te: executa até encontrar um breakpoint ou HALT." << std::endl;
    std::cout << "\tn: executa a próxima operação." << std::endl;
    std::cout << "\tb: exibe todos os breakpoints." << std::endl;    
    std::cout << "\tb <NUM>: adiciona ou remove breakpoint na posição <NUM>." << std::endl;
    std::cout << "\tm: exibe todos os endereços de memória na área de dados." << std::endl;
    std::cout << "\tm <NUM>: exibe o conteúdo de memória na posição <NUM>." << std::endl;
    std::cout << "\tp: exibe os endereços de memória contendo o programa em execução." << std::endl;
    std::cout << "\tr: exibe o conteúdo de todos os registradores em uso." << std::endl;
    std::cout << "\tr <REG>: exibe o conteúdo do registrador <REG>." << std::endl;
    std::cout << "\tq: encerra o simulador." << std::endl;
}

/** \brief Executa o programa em modo de depuração

    Recebe um ambiente de execução Machine e gera um prompt de comando.
    Esse prompt consome um caractere e, quando cabível, um argumento conforme as opções a seguir: 

    ?/h: lista comandos disponíveis.
    e: executa até encontrar um breakpoint ou HALT.
    n: executa a próxima operação.
    b: exibe todos os breakpoints.    
    b <NUM>: adiciona ou remove breakpoint na posição <NUM>.
    m: exibe todos os endereços de memória na área de dados.
    m <NUM>: exibe o conteúdo de memória na posição <NUM>.
    p: exibe os endereços de memória contendo o programa em execução.
    r: exibe o conteúdo de todos os registradores em uso.
    r <REG>: exibe o conteúdo do registrador <REG>.
    q: encerra o simulador.

    \param mac O ambiente de execução
*/
void debug(Machine* mac){
    std::map<size_t,bool> breakpoints;
    char command;
    size_t num;
    std::string reg, line;
    bool running=true; 
    list_commands();
    do {
        command = '\0';
        std::cout << "> ";
        std::getline (std::cin, line);
        std::stringstream ss;
        ss << line;
        ss >> command;
        switch(command){
            case '?': case 'h': list_commands(); break;
            case 'e': while((running=mac->execute_operation()) && !breakpoints[mac->get_register("pc")]); break;
            case 'n': running=mac->execute_operation(); break;
            case 'b':
                if((ss >> num))
                    if(num/4 < program.get_size())
                        breakpoints[num/4] = !breakpoints[num/4];
                    else
                        std::cout << "Breakpoint fora dos limites do programa." << std::endl;
                else
                    for(auto& b : breakpoints)
                        if(b.second)
                            std::cout << mac->prog_state(b.first) << std::endl;
                break;
            case 'm':
                if((ss >> num)){
                    if(num/4 < program.get_size())
                        std::cout << mac->prog_state(num/4) << std::endl;
                    else
                        std::cout << mac->mem_state(num) << std::endl;
                } else
                    std::cout << mac->mem_state();
                break;
            case 'p': std::cout << mac->prog_state(); break;
            case 'r':
                if((ss >> reg)){
                    if ((reg=="pc") || (reg.front()=='r'))
                        std::cout << reg << ": " << mac->get_register(reg) << std::endl;
                    else
                        std::cout << "Registrador inválido." << std::endl;
                } else
                    std::cout << mac->reg_state();
                break;
            case 'q': running = false; break;
            default: std::cout << "Comando inválido." << std::endl; break;
        }
    } while(running);
}

int main(int argc, char** argv) {
    setlocale (LC_ALL, "");
    textdomain("libc");
    try{
        struct arguments arguments = {"","",false,false,false,false,false,DEFAULT_FRAME_ADDRESS, DEFAULT_MEMORY_SIZE};    
        if (argp_parse (&argp, argc, argv, 0, 0, &arguments) == ARGP_KEY_ERROR){
            std::stringstream ss;
            ss << argv[0] << " argumentos inválidos." << std::endl;
            throw SimulationError(ss.str());
        }
        if(!arguments.costs_file.empty())
            read_costs(arguments.costs_file);

        read_iloc(arguments.iloc_file);

        Machine mach(program, arguments.mem_size, arguments.fp);
        if(arguments.debug)
            debug(&mach);
        else{
            mach.run();
            if(arguments.prog)
                std::cout << "Programa:\n" << mach.prog_state() << std::endl;
            if(arguments.mem)
                std::cout << "Memória:\n" << mach.mem_state() << std::endl;
            if(arguments.reg)
                std::cout << "Registradores:\n" << mach.reg_state() << std::endl;
            if(arguments.stat)
                std::cout << mach.exec_state();
        }
    } catch (SimulationError& simerr) {
        std::cerr << simerr.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
};

