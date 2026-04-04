#include "env.h"

#include "val.h"
#include <stdexcept>

PTR(Env) Env::empty = NEW(EmptyEnv)();

PTR(Val) EmptyEnv::lookup(std::string) {
    throw std::runtime_error("no value for variable");
}

ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest) {
    this->name = name;
    this->val = val;
    this->rest = rest;
}

PTR(Val) ExtendedEnv::lookup(std::string lookup_name) {
    if (lookup_name == this->name) {
        return this->val;
    }
    return this->rest->lookup(lookup_name);
}
