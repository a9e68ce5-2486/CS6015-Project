#include "expr.h"
#include <iostream>

NumExpr::NumExpr(int val) : val(val) {}

bool NumExpr::equals(Expr *e){

NumExpr *n = dynamic_cast<NumExpr*>(e);
if(n == NULL) return false;
return this->val == n->val ;
}

AddExpr::AddExpr(Expr *lhs , Expr *rhs) : lhs(lhs) , rhs(rhs) {}

bool AddExpr::equals(Expr *e){
AddExpr *a = dynamic_cast<AddExpr*>(e) ;
if(a == NULL) return false ;
return this -> lhs -> equals(a -> lhs) && this -> rhs -> equals(a -> rhs) ;
}

MultExpr::MultExpr(Expr *lhs , Expr *rhs) : lhs(lhs) , rhs(rhs) {}

bool MultExpr::equals(Expr *e) {

MultExpr *m = dynamic_cast<MultExpr*>(e);
if(m == NULL) return false ;
return this -> lhs -> equals(m -> lhs) && this -> rhs -> equals(m -> rhs) ;
}

VarExpr::VarExpr(std::string name) : name(name) {}

bool VarExpr::equals(Expr *e) {

VarExpr *v = dynamic_cast<VarExpr*>(e) ;
if(v == NULL) return false ;
return this -> name == v -> name ;
}

