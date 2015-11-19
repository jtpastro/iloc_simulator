#include <map>
#include "Operation.hpp"

class Program{
        std::map<std::string,uint> symbol_table;
        std::map<uint,std::string> line_table;
        std::map<std::string,uint> unused_labels;
        std::vector<Operation> op_list;
    public:
        Operation get_operation(uint);
        void add_operation(Operation);
        void add_operation(std::string, Operation);
        uint get_label(std::string);
        uint get_size();
        std::string get_line(uint);
        const std::map<std::string,uint>& get_unused_labels() const;
};
