#include "argc/argc.h"

int main(int argc, char* argv[]) {
    Argc arg{};
    arg.parse(argc, argv);
    //argv_parse(argc, argv);
    return 0;
}
