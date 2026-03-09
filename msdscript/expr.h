#ifndef EXPR_H
#define EXPR_H

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

class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    virtual Val* interp() = 0;
    virtual Expr* subst(std::string name, Expr* replacement) = 0;

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
    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string name, Expr* replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class BoolExpr : public Expr {
public:
    bool val;
    BoolExpr(bool val);
    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string name, Expr* replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class AddExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    AddExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string name, Expr* replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class MultExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    MultExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string name, Expr* replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class EqExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    EqExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string name, Expr* replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class VarExpr : public Expr {
public:
    std::string name;
    VarExpr(std::string name);
    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string name, Expr* replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class LetExpr : public Expr {
public:
    std::string lhs;
    Expr *rhs;
    Expr *body;

    LetExpr(std::string lhs, Expr *rhs, Expr *body);
    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string name, Expr* replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class IfExpr : public Expr {
public:
    Expr *test_part;
    Expr *then_part;
    Expr *else_part;

    IfExpr(Expr *test_part, Expr *then_part, Expr *else_part);
    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string name, Expr* replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class FunExpr : public Expr {
public:
    std::string formal_arg;
    Expr *body;

    FunExpr(std::string formal_arg, Expr *body);
    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string name, Expr* replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

class CallExpr : public Expr {
public:
    Expr *to_be_called;
    Expr *actual_arg;

    CallExpr(Expr *to_be_called, Expr *actual_arg);
    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string name, Expr* replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) override;
};

#endif
