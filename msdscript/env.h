#ifndef ENV_H
#define ENV_H

#include "pointer.h"
#include <string>

class Val;

CLASS(Env) {
public:
    static PTR(Env) empty;

    virtual PTR(Val) lookup(std::string name) = 0;
    virtual ~Env() {}
};

class EmptyEnv : public Env {
public:
    PTR(Val) lookup(std::string name) override;
};

class ExtendedEnv : public Env {
public:
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;

    ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest);
    PTR(Val) lookup(std::string lookup_name) override;
};

#endif
