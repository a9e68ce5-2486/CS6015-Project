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
        throw std::runtime_error("add of non-number");
    }
    return new NumVal(this->val + n->val);
}

Val* NumVal::mult_with(Val* other) {
    NumVal* n = dynamic_cast<NumVal*>(other);
    if (n == NULL) {
        throw std::runtime_error("mult of non-number");
    }
    return new NumVal(this->val * n->val);
}

Expr* NumVal::to_expr() {
    return new NumExpr(this->val);
}

std::string NumVal::to_string() {
    return std::to_string(this->val);
}
