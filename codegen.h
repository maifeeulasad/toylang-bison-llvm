#include <stack>
#include <typeinfo>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/PassManager.h>
//#include <llvm/Bitcode/ReaderWriter.h>
//#include <llvm/Analysis/Verifier.h>
//#include <llvm/Assembly/PrintModulePass.h>
//#include <llvm/Target/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
//#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
//#include <llvm/Support/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/raw_ostream.h>


using namespace llvm;

class NBlock;

class CodeGenBlock {
public:
	
	BasicBlock *block;
	std::map<std::string, Value*> locals;
};

class CodeGenContext {
	std::stack<CodeGenBlock *> blocks;
	Function *mainFunction;
public:
	static LLVMContext context;
	
	Module *module;
	CodeGenContext() { module = new Module("main", CodeGenContext::context); }

	void generateCode(NBlock& root);
	GenericValue runCode();
	std::map<std::string, Value*>& locals() { return blocks.top()->locals; }
	BasicBlock *currentBlock() { return blocks.top()->block; }
	void pushBlock(BasicBlock *block) { blocks.push(new CodeGenBlock()); blocks.top()->block = block; }
	void popBlock() { CodeGenBlock *top = blocks.top(); blocks.pop(); delete top; }
};
