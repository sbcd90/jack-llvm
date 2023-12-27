add_executable(jack main.cpp TypeIR.cpp IR_Codegen_Visitor.cpp ExprIR.cpp)
target_link_libraries(jack ${REQUIRED_LLVM_LIBS})
target_include_directories(jack PUBLIC include)