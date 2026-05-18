add_executable(jack1 main.cpp TypeIR.cpp IR_Codegen_Visitor.cpp ExprIR.cpp include/Parser.h)
target_link_libraries(jack1 ${REQUIRED_LLVM_LIBS})
target_include_directories(jack1 PUBLIC include)