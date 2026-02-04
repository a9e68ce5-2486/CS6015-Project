#include "catch.h"
#include "expr.h"

TEST_CASE("Expr Equals Tests") {
    // 測試數字
    CHECK( (new NumExpr(1))->equals(new NumExpr(1)) == true );
    CHECK( (new NumExpr(1))->equals(new NumExpr(2)) == false );
    
    // 測試加法
    CHECK( (new AddExpr(new NumExpr(2), new NumExpr(3)))->equals(new AddExpr(new NumExpr(2), new NumExpr(3))) == true );
    CHECK( (new AddExpr(new NumExpr(2), new NumExpr(3)))->equals(new AddExpr(new NumExpr(3), new NumExpr(2))) == false );

    // 測試變數 (作業特別要求)
    CHECK( (new VarExpr("x"))->equals(new VarExpr("x")) == true );
    CHECK( (new VarExpr("x"))->equals(new VarExpr("y")) == false );
    CHECK( (new NumExpr(1))->equals(new VarExpr("x")) == false );
}

TEST_CASE("Expr Interp") {
    // 基礎運算測試
    Expr* e1 = new MultExpr(new NumExpr(3), new NumExpr(2)); // 3 * 2
    CHECK( e1->interp() == 6 );

    // 巢狀運算測試: (10 + 15) + (20 + 20) = 25 + 40 = 65
    Expr* e2 = new AddExpr(
        new AddExpr(new NumExpr(10), new NumExpr(15)),
        new AddExpr(new NumExpr(20), new NumExpr(20))
    );
    CHECK( e2->interp() == 65 );

    // 異常測試：對變數求值應該報錯
    CHECK_THROWS_WITH( (new VarExpr("x"))->interp(), "no value for variable" );
}

TEST_CASE("Expr Has Variable") {
    // 加法包含變數
    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(1)))->has_variable() == true );
    
    // 純數字運算
    CHECK( (new MultExpr(new NumExpr(2), new NumExpr(1)))->has_variable() == false );
}

TEST_CASE("Expr Subst") {
    // 測試 1: x + 7, 把 x 換成 y -> y + 7
    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))
          ->subst("x", new VarExpr("y"))
          ->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );
    
    // 測試 2: x, 把 x 換成 (y + 7) -> y + 7
    CHECK( (new VarExpr("x"))
          ->subst("x", new AddExpr(new VarExpr("y"), new NumExpr(7)))
          ->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );
    
    // 測試 3: 不存在的變數替換 (y + 7, 把 x 換成 5) -> 應該還是 y + 7
    CHECK( (new AddExpr(new VarExpr("y"), new NumExpr(7)))
          ->subst("x", new NumExpr(5))
          ->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );
}
// ==========================================
// Test: to_string (Strict format)
// ==========================================
TEST_CASE("Expr to_string") {
    // Num
    CHECK( (new NumExpr(10))->to_string() == "10" );
    
    // Add: (1+2)
    CHECK( (new AddExpr(new NumExpr(1), new NumExpr(2)))->to_string() == "(1+2)" );
    
    // Mult: (3*4)
    CHECK( (new MultExpr(new NumExpr(3), new NumExpr(4)))->to_string() == "(3*4)" );
    
    // Nested: ((1+2)*3)
    CHECK( (new MultExpr(new AddExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3)))
           ->to_string() == "((1+2)*3)" );
}

// ==========================================
// Test: pretty_print (Associativity & Precedence)
// ==========================================
TEST_CASE("Expr pretty_print") {
    // 1. Basic Precedence: 1 * (2 + 3)
    // Mult is root. RHS is Add. Add < Mult, so RHS needs parens.
    CHECK ( (new MultExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3))))
            ->to_pretty_string() == "1 * (2 + 3)" );
    
    // 2. Associativity (Mult): (8 * 1) * y
    // Root is Mult. LHS is Mult.
    // Right associative rule: LHS needs parens if same precedence.
    CHECK ( (new MultExpr(new MultExpr(new NumExpr(8), new NumExpr(1)), new VarExpr("y")))
            ->to_pretty_string() == "(8 * 1) * y" );

    // 3. Mixed Precedence + Associativity
    // (3 + 5) * 6 * 1
    // Struct: Mult( Mult(Add(3,5), 6), 1 )
    // Inner LHS: Mult(Add(3,5), 6) -> Add needs parens (Add < Mult) -> "(3 + 5) * 6"
    // Outer LHS: Inner Mult needs parens because it is LHS of Outer Mult -> "((3 + 5) * 6) * 1"
    // WAIT: The prompt example says "(3 + 5) * 6 * 1".
    // Let's re-read prompt example carefully:
    // "CHECK ( (new Mult(new Add(new Num(3), new Num(5)), new Mult(new Num(6), new Num(1))))->to_pretty_string() ==  "(3 + 5) * 6 * 1" );"
    // My previous trace was assuming (A*B)*C. The prompt example structure is A*(B*C).
    // Let's match the prompt structure:
    // Mult( Add(3,5), Mult(6,1) )
    // LHS Add(3,5) inside Mult -> Needs parens "(3 + 5)"
    // RHS Mult(6,1) inside Mult -> Same prec, RHS -> No parens "6 * 1"
    // Result: "(3 + 5) * 6 * 1" -> MATCHES.
    CHECK ( (new MultExpr(new AddExpr(new NumExpr(3), new NumExpr(5)), new MultExpr(new NumExpr(6), new NumExpr(1))))
            ->to_pretty_string() == "(3 + 5) * 6 * 1" );

    // 4. Another complex case
    // (7 * 7) * (9 + 2)
    // Mult( Mult(7,7), Add(9,2) )
    // LHS Mult(7,7) inside Mult -> Needs parens (LHS same prec) -> "(7 * 7)"
    // RHS Add(9,2) inside Mult -> Needs parens (Add < Mult) -> "(9 + 2)"
    CHECK ( (new MultExpr(new MultExpr(new NumExpr(7), new NumExpr(7)), new AddExpr(new NumExpr(9), new NumExpr(2))) )
            ->to_pretty_string() == "(7 * 7) * (9 + 2)" );
            
    // 5. Right Associativity Add: 1 + 2 + 3
    // Add(1, Add(2, 3)) -> Should be "1 + 2 + 3"
    CHECK( (new AddExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3))))
           ->to_pretty_string() == "1 + 2 + 3" );

    // 6. Left Associativity Add (Force Parens): (1 + 2) + 3
    // Add(Add(1, 2), 3) -> Should be "(1 + 2) + 3"
    CHECK( (new AddExpr(new AddExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3)))
           ->to_pretty_string() == "(1 + 2) + 3" );
}
