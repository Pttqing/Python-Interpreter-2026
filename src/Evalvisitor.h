#pragma once
#include <string>
#include <vector>
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H


#include "Python3ParserBaseVisitor.h"
#include "int2048.h"

struct none {
};

//函数体
struct Function {
  std::string funcname;
  std::vector<std::string> argname;
  std::vector<std::any> initval; 
  std::map<std::string, int> position;
  Python3Parser::SuiteContext *func;
};

//函数调用时的参数列表
struct Argument {
  int id = 0;
  std::string name = "";
  std::any value;
  
};

//返回的流
struct Flow {
  bool Continue, Break, Return;
  std::any val;
};

class EvalVisitor : public Python3ParserBaseVisitor {
	// TODO: override all methods of Python3ParserBaseVisitor
private:
  //每一层内的变量以及相应的值
  std::vector<std::map<std::string, std::any>> variables;
  //函数表
  std::map<std::string, Function> def_function;

  double ChangeToFloat(std::any x);
  sjtu::int2048 ChangeToInt(std::any x);
  bool ChangeToBool(std::any x);
  std::string ChangeToStr(std::any x);
  std::any calc(std::string s, std::any x, std::any y);
  void SetVar(std::string name, std::any val);
  std::any GetVar(std::string name);
  void AssignOnce(Python3Parser::TestlistContext *ctx, std::any exprN);
  void AugassignOnce(Python3Parser::TestlistContext *ctx, std::any exprN, std::string Augassign,  std::vector<std::any> old_val);
public:
  EvalVisitor();
  //文件输入，分别执行每一行
  std::any visitFile_input(Python3Parser::File_inputContext *ctx) override;

  //函数定义相关
  virtual std::any visitFuncdef(Python3Parser::FuncdefContext *ctx) override;

  virtual std::any visitParameters(Python3Parser::ParametersContext *ctx) override;

  virtual std::any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override;

  virtual std::any visitTfpdef(Python3Parser::TfpdefContext *ctx) override;

  //运行每一个语句(可能是简单语句或是复杂语句)
  virtual std::any visitStmt(Python3Parser::StmtContext *ctx) override;
  
  //简单语句(由小语句+换行组成)
  virtual std::any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override;

  //小语句(由表达语句或流语句组成)
  virtual std::any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override;

  //表达式语句：testlist + 复合赋值/连等/无
  virtual std::any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override;

  virtual std::any visitAugassign(Python3Parser::AugassignContext *ctx) override {
    return visitChildren(ctx);
  }

  //流语句
  virtual std::any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override;

  virtual std::any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override;

  virtual std::any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override;

  virtual std::any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override;

  //if while funcdef
  virtual std::any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override;

  virtual std::any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override;


  virtual std::any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override;

  virtual std::any visitSuite(Python3Parser::SuiteContext *ctx) override;
  
  //访问单个test 即or_test
  virtual std::any visitTest(Python3Parser::TestContext *ctx) override;

  //或表达式，即若干个与表达式用'or连接，注意短路，bool，允许隐式类型转换
  virtual std::any visitOr_test(Python3Parser::Or_testContext *ctx) override;

  //与表达式，若干个非表达式连接，注意事项同上
  virtual std::any visitAnd_test(Python3Parser::And_testContext *ctx) override;

  //非表达式，not + 非表达式/比较
  virtual std::any visitNot_test(Python3Parser::Not_testContext *ctx) override;

  //比较,允许使用隐式转换的前提下是同一个类型，注意==与!=，链式比较
  virtual std::any visitComparison(Python3Parser::ComparisonContext *ctx) override;

  virtual std::any visitComp_op(Python3Parser::Comp_opContext *ctx) override {
    return visitChildren(ctx);
  }

  //+或-
  virtual std::any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override;

  virtual std::any visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx) override {
    return visitChildren(ctx);
  }

  //*或/
  virtual std::any visitTerm(Python3Parser::TermContext *ctx) override;

  virtual std::any visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx) override {
    return visitChildren(ctx);
  }

  //带+-或是atom_expr
  virtual std::any visitFactor(Python3Parser::FactorContext *ctx) override;

  //atom + (可选)trailer
  virtual std::any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override;

  //(参数列表)
  virtual std::any visitTrailer(Python3Parser::TrailerContext *ctx) override;

  //最基本的
  virtual std::any visitAtom(Python3Parser::AtomContext *ctx) override;

  virtual std::any visitFormat_string(Python3Parser::Format_stringContext *ctx) override {
    return visitChildren(ctx);
  }

  //若干个test，返回一个any或者any的vector
  virtual std::any visitTestlist(Python3Parser::TestlistContext *ctx) override;

  //参数列表
  virtual std::any visitArglist(Python3Parser::ArglistContext *ctx) override;

  //参数，分为：位置参数和关键字参数
  virtual std::any visitArgument(Python3Parser::ArgumentContext *ctx) override;
};


#endif//PYTHON_INTERPRETER_EVALVISITOR_H
