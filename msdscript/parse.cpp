#include "parse.h"
#include <cctype>
#include <sstream>
#include <stdexcept>

void consume_whitespace(std::istream &in);
void consume(std::istream &in, int expect);
void consume_keyword(std::istream &in, std::string keyword);
std::string parse_keyword_token(std::istream &in);
Expr* parse_expr(std::istream &in);
Expr* parse_comparg(std::istream &in);
Expr* parse_add(std::istream &in);
Expr* parse_mult(std::istream &in);
Expr* parse_call(std::istream &in);
Expr* parse_inner(std::istream &in);
Expr* parse_num(std::istream &in);
Expr* parse_var(std::istream &in);
Expr* parse_let(std::istream &in);
Expr* parse_if(std::istream &in);
Expr* parse_fun(std::istream &in);

Expr* parse(std::istream &in) {
    consume_whitespace(in);
    Expr* e = parse_expr(in);
    consume_whitespace(in);
    return e;
}

Expr* parse_str(std::string s) {
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

Expr* parse_expr(std::istream &in) {
    Expr* lhs = parse_comparg(in);
    consume_whitespace(in);

    if (in.peek() == '=') {
        in.get();
        if (in.get() != '=') throw std::runtime_error("invalid input");
        Expr* rhs = parse_expr(in);
        return new EqExpr(lhs, rhs);
    }

    return lhs;
}

Expr* parse_comparg(std::istream &in) { return parse_add(in); }

Expr* parse_add(std::istream &in) {
    Expr* lhs = parse_mult(in);
    consume_whitespace(in);

    if (in.peek() == '+') {
        in.get();
        Expr* rhs = parse_add(in);
        return new AddExpr(lhs, rhs);
    }

    return lhs;
}

Expr* parse_mult(std::istream &in) {
    Expr* lhs = parse_call(in);
    consume_whitespace(in);

    if (in.peek() == '*') {
        in.get();
        Expr* rhs = parse_mult(in);
        return new MultExpr(lhs, rhs);
    }

    return lhs;
}

Expr* parse_call(std::istream &in) {
    Expr* e = parse_inner(in);
    consume_whitespace(in);

    while (in.peek() == '(') {
        consume(in, '(');
        Expr* arg = parse_expr(in);
        consume_whitespace(in);
        consume(in, ')');
        e = new CallExpr(e, arg);
        consume_whitespace(in);
    }

    return e;
}

Expr* parse_inner(std::istream &in) {
    consume_whitespace(in);
    int c = in.peek();

    if (c == '(') {
        consume(in, '(');
        Expr* e = parse_expr(in);
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
        if (kw == "_true") return new BoolExpr(true);
        if (kw == "_false") return new BoolExpr(false);
        throw std::runtime_error("invalid input");
    }

    consume(in, c);
    throw std::runtime_error("invalid input");
}

Expr* parse_num(std::istream &in) {
    bool negative = false;
    if (in.peek() == '-') {
        negative = true;
        in.get();
    }

    if (!isdigit(in.peek())) throw std::runtime_error("invalid input");

    int num = 0;
    while (isdigit(in.peek())) num = num * 10 + (in.get() - '0');

    if (negative) num = -num;
    return new NumExpr(num);
}

Expr* parse_var(std::istream &in) {
    std::string name = "";
    while (isalpha(in.peek())) name += static_cast<char>(in.get());
    return new VarExpr(name);
}

Expr* parse_let(std::istream &in) {
    consume_whitespace(in);

    std::string lhs = "";
    while (isalpha(in.peek())) lhs += static_cast<char>(in.get());
    if (lhs == "") throw std::runtime_error("invalid input");

    consume_whitespace(in);
    consume(in, '=');

    Expr* rhs = parse_expr(in);

    consume_whitespace(in);
    consume_keyword(in, "_in");

    Expr* body = parse_expr(in);
    return new LetExpr(lhs, rhs, body);
}

Expr* parse_if(std::istream &in) {
    consume_whitespace(in);
    Expr* test_part = parse_expr(in);

    consume_whitespace(in);
    consume_keyword(in, "_then");

    consume_whitespace(in);
    Expr* then_part = parse_expr(in);

    consume_whitespace(in);
    consume_keyword(in, "_else");

    consume_whitespace(in);
    Expr* else_part = parse_expr(in);

    return new IfExpr(test_part, then_part, else_part);
}

Expr* parse_fun(std::istream &in) {
    consume_whitespace(in);
    consume(in, '(');

    std::string formal_arg = "";
    while (isalpha(in.peek())) formal_arg += static_cast<char>(in.get());
    if (formal_arg == "") throw std::runtime_error("invalid input");

    consume(in, ')');
    consume_whitespace(in);

    Expr* body = parse_expr(in);
    return new FunExpr(formal_arg, body);
}
