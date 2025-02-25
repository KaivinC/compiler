#include "codegen/CodeGenerator.hpp"
#include "visitor/AstNodeInclude.hpp"

#include <cassert>
#include <cstdarg>
#include <cstdio>

CodeGenerator::CodeGenerator(const char *in_file_name, const char *out_file_name, SymbolManager *symbol_manager) 
    : in_file_name(in_file_name), symbol_manager(symbol_manager) {
    this->out_fp = fopen(out_file_name, "w");
    assert(this->out_fp != NULL && "fopen() fails");
}

CodeGenerator::~CodeGenerator() {
    fclose(this->out_fp);
}

void CodeGenerator::dumpInstrs(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(this->out_fp, format, args);
    va_end(args);
}

void CodeGenerator::visit(ProgramNode &p_program) {
    // Reconstruct the hash table for looking up the symbol entry
    // Hint: Use symbol_manager->lookup(symbol_name) to get the symbol entry.
    symbol_manager->reconstructHashTableFromSymbolTable(p_program.getSymbolTable());

    // Generate RISC-V instructions for program header
    dumpInstrs("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
        "   .file \"", this->in_file_name, "\"\n",
        "   .option nopic\n",
        ".section    .text\n",
        "   .align 2\n",
        "   .globl main\n",
        "   .type main, @function\n",
        "main:\n",
        "   addi sp, sp, -128\n",
        "   sw ra, 124(sp)\n",
        "   sw s0, 120(sp)\n",       
        "   addi s0, sp, 128\n",

        "   lw ra, 124(sp)\n",     
        "   addi sp, sp, 128\n",   
        "   jr ra\n",              
        "   .size main, .-main\n" 
    );

    p_program.visitChildNodes(*this);

    // Remove the entries in the hash table
    symbol_manager->removeSymbolsFromHashTable(p_program.getSymbolTable());
}

void CodeGenerator::visit(DeclNode &p_decl) {
    p_decl.visitChildNodes(*this);
}

void CodeGenerator::visit(VariableNode &p_variable) {
    const SymbolEntry* tmp_entry = symbol_manager->lookup(p_variable.getName());
    if(tmp_entry->getLevel() ==0){
        dumpInstrs("%s%s%s%s",
            ".comm ", tmp_entry->getName(), " 4", " 4"
        );
    }
    if(p_variable.getConstantPtr()){
        
    }
}

void CodeGenerator::visit(ConstantValueNode &p_constant_value) {

}

void CodeGenerator::visit(FunctionNode &p_function) {
    // Reconstruct the hash table for looking up the symbol entry
    symbol_manager->reconstructHashTableFromSymbolTable(p_function.getSymbolTable());
    
    // Remove the entries in the hash table
    symbol_manager->removeSymbolsFromHashTable(p_function.getSymbolTable());
}

void CodeGenerator::visit(CompoundStatementNode &p_compound_statement) {
    // Reconstruct the hash table for looking up the symbol entry
    symbol_manager->reconstructHashTableFromSymbolTable(p_compound_statement.getSymbolTable());

    // Remove the entries in the hash table
    symbol_manager->removeSymbolsFromHashTable(p_compound_statement.getSymbolTable());
}

void CodeGenerator::visit(PrintNode &p_print) {
    
}

void CodeGenerator::visit(BinaryOperatorNode &p_bin_op) {
    
}

void CodeGenerator::visit(UnaryOperatorNode &p_un_op) {
    
}

void CodeGenerator::visit(FunctionInvocationNode &p_func_invocation) {
    
}

void CodeGenerator::visit(VariableReferenceNode &p_variable_ref) {
    
}

void CodeGenerator::visit(AssignmentNode &p_assignment) {
    
}

void CodeGenerator::visit(ReadNode &p_read) {
    
}

void CodeGenerator::visit(IfNode &p_if) {
    
}

void CodeGenerator::visit(WhileNode &p_while) {
    
}

void CodeGenerator::visit(ForNode &p_for) {
    // Reconstruct the hash table for looking up the symbol entry
    symbol_manager->reconstructHashTableFromSymbolTable(p_for.getSymbolTable());

    // Remove the entries in the hash table
    symbol_manager->removeSymbolsFromHashTable(p_for.getSymbolTable());
}

void CodeGenerator::visit(ReturnNode &p_return) {
    
}
