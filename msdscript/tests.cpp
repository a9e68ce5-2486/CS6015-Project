#include "catch.h"
#include "expr.h"
#include "parse.h"
#include "val.h"
#include <stdexcept>

TEST_CASE("Expr Equals") {
    CHECK((new NumExpr(1))->equals(new NumExpr(1)) == true);
    CHECK((new NumExpr(1))->equals(new NumExpr(2)) == false);
    CHECK((new VarExpr("x"))->equals(new VarExpr("x")) == true);
    CHECK((new VarExpr("x"))->equals(new VarExpr("y")) == false);
    CHECK((new BoolExpr(true))->equals(new BoolExpr(true)) == true);
    CHECK((new BoolExpr(true))->equals(new BoolExpr(false)) == false);
    CHECK((new EqExpr(new NumExpr(1), new NumExpr(2)))
          ->equals(new EqExpr(new NumExpr(1), new NumExpr(2))) == true);
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

    CHECK((new NumVal(7))->to_expr()->equals(new NumExpr(7)));
    CHECK((new BoolVal(true))->to_expr()->equals(new BoolExpr(true)));
    CHECK((new BoolVal(false))->to_expr()->equals(new BoolExpr(false)));
    CHECK((new NumVal(7))->to_string() == "7");
    CHECK((new BoolVal(true))->to_string() == "_true");
    CHECK((new BoolVal(false))->to_string() == "_false");
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
    CHECK((new EqExpr(new BoolExpr(true), new BoolExpr(true)))->interp()->equals(new BoolVal(true)));
    CHECK((new EqExpr(new BoolExpr(true), new NumExpr(1)))->interp()->equals(new BoolVal(false)));

    CHECK((new IfExpr(new BoolExpr(true), new NumExpr(2), new NumExpr(3)))->interp()->equals(new NumVal(2)));
    CHECK((new IfExpr(new BoolExpr(false), new NumExpr(2), new NumExpr(3)))->interp()->equals(new NumVal(3)));
    CHECK_THROWS_WITH((new IfExpr(new AddExpr(new NumExpr(4), new NumExpr(1)), new NumExpr(2), new NumExpr(3)))->interp(),
                      "Condition is not a boolean");

    CHECK((new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)),
                      new AddExpr(new BoolExpr(false), new NumExpr(5)),
                      new NumExpr(88)))->interp()->equals(new NumVal(88)));

    CHECK_THROWS_WITH((new AddExpr(new BoolExpr(true), new NumExpr(1)))->interp(), "Adding non-numbers");
    CHECK_THROWS_WITH((new MultExpr(new BoolExpr(false), new NumExpr(2)))->interp(), "Multiplying non-numbers");
}

TEST_CASE("Expr ToString and Pretty") {
    CHECK((new NumExpr(42))->to_string() == "42");
    CHECK((new VarExpr("abc"))->to_string() == "abc");
    CHECK((new BoolExpr(true))->to_string() == "_true");
    CHECK((new BoolExpr(false))->to_string() == "_false");
    CHECK((new AddExpr(new NumExpr(1), new NumExpr(2)))->to_string() == "(1+2)");
    CHECK((new MultExpr(new NumExpr(3), new NumExpr(4)))->to_string() == "(3*4)");
    CHECK((new EqExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3))))->to_string() == "(1==(2+3))");
    CHECK((new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new BoolExpr(false), new NumExpr(88)))
          ->to_string() == "(_if (1==2) _then _false _else 88)");

    CHECK((new EqExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3))))->to_pretty_string() == "1 == 2 + 3");
    CHECK((new AddExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3)))->to_pretty_string() == "(1 == 2) + 3");
    CHECK((new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)),
                      new AddExpr(new NumExpr(1), new NumExpr(2)),
                      new NumExpr(3)))->to_pretty_string()
          == "_if 1 == 2\n_then 1 + 2\n_else 3");
    CHECK((new LetExpr("same",
                       new EqExpr(new NumExpr(1), new NumExpr(2)),
                       new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)),
                                  new AddExpr(new BoolExpr(false), new NumExpr(5)),
                                  new NumExpr(88))))->to_pretty_string()
          == "_let same = 1 == 2\n_in  _if 1 == 2\n     _then _false + 5\n     _else 88");
}

TEST_CASE("Expr Substitute") {
    CHECK((new NumExpr(7))->subst("x", new NumExpr(9))->equals(new NumExpr(7)));
    CHECK((new VarExpr("x"))->subst("x", new NumExpr(9))->equals(new NumExpr(9)));
    CHECK((new VarExpr("y"))->subst("x", new NumExpr(9))->equals(new VarExpr("y")));

    CHECK((new BoolExpr(true))->subst("x", new NumExpr(9))->equals(new BoolExpr(true)));
    CHECK((new EqExpr(new VarExpr("x"), new NumExpr(1)))->subst("x", new NumExpr(9))
          ->equals(new EqExpr(new NumExpr(9), new NumExpr(1))));
    CHECK((new IfExpr(new VarExpr("x"), new VarExpr("y"), new NumExpr(0)))->subst("x", new BoolExpr(true))
          ->equals(new IfExpr(new BoolExpr(true), new VarExpr("y"), new NumExpr(0))));

    CHECK((new LetExpr("x", new VarExpr("y"), new AddExpr(new VarExpr("x"), new VarExpr("y"))))
          ->subst("x", new NumExpr(9))
          ->equals(new LetExpr("x", new VarExpr("y"), new AddExpr(new VarExpr("x"), new VarExpr("y")))));
}

TEST_CASE("parse") {
    CHECK_THROWS_WITH(parse_str("()"), "invalid input");
    CHECK(parse_str("(1)")->equals(new NumExpr(1)));
    CHECK(parse_str("(((1)))")->equals(new NumExpr(1)));
    CHECK_THROWS_WITH(parse_str("(1"), "invalid input");

    CHECK(parse_str("1")->equals(new NumExpr(1)));
    CHECK(parse_str("10")->equals(new NumExpr(10)));
    CHECK(parse_str("-3")->equals(new NumExpr(-3)));
    CHECK(parse_str("  \n 5  ")->equals(new NumExpr(5)));
    CHECK_THROWS_WITH(parse_str("-"), "invalid input");

    CHECK(parse_str("x")->equals(new VarExpr("x")));
    CHECK(parse_str("xyz")->equals(new VarExpr("xyz")));
    CHECK(parse_str("xYz")->equals(new VarExpr("xYz")));

    CHECK(parse_str("_true")->equals(new BoolExpr(true)));
    CHECK(parse_str("_false")->equals(new BoolExpr(false)));

    CHECK(parse_str("x + y")->equals(new AddExpr(new VarExpr("x"), new VarExpr("y"))));
    CHECK(parse_str("x * y")->equals(new MultExpr(new VarExpr("x"), new VarExpr("y"))));
    CHECK(parse_str("z * x + y")->equals(new AddExpr(new MultExpr(new VarExpr("z"), new VarExpr("x")), new VarExpr("y"))));
    CHECK(parse_str("z * (x + y)")->equals(new MultExpr(new VarExpr("z"), new AddExpr(new VarExpr("x"), new VarExpr("y")))));

    CHECK(parse_str("1==2+3")->equals(new EqExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3)))));
    CHECK(parse_str("(1==2)+3")->equals(new AddExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3))));
    CHECK(parse_str("1+1==2+0")->equals(new EqExpr(new AddExpr(new NumExpr(1), new NumExpr(1)), new AddExpr(new NumExpr(2), new NumExpr(0)))));

    CHECK(parse_str("_if 1==2 _then 3 _else 4")
          ->equals(new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3), new NumExpr(4))));
    CHECK(parse_str("(_if _true _then 1 _else 2)")
          ->equals(new IfExpr(new BoolExpr(true), new NumExpr(1), new NumExpr(2))));

    CHECK(parse_str("_let x = 5 _in x")
          ->equals(new LetExpr("x", new NumExpr(5), new VarExpr("x"))));
    CHECK(parse_str("_let x=5 _in x+1")
          ->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))));
    CHECK(parse_str("_let x = 2 _in _let y = x+1 _in y*3")
          ->equals(new LetExpr("x", new NumExpr(2),
                               new LetExpr("y", new AddExpr(new VarExpr("x"), new NumExpr(1)),
                                           new MultExpr(new VarExpr("y"), new NumExpr(3))))));

    CHECK_THROWS_WITH(parse_str("_let = 5 _in x"), "invalid input");
    CHECK_THROWS_WITH(parse_str("_let x 5 _in x"), "invalid input");
    CHECK_THROWS_WITH(parse_str("_let x = _in x"), "invalid input");
    CHECK_THROWS_WITH(parse_str("_let x = 5 in x"), "invalid input");
    CHECK_THROWS_WITH(parse_str("_let x = 5 _in"), "invalid input");

    CHECK_THROWS_WITH(parse_str("_if _true 1 _else 2"), "invalid input");
    CHECK_THROWS_WITH(parse_str("_if _true _then 1 2"), "invalid input");
    CHECK_THROWS_WITH(parse_str("_if _true _then 1 _else"), "invalid input");
}
