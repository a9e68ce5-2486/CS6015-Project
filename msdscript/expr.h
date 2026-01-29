/**
 * expr.h
 */
#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <iostream>
#include <sstream>

// 優先權列舉 (根據提示)
typedef enum {
  prec_none = 0,
  prec_add = 1,
  prec_mult = 2
} precedence_t;

class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    virtual int interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst(std::string name, Expr* replacement) = 0;

    // --- 作業 3 新增部分 ---

    // 1. 嚴格列印模式 (無空格、全括號)
    virtual void printExp(std::ostream &ot) = 0;
    
    // 呼叫 printExp 的輔助函數
    std::string to_string();

    // 2. 美觀列印模式 (有空格、根據優先權省略括號)
    // 這是公開介面，不需要子類別實作
    void pretty_print(std::ostream &ot);
    
    // 呼叫 pretty_print 的輔助函數 (為了測試方便)
    std::string to_pretty_string();

    // 實際的遞迴邏輯，需要傳遞當前的優先權上下文
    // 子類別必須實作這個邏輯
    virtual void pretty_print_at(std::ostream &ot, precedence_t p_prec) = 0;

    virtual ~Expr() {}
};

class NumExpr : public Expr {
public:
    int val;
    NumExpr(int val);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string name, Expr* replacement) override;

    // 新增實作
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec) override;
};

class AddExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    AddExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string name, Expr* replacement) override;

    // 新增實作
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec) override;
};

class MultExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    MultExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string name, Expr* replacement) override;

    // 新增實作
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec) override;
};

class VarExpr : public Expr {
public:
    std::string name;
    VarExpr(std::string name);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string name, Expr* replacement) override;

    // 新增實作
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec) override;
};

#endif
