/**
 * \file expr.h
 * \brief Defines the abstract Expression class and its concrete subclasses.
 *
 * This file contains the definition of the abstract base class Expr,
 * along with its derived classes: NumExpr (numbers), AddExpr (addition),
 * MultExpr (multiplication), VarExpr (variables), and LetExpr (let-bindings).
 */

#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <iostream>
#include <sstream>

/**
 * \brief Specifies the precedence level of operators for pretty printing.
 */
typedef enum {
  prec_none = 0,  ///< No precedence (lowest)
  prec_add = 1,   ///< Precedence level for addition
  prec_mult = 2   ///< Precedence level for multiplication (highest)
} precedence_t;

/**
 * \brief Abstract base class for all expression types.
 */
class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    virtual int interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst(std::string name, Expr* replacement) = 0;

    // Strict format printing
    virtual void printExp(std::ostream &ot) = 0;
    
    // Helpers
    std::string to_string();
    void pretty_print(std::ostream &ot);
    std::string to_pretty_string();

    /**
     * \brief Helper method for pretty printing with precedence and indentation context.
     *
     * \param ot The output stream.
     * \param p_prec The precedence level of the parent expression.
     * \param let_paren Whether a LetExpr at this position needs parentheses.
     * \param pos The position of the last newline character (for indentation).
     */
    virtual void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool let_paren, std::streampos& pos) = 0;

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
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool let_paren, std::streampos& pos) override;
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
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool let_paren, std::streampos& pos) override;
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
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool let_paren, std::streampos& pos) override;
};

class VarExpr : public Expr {
public:
    std::string name;
    VarExpr(std::string name);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string name, Expr* replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool let_paren, std::streampos& pos) override;
};

class LetExpr : public Expr {
public:
    std::string lhs;
    Expr *rhs;
    Expr *body;

    LetExpr(std::string lhs, Expr *rhs, Expr *body);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string name, Expr* replacement) override;
    void printExp(std::ostream &ot) override;
    void pretty_print_at(std::ostream &ot, precedence_t p_prec, bool let_paren, std::streampos& pos) override;
};

#endif
