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

NumExpr::NumExpr(int val) { this->val = val; }

bool NumExpr::equals(PTR(Expr) e) {
    PTR(NumExpr) n = CAST(NumExpr)(e);
    return n != nullptr && this->val == n->val;
}

PTR(Val) NumExpr::interp() { return NEW(NumVal)(this->val); }
PTR(Expr) NumExpr::subst(std::string, PTR(Expr)) { return NEW(NumExpr)(this->val); }
void NumExpr::printExp(std::ostream &ot) { ot << this->val; }
void NumExpr::pretty_print_at(std::ostream &ot, precedence_t, bool, std::streampos&) { ot << this->val; }

BoolExpr::BoolExpr(bool val) { this->val = val; }

bool BoolExpr::equals(PTR(Expr) e) {
    PTR(BoolExpr) b = CAST(BoolExpr)(e);
    return b != nullptr && this->val == b->val;
}

PTR(Val) BoolExpr::interp() { return NEW(BoolVal)(this->val); }
PTR(Expr) BoolExpr::subst(std::string, PTR(Expr)) { return NEW(BoolExpr)(this->val); }
void BoolExpr::printExp(std::ostream &ot) { ot << (this->val ? "_true" : "_false"); }
void BoolExpr::pretty_print_at(std::ostream &ot, precedence_t, bool, std::streampos&) { ot << (this->val ? "_true" : "_false"); }

AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs) { this->lhs = lhs; this->rhs = rhs; }

bool AddExpr::equals(PTR(Expr) e) {
    PTR(AddExpr) a = CAST(AddExpr)(e);
    return a != nullptr && this->lhs->equals(a->lhs) && this->rhs->equals(a->rhs);
}

PTR(Val) AddExpr::interp() { return this->lhs->interp()->add_to(this->rhs->interp()); }
PTR(Expr) AddExpr::subst(std::string name, PTR(Expr) replacement) {
    return NEW(AddExpr)(this->lhs->subst(name, replacement), this->rhs->subst(name, replacement));
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

MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs) { this->lhs = lhs; this->rhs = rhs; }

bool MultExpr::equals(PTR(Expr) e) {
    PTR(MultExpr) m = CAST(MultExpr)(e);
    return m != nullptr && this->lhs->equals(m->lhs) && this->rhs->equals(m->rhs);
}

PTR(Val) MultExpr::interp() { return this->lhs->interp()->mult_with(this->rhs->interp()); }
PTR(Expr) MultExpr::subst(std::string name, PTR(Expr) replacement) {
    return NEW(MultExpr)(this->lhs->subst(name, replacement), this->rhs->subst(name, replacement));
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

EqExpr::EqExpr(PTR(Expr) lhs, PTR(Expr) rhs) { this->lhs = lhs; this->rhs = rhs; }

bool EqExpr::equals(PTR(Expr) e) {
    PTR(EqExpr) eq = CAST(EqExpr)(e);
    return eq != nullptr && this->lhs->equals(eq->lhs) && this->rhs->equals(eq->rhs);
}

PTR(Val) EqExpr::interp() {
    PTR(Val) lhs_val = this->lhs->interp();
    PTR(Val) rhs_val = this->rhs->interp();
    return NEW(BoolVal)(lhs_val->equals(rhs_val));
}

PTR(Expr) EqExpr::subst(std::string name, PTR(Expr) replacement) {
    return NEW(EqExpr)(this->lhs->subst(name, replacement), this->rhs->subst(name, replacement));
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

VarExpr::VarExpr(std::string name) { this->name = name; }

bool VarExpr::equals(PTR(Expr) e) {
    PTR(VarExpr) v = CAST(VarExpr)(e);
    return v != nullptr && this->name == v->name;
}

PTR(Val) VarExpr::interp() { throw std::runtime_error("no value for variable"); }
PTR(Expr) VarExpr::subst(std::string name, PTR(Expr) replacement) {
    if (this->name == name) return replacement;
    return NEW(VarExpr)(this->name);
}
void VarExpr::printExp(std::ostream &ot) { ot << this->name; }
void VarExpr::pretty_print_at(std::ostream &ot, precedence_t, bool, std::streampos&) { ot << this->name; }

LetExpr::LetExpr(std::string lhs, PTR(Expr) rhs, PTR(Expr) body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

bool LetExpr::equals(PTR(Expr) e) {
    PTR(LetExpr) l = CAST(LetExpr)(e);
    return l != nullptr && this->lhs == l->lhs && this->rhs->equals(l->rhs) && this->body->equals(l->body);
}

PTR(Val) LetExpr::interp() {
    PTR(Val) rhs_val = this->rhs->interp();
    PTR(Expr) new_body = this->body->subst(this->lhs, rhs_val->to_expr());
    return new_body->interp();
}

PTR(Expr) LetExpr::subst(std::string name, PTR(Expr) replacement) {
    PTR(Expr) new_rhs = this->rhs->subst(name, replacement);
    PTR(Expr) new_body = (name == this->lhs) ? this->body : this->body->subst(name, replacement);
    return NEW(LetExpr)(this->lhs, new_rhs, new_body);
}

void LetExpr::printExp(std::ostream &ot) {
    ot << "(_let " << this->lhs << "=";
    this->rhs->printExp(ot);
    ot << " _in ";
    this->body->printExp(ot);
    ot << ")";
}

void LetExpr::pretty_print_at(std::ostream &ot, precedence_t, bool keyword_paren, std::streampos& pos) {
    if (keyword_paren) ot << "(";

    long indent = (long)(ot.tellp() - pos);
    ot << "_let " << this->lhs << " = ";
    this->rhs->pretty_print_at(ot, prec_none, false, pos);

    ot << "\n";
    pos = ot.tellp();
    for (int i = 0; i < indent; i++) ot << " ";
    ot << "_in  ";
    this->body->pretty_print_at(ot, prec_none, false, pos);

    if (keyword_paren) ot << ")";
}

IfExpr::IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part) {
    this->test_part = test_part;
    this->then_part = then_part;
    this->else_part = else_part;
}

bool IfExpr::equals(PTR(Expr) e) {
    PTR(IfExpr) i = CAST(IfExpr)(e);
    return i != nullptr && this->test_part->equals(i->test_part)
        && this->then_part->equals(i->then_part)
        && this->else_part->equals(i->else_part);
}

PTR(Val) IfExpr::interp() {
    PTR(Val) test_val = this->test_part->interp();
    if (test_val->is_true()) return this->then_part->interp();
    return this->else_part->interp();
}

PTR(Expr) IfExpr::subst(std::string name, PTR(Expr) replacement) {
    return NEW(IfExpr)(this->test_part->subst(name, replacement),
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
    if (keyword_paren) ot << "(";

    long indent = (long)(ot.tellp() - pos);
    ot << "_if ";
    this->test_part->pretty_print_at(ot, prec_none, true, pos);

    ot << "\n";
    pos = ot.tellp();
    for (int i = 0; i < indent; i++) ot << " ";
    ot << "_then ";
    this->then_part->pretty_print_at(ot, prec_none, false, pos);

    ot << "\n";
    pos = ot.tellp();
    for (int i = 0; i < indent; i++) ot << " ";
    ot << "_else ";
    this->else_part->pretty_print_at(ot, prec_none, false, pos);

    if (keyword_paren) ot << ")";
}

FunExpr::FunExpr(std::string formal_arg, PTR(Expr) body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals(PTR(Expr) e) {
    PTR(FunExpr) f = CAST(FunExpr)(e);
    return f != nullptr && this->formal_arg == f->formal_arg && this->body->equals(f->body);
}

PTR(Val) FunExpr::interp() {
    return NEW(FunVal)(this->formal_arg, this->body);
}

PTR(Expr) FunExpr::subst(std::string name, PTR(Expr) replacement) {
    if (name == this->formal_arg) {
        return NEW(FunExpr)(this->formal_arg, this->body);
    }
    return NEW(FunExpr)(this->formal_arg, this->body->subst(name, replacement));
}

void FunExpr::printExp(std::ostream &ot) {
    ot << "(_fun (" << this->formal_arg << ") ";
    this->body->printExp(ot);
    ot << ")";
}

void FunExpr::pretty_print_at(std::ostream &ot, precedence_t, bool keyword_paren, std::streampos& pos) {
    if (keyword_paren) ot << "(";

    long indent = (long)(ot.tellp() - pos);
    ot << "_fun (" << this->formal_arg << ")";

    ot << "\n";
    pos = ot.tellp();
    for (int i = 0; i < indent + 2; i++) ot << " ";
    this->body->pretty_print_at(ot, prec_none, false, pos);

    if (keyword_paren) ot << ")";
}

CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals(PTR(Expr) e) {
    PTR(CallExpr) c = CAST(CallExpr)(e);
    return c != nullptr && this->to_be_called->equals(c->to_be_called) && this->actual_arg->equals(c->actual_arg);
}

PTR(Val) CallExpr::interp() {
    PTR(Val) fun_val = this->to_be_called->interp();
    PTR(Val) arg_val = this->actual_arg->interp();
    return fun_val->call(arg_val);
}

PTR(Expr) CallExpr::subst(std::string name, PTR(Expr) replacement) {
    return NEW(CallExpr)(this->to_be_called->subst(name, replacement), this->actual_arg->subst(name, replacement));
}

void CallExpr::printExp(std::ostream &ot) {
    this->to_be_called->printExp(ot);
    ot << "(";
    this->actual_arg->printExp(ot);
    ot << ")";
}

void CallExpr::pretty_print_at(std::ostream &ot, precedence_t, bool, std::streampos& pos) {
    this->to_be_called->pretty_print_at(ot, prec_call, true, pos);
    ot << "(";
    this->actual_arg->pretty_print_at(ot, prec_none, false, pos);
    ot << ")";
}
