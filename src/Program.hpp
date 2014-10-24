#include <string>
#include <vector>
#include <map>
#include "Operation.hpp"

class Program{
        std::map<std::string,int> symbol_table;
        std::vector<Operation> op_list;
        int missing_refs;
    public:
        Operation get_operation(int);
        int get_missing_refs();
        void add_operation(Operation);
        void add_operation(std::string, Operation);
        int get_label(std::string);
        bool check_labels();
};
