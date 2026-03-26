#include "val.h"

#include "expr.h"
#include <cstdint>
#include <limits>
#include <stdexcept>

NumVal::NumVal(int val) {
    this->val = val;
}

bool NumVal::equals(PTR(Val) other) {
    PTR(NumVal) n = CAST(NumVal)(other);
    return n != nullptr && this->val == n->val;
}

PTR(Val) NumVal::add_to(PTR(Val) other) {
    PTR(NumVal) n = CAST(NumVal)(other);
    if (n == nullptr) throw std::runtime_error("Adding non-numbers");
    int64_t sum = static_cast<int64_t>(this->val) + static_cast<int64_t>(n->val);
    if (sum < std::numeric_limits<int>::min() || sum > std::numeric_limits<int>::max()) {
        throw std::runtime_error("integer overflow");
    }
    return NEW(NumVal)(static_cast<int>(sum));
}

PTR(Val) NumVal::mult_with(PTR(Val) other) {
    PTR(NumVal) n = CAST(NumVal)(other);
    if (n == nullptr) throw std::runtime_error("Multiplying non-numbers");
    int64_t product = static_cast<int64_t>(this->val) * static_cast<int64_t>(n->val);
    if (product < std::numeric_limits<int>::min() || product > std::numeric_limits<int>::max()) {
        throw std::runtime_error("integer overflow");
    }
    return NEW(NumVal)(static_cast<int>(product));
}

bool NumVal::is_true() {
    throw std::runtime_error("Condition is not a boolean");
}

PTR(Val) NumVal::call(PTR(Val)) {
    throw std::runtime_error("Calling a non-function");
}

PTR(Expr) NumVal::to_expr() {
    return NEW(NumExpr)(this->val);
}

std::string NumVal::to_string() {
    return std::to_string(this->val);
}

BoolVal::BoolVal(bool val) {
    this->val = val;
}

bool BoolVal::equals(PTR(Val) other) {
    PTR(BoolVal) b = CAST(BoolVal)(other);
    return b != nullptr && this->val == b->val;
}

PTR(Val) BoolVal::add_to(PTR(Val)) {
    throw std::runtime_error("Adding non-numbers");
}

PTR(Val) BoolVal::mult_with(PTR(Val)) {
    throw std::runtime_error("Multiplying non-numbers");
}

bool BoolVal::is_true() {
    return this->val;
}

PTR(Val) BoolVal::call(PTR(Val)) {
    throw std::runtime_error("Calling a non-function");
}

PTR(Expr) BoolVal::to_expr() {
    return NEW(BoolExpr)(this->val);
}

std::string BoolVal::to_string() {
    return this->val ? "_true" : "_false";
}

FunVal::FunVal(std::string formal_arg, PTR(Expr) body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunVal::equals(PTR(Val) other) {
    PTR(FunVal) f = CAST(FunVal)(other);
    if (f == nullptr) return false;
    return this->formal_arg == f->formal_arg && this->body->equals(f->body);
}

PTR(Val) FunVal::add_to(PTR(Val)) {
    throw std::runtime_error("Adding non-numbers");
}

PTR(Val) FunVal::mult_with(PTR(Val)) {
    throw std::runtime_error("Multiplying non-numbers");
}

bool FunVal::is_true() {
    throw std::runtime_error("Condition is not a boolean");
}

PTR(Val) FunVal::call(PTR(Val) actual_arg) {
    PTR(Expr) body_after_subst = this->body->subst(this->formal_arg, actual_arg->to_expr());
    return body_after_subst->interp();
}

PTR(Expr) FunVal::to_expr() {
    return NEW(FunExpr)(this->formal_arg, this->body);
}

std::string FunVal::to_string() {
    return this->to_expr()->to_string();
}
