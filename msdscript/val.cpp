#include "val.h"

#include "expr.h"
#include <stdexcept>

NumVal::NumVal(int val) {
    this->val = val;
}

bool NumVal::equals(Val* other) {
    NumVal* n = dynamic_cast<NumVal*>(other);
    if (n == NULL) {
        return false;
    }
    return this->val == n->val;
}

Val* NumVal::add_to(Val* other) {
    NumVal* n = dynamic_cast<NumVal*>(other);
    if (n == NULL) {
        throw std::runtime_error("Adding non-numbers");
    }
    return new NumVal(this->val + n->val);
}

Val* NumVal::mult_with(Val* other) {
    NumVal* n = dynamic_cast<NumVal*>(other);
    if (n == NULL) {
        throw std::runtime_error("Multiplying non-numbers");
    }
    return new NumVal(this->val * n->val);
}

bool NumVal::is_true() {
    throw std::runtime_error("Condition is not a boolean");
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
    if (b == NULL) {
        return false;
    }
    return this->val == b->val;
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

Expr* BoolVal::to_expr() {
    return new BoolExpr(this->val);
}

std::string BoolVal::to_string() {
    return this->val ? "_true" : "_false";
}
