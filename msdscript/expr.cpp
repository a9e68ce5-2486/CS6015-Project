/**
 * expr.cpp
 * 完整的實作檔：包含建構子、equals、interp、subst、printExp、pretty_print
 */
#include "expr.h"
#include <stdexcept>
#include <sstream>

// ==================================================
// Expr Base Class Helpers (父類別輔助函數)
// ==================================================

// 嚴格模式轉字串 (呼叫 printExp)
std::string Expr::to_string() {
    std::stringstream st("");
    this->printExp(st);
    return st.str();
}

// 美觀模式入口 (預設優先權為 none)
void Expr::pretty_print(std::ostream &ot) {
    this->pretty_print_at(ot, prec_none);
}

// 美觀模式轉字串 (呼叫 pretty_print)
std::string Expr::to_pretty_string() {
    std::stringstream st("");
    this->pretty_print(st);
    return st.str();
}

// ==================================================
// 1. NumExpr Implementation
// ==================================================

// Constructor
NumExpr::NumExpr(int val) {
    this->val = val;
}

// equals
bool NumExpr::equals(Expr *e) {
    NumExpr *n = dynamic_cast<NumExpr*>(e);
    if (n == NULL) return false;
    return this->val == n->val;
}

// interp: 回傳數值本身
int NumExpr::interp() {
    return this->val;
}

// has_variable: 數字不含變數
bool NumExpr::has_variable() {
    return false;
}

// subst: 無法替換，回傳新的自己 (複製)
Expr* NumExpr::subst(std::string, Expr*) {
    return new NumExpr(this->val);
}

// printExp: 直接印出數字
void NumExpr::printExp(std::ostream &ot) {
    ot << this->val;
}

// pretty_print_at: 數字不需要括號
void NumExpr::pretty_print_at(std::ostream &ot, precedence_t p_prec) {
    (void)p_prec; // 消除編譯器警告
    ot << this->val;
}

// ==================================================
// 2. AddExpr Implementation
// ==================================================

// Constructor
AddExpr::AddExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// equals
bool AddExpr::equals(Expr *e) {
    AddExpr *a = dynamic_cast<AddExpr*>(e);
    if (a == NULL) return false;
    return this->lhs->equals(a->lhs) && this->rhs->equals(a->rhs);
}

// interp: 左右相加
int AddExpr::interp() {
    return this->lhs->interp() + this->rhs->interp();
}

// has_variable: 檢查左右是否含有變數
bool AddExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

// subst: 遞迴替換左右
Expr* AddExpr::subst(std::string name, Expr* replacement) {
    return new AddExpr(
        this->lhs->subst(name, replacement),
        this->rhs->subst(name, replacement)
    );
}

// printExp: (lhs+rhs) 嚴格加上括號
void AddExpr::printExp(std::ostream &ot) {
    ot << "(";
    lhs->printExp(ot);
    ot << "+";
    rhs->printExp(ot);
    ot << ")";
}

// pretty_print_at: 根據優先權判斷括號
void AddExpr::pretty_print_at(std::ostream &ot, precedence_t p_prec) {
    // 如果外部環境優先權更高 (例如乘法)，加法需要括號保護
    if (p_prec > prec_add) ot << "(";
    
    // 左子樹：右結合規則，同級(加法)需要括號，傳入 prec_add + 1
    lhs->pretty_print_at(ot, (precedence_t)(prec_add + 1));
    
    ot << " + ";
    
    // 右子樹：同級不需要括號
    rhs->pretty_print_at(ot, prec_add);
    
    if (p_prec > prec_add) ot << ")";
}

// ==================================================
// 3. MultExpr Implementation
// ==================================================

// Constructor
MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

// equals
bool MultExpr::equals(Expr *e) {
    MultExpr *m = dynamic_cast<MultExpr*>(e);
    if (m == NULL) return false;
    return this->lhs->equals(m->lhs) && this->rhs->equals(m->rhs);
}

// interp: 左右相乘
int MultExpr::interp() {
    return this->lhs->interp() * this->rhs->interp();
}

// has_variable
bool MultExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

// subst
Expr* MultExpr::subst(std::string name, Expr* replacement) {
    return new MultExpr(
        this->lhs->subst(name, replacement),
        this->rhs->subst(name, replacement)
    );
}

// printExp: (lhs*rhs)
void MultExpr::printExp(std::ostream &ot) {
    ot << "(";
    lhs->printExp(ot);
    ot << "*";
    rhs->printExp(ot);
    ot << ")";
}

// pretty_print_at
void MultExpr::pretty_print_at(std::ostream &ot, precedence_t p_prec) {
    // 如果外部優先權更高，需要括號 (目前乘法已是最高，但為了通用性保留邏輯)
    if (p_prec > prec_mult) ot << "(";
    
    // 左子樹：右結合，同級需括號
    lhs->pretty_print_at(ot, (precedence_t)(prec_mult + 1));
    
    ot << " * ";
    
    // 右子樹：同級不需括號
    rhs->pretty_print_at(ot, prec_mult);
    
    if (p_prec > prec_mult) ot << ")";
}

// ==================================================
// 4. VarExpr Implementation
// ==================================================

// Constructor
VarExpr::VarExpr(std::string name) {
    this->name = name;
}

// equals
bool VarExpr::equals(Expr *e) {
    VarExpr *v = dynamic_cast<VarExpr*>(e);
    if (v == NULL) return false;
    return this->name == v->name;
}

// interp: 變數無法求值，拋出異常
int VarExpr::interp() {
    throw std::runtime_error("no value for variable");
}

// has_variable: 當然有
bool VarExpr::has_variable() {
    return true;
}

// subst: 檢查名字是否符合，符合就替換
Expr* VarExpr::subst(std::string name, Expr* replacement) {
    if (this->name == name) {
        // 這裡應該做深拷貝 (Clone) 比較好，但為了作業簡化直接回傳
        return replacement;
    } else {
        return new VarExpr(this->name);
    }
}

// printExp: 印出變數名
void VarExpr::printExp(std::ostream &ot) {
    ot << this->name;
}

// pretty_print_at: 變數不需要括號
void VarExpr::pretty_print_at(std::ostream &ot, precedence_t p_prec) {
    (void)p_prec;
    ot << this->name;
}
