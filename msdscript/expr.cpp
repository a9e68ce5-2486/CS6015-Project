/**
 * \file expr.cpp
 * \brief Implementation of the Expression classes.
 *
 * This file contains the implementation of methods for NumExpr, AddExpr,
 * MultExpr, and VarExpr, including evaluation, substitution, and printing logic.
 */

#include "expr.h"
#include <stdexcept>
#include <sstream>

// ==================================================
// Expr Base Class Helpers
// ==================================================

std::string Expr::to_string() {
    std::stringstream st("");
    this->printExp(st);
    return st.str();
}

void Expr::pretty_print(std::ostream &ot) {
    this->pretty_print_at(ot, prec_none);
}

std::string Expr::to_pretty_string() {
    std::stringstream st("");
    this->pretty_print(st);
    return st.str();
}

// ==================================================
// NumExpr Implementation
// ==================================================

NumExpr::NumExpr(int val) {
    this->val = val;
}

bool NumExpr::equals(Expr *e) {
    NumExpr *n = dynamic_cast<NumExpr*>(e);
    if (n == NULL) return false;
    return this->val == n->val;
}

int NumExpr::interp() {
    return this->val;
}

bool NumExpr::has_variable() {
    return false;
}

Expr* NumExpr::subst(std::string, Expr*) {
    // Numbers don't contain variables, so return a copy of self
    return new NumExpr(this->val);
}

void NumExpr::printExp(std::ostream &ot) {
    ot << this->val;
}

void NumExpr::pretty_print_at(std::ostream &ot, precedence_t p_prec) {
    (void)p_prec; // Unused
    ot << this->val;
}

// ==================================================
// AddExpr Implementation
// ==================================================

AddExpr::AddExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool AddExpr::equals(Expr *e) {
    AddExpr *a = dynamic_cast<AddExpr*>(e);
    if (a == NULL) return false;
    return this->lhs->equals(a->lhs) && this->rhs->equals(a->rhs);
}

int AddExpr::interp() {
    return this->lhs->interp() + this->rhs->interp();
}

bool AddExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* AddExpr::subst(std::string name, Expr* replacement) {
    return new AddExpr(
        this->lhs->subst(name, replacement),
        this->rhs->subst(name, replacement)
    );
}

void AddExpr::printExp(std::ostream &ot) {
    ot << "(";
    lhs->printExp(ot);
    ot << "+";
    rhs->printExp(ot);
    ot << ")";
}

void AddExpr::pretty_print_at(std::ostream &ot, precedence_t p_prec) {
    if (p_prec > prec_add) ot << "(";
    
    // Left side: prec_add + 1 to force parens if LHS is also Add (Right Associativity)
    lhs->pretty_print_at(ot, (precedence_t)(prec_add + 1));
    
    ot << " + ";
    
    // Right side: same precedence, no parens needed for same operator
    rhs->pretty_print_at(ot, prec_add);
    
    if (p_prec > prec_add) ot << ")";
}

// ==================================================
// MultExpr Implementation
// ==================================================

MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool MultExpr::equals(Expr *e) {
    MultExpr *m = dynamic_cast<MultExpr*>(e);
    if (m == NULL) return false;
    return this->lhs->equals(m->lhs) && this->rhs->equals(m->rhs);
}

int MultExpr::interp() {
    return this->lhs->interp() * this->rhs->interp();
}

bool MultExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* MultExpr::subst(std::string name, Expr* replacement) {
    return new MultExpr(
        this->lhs->subst(name, replacement),
        this->rhs->subst(name, replacement)
    );
}

void MultExpr::printExp(std::ostream &ot) {
    ot << "(";
    lhs->printExp(ot);
    ot << "*";
    rhs->printExp(ot);
    ot << ")";
}

void MultExpr::pretty_print_at(std::ostream &ot, precedence_t p_prec) {
    if (p_prec > prec_mult) ot << "(";
    
    // Left side: prec_mult + 1 (Right Associativity)
    lhs->pretty_print_at(ot, (precedence_t)(prec_mult + 1));
    
    ot << " * ";
    
    // Right side: same precedence
    rhs->pretty_print_at(ot, prec_mult);
    
    if (p_prec > prec_mult) ot << ")";
}

// ==================================================
// VarExpr Implementation
// ==================================================

VarExpr::VarExpr(std::string name) {
    this->name = name;
}

bool VarExpr::equals(Expr *e) {
    VarExpr *v = dynamic_cast<VarExpr*>(e);
    if (v == NULL) return false;
    return this->name == v->name;
}

int VarExpr::interp() {
    throw std::runtime_error("no value for variable");
}

bool VarExpr::has_variable() {
    return true;
}

Expr* VarExpr::subst(std::string name, Expr* replacement) {
    if (this->name == name) {
        return replacement; // Should clone in a real system, but acceptable here
    } else {
        return new VarExpr(this->name);
    }
}

void VarExpr::printExp(std::ostream &ot) {
    ot << this->name;
}

void VarExpr::pretty_print_at(std::ostream &ot, precedence_t p_prec) {
    (void)p_prec; // Unused
    ot << this->name;
}
