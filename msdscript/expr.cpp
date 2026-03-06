#include "expr.h"
#include "val.h"
#include <sstream>
#include <stdexcept>

std::string Expr::to_string() {
    std::stringstream st("");
    this->printExp(st);
    return st.str();
}

void Expr::pretty_print(std::ostream &ot) {
    std::streampos pos = 0;
    this->pretty_print_at(ot, prec_none, false, pos);
}

std::string Expr::to_pretty_string() {
    std::stringstream st("");
    this->pretty_print(st);
    return st.str();
}

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

BoolExpr::BoolExpr(bool val) {
    this->val = val;
}

bool BoolExpr::equals(Expr *e) {
    BoolExpr *b = dynamic_cast<BoolExpr*>(e);
    if (b == NULL) return false;
    return this->val == b->val;
}

Val* BoolExpr::interp() {
    return new BoolVal(this->val);
}

bool BoolExpr::has_variable() {
    return false;
}

Expr* BoolExpr::subst(std::string, Expr*) {
    return new BoolExpr(this->val);
}

void BoolExpr::printExp(std::ostream &ot) {
    ot << (this->val ? "_true" : "_false");
}

void BoolExpr::pretty_print_at(std::ostream &ot, precedence_t, bool, std::streampos&) {
    ot << (this->val ? "_true" : "_false");
}

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
    return this->lhs->interp()->add_to(this->rhs->interp());
}

bool AddExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* AddExpr::subst(std::string name, Expr* replacement) {
    return new AddExpr(this->lhs->subst(name, replacement), this->rhs->subst(name, replacement));
}

void AddExpr::printExp(std::ostream &ot) {
    ot << "(";
    this->lhs->printExp(ot);
    ot << "+";
    this->rhs->printExp(ot);
    ot << ")";
}

void AddExpr::pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) {
    bool prec_parens = (p_prec > prec_add);
    if (prec_parens) ot << "(";

    this->lhs->pretty_print_at(ot, (precedence_t)(prec_add + 1), true, pos);
    ot << " + ";

    bool rhs_keyword_paren = prec_parens ? false : keyword_paren;
    this->rhs->pretty_print_at(ot, prec_add, rhs_keyword_paren, pos);

    if (prec_parens) ot << ")";
}

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
    return this->lhs->interp()->mult_with(this->rhs->interp());
}

bool MultExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* MultExpr::subst(std::string name, Expr* replacement) {
    return new MultExpr(this->lhs->subst(name, replacement), this->rhs->subst(name, replacement));
}

void MultExpr::printExp(std::ostream &ot) {
    ot << "(";
    this->lhs->printExp(ot);
    ot << "*";
    this->rhs->printExp(ot);
    ot << ")";
}

void MultExpr::pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) {
    bool prec_parens = (p_prec > prec_mult);
    if (prec_parens) ot << "(";

    this->lhs->pretty_print_at(ot, (precedence_t)(prec_mult + 1), true, pos);
    ot << " * ";

    bool rhs_keyword_paren = prec_parens ? false : keyword_paren;
    this->rhs->pretty_print_at(ot, prec_mult, rhs_keyword_paren, pos);

    if (prec_parens) ot << ")";
}

EqExpr::EqExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqExpr::equals(Expr *e) {
    EqExpr *eq = dynamic_cast<EqExpr*>(e);
    if (eq == NULL) return false;
    return this->lhs->equals(eq->lhs) && this->rhs->equals(eq->rhs);
}

Val* EqExpr::interp() {
    Val* lhs_val = this->lhs->interp();
    Val* rhs_val = this->rhs->interp();
    return new BoolVal(lhs_val->equals(rhs_val));
}

bool EqExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* EqExpr::subst(std::string name, Expr* replacement) {
    return new EqExpr(this->lhs->subst(name, replacement), this->rhs->subst(name, replacement));
}

void EqExpr::printExp(std::ostream &ot) {
    ot << "(";
    this->lhs->printExp(ot);
    ot << "==";
    this->rhs->printExp(ot);
    ot << ")";
}

void EqExpr::pretty_print_at(std::ostream &ot, precedence_t p_prec, bool keyword_paren, std::streampos& pos) {
    bool prec_parens = (p_prec > prec_eq);
    if (prec_parens) ot << "(";

    this->lhs->pretty_print_at(ot, (precedence_t)(prec_eq + 1), true, pos);
    ot << " == ";

    bool rhs_keyword_paren = prec_parens ? false : keyword_paren;
    this->rhs->pretty_print_at(ot, prec_eq, rhs_keyword_paren, pos);

    if (prec_parens) ot << ")";
}

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
    }
    return new VarExpr(this->name);
}

void VarExpr::printExp(std::ostream &ot) {
    ot << this->name;
}

void VarExpr::pretty_print_at(std::ostream &ot, precedence_t, bool, std::streampos&) {
    ot << this->name;
}

LetExpr::LetExpr(std::string lhs, Expr *rhs, Expr *body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

bool LetExpr::equals(Expr *e) {
    LetExpr *l = dynamic_cast<LetExpr*>(e);
    if (l == NULL) return false;
    return this->lhs == l->lhs && this->rhs->equals(l->rhs) && this->body->equals(l->body);
}

Val* LetExpr::interp() {
    Val* rhs_val = this->rhs->interp();
    Expr* new_body = this->body->subst(this->lhs, rhs_val->to_expr());
    return new_body->interp();
}

bool LetExpr::has_variable() {
    return this->rhs->has_variable() || this->body->has_variable();
}

Expr* LetExpr::subst(std::string name, Expr* replacement) {
    Expr* new_rhs = this->rhs->subst(name, replacement);
    Expr* new_body;
    if (name == this->lhs) {
        new_body = this->body;
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

void LetExpr::pretty_print_at(std::ostream &ot, precedence_t, bool keyword_paren, std::streampos& pos) {
    if (keyword_paren) {
        ot << "(";
    }

    long indent = (long)(ot.tellp() - pos);
    ot << "_let " << this->lhs << " = ";
    this->rhs->pretty_print_at(ot, prec_none, false, pos);

    ot << "\n";
    pos = ot.tellp();

    for (int i = 0; i < indent; i++) {
        ot << " ";
    }

    ot << "_in  ";
    this->body->pretty_print_at(ot, prec_none, false, pos);

    if (keyword_paren) {
        ot << ")";
    }
}

IfExpr::IfExpr(Expr *test_part, Expr *then_part, Expr *else_part) {
    this->test_part = test_part;
    this->then_part = then_part;
    this->else_part = else_part;
}

bool IfExpr::equals(Expr *e) {
    IfExpr *i = dynamic_cast<IfExpr*>(e);
    if (i == NULL) return false;
    return this->test_part->equals(i->test_part)
        && this->then_part->equals(i->then_part)
        && this->else_part->equals(i->else_part);
}

Val* IfExpr::interp() {
    Val* test_val = this->test_part->interp();
    if (test_val->is_true()) {
        return this->then_part->interp();
    }
    return this->else_part->interp();
}

bool IfExpr::has_variable() {
    return this->test_part->has_variable() || this->then_part->has_variable() || this->else_part->has_variable();
}

Expr* IfExpr::subst(std::string name, Expr* replacement) {
    return new IfExpr(this->test_part->subst(name, replacement),
                      this->then_part->subst(name, replacement),
                      this->else_part->subst(name, replacement));
}

void IfExpr::printExp(std::ostream &ot) {
    ot << "(_if ";
    this->test_part->printExp(ot);
    ot << " _then ";
    this->then_part->printExp(ot);
    ot << " _else ";
    this->else_part->printExp(ot);
    ot << ")";
}

void IfExpr::pretty_print_at(std::ostream &ot, precedence_t, bool keyword_paren, std::streampos& pos) {
    if (keyword_paren) {
        ot << "(";
    }

    long indent = (long)(ot.tellp() - pos);
    ot << "_if ";
    this->test_part->pretty_print_at(ot, prec_none, true, pos);

    ot << "\n";
    pos = ot.tellp();
    for (int i = 0; i < indent; i++) {
        ot << " ";
    }
    ot << "_then ";
    this->then_part->pretty_print_at(ot, prec_none, false, pos);

    ot << "\n";
    pos = ot.tellp();
    for (int i = 0; i < indent; i++) {
        ot << " ";
    }
    ot << "_else ";
    this->else_part->pretty_print_at(ot, prec_none, false, pos);

    if (keyword_paren) {
        ot << ")";
    }
}
