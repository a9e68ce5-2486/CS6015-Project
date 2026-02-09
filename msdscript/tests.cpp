/**
 * \file tests.cpp
 * \brief Unit tests for the MSDScript project.
 */
#include "catch.h"
#include "expr.h"
#include <stdexcept>

// ==========================================
// Basic Expression Tests
// ==========================================
TEST_CASE("Expr Equals") {
    CHECK((new NumExpr(1))->equals(new NumExpr(1)) == true);
    CHECK((new NumExpr(1))->equals(new NumExpr(2)) == false);
    CHECK((new VarExpr("x"))->equals(new VarExpr("x")) == true);
    CHECK((new VarExpr("x"))->equals(new VarExpr("y")) == false);
}

TEST_CASE("Expr Interp") {
    CHECK((new NumExpr(10))->interp() == 10);
    CHECK((new AddExpr(new NumExpr(2), new NumExpr(3)))->interp() == 5);
    CHECK_THROWS_WITH((new VarExpr("x"))->interp(), "no value for variable");
}

// ==========================================
// LetExpr Tests
// ==========================================
TEST_CASE("LetExpr Basic") {
    // Equals
    CHECK((new LetExpr("x", new NumExpr(5), new VarExpr("x")))
          ->equals(new LetExpr("x", new NumExpr(5), new VarExpr("x"))) == true);
    
    // Interp: _let x=5 _in x+1 -> 6
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))
          ->interp() == 6);
          
    // Subst (Shadowing): (_let x=1 _in x) subst x->5 should remain 1
    CHECK((new LetExpr("x", new NumExpr(1), new VarExpr("x")))
          ->subst("x", new NumExpr(5))
          ->interp() == 1);
}

TEST_CASE("LetExpr Pretty Print") {
    // 1. Basic indentation
    Expr *e1 = new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)));
    CHECK(e1->to_pretty_string() == "_let x = 5\n_in  x + 1");

    // 2. Nested Let
    Expr *e2 = new LetExpr("x", new NumExpr(5),
                 new LetExpr("y", new NumExpr(3),
                   new AddExpr(new VarExpr("y"), new NumExpr(2))));
    CHECK(e2->to_pretty_string() == "_let x = 5\n_in  _let y = 3\n     _in  y + 2");

    // 3. Parentheses on LHS of Add
    Expr *e3 = new AddExpr(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1));
    CHECK(e3->to_pretty_string() == "(_let x = 5\n _in  x) + 1");
    
    // 4. No Parentheses on RHS of Mult (Prompt example: 5 * (_let x=5 _in x+1) -> 30)
    Expr *e4 = new MultExpr(new NumExpr(5),
                 new LetExpr("x", new NumExpr(5),
                   new AddExpr(new VarExpr("x"), new NumExpr(1))));
    CHECK(e4->to_pretty_string() == "5 * _let x = 5\n    _in  x + 1");
    
    // 5. Parentheses needed inside RHS of Mult (Prompt example: (2 * (_let...)) * 3)
    // 這裡驗證剛剛的 bug fix
    Expr *e5 = new MultExpr(
                 new MultExpr(new NumExpr(2),
                   new LetExpr("x", new NumExpr(5),
                     new AddExpr(new VarExpr("x"), new NumExpr(1)))),
                 new NumExpr(3));
                 
    // 正確的結果應該是 (2 * _let ... ) * 3，_let 本身不需要再被括號包住
    CHECK(e5->to_pretty_string() == "(2 * _let x = 5\n     _in  x + 1) * 3");
}
