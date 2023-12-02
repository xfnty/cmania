#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>

#include "file.h"
#include "error.h"


TEST_CASE("File") {
    file_t f;
    err_t err = file_read(&f, "./assets/word.txt");

    REQUIRE(err == ERROR_SUCCESS);
    REQUIRE(f.size - 1 == 9);  // accounting for additional '\0' at the end
    REQUIRE(strcmp(f.data, "pineapple") == 0);

    file_free(&f);
}
