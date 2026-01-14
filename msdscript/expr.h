#ifndef EXPR_H
#define EXPR_H

#include <string>

class Expr{
public:
    
    virtual bool equals(Expr *e) = 0;
    
    virtual ~Expr() {}

};

class NumExpr : public Expr{
public:
    
    int val;
    NumExpr(int val);
    bool equals(Expr *e) override ;
};

class AddExpr: public Expr{
public:
    Expr *lhs;
    Expr *rhs;
    AddExpr(Expr *lhs , Expr *rhs);
    bool equals(Expr *e) override;
};

class MultExpr : public Expr{
public:
    Expr *lhs ;
    Expr *rhs ;
    MultExpr(Expr *lhs , Expr *rhs);
    bool equals(Expr *e) override;
};

class VarExpr: public Expr{
public:
    std::string name ;
    VarExpr(std::string name);
    bool equals(Expr *e) override;
};

#endif
