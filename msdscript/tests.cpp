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

