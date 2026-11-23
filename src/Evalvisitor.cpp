#include "Evalvisitor.h"
#include "Python3Parser.h"
#include "int2048.h"
#include <any>
#include <string>
#include <vector>
#include <iomanip>
#include <map>

EvalVisitor::EvalVisitor() {
  std::map<std::string, std::any> mp;
  mp.clear();
  variables.push_back(mp);
}

double EvalVisitor::ChangeToFloat(std::any x) {
  if (std::any_cast<std::string>(&x) != nullptr) {
    return std::stod(std::any_cast<std::string>(x));
  }
  if (std::any_cast<sjtu::int2048>(&x) != nullptr) {
    double res = (std::any_cast<sjtu::int2048>(x).ChangeToDouble());
    return res;
  }
  if (std::any_cast<bool>(&x) != nullptr) {
    double res = std::any_cast<bool>(x);
    return res;
  }
}

sjtu::int2048 EvalVisitor::ChangeToInt(std::any x) {
  if (std::any_cast<std::string>(&x) != nullptr) {
    sjtu::int2048 res(std::any_cast<std::string>(x));
    return res;
  }
  if (std::any_cast<bool>(&x) != nullptr) {
    sjtu::int2048 res(std::any_cast<bool>(x));
    return res;
  }
  if (std::any_cast<double>(&x) != nullptr) {
    sjtu::int2048 res((long long)std::any_cast<double>(x));
    return res;
  }
  if (std::any_cast<sjtu::int2048>(&x) != nullptr) {
    return std::any_cast<sjtu::int2048>(x);
  }
}

bool EvalVisitor::ChangeToBool(std::any x) {
  if (std::any_cast<none>(&x) != nullptr) {
    return false;
  }
  if (std::any_cast<bool>(&x) != nullptr) {
    return std::any_cast<bool>(x);
  }
  if (std::any_cast<sjtu::int2048>(&x) != nullptr) {
    return std::any_cast<sjtu::int2048>(x) != 0;
  }
  if (std::any_cast<double>(&x) != nullptr) {
    return std::any_cast<double>(x) != 0;
  }
  if (std::any_cast<std::string>(&x) != nullptr) {
    return std::any_cast<std::string>(x) != "";
  }
}

std::any EvalVisitor::calc(std::string s, std::any x, std::any y) {
  if (s == "+") {
    if (std::any_cast<std::string>(&x) != nullptr) {
      return std::any_cast<std::string>(x) + std::any_cast<std::string>(y);
    }
    if (std::any_cast<double>(&x) != nullptr || std::any_cast<double>(&y) != nullptr) {
      return ChangeToFloat(x) + ChangeToFloat(y);
    }
    return ChangeToInt(x) + ChangeToInt(y);
  }
  if (s == "-") {
    if (std::any_cast<double>(&x) != nullptr || std::any_cast<double>(&y) != nullptr) {
      return ChangeToFloat(x) - ChangeToFloat(y);
    }
    return ChangeToInt(x) - ChangeToInt(y);
  }
  if (s == "*") {
    if (std::any_cast<std::string>(&x) != nullptr || std::any_cast<std::string>(&y) != nullptr) {
      if (std::any_cast<std::string>(&y) != nullptr) {
        swap(x, y);
      }
      sjtu::int2048 t = ChangeToInt(y);
      std::string res = "", a = std::any_cast<std::string>(x);
      while (t > 0) {
        res += a;
        t -= 1;
      }
      return res;
    }
    if (std::any_cast<double>(&x) != nullptr || std::any_cast<double>(&y) != nullptr) {
      return ChangeToFloat(x) * ChangeToFloat(y);
    }
    return ChangeToInt(x) * ChangeToInt(y);
  }
  if (s == "/") {
    return ChangeToFloat(x) / ChangeToFloat(y);
  }
  if (s == "//") {
    if (std::any_cast<double>(&x) != nullptr || std::any_cast<double>(&y) != nullptr) {
      double xfloat = ChangeToFloat(x), yfloat = ChangeToFloat(y);
      return floor(xfloat / yfloat);
    }
    return ChangeToInt(x) / ChangeToInt(y);
  }
  if (s == "%") {
    if (std::any_cast<double>(&x) != nullptr || std::any_cast<double>(&y) != nullptr) {
      double xfloat = ChangeToFloat(x), yfloat = ChangeToFloat(y);
      return xfloat - yfloat * floor(xfloat / yfloat);
    }
    return ChangeToInt(x) % ChangeToInt(y);
  }
  if (s == "<") {
    if (std::any_cast<std::string>(&x) != nullptr && std::any_cast<std::string>(&y) != nullptr) {
      return std::any_cast<std::string>(x) < std::any_cast<std::string>(y);
    }
    if (std::any_cast<double>(&x) != nullptr || std::any_cast<double>(&y) != nullptr) {
      return ChangeToFloat(x) < ChangeToFloat(y);
    }
    return ChangeToInt(x) < ChangeToInt(y);
  }
  if (s == ">") {
    return calc("<", y, x);
  }
  if (s == "<=") {
    return !std::any_cast<bool>(calc(">", x, y));
  }
  if (s == ">=") {
    return !std::any_cast<bool>(calc("<", x, y));
  }
  if (s == "==") {
    if (std::any_cast<std::string>(&x) != nullptr && std::any_cast<std::string>(&y) != nullptr) {
      return std::any_cast<std::string>(x) == std::any_cast<std::string>(y);
    }
    if (std::any_cast<std::string>(&x) != nullptr || std::any_cast<std::string>(&y) != nullptr) {
      return false;
    }
    if (std::any_cast<none>(&x) != nullptr && std::any_cast<none>(&y) != nullptr) {
      return true;
    }
    if (std::any_cast<none>(&x) != nullptr || std::any_cast<none>(&y) != nullptr) {
      return false;
    }
    if (std::any_cast<double>(&x) != nullptr || std::any_cast<double>(&y) != nullptr) {
      return ChangeToFloat(x) == ChangeToFloat(y);
    }
    return ChangeToInt(x) == ChangeToInt(y);
  }
  if (s == "!=") {
    return  !std::any_cast<bool>(calc("==", x, y));
  }
}

std::any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) {
  for (auto stmt : ctx->stmt()) {
    visit(stmt);
  }
  return std::any();
}

std::any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) {
  if (ctx -> simple_stmt()) {
    return visit(ctx -> simple_stmt());
  } else if (ctx -> compound_stmt()){
    return visit(ctx -> compound_stmt());
  } else {
    return std::any();
  }
}

std::any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
  auto small_stmt = ctx -> small_stmt();
  // std::cout << "visiting simple : " << ctx -> getText() << "\n";
  return visit(small_stmt);
}

std::any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
  if (ctx -> flow_stmt()) {
    return visit(ctx -> flow_stmt());
  } else if (ctx -> expr_stmt()){
    return visit(ctx -> expr_stmt());
  } else {
    return std::any();
  }
}

void EvalVisitor::SetVar(std::string name, std::any val) {
  int sz = variables.size();
  if (variables[sz - 1].find(name) != variables[sz - 1].end()) {
    variables[sz - 1][name] = val;
  } else if (variables[0].find(name) != variables[0].end()) {
    variables[0][name] = val;
  } else {
    variables[sz - 1][name] = val;
  }
}

void EvalVisitor::AssignOnce(Python3Parser::TestlistContext *ctx, std::any exprN) {
  if (ctx -> test().size() == 1) {
    SetVar(ctx -> test(0) -> getText(), exprN);
    return ;
  }
  std::vector<std::any> expr = std::any_cast<std::vector<std::any>>(exprN);
  for (int i = 0; i < ctx -> test().size(); i++) {
    SetVar(ctx -> test(i) -> getText(), expr[i]);
  }
}

void EvalVisitor::AugassignOnce(Python3Parser::TestlistContext *ctx, std::any exprN, std::string Augassign, std::vector<std::any> old_val) {
  std::string op;
  if (Augassign.size() == 3) {
    op ="//";
  } else {
    op = Augassign.substr(0, 1);
  }
  // std::cout << "doing Augassign Once : " << ctx -> getText() << " " << op << " " << Augassign << "\n";
  if (ctx -> test().size() == 1) {
    // std::cout << std::any_cast<sjtu::int2048>(old_val[0]) << " " << std::any_cast<sjtu::int2048>(exprN) << "\n";
    std::any new_val = calc(op, old_val[0], exprN);
    SetVar(ctx -> test(0) -> getText(), new_val);
    // std::cout <<"finished\n";
    return ;
  }
  std::vector<std::any> expr = std::any_cast<std::vector<std::any>>(exprN);
  for (int i = 0; i < ctx -> test().size(); i++) {
    expr[i] = calc(op, old_val[i], expr[i]);
  }
  for (int i = 0; i < ctx -> test().size(); i++) {
    SetVar(ctx -> test(i) -> getText(), expr[i]);
  }
}

std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
  // std::cout << "visiting Expr_stmt : " << ctx -> getText() << "\n";
  auto testlist = ctx -> testlist();
  if (ctx -> augassign()) {
    std::vector<std::any> olds;
    olds.clear();
    Python3Parser::TestlistContext *testtodo = testlist[0];
    for (int i = 0; i < testtodo -> test().size(); i++) {
      olds.push_back(GetVar(testtodo -> test(i) -> getText()));
    }
    std::any exprN = (visit(ctx -> testlist(1)));
    AugassignOnce(ctx -> testlist(0), exprN, ctx -> augassign() -> getText(), olds);
    // std::cout << "finished visiting : " << ctx -> getText() << "\n";
    return std::any();
  } else {
    int num = ctx -> ASSIGN().size();
    if (num == 0) {
      return visit(ctx -> testlist(0));
    }
    // std::cout << num << " testlist: " << testlist[0] -> getText() << " " << testlist[1] -> getText() << "\n";
    std::any exprN = (visit(ctx -> testlist(num)));
    for (int i = num - 1; i >= 0; i--) {
      AssignOnce(testlist[i], exprN);
    }
    return std::any();
  }
}

std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
  // std::cout << "visiting testlist : " << ctx -> getText() << "\n";
  std::vector<Python3Parser::TestContext *> test = ctx -> test();
  if (test.size() == 1) {
    return visit(test[0]);
  }
  std::vector<std::any> res;
  for (auto x : test) {
    res.push_back(visit(x));
  }
  return res;
}

std::any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
  return visit(ctx -> or_test());
}

std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
  std::vector<Python3Parser::And_testContext *> and_test = ctx ->and_test();
  if (and_test.size() == 1) {
    return visit(and_test[0]);
  }
  for (int i = 0; i < and_test.size(); i++) {
    bool x = ChangeToBool(visit(and_test[i]));
    if (x) {
      return true;
    }
  }
  return false;
}

std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
  std::vector<Python3Parser::Not_testContext *> not_test = ctx -> not_test();
  if (not_test.size() == 1) {
    return visit(not_test[0]);
  }
  for (int i = 0; i < not_test.size(); i++) {
    bool x = ChangeToBool(visit(not_test[i]));
    if (!x) {
      return false;
    }
  }
  return true;
}

std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
  if(ctx -> NOT()) {
    return !ChangeToBool(visit(ctx -> not_test()));
  } else {
    return visit(ctx -> comparison());
  }
}

std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
  std::vector<Python3Parser::Arith_exprContext *> arith_expr = ctx -> arith_expr();
  std::vector<Python3Parser::Comp_opContext *> comp_op = ctx -> comp_op();
  // std::cout << "visiting Comparison : " << ctx -> getText() << "\n";
  if (comp_op.size() == 0) {
    return visit(arith_expr[0]);
  }
  std::any l = visit(arith_expr[0]);
  for (int i = 0; i < comp_op.size(); i++) {
    std::any r = visit(arith_expr[i + 1]);
    bool res = std::any_cast<bool>(calc(comp_op[i]->getText(), l, r));
    if (!res) {
      return false;
    }
    l = r;
  }
  return true;
}

std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
  std::vector<Python3Parser::TermContext *> term = ctx -> term();
  std::vector<Python3Parser::Addorsub_opContext *> addorsub_op = ctx -> addorsub_op();
  if (addorsub_op.size() == 0) {
    return visit(term[0]);
  }
  std::any res = visit(term[0]);
  for (int i = 0; i < addorsub_op.size(); i++) {
    std::any r = visit(term[i + 1]);
    res = calc(addorsub_op[i]->getText(), res, r);
  }
  return res;
}

std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
  std::vector<Python3Parser::FactorContext *> factor = ctx -> factor();
  std::vector<Python3Parser::Muldivmod_opContext *> muldivmod = ctx -> muldivmod_op();
  if (muldivmod.size() == 0) {
    return visit(factor[0]);
  }
  std::any res = visit(factor[0]);
  for (int i = 0; i < muldivmod.size(); i++) {
    std::any r = visit(factor[i + 1]);
    res = calc(muldivmod[i]->getText(), res, r);
  }
  return res;
}

std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
  if (ctx ->ADD()) {
    return visit(ctx -> factor());
  } else if (ctx -> MINUS()) {
    return calc("-", (sjtu::int2048)0, visit(ctx -> factor()));
  } else {
    return visit(ctx -> atom_expr());
  }
}

std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
  // std::cout << "visiting Atom_expr " << ctx ->getText() << "\n";
  if (ctx -> trailer()) {
    std::string func_name = ctx -> atom() -> getText();
    if (func_name == "print") {
      std::vector<Argument> x = std::any_cast<std::vector<Argument>>(visit(ctx -> trailer()));
      // std::cout << "finished trailer" << x.size() << "\n";
      for (int i = 0; i < x.size(); i++) {
        Argument arg = x[i];
        if (i != 0) {
          std::cout << " ";
        }
        PrintOne(arg.value);
      }
      std::cout << "\n";
      return std::any();
    }
  } else {
    return visit(ctx -> atom());
  }
}

void EvalVisitor::PrintOne(std::any x) {
  if (std::any_cast<std::string>(&x) != nullptr) {
    std::cout << std::any_cast<std::string>(x);
  }
  if (std::any_cast<sjtu::int2048>(&x) != nullptr) {
    std::cout << std::any_cast<sjtu::int2048>(x);
  }
  if (std::any_cast<double>(&x) != nullptr) {
    std::cout << std::fixed << std::setprecision(6) << std::any_cast<double>(x);
  }
  if (std::any_cast<bool>(&x) != nullptr) {
    bool t = std::any_cast<bool>(x);
    std::cout << (t ? "True" : "False");
  }
  if (std::any_cast<none>(&x) != nullptr) {
    std::cout << "None";
  }
}

std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
  if (ctx -> arglist()) {
    return visit(ctx -> arglist());
  } else {
    return std::vector<Argument>{};
  }
}

std::any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
  std::vector<Argument> arglist;
  int cnt = 0;
  for (auto x : ctx -> argument()) {
    Argument u = std::any_cast<Argument>(visit(x));
    if (u.name == "") {
      u.id = cnt++;
    }
    arglist.push_back(u);
  }
  return arglist;
}

std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
  Argument arg;
  if (ctx -> ASSIGN()) {
    arg.name = ctx -> test(0) -> getText();
    arg.value = visit(ctx -> test(1));
    return arg;
  } else {
    // std::cout << "visiting Argument " << ctx -> getText() << "\n";
    arg.name = "";
    arg.value = visit(ctx -> test(0));
    return arg;
  }
}

std::any EvalVisitor::GetVar(std::string name) {
  int sz = variables.size();
  if (variables[sz - 1].find(name) != variables[sz - 1].end()) {
    return variables[sz - 1][name];
  }
  if (variables[0].find(name) != variables[0].end()) {
    return variables[0][name];
  }
}

std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
// std::cout << "visiting atom : " << ctx -> getText() << "\n";
  if (ctx -> NAME()) {
    std::string tar = ctx -> NAME() -> getText();
    return GetVar(tar);
  }
  if (ctx -> NUMBER()) {
    std::string name = ctx ->NUMBER() -> getText();
    // std::cout << "visiting Atom : " << name << "\n";
    if (name.find('.') != std::string::npos) {
      return ChangeToFloat(name);
    } else {
      sjtu::int2048 x(name);
      return x;
    }
  }
  if (ctx -> NONE()) {
    return none();
  }
  if (ctx -> TRUE()) {
    return true;
  }
  if (ctx -> FALSE()) {
    return false;
  }
  if (ctx -> format_string()) {
    return visitFormat_string(ctx -> format_string());
  }
  if (ctx -> OPEN_PAREN()) {
    return visit(ctx -> test());
  }
  if (!ctx -> STRING().empty()) {
    std::string res = "";
    for (auto singleSTRING : ctx -> STRING()) {
      std::string x = singleSTRING -> getText();
      x = x.substr(1, x.size() - 2);
      for (int i = 0; i < x.size(); i++) {
        if (x[i] == '\\') {
          char ch = x[i + 1];
          switch (ch) {
            case 'n': res += '\n';break;
            case 't': res += '\t';break;
            case 'r': res += '\r';break;
            case 'a': res += '\a';break;
            case 'b': res += '\b';break;
            case 'f': res += '\f';break;
            case 'v': res += '\v';break;
            case '\\': res += '\\';break;
            case '\"': res += '\"';break;
            case '\'': res += '\'';break;
            default: res += ch;
          }
          i++;
        } else {
          res += x[i];
        }
      }
    }
    // std::cout << "finished with return : " << res << "\n";
    return res;
  }
}

std::any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
  if (ctx -> if_stmt()) {
    return visit(ctx -> if_stmt());
  }
  if (ctx -> while_stmt()) {
    return visit(ctx -> while_stmt());
  }
  if (ctx -> funcdef()) {
    return visit(ctx -> funcdef());
  }
}

std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
  for (int i = 0; i < ctx -> test().size(); i++) {
    if (ChangeToBool(visit(ctx -> test(i)))) {
      return visit(ctx -> suite(i));
    }
  }
  if (ctx -> ELSE()) {
    return visit(ctx -> suite(ctx -> test().size()));
  }
}

std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
  while(ChangeToBool(visit(ctx -> test()))) {
    // std::cout << "whiling\n";
    auto x = visit(ctx -> suite());
    // std::cout << "here\n";
    if (std::any_cast<Flow>(&x) != nullptr) {
      Flow t = std::any_cast<Flow>(x);
      if (t.Break) {
        break;
      }
      if (t.Return) {
        return t;
      }
    }
    // std::cout << "nothing\n";
    // std::cout << std::any_cast<sjtu::int2048>(GetVar("i")) << "\n";
  }
  return std::any();
}

std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
  if (ctx -> simple_stmt()) {
    return visit(ctx -> simple_stmt());
  } else {
    for (int i = 0; i < ctx -> stmt().size(); i++) {
      // std::cout << "visiting suite" << ctx -> stmt(i) -> getText() << "\n";
      auto x = visit(ctx -> stmt(i));
      // std::cout << "finishing " << ctx -> getText() << "\n";
      if (std::any_cast<Flow>(&x) != nullptr) {
        Flow t = std::any_cast<Flow>(x);
        return t;
      }
    }
    return std::any();
  }
}

std::any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
  if (ctx -> break_stmt()) {
    return visit(ctx -> break_stmt());
  }
  if (ctx -> continue_stmt()) {
    return visit(ctx -> continue_stmt());
  }
  if (ctx -> return_stmt()) {
    return visit(ctx -> return_stmt());
  }
}

std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) {
  Flow t;
  t.Break = 1;
  t.Continue = 0;
  t.Return = 0;
  return t;
}

std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) {
  Flow t;
  t.Break = 0;
  t.Continue = 1;
  t.Return = 0;
  return t;
}

std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
  Flow t;
  t.Break = 0;
  t.Continue = 0;
  t.Return = 1;
  if (ctx -> testlist()) {
    t.val = visit(ctx -> testlist());
  }
  
  return t;
}