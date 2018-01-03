#define CATCH_CONFIG_MAIN

#include <dirent.h>
#include "catch.hpp"
#include "../TreeWalk/Interpreter/Interpreter.h"

std::string from_path(const std::string &path) {
    std::ifstream source(path);
    assert (source.good());
    return std::string((std::istreambuf_iterator<char>(source)),
                       (std::istreambuf_iterator<char>()));
}

std::vector<std::string> list_dir(const char *path) {
    DIR *dir;
    class dirent *ent;
    auto result = std::vector<std::string>();
    dir = opendir(path);
    while ((ent = readdir(dir)) != nullptr) {
        if (ent->d_name[0] != '.')
            result.push_back(ent->d_name);
    }
    closedir(dir);
    return result;
}

TEST_CASE("test") {
    for (auto &&file : list_dir("output")) {
        Interpreter interpreter = Interpreter();
        std::stringstream buffer;
        std::streambuf *sbuf = std::cout.rdbuf();
        std::cout.rdbuf(buffer.rdbuf());

        REQUIRE_NOTHROW(interpreter._interpret(from_path(("sources/" + file))));
        REQUIRE(buffer.str() == from_path(("output/" + file)));

        std::cout.rdbuf(sbuf);
    }
}
