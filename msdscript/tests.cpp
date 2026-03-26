#include "catch.h"
#include "expr.h"
#include "parse.h"
#include "val.h"
#include <limits>
#include <stdexcept>

TEST_CASE("Expr Equals") {
    CHECK((NEW(NumExpr)(1))->equals(NEW(NumExpr)(1)) == true);
    CHECK((NEW(NumExpr)(1))->equals(NEW(NumExpr)(2)) == false);
    CHECK((NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(true)) == true);
    CHECK((NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(false)) == false);
    CHECK((NEW(VarExpr)("x"))->equals(NEW(VarExpr)("x")) == true);

    CHECK((NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
          ->equals(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))) == true);
    CHECK((NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(10)))
          ->equals(NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(10))) == true);
}

TEST_CASE("Value Basics") {
    CHECK((NEW(NumVal)(10))->equals(NEW(NumVal)(10)));
    CHECK((NEW(NumVal)(10))->equals(NEW(NumVal)(11)) == false);
    CHECK((NEW(NumVal)(10))->equals(nullptr) == false);

    CHECK((NEW(BoolVal)(true))->equals(NEW(BoolVal)(true)));
    CHECK((NEW(BoolVal)(true))->equals(NEW(BoolVal)(false)) == false);
    CHECK((NEW(BoolVal)(true))->equals(NEW(NumVal)(1)) == false);

    CHECK((NEW(NumVal)(4))->add_to(NEW(NumVal)(6))->equals(NEW(NumVal)(10)));
    CHECK((NEW(NumVal)(4))->mult_with(NEW(NumVal)(6))->equals(NEW(NumVal)(24)));
    CHECK_THROWS_WITH((NEW(NumVal)(4))->add_to(NEW(BoolVal)(true)), "Adding non-numbers");
    CHECK_THROWS_WITH((NEW(NumVal)(4))->mult_with(NEW(BoolVal)(true)), "Multiplying non-numbers");
    CHECK_THROWS_WITH((NEW(NumVal)(std::numeric_limits<int>::max()))->add_to(NEW(NumVal)(1)), "integer overflow");
    CHECK_THROWS_WITH((NEW(NumVal)(214748364))->mult_with(NEW(NumVal)(214748364)), "integer overflow");
    CHECK_THROWS_WITH((NEW(NumVal)(0))->is_true(), "Condition is not a boolean");

    CHECK((NEW(BoolVal)(true))->is_true() == true);
    CHECK((NEW(BoolVal)(false))->is_true() == false);
    CHECK_THROWS_WITH((NEW(BoolVal)(true))->add_to(NEW(NumVal)(1)), "Adding non-numbers");
    CHECK_THROWS_WITH((NEW(BoolVal)(true))->mult_with(NEW(NumVal)(1)), "Multiplying non-numbers");

    CHECK_THROWS_WITH((NEW(NumVal)(1))->call(NEW(NumVal)(2)), "Calling a non-function");
    CHECK_THROWS_WITH((NEW(BoolVal)(true))->call(NEW(NumVal)(2)), "Calling a non-function");

    CHECK((NEW(NumVal)(7))->to_expr()->equals(NEW(NumExpr)(7)));
    CHECK((NEW(BoolVal)(true))->to_expr()->equals(NEW(BoolExpr)(true)));
    CHECK((NEW(BoolVal)(false))->to_expr()->equals(NEW(BoolExpr)(false)));
    CHECK((NEW(NumVal)(7))->to_string() == "7");
    CHECK((NEW(BoolVal)(true))->to_string() == "_true");
    CHECK((NEW(BoolVal)(false))->to_string() == "_false");

    CHECK((NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
          ->call(NEW(NumVal)(5))->equals(NEW(NumVal)(6)));
}

TEST_CASE("Expr Interp") {
    CHECK((NEW(NumExpr)(10))->interp()->equals(NEW(NumVal)(10)));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))->interp()->equals(NEW(NumVal)(5)));
    CHECK((NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)), NEW(NumExpr)(4)))->interp()->equals(NEW(NumVal)(20)));
    CHECK_THROWS_WITH((NEW(VarExpr)("x"))->interp(), "no value for variable");

    CHECK((NEW(BoolExpr)(true))->interp()->equals(NEW(BoolVal)(true)));
    CHECK((NEW(BoolExpr)(false))->interp()->equals(NEW(BoolVal)(false)));

    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)))->interp()->equals(NEW(BoolVal)(true)));
    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))->interp()->equals(NEW(BoolVal)(false)));
    CHECK((NEW(EqExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1)))->interp()->equals(NEW(BoolVal)(false)));

    CHECK((NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(2), NEW(NumExpr)(3)))->interp()->equals(NEW(NumVal)(2)));
    CHECK((NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(2), NEW(NumExpr)(3)))->interp()->equals(NEW(NumVal)(3)));
    CHECK_THROWS_WITH((NEW(IfExpr)(NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(1)), NEW(NumExpr)(2), NEW(NumExpr)(3)))->interp(),
                      "Condition is not a boolean");

    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)),
                      NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)),
                      NEW(NumExpr)(88)))->interp()->equals(NEW(NumVal)(88)));

    CHECK((NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(NumExpr)(5)))
          ->interp()->equals(NEW(NumVal)(6)));
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(100),
                       NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))), NEW(NumExpr)(5))))
          ->interp()->equals(NEW(NumVal)(6)));

    CHECK_THROWS_WITH((NEW(CallExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))->interp(), "Calling a non-function");

    CHECK(parse_str("_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in factrl(factrl)(10)")
          ->interp()->equals(NEW(NumVal)(3628800)));
}

TEST_CASE("Expr ToString and Pretty") {
    CHECK((NEW(BoolExpr)(true))->to_string() == "_true");
    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_string() == "(1==(2+3))");
    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(BoolExpr)(false), NEW(NumExpr)(88)))
          ->to_string() == "(_if (1==2) _then _false _else 88)");

    CHECK((NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
          ->to_string() == "(_fun (x) (x+1))");
    CHECK((NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(10)))->to_string() == "f(10)");
    CHECK((NEW(CallExpr)(NEW(FunExpr)("x", NEW(VarExpr)("x")), NEW(NumExpr)(10)))->to_string() == "(_fun (x) x)(10)");

    CHECK((NEW(EqExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_pretty_string() == "1 == 2 + 3");
    CHECK((NEW(AddExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->to_pretty_string() == "(1 == 2) + 3");
    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))
          ->to_pretty_string() == "_if 1 == 2\n_then 1 + 2\n_else 3");

    CHECK((NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))
          ->to_pretty_string() == "_fun (x)\n  x + 1");
    std::string nested_fun_pretty =
        (NEW(AddExpr)(NEW(FunExpr)("x", NEW(VarExpr)("x")), NEW(NumExpr)(1)))->to_pretty_string();
    CHECK(nested_fun_pretty.find("(_fun (x)\n") == 0);
    CHECK(nested_fun_pretty.find("x) + 1") != std::string::npos);
    CHECK((NEW(CallExpr)(NEW(VarExpr)("f"), NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2))))
          ->to_pretty_string() == "f(1 + 2)");
}

TEST_CASE("Expr Substitute") {
    CHECK((NEW(NumExpr)(7))->subst("x", NEW(NumExpr)(9))->equals(NEW(NumExpr)(7)));
    CHECK((NEW(VarExpr)("x"))->subst("x", NEW(NumExpr)(9))->equals(NEW(NumExpr)(9)));
    CHECK((NEW(VarExpr)("y"))->subst("x", NEW(NumExpr)(9))->equals(NEW(VarExpr)("y")));

    CHECK((NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))->subst("x", NEW(NumExpr)(9))
          ->equals(NEW(EqExpr)(NEW(NumExpr)(9), NEW(NumExpr)(1))));

    CHECK((NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))))->subst("x", NEW(NumExpr)(9))
          ->equals(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")))));
    CHECK((NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))))->subst("y", NEW(NumExpr)(9))
          ->equals(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(9)))));

    CHECK((NEW(CallExpr)(NEW(VarExpr)("f"), NEW(VarExpr)("x")))->subst("x", NEW(NumExpr)(10))
          ->equals(NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(10))));
}

TEST_CASE("parse") {
    CHECK(parse_str("_true")->equals(NEW(BoolExpr)(true)));
    CHECK(parse_str("_false")->equals(NEW(BoolExpr)(false)));
    CHECK(parse_str("2147483647")->equals(NEW(NumExpr)(std::numeric_limits<int>::max())));
    CHECK(parse_str("-2147483648")->equals(NEW(NumExpr)(std::numeric_limits<int>::min())));
    CHECK_THROWS_WITH(parse_str("2147483648"), "number too large");
    CHECK_THROWS_WITH(parse_str("1000000000000000"), "number too large");

    CHECK(parse_str("1==2+3")->equals(NEW(EqExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)))));
    CHECK(parse_str("(1==2)+3")->equals(NEW(AddExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3))));

    CHECK(parse_str("_if 1==2 _then 3 _else 4")
          ->equals(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3), NEW(NumExpr)(4))));

    CHECK(parse_str("_fun (x) x")->equals(NEW(FunExpr)("x", NEW(VarExpr)("x"))));
    CHECK(parse_str("_fun (x) x+1")->equals(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))));

    CHECK(parse_str("f(10)")->equals(NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(10))));
    CHECK(parse_str("f(10)(20)")
          ->equals(NEW(CallExpr)(NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(10)), NEW(NumExpr)(20))));
    CHECK(parse_str("(_fun (x) x)(10)")
          ->equals(NEW(CallExpr)(NEW(FunExpr)("x", NEW(VarExpr)("x")), NEW(NumExpr)(10))));

    CHECK(parse_str("_let x = _fun (y) y+1 _in x(10)")
          ->equals(NEW(LetExpr)("x", NEW(FunExpr)("y", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(1))),
                               NEW(CallExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(10)))));

    CHECK_THROWS_WITH(parse_str("_fun x x"), "invalid input");
    CHECK_THROWS_WITH(parse_str("_fun () x"), "invalid input");
    CHECK_THROWS_WITH(parse_str("f(10"), "invalid input");
    CHECK_THROWS_WITH(parse_str("_if _true 1 _else 2"), "invalid input");
}
