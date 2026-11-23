#include "Evalvisitor.h"
#include "Python3Parser.h"
#include "int2048.h"
#include <any>
#include <string>
#include <vector>
#include <iomanip>

std::any EvalVisitor::ChangeToFloat(std::any x) {
  if (std::any_cast<std::string>(&x) != nullptr) {
    std::string a = std::any_cast<std::string>(x);
    double b = 0;
    int sz = a.size(), cnt = 0;
    for (int i = sz - 1; i >= 0; i--) {
      if (a[i] != '.') {
        b = b + a[i] - '0';
        b *= 10;
        cnt ++;
      } else {
        while (cnt--) {
          b /= 10;
        }
      }
      
    }
    return b;
  }

}

std::any EvalVisitor::ChangeToInt(std::any x) {
  if (std::any_cast<std::string>(&x) != nullptr) {
    sjtu::int2048 res(std::any_cast<std::string>(x));
    return res;
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

std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
  auto testlist = ctx -> testlist();
  if (ctx -> augassign()) {

  } else {
    return visit(ctx->testlist(0));
  }
}

std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
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
  bool res = 0;

  return res;
}

std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
  std::vector<Python3Parser::Not_testContext *> not_test = ctx -> not_test();
  if (not_test.size() == 1) {
    return visit(not_test[0]);
  }
  bool res = 0;

  return res;
}

std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
  if(ctx -> NOT()) {

  } else {
    return visit(ctx -> comparison());
  }
}

std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
  std::vector<Python3Parser::Arith_exprContext *> arith_expr = ctx -> arith_expr();
  std::vector<Python3Parser::Comp_opContext *> comp_op = ctx -> comp_op();
  if (comp_op.size() == 0) {
    return visit(arith_expr[0]);
  }

}

std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
  std::vector<Python3Parser::TermContext *> term = ctx -> term();
  std::vector<Python3Parser::Addorsub_opContext *> addorsub_op = ctx -> addorsub_op();
  if (addorsub_op.size() == 0) {
    return visit(term[0]);
  }

}

std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
  std::vector<Python3Parser::FactorContext *> factor = ctx -> factor();
  std::vector<Python3Parser::Muldivmod_opContext *> muldivmod = ctx -> muldivmod_op();
  if (muldivmod.size() == 0) {
    return visit(factor[0]);
  }

}

std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
  if (ctx ->ADD()) {

  } else if (ctx -> MINUS()) {

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
    std::cout << std::fixed << std::setprecision(6) << std::any_cast<std::string>(x);
  }
  if (std::any_cast<bool>(&x) != nullptr) {
    bool t = std::any_cast<bool>(x);
    std::cout << (t ? "TRUE" : "FALSE");
  }
  if (std::any_cast<none>(&x) != nullptr) {
    std::cout << "NONE";
  }
}

std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
  if (ctx -> arglist()) {
    return visit(ctx -> arglist());
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
    
  } else {
    // std::cout << "visiting Argument " << ctx -> getText() << "\n";
    arg.name = "";
    arg.value = visit(ctx -> test(0));
    return arg;
  }
}

std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
  if (ctx -> NAME()) {
    return ctx -> NAME() -> getText();
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
    // std::cout << "visiting atom : " << ctx -> getText() << "\n";
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
          }
          i++;
        } else {
          res += x[i];
        }
      }
    }
    return res;
  }
}