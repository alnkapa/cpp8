#include "argc/argc.h"

int main(int argc, char* argv[]) {
    arg::Argc arg{};
    if (arg.parse(argc, argv) != 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
