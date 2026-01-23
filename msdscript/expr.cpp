#include "expr.h"
#include <iostream>
#include <stdexcept>

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

int NumExpr::interp() {
    return this->val; // 數字的值就是它自己
}

int AddExpr::interp() {
    // 加法的值 = 左邊算出來的值 + 右邊算出來的值
    return this->lhs->interp() + this->rhs->interp();
}

int MultExpr::interp() {
    return this->lhs->interp() * this->rhs->interp();
}

int VarExpr::interp() {
    // 變數沒有數值，依題目要求拋出異常
    throw std::runtime_error("no value for variable");
}

// ==================================================
// 2. has_variable() 實作
// ==================================================

bool NumExpr::has_variable() {
    return false; // 數字當然不包含變數
}

bool AddExpr::has_variable() {
    // 只要左邊有 OR 右邊有，整體就算有
    return this->lhs->has_variable() || this->rhs->has_variable();
}

bool MultExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

bool VarExpr::has_variable() {
    return true; // 我就是變數
}

// ==================================================
// 3. subst() 實作
// 重點：絕對不能修改 this，必須 new 一個新的物件回傳
// ==================================================

Expr* NumExpr::subst(std::string name, Expr* replacement) {
    // 數字無法被替換，但要回傳新的物件 (複製一份自己)
    // 雖然這裡回傳 this 也可以，但為了記憶體管理的一致性，通常建議 new 一個新的
    return new NumExpr(this->val);
}

Expr* AddExpr::subst(std::string name, Expr* replacement) {
    // 遞迴替換左右兩邊，然後建立一個新的加法節點
    return new AddExpr(
        this->lhs->subst(name, replacement),
        this->rhs->subst(name, replacement)
    );
}

Expr* MultExpr::subst(std::string name, Expr* replacement) {
    return new MultExpr(
        this->lhs->subst(name, replacement),
        this->rhs->subst(name, replacement)
    );
}

Expr* VarExpr::subst(std::string name, Expr* replacement) {
    if (this->name == name) {
        // 發現目標！替換！
        // 這裡我們直接回傳 replacement。
        // 注意：在複雜的系統中，這裡可能需要 replacement->clone() 來避免記憶體重複釋放的問題
        // 但依照目前的作業要求，直接回傳即可。
        return replacement;
    } else {
        // 名字不一樣 (例如要把 x 換成 5，但我自己是 y)，則不變
        return new VarExpr(this->name);
    }
}


