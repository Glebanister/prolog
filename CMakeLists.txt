cmake_minimum_required(VERSION 3.0.0)
project(fp_lexer VERSION 0.1.0)

set(EXE fp_lexer)

add_executable(${EXE} src/main.cpp)

target_include_directories(${EXE} PUBLIC peach/core/include)

target_include_directories(${EXE} PRIVATE peach/core/include)
target_include_directories(${EXE} PRIVATE peach/core/include/Evaluation)
target_include_directories(${EXE} PRIVATE peach/core/include/Interpretation)
target_include_directories(${EXE} PRIVATE peach/core/include/Tokenization)
target_include_directories(${EXE} PRIVATE peach/core/include/Tokenization/Finders)
target_include_directories(${EXE} PRIVATE peach/core/include/Tools)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set_property(TARGET ${EXE}
             PROPERTY CXX_STANDARD 17)
