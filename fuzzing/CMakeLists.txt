
set(FUZZER_COMPILE_OPTS "-fsanitize=address" "-g" "-O1" "-fstack-protector-all")
set(FUZZER_COMPILE_DEFS "_FORTIFY_SOURCE=2")
set(FUZZER_LINK_FLAGS "-fsanitize=address")

include_directories(../include)

add_executable(vector_afl vector_afl.cpp)

set_target_properties(vector_afl PROPERTIES
    LINK_FLAGS ${FUZZER_LINK_FLAGS}
    COMPILE_OPTIONS "${FUZZER_COMPILE_OPTS}"
    COMPILE_DEFINITIONS "${FUZZER_COMPILE_DEFS}"
    INSTALL_RPATH_USE_LINK_PATH ON)
