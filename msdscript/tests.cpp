#include "catch.h"
#include "expr.h"
#include "parse.h"
#include "val.h"
#include <stdexcept>

TEST_CASE("Expr Equals") {
    CHECK((new NumExpr(1))->equals(new NumExpr(1)) == true);
    CHECK((new NumExpr(1))->equals(new NumExpr(2)) == false);
    CHECK((new BoolExpr(true))->equals(new BoolExpr(true)) == true);
    CHECK((new BoolExpr(true))->equals(new BoolExpr(false)) == false);
    CHECK((new VarExpr("x"))->equals(new VarExpr("x")) == true);

    CHECK((new FunExpr("x", new AddExpr(new VarExpr("x"), new NumExpr(1))))
          ->equals(new FunExpr("x", new AddExpr(new VarExpr("x"), new NumExpr(1)))) == true);
    CHECK((new CallExpr(new VarExpr("f"), new NumExpr(10)))
          ->equals(new CallExpr(new VarExpr("f"), new NumExpr(10))) == true);
}

TEST_CASE("Value Basics") {
    CHECK((new NumVal(10))->equals(new NumVal(10)));
    CHECK((new NumVal(10))->equals(new NumVal(11)) == false);
    CHECK((new NumVal(10))->equals(nullptr) == false);

    CHECK((new BoolVal(true))->equals(new BoolVal(true)));
    CHECK((new BoolVal(true))->equals(new BoolVal(false)) == false);
    CHECK((new BoolVal(true))->equals(new NumVal(1)) == false);

    CHECK((new NumVal(4))->add_to(new NumVal(6))->equals(new NumVal(10)));
    CHECK((new NumVal(4))->mult_with(new NumVal(6))->equals(new NumVal(24)));
    CHECK_THROWS_WITH((new NumVal(4))->add_to(new BoolVal(true)), "Adding non-numbers");
    CHECK_THROWS_WITH((new NumVal(4))->mult_with(new BoolVal(true)), "Multiplying non-numbers");
    CHECK_THROWS_WITH((new NumVal(0))->is_true(), "Condition is not a boolean");

    CHECK((new BoolVal(true))->is_true() == true);
    CHECK((new BoolVal(false))->is_true() == false);
    CHECK_THROWS_WITH((new BoolVal(true))->add_to(new NumVal(1)), "Adding non-numbers");
    CHECK_THROWS_WITH((new BoolVal(true))->mult_with(new NumVal(1)), "Multiplying non-numbers");

    CHECK_THROWS_WITH((new NumVal(1))->call(new NumVal(2)), "Calling a non-function");
    CHECK_THROWS_WITH((new BoolVal(true))->call(new NumVal(2)), "Calling a non-function");

    CHECK((new NumVal(7))->to_expr()->equals(new NumExpr(7)));
    CHECK((new BoolVal(true))->to_expr()->equals(new BoolExpr(true)));
    CHECK((new BoolVal(false))->to_expr()->equals(new BoolExpr(false)));
    CHECK((new NumVal(7))->to_string() == "7");
    CHECK((new BoolVal(true))->to_string() == "_true");
    CHECK((new BoolVal(false))->to_string() == "_false");

    CHECK((new FunVal("x", new AddExpr(new VarExpr("x"), new NumExpr(1))))
          ->call(new NumVal(5))->equals(new NumVal(6)));
}

TEST_CASE("Expr Interp") {
    CHECK((new NumExpr(10))->interp()->equals(new NumVal(10)));
    CHECK((new AddExpr(new NumExpr(2), new NumExpr(3)))->interp()->equals(new NumVal(5)));
    CHECK((new MultExpr(new AddExpr(new NumExpr(2), new NumExpr(3)), new NumExpr(4)))->interp()->equals(new NumVal(20)));
    CHECK_THROWS_WITH((new VarExpr("x"))->interp(), "no value for variable");

    CHECK((new BoolExpr(true))->interp()->equals(new BoolVal(true)));
    CHECK((new BoolExpr(false))->interp()->equals(new BoolVal(false)));

    CHECK((new EqExpr(new NumExpr(1), new NumExpr(1)))->interp()->equals(new BoolVal(true)));
    CHECK((new EqExpr(new NumExpr(1), new NumExpr(2)))->interp()->equals(new BoolVal(false)));
    CHECK((new EqExpr(new BoolExpr(true), new NumExpr(1)))->interp()->equals(new BoolVal(false)));

    CHECK((new IfExpr(new BoolExpr(true), new NumExpr(2), new NumExpr(3)))->interp()->equals(new NumVal(2)));
    CHECK((new IfExpr(new BoolExpr(false), new NumExpr(2), new NumExpr(3)))->interp()->equals(new NumVal(3)));
    CHECK_THROWS_WITH((new IfExpr(new AddExpr(new NumExpr(4), new NumExpr(1)), new NumExpr(2), new NumExpr(3)))->interp(),
                      "Condition is not a boolean");

    CHECK((new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)),
                      new AddExpr(new BoolExpr(false), new NumExpr(5)),
                      new NumExpr(88)))->interp()->equals(new NumVal(88)));

    CHECK((new CallExpr(new FunExpr("x", new AddExpr(new VarExpr("x"), new NumExpr(1))), new NumExpr(5)))
          ->interp()->equals(new NumVal(6)));
    CHECK((new LetExpr("x", new NumExpr(100),
                       new CallExpr(new FunExpr("x", new AddExpr(new VarExpr("x"), new NumExpr(1))), new NumExpr(5))))
          ->interp()->equals(new NumVal(6)));

    CHECK_THROWS_WITH((new CallExpr(new NumExpr(1), new NumExpr(2)))->interp(), "Calling a non-function");

    CHECK(parse_str("_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in factrl(factrl)(10)")
          ->interp()->equals(new NumVal(3628800)));
}

TEST_CASE("Expr ToString and Pretty") {
    CHECK((new BoolExpr(true))->to_string() == "_true");
    CHECK((new EqExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3))))->to_string() == "(1==(2+3))");
    CHECK((new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new BoolExpr(false), new NumExpr(88)))
          ->to_string() == "(_if (1==2) _then _false _else 88)");

    CHECK((new FunExpr("x", new AddExpr(new VarExpr("x"), new NumExpr(1))))
          ->to_string() == "(_fun (x) (x+1))");
    CHECK((new CallExpr(new VarExpr("f"), new NumExpr(10)))->to_string() == "f(10)");
    CHECK((new CallExpr(new FunExpr("x", new VarExpr("x")), new NumExpr(10)))->to_string() == "(_fun (x) x)(10)");

    CHECK((new EqExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3))))->to_pretty_string() == "1 == 2 + 3");
    CHECK((new AddExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3)))->to_pretty_string() == "(1 == 2) + 3");
    CHECK((new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new AddExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3)))
          ->to_pretty_string() == "_if 1 == 2\n_then 1 + 2\n_else 3");

    CHECK((new FunExpr("x", new AddExpr(new VarExpr("x"), new NumExpr(1))))
          ->to_pretty_string() == "_fun (x)\n  x + 1");
    std::string nested_fun_pretty =
        (new AddExpr(new FunExpr("x", new VarExpr("x")), new NumExpr(1)))->to_pretty_string();
    CHECK(nested_fun_pretty.find("(_fun (x)\n") == 0);
    CHECK(nested_fun_pretty.find("x) + 1") != std::string::npos);
    CHECK((new CallExpr(new VarExpr("f"), new AddExpr(new NumExpr(1), new NumExpr(2))))
          ->to_pretty_string() == "f(1 + 2)");
}

TEST_CASE("Expr Substitute") {
    CHECK((new NumExpr(7))->subst("x", new NumExpr(9))->equals(new NumExpr(7)));
    CHECK((new VarExpr("x"))->subst("x", new NumExpr(9))->equals(new NumExpr(9)));
    CHECK((new VarExpr("y"))->subst("x", new NumExpr(9))->equals(new VarExpr("y")));

    CHECK((new EqExpr(new VarExpr("x"), new NumExpr(1)))->subst("x", new NumExpr(9))
          ->equals(new EqExpr(new NumExpr(9), new NumExpr(1))));

    CHECK((new FunExpr("x", new AddExpr(new VarExpr("x"), new VarExpr("y"))))->subst("x", new NumExpr(9))
          ->equals(new FunExpr("x", new AddExpr(new VarExpr("x"), new VarExpr("y")))));
    CHECK((new FunExpr("x", new AddExpr(new VarExpr("x"), new VarExpr("y"))))->subst("y", new NumExpr(9))
          ->equals(new FunExpr("x", new AddExpr(new VarExpr("x"), new NumExpr(9)))));

    CHECK((new CallExpr(new VarExpr("f"), new VarExpr("x")))->subst("x", new NumExpr(10))
          ->equals(new CallExpr(new VarExpr("f"), new NumExpr(10))));
}

TEST_CASE("parse") {
    CHECK(parse_str("_true")->equals(new BoolExpr(true)));
    CHECK(parse_str("_false")->equals(new BoolExpr(false)));

    CHECK(parse_str("1==2+3")->equals(new EqExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3)))));
    CHECK(parse_str("(1==2)+3")->equals(new AddExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3))));

    CHECK(parse_str("_if 1==2 _then 3 _else 4")
          ->equals(new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3), new NumExpr(4))));

    CHECK(parse_str("_fun (x) x")->equals(new FunExpr("x", new VarExpr("x"))));
    CHECK(parse_str("_fun (x) x+1")->equals(new FunExpr("x", new AddExpr(new VarExpr("x"), new NumExpr(1)))));

    CHECK(parse_str("f(10)")->equals(new CallExpr(new VarExpr("f"), new NumExpr(10))));
    CHECK(parse_str("f(10)(20)")
          ->equals(new CallExpr(new CallExpr(new VarExpr("f"), new NumExpr(10)), new NumExpr(20))));
    CHECK(parse_str("(_fun (x) x)(10)")
          ->equals(new CallExpr(new FunExpr("x", new VarExpr("x")), new NumExpr(10))));

    CHECK(parse_str("_let x = _fun (y) y+1 _in x(10)")
          ->equals(new LetExpr("x", new FunExpr("y", new AddExpr(new VarExpr("y"), new NumExpr(1))),
                               new CallExpr(new VarExpr("x"), new NumExpr(10)))));

    CHECK_THROWS_WITH(parse_str("_fun x x"), "invalid input");
    CHECK_THROWS_WITH(parse_str("_fun () x"), "invalid input");
    CHECK_THROWS_WITH(parse_str("f(10"), "invalid input");
    CHECK_THROWS_WITH(parse_str("_if _true 1 _else 2"), "invalid input");
}
