#include "block.h"

#include <fstream>

// закрыть файл
void close_file(std::fstream* ptr) {
    if (ptr->is_open()) ptr->close();
};
