/**
 * \file expr.h
 * \brief Defines the abstract Expression class and its concrete subclasses.
 *
 * This file contains the definition of the abstract base class Expr,
 * along with its derived classes: NumExpr (numbers), AddExpr (addition),
 * MultExpr (multiplication), and VarExpr (variables).
 *
 * \author MSDScript Student
 * \date 2024
 */

#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <iostream>
#include <sstream>

/**
 * \brief Specifies the precedence level of operators for pretty printing.
 *
 * Used to determine when to add parentheses around expressions.
 */
typedef enum {
  prec_none = 0,  ///< No precedence (lowest), usually for the root expression.
  prec_add = 1,   ///< Precedence level for addition.
  prec_mult = 2   ///< Precedence level for multiplication (highest).
} precedence_t;

/**
 * \brief Abstract base class for all expression types.
 *
 * The Expr class defines the interface that all expression nodes must implement,
 * including evaluation, equality checking, variable substitution, and printing.
 */
class Expr {
public:
    /**
     * \brief Checks if this expression is structurally equal to another expression.
     * \param e The expression to compare against.
     * \return true if the expressions are structurally equivalent, false otherwise.
     */
    virtual bool equals(Expr *e) = 0;

    /**
     * \brief Evaluates the expression to an integer value.
     * \return The calculated integer value.
     * \throws std::runtime_error if the expression contains a variable (cannot evaluate).
     */
    virtual int interp() = 0;

    /**
     * \brief Checks if the expression contains a variable.
     * \return true if a variable is present in the expression tree, false otherwise.
     */
    virtual bool has_variable() = 0;

    /**
     * \brief Substitutes a variable with another expression.
     * * Creates a new expression tree where every occurrence of the specified variable
     * is replaced by the replacement expression. The original expression is not modified.
     *
     * \param name The name of the variable to substitute.
     * \param replacement The expression to replace the variable with.
     * \return A new Expr object with the substitution applied.
     */
    virtual Expr* subst(std::string name, Expr* replacement) = 0;

    /**
     * \brief Prints the expression in a strict format to an output stream.
     *
     * The strict format includes parentheses around every operation and contains no spaces.
     * Example: ((1+2)*3)
     *
     * \param ot The output stream to print to.
     */
    virtual void printExp(std::ostream &ot) = 0;
    
    /**
     * \brief Converts the expression to a string using the strict format.
     * \return A string representation of the expression (strict format).
     */
    std::string to_string();

    /**
     * \brief Prints the expression in a pretty format.
     *
     * This method handles spacing and omits unnecessary parentheses based on
     * operator precedence and associativity.
     *
     * \param ot The output stream to print to.
     */
    void pretty_print(std::ostream &ot);
    
    /**
     * \brief Converts the expression to a string using the pretty format.
     * \return A string representation of the expression (pretty format).
     */
    std::string to_pretty_string();

    /**
     * \brief Helper method for pretty printing with precedence context.
     *
     * \param ot The output stream to print to.
     * \param p_prec The precedence level of the parent expression.
     */
    virtual void pretty_print_at(std::ostream &ot, precedence_t p_prec) = 0;

    /**
     * \brief Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~Expr() {}
};

/**
 * \brief Represents an integer number in an expression.
 */
class NumExpr : public Expr {
public:
    int val; ///< The integer value of the number.

    /**
     * \brief Constructs a NumExpr.
     * \param val The integer value.
     */
    NumExpr(int val);

    /** \brief Checks equality with another expression. */
    bool equals(Expr *e) override;
    
    /** \brief Returns the integer value. */
    int interp() override;
    
    /** \brief Returns false as numbers contain no variables. */
    bool has_variable() override;
    
    /** \brief Returns a copy of itself as numbers cannot be substituted. */
    Expr* subst(std::string name, Expr* replacement) override;

    /** \brief Prints the number. */
    void printExp(std::ostream &ot) override;
    
    /** \brief Prints the number (ignores precedence). */
    void pretty_print_at(std::ostream &ot, precedence_t p_prec) override;
};

/**
 * \brief Represents an addition operation (e.g., 1 + 2).
 */
class AddExpr : public Expr {
public:
    Expr *lhs; ///< The left-hand side operand.
    Expr *rhs; ///< The right-hand side operand.

    /**
     * \brief Constructs an AddExpr.
     * \param lhs The left operand.
     * \param rhs The right operand.
     */
    AddExpr(Expr *lhs, Expr *rhs);

    /** \brief Checks equality with another expression. */
    bool equals(Expr *e) override;
    
    /** \brief Returns the sum of the left and right operands. */
    int interp() override;
    
    /** \brief Returns true if either operand contains a variable. */
    bool has_variable() override;
    
    /** \brief Returns a new AddExpr with substitutions applied to children. */
    Expr* subst(std::string name, Expr* replacement) override;

    /** \brief Prints (lhs+rhs) with parentheses. */
    void printExp(std::ostream &ot) override;
    
    /** \brief Prints the addition, handling precedence to minimize parentheses. */
    void pretty_print_at(std::ostream &ot, precedence_t p_prec) override;
};

/**
 * \brief Represents a multiplication operation (e.g., 3 * 4).
 */
class MultExpr : public Expr {
public:
    Expr *lhs; ///< The left-hand side operand.
    Expr *rhs; ///< The right-hand side operand.

    /**
     * \brief Constructs a MultExpr.
     * \param lhs The left operand.
     * \param rhs The right operand.
     */
    MultExpr(Expr *lhs, Expr *rhs);

    /** \brief Checks equality with another expression. */
    bool equals(Expr *e) override;
    
    /** \brief Returns the product of the left and right operands. */
    int interp() override;
    
    /** \brief Returns true if either operand contains a variable. */
    bool has_variable() override;
    
    /** \brief Returns a new MultExpr with substitutions applied to children. */
    Expr* subst(std::string name, Expr* replacement) override;

    /** \brief Prints (lhs*rhs) with parentheses. */
    void printExp(std::ostream &ot) override;
    
    /** \brief Prints the multiplication, handling precedence to minimize parentheses. */
    void pretty_print_at(std::ostream &ot, precedence_t p_prec) override;
};

/**
 * \brief Represents a variable in an expression (e.g., x).
 */
class VarExpr : public Expr {
public:
    std::string name; ///< The name of the variable.

    /**
     * \brief Constructs a VarExpr.
     * \param name The variable name.
     */
    VarExpr(std::string name);

    /** \brief Checks equality with another expression. */
    bool equals(Expr *e) override;
    
    /** \brief Throws an exception as variables have no value. */
    int interp() override;
    
    /** \brief Returns true. */
    bool has_variable() override;
    
    /** \brief Returns replacement if name matches, otherwise returns a copy of itself. */
    Expr* subst(std::string name, Expr* replacement) override;

    /** \brief Prints the variable name. */
    void printExp(std::ostream &ot) override;
    
    /** \brief Prints the variable name (ignores precedence). */
    void pretty_print_at(std::ostream &ot, precedence_t p_prec) override;
};

#endif
