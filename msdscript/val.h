#ifndef VAL_H
#define VAL_H

#include <string>

class Expr;

class Val {
public:
    virtual bool equals(Val* other) = 0;
    virtual Val* add_to(Val* other) = 0;
    virtual Val* mult_with(Val* other) = 0;
    virtual Expr* to_expr() = 0;
    virtual std::string to_string() = 0;
    virtual ~Val() {}
};

class NumVal : public Val {
public:
    int val;

    explicit NumVal(int val);
    bool equals(Val* other) override;
    Val* add_to(Val* other) override;
    Val* mult_with(Val* other) override;
    Expr* to_expr() override;
    std::string to_string() override;
};

#endif
