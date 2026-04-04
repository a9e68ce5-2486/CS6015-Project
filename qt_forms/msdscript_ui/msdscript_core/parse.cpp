#include "parse.h"
#include <cctype>
#include <cstdint>
#include <limits>
#include <sstream>
#include <stdexcept>

void consume_whitespace(std::istream &in);
void consume(std::istream &in, int expect);
void consume_keyword(std::istream &in, std::string keyword);
std::string parse_keyword_token(std::istream &in);
PTR(Expr) parse_expr(std::istream &in);
PTR(Expr) parse_comparg(std::istream &in);
PTR(Expr) parse_add(std::istream &in);
PTR(Expr) parse_mult(std::istream &in);
PTR(Expr) parse_call(std::istream &in);
PTR(Expr) parse_inner(std::istream &in);
PTR(Expr) parse_num(std::istream &in);
PTR(Expr) parse_var(std::istream &in);
PTR(Expr) parse_let(std::istream &in);
PTR(Expr) parse_if(std::istream &in);
PTR(Expr) parse_fun(std::istream &in);

PTR(Expr) parse(std::istream &in) {
    consume_whitespace(in);
    PTR(Expr) e = parse_expr(in);
    consume_whitespace(in);
    return e;
}

PTR(Expr) parse_str(std::string s) {
    std::stringstream ss(s);
    return parse(ss);
}

void consume_whitespace(std::istream &in) {
    while (true) {
        int c = in.peek();
        if (isspace(c)) in.get();
        else break;
    }
}

void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect) throw std::runtime_error("invalid input");
}

void consume_keyword(std::istream &in, std::string keyword) {
    for (char c : keyword) {
        if (in.get() != c) throw std::runtime_error("invalid input");
    }
}

std::string parse_keyword_token(std::istream &in) {
    consume(in, '_');
    std::string token = "_";
    while (isalpha(in.peek())) token += static_cast<char>(in.get());
    return token;
}

PTR(Expr) parse_expr(std::istream &in) {
    PTR(Expr) lhs = parse_comparg(in);
    consume_whitespace(in);

    if (in.peek() == '=') {
        in.get();
        if (in.get() != '=') throw std::runtime_error("invalid input");
        PTR(Expr) rhs = parse_expr(in);
        return NEW(EqExpr)(lhs, rhs);
    }

    return lhs;
}

PTR(Expr) parse_comparg(std::istream &in) { return parse_add(in); }

PTR(Expr) parse_add(std::istream &in) {
    PTR(Expr) lhs = parse_mult(in);
    consume_whitespace(in);

    if (in.peek() == '+') {
        in.get();
        PTR(Expr) rhs = parse_add(in);
        return NEW(AddExpr)(lhs, rhs);
    }

    return lhs;
}

PTR(Expr) parse_mult(std::istream &in) {
    PTR(Expr) lhs = parse_call(in);
    consume_whitespace(in);

    if (in.peek() == '*') {
        in.get();
        PTR(Expr) rhs = parse_mult(in);
        return NEW(MultExpr)(lhs, rhs);
    }

    return lhs;
}

PTR(Expr) parse_call(std::istream &in) {
    PTR(Expr) e = parse_inner(in);
    consume_whitespace(in);

    while (in.peek() == '(') {
        consume(in, '(');
        PTR(Expr) arg = parse_expr(in);
        consume_whitespace(in);
        consume(in, ')');
        e = NEW(CallExpr)(e, arg);
        consume_whitespace(in);
    }

    return e;
}

PTR(Expr) parse_inner(std::istream &in) {
    consume_whitespace(in);
    int c = in.peek();

    if (c == '(') {
        consume(in, '(');
        PTR(Expr) e = parse_expr(in);
        consume_whitespace(in);
        consume(in, ')');
        return e;
    }
    if (isdigit(c) || c == '-') return parse_num(in);
    if (isalpha(c)) return parse_var(in);

    if (c == '_') {
        std::string kw = parse_keyword_token(in);
        if (kw == "_let") return parse_let(in);
        if (kw == "_if") return parse_if(in);
        if (kw == "_fun") return parse_fun(in);
        if (kw == "_true") return NEW(BoolExpr)(true);
        if (kw == "_false") return NEW(BoolExpr)(false);
        throw std::runtime_error("invalid input");
    }

    consume(in, c);
    throw std::runtime_error("invalid input");
}

PTR(Expr) parse_num(std::istream &in) {
    bool negative = false;
    if (in.peek() == '-') {
        negative = true;
        in.get();
    }

    if (!isdigit(in.peek())) throw std::runtime_error("invalid input");

    uint64_t limit = negative
        ? static_cast<uint64_t>(std::numeric_limits<int>::max()) + 1
        : static_cast<uint64_t>(std::numeric_limits<int>::max());
    uint64_t num = 0;

    while (isdigit(in.peek())) {
        int digit = in.get() - '0';
        if (num > (limit - static_cast<uint64_t>(digit)) / 10) {
            throw std::runtime_error("number too large");
        }
        num = num * 10 + static_cast<uint64_t>(digit);
    }

    if (negative) {
        if (num == static_cast<uint64_t>(std::numeric_limits<int>::max()) + 1) {
            return NEW(NumExpr)(std::numeric_limits<int>::min());
        }
        return NEW(NumExpr)(-static_cast<int>(num));
    }

    return NEW(NumExpr)(static_cast<int>(num));
}

PTR(Expr) parse_var(std::istream &in) {
    std::string name = "";
    while (isalpha(in.peek())) name += static_cast<char>(in.get());
    return NEW(VarExpr)(name);
}

PTR(Expr) parse_let(std::istream &in) {
    consume_whitespace(in);

    std::string lhs = "";
    while (isalpha(in.peek())) lhs += static_cast<char>(in.get());
    if (lhs == "") throw std::runtime_error("invalid input");

    consume_whitespace(in);
    consume(in, '=');

    PTR(Expr) rhs = parse_expr(in);

    consume_whitespace(in);
    consume_keyword(in, "_in");

    PTR(Expr) body = parse_expr(in);
    return NEW(LetExpr)(lhs, rhs, body);
}

PTR(Expr) parse_if(std::istream &in) {
    consume_whitespace(in);
    PTR(Expr) test_part = parse_expr(in);

    consume_whitespace(in);
    consume_keyword(in, "_then");

    consume_whitespace(in);
    PTR(Expr) then_part = parse_expr(in);

    consume_whitespace(in);
    consume_keyword(in, "_else");

    consume_whitespace(in);
    PTR(Expr) else_part = parse_expr(in);

    return NEW(IfExpr)(test_part, then_part, else_part);
}

PTR(Expr) parse_fun(std::istream &in) {
    consume_whitespace(in);
    consume(in, '(');

    std::string formal_arg = "";
    while (isalpha(in.peek())) formal_arg += static_cast<char>(in.get());
    if (formal_arg == "") throw std::runtime_error("invalid input");

    consume(in, ')');
    consume_whitespace(in);

    PTR(Expr) body = parse_expr(in);
    return NEW(FunExpr)(formal_arg, body);
}
