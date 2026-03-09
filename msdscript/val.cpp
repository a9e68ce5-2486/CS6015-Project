#include "val.h"

#include "expr.h"
#include <stdexcept>

NumVal::NumVal(int val) {
    this->val = val;
}

bool NumVal::equals(Val* other) {
    NumVal* n = dynamic_cast<NumVal*>(other);
    return n != NULL && this->val == n->val;
}

Val* NumVal::add_to(Val* other) {
    NumVal* n = dynamic_cast<NumVal*>(other);
    if (n == NULL) throw std::runtime_error("Adding non-numbers");
    return new NumVal(this->val + n->val);
}

Val* NumVal::mult_with(Val* other) {
    NumVal* n = dynamic_cast<NumVal*>(other);
    if (n == NULL) throw std::runtime_error("Multiplying non-numbers");
    return new NumVal(this->val * n->val);
}

bool NumVal::is_true() {
    throw std::runtime_error("Condition is not a boolean");
}

Val* NumVal::call(Val*) {
    throw std::runtime_error("Calling a non-function");
}

Expr* NumVal::to_expr() {
    return new NumExpr(this->val);
}

std::string NumVal::to_string() {
    return std::to_string(this->val);
}

BoolVal::BoolVal(bool val) {
    this->val = val;
}

bool BoolVal::equals(Val* other) {
    BoolVal* b = dynamic_cast<BoolVal*>(other);
    return b != NULL && this->val == b->val;
}

Val* BoolVal::add_to(Val*) {
    throw std::runtime_error("Adding non-numbers");
}

Val* BoolVal::mult_with(Val*) {
    throw std::runtime_error("Multiplying non-numbers");
}

bool BoolVal::is_true() {
    return this->val;
}

Val* BoolVal::call(Val*) {
    throw std::runtime_error("Calling a non-function");
}

Expr* BoolVal::to_expr() {
    return new BoolExpr(this->val);
}

std::string BoolVal::to_string() {
    return this->val ? "_true" : "_false";
}

FunVal::FunVal(std::string formal_arg, Expr *body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunVal::equals(Val* other) {
    FunVal* f = dynamic_cast<FunVal*>(other);
    if (f == NULL) return false;
    return this->formal_arg == f->formal_arg && this->body->equals(f->body);
}

Val* FunVal::add_to(Val*) {
    throw std::runtime_error("Adding non-numbers");
}

Val* FunVal::mult_with(Val*) {
    throw std::runtime_error("Multiplying non-numbers");
}

bool FunVal::is_true() {
    throw std::runtime_error("Condition is not a boolean");
}

Val* FunVal::call(Val* actual_arg) {
    Expr* body_after_subst = this->body->subst(this->formal_arg, actual_arg->to_expr());
    return body_after_subst->interp();
}

Expr* FunVal::to_expr() {
    return new FunExpr(this->formal_arg, this->body);
}

std::string FunVal::to_string() {
    return this->to_expr()->to_string();
}
