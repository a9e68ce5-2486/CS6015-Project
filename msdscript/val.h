#ifndef VAL_H
#define VAL_H

#include "pointer.h"
#include <string>

class Expr;

CLASS(Val) {
public:
    virtual bool equals(PTR(Val) other) = 0;
    virtual PTR(Val) add_to(PTR(Val) other) = 0;
    virtual PTR(Val) mult_with(PTR(Val) other) = 0;
    virtual bool is_true() = 0;
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
    virtual PTR(Expr) to_expr() = 0;
    virtual std::string to_string() = 0;
    virtual ~Val() {}
};

class NumVal : public Val {
public:
    int val;

    explicit NumVal(int val);
    bool equals(PTR(Val) other) override;
    PTR(Val) add_to(PTR(Val) other) override;
    PTR(Val) mult_with(PTR(Val) other) override;
    bool is_true() override;
    PTR(Val) call(PTR(Val) actual_arg) override;
    PTR(Expr) to_expr() override;
    std::string to_string() override;
};

class BoolVal : public Val {
public:
    bool val;

    explicit BoolVal(bool val);
    bool equals(PTR(Val) other) override;
    PTR(Val) add_to(PTR(Val) other) override;
    PTR(Val) mult_with(PTR(Val) other) override;
    bool is_true() override;
    PTR(Val) call(PTR(Val) actual_arg) override;
    PTR(Expr) to_expr() override;
    std::string to_string() override;
};

class FunVal : public Val {
public:
    std::string formal_arg;
    PTR(Expr) body;

    FunVal(std::string formal_arg, PTR(Expr) body);
    bool equals(PTR(Val) other) override;
    PTR(Val) add_to(PTR(Val) other) override;
    PTR(Val) mult_with(PTR(Val) other) override;
    bool is_true() override;
    PTR(Val) call(PTR(Val) actual_arg) override;
    PTR(Expr) to_expr() override;
    std::string to_string() override;
};

#endif
