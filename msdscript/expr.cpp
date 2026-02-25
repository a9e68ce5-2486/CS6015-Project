/**
 * \file expr.cpp
 * \brief Implementation of the Expression classes.
 */

#include "expr.h"
#include "val.h"
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
    std::streampos pos = 0; // Initialize indentation tracking
    this->pretty_print_at(ot, prec_none, false, pos);
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

Val* NumExpr::interp() {
    return new NumVal(this->val);
}

bool NumExpr::has_variable() {
    return false;
}

Expr* NumExpr::subst(std::string, Expr*) {
    return new NumExpr(this->val);
}

void NumExpr::printExp(std::ostream &ot) {
    ot << this->val;
}

void NumExpr::pretty_print_at(std::ostream &ot, precedence_t, bool, std::streampos&) {
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

Val* AddExpr::interp() {
    Val* lhs_val = this->lhs->interp();
    Val* rhs_val = this->rhs->interp();
    return lhs_val->add_to(rhs_val);
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

void AddExpr::pretty_print_at(std::ostream &ot, precedence_t p_prec, bool let_paren, std::streampos& pos) {
    // 1. 判斷自己是否因優先權需要括號
    bool prec_parens = (p_prec > prec_add);
    
    if (prec_parens) ot << "(";
    
    // 左邊：永遠需要保護 Let (因為 Let 優先權最低，且在左邊如果不加括號會被吃掉)
    lhs->pretty_print_at(ot, (precedence_t)(prec_add + 1), true, pos);
    
    ot << " + ";
    
    // 右邊：修正邏輯！
    // 如果我自己(Add)已經加了括號，那我的括號已經保護了裡面的內容，
    // 所以右邊小孩不需要繼承外面的 let_paren 壓力。
    bool rhs_let_paren = prec_parens ? false : let_paren;

    rhs->pretty_print_at(ot, prec_add, rhs_let_paren, pos);
    
    if (prec_parens) ot << ")";
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

Val* MultExpr::interp() {
    Val* lhs_val = this->lhs->interp();
    Val* rhs_val = this->rhs->interp();
    return lhs_val->mult_with(rhs_val);
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

void MultExpr::pretty_print_at(std::ostream &ot, precedence_t p_prec, bool let_paren, std::streampos& pos) {
    // 1. 判斷自己是否因優先權需要括號
    bool prec_parens = (p_prec > prec_mult);

    if (prec_parens) ot << "(";
    
    // 左邊：Let 永遠需要括號
    lhs->pretty_print_at(ot, (precedence_t)(prec_mult + 1), true, pos);
    
    ot << " * ";
    
    // 右邊：修正邏輯！
    // 如果我自己(Mult)已經加了括號，右邊小孩就安全了。
    bool rhs_let_paren = prec_parens ? false : let_paren;

    rhs->pretty_print_at(ot, prec_mult, rhs_let_paren, pos);
    
    if (prec_parens) ot << ")";
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

Val* VarExpr::interp() {
    throw std::runtime_error("no value for variable");
}

bool VarExpr::has_variable() {
    return true;
}

Expr* VarExpr::subst(std::string name, Expr* replacement) {
    if (this->name == name) {
        return replacement;
    } else {
        return new VarExpr(this->name);
    }
}

void VarExpr::printExp(std::ostream &ot) {
    ot << this->name;
}

void VarExpr::pretty_print_at(std::ostream &ot, precedence_t, bool, std::streampos&) {
    ot << this->name;
}

// ==================================================
// LetExpr Implementation
// ==================================================

LetExpr::LetExpr(std::string lhs, Expr *rhs, Expr *body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

bool LetExpr::equals(Expr *e) {
    LetExpr *l = dynamic_cast<LetExpr*>(e);
    if (l == NULL) return false;
    return (this->lhs == l->lhs) &&
           (this->rhs->equals(l->rhs)) &&
           (this->body->equals(l->body));
}

Val* LetExpr::interp() {
    Val* rhs_val = this->rhs->interp();
    Expr* new_body = this->body->subst(this->lhs, rhs_val->to_expr());
    return new_body->interp();
}

bool LetExpr::has_variable() {
    return (this->rhs->has_variable() || this->body->has_variable());
}

Expr* LetExpr::subst(std::string name, Expr* replacement) {
    Expr* new_rhs = this->rhs->subst(name, replacement);
    
    Expr* new_body;
    if (name == this->lhs) {
        new_body = this->body; // Shadowing
    } else {
        new_body = this->body->subst(name, replacement);
    }

    return new LetExpr(this->lhs, new_rhs, new_body);
}

void LetExpr::printExp(std::ostream &ot) {
    ot << "(_let " << this->lhs << "=";
    this->rhs->printExp(ot);
    ot << " _in ";
    this->body->printExp(ot);
    ot << ")";
}

void LetExpr::pretty_print_at(std::ostream &ot, precedence_t, bool let_paren, std::streampos& pos) {
    // 1. If parent requested parentheses (let_paren is true), print '('
    if (let_paren) {
        ot << "(";
    }

    // 2. Calculate indentation
    long indent = (long)(ot.tellp() - pos);
    
    ot << "_let " << this->lhs << " = ";
    
    this->rhs->pretty_print_at(ot, prec_none, false, pos);
    
    ot << "\n";
    
    // 3. Update last newline position
    pos = ot.tellp();
    
    // 4. Print spaces
    for (int i = 0; i < indent; i++) {
        ot << " ";
    }
    
    ot << "_in  ";
    
    this->body->pretty_print_at(ot, prec_none, false, pos);

    if (let_paren) {
        ot << ")";
    }
}
