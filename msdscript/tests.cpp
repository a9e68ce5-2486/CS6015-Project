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
