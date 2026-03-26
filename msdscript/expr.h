#ifndef EXPR_H
#define EXPR_H

#include "pointer.h"
#include <iostream>
#include <sstream>
#include <string>

class Val;

typedef enum {
  prec_none = 0,
  prec_eq = 1,
  prec_add = 2,
  prec_mult = 3,
  prec_call = 4
} precedence_t;

CLASS(Expr) {
public:
    virtual bool equals(PTR(Expr) e) = 0;
    virtual PTR(Val) interp() = 0;
    virtual PTR(Expr) subst(std::string name, PTR(Expr) replacement) = 0;

    virtual void printExp(std::ostream &ot) = 0;

    std::string to_string();
    void pretty_print(std::ostream &ot);
    std::string to_pretty_string();

    virtual void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) = 0;

    virtual ~Expr() {}
};

class NumExpr : public Expr {
public:
    int val;
    NumExpr(int val);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp() override;
    PTR(Expr) subst(std::string name, PTR(Expr) replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class BoolExpr : public Expr {
public:
    bool val;
    BoolExpr(bool val);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp() override;
    PTR(Expr) subst(std::string name, PTR(Expr) replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class AddExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    AddExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp() override;
    PTR(Expr) subst(std::string name, PTR(Expr) replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class MultExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp() override;
    PTR(Expr) subst(std::string name, PTR(Expr) replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class EqExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    EqExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp() override;
    PTR(Expr) subst(std::string name, PTR(Expr) replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class VarExpr : public Expr {
public:
    std::string name;
    VarExpr(std::string name);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp() override;
    PTR(Expr) subst(std::string name, PTR(Expr) replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class LetExpr : public Expr {
public:
    std::string lhs;
    PTR(Expr) rhs;
    PTR(Expr) body;

    LetExpr(std::string lhs, PTR(Expr) rhs, PTR(Expr) body);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp() override;
    PTR(Expr) subst(std::string name, PTR(Expr) replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class IfExpr : public Expr {
public:
    PTR(Expr) test_part;
    PTR(Expr) then_part;
    PTR(Expr) else_part;

    IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp() override;
    PTR(Expr) subst(std::string name, PTR(Expr) replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class FunExpr : public Expr {
public:
    std::string formal_arg;
    PTR(Expr) body;

    FunExpr(std::string formal_arg, PTR(Expr) body);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp() override;
    PTR(Expr) subst(std::string name, PTR(Expr) replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class CallExpr : public Expr {
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;

    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp() override;
    PTR(Expr) subst(std::string name, PTR(Expr) replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

#endif
