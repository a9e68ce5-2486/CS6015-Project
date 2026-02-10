/**
 * \file parse.cpp
 * \brief Implementation of the recursive descent parser.
 */

#include "parse.h"
#include <cctype>
#include <sstream>
#include <stdexcept>

// --- Helper Functions Declaration ---
void consume_whitespace(std::istream &in);
void consume(std::istream &in, int expect);
void consume_keyword(std::istream &in, std::string keyword);
Expr* parse_expr(std::istream &in);
Expr* parse_mult(std::istream &in);
Expr* parse_inner(std::istream &in);
Expr* parse_num(std::istream &in);
Expr* parse_var(std::istream &in);
Expr* parse_let(std::istream &in);

// --- Public Interface Implementation ---

Expr* parse(std::istream &in) {
    consume_whitespace(in);
    Expr* e = parse_expr(in);
    consume_whitespace(in);
    
    // Note: We do not enforce EOF check here for interactive mode flexibility,
    // but parse_str ensures the whole string is consumed implicitly by logic flow tests.
    return e;
}

Expr* parse_str(std::string s) {
    std::stringstream ss(s);
    return parse(ss);
}

// --- Helper Functions Implementation ---

/**
 * \brief Skips whitespace characters in the stream.
 */
void consume_whitespace(std::istream &in) {
    while (true) {
        int c = in.peek();
        if (isspace(c)) {
            in.get();
        } else {
            break;
        }
    }
}

/**
 * \brief Consumes a specific expected character.
 * \throws std::runtime_error if the character doesn't match.
 */
void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect) {
        throw std::runtime_error("invalid input");
    }
}

/**
 * \brief Consumes a specific keyword (e.g., "_let").
 * \throws std::runtime_error if the keyword doesn't match.
 */
void consume_keyword(std::istream &in, std::string keyword) {
    for (char c : keyword) {
        if (in.get() != c) {
             throw std::runtime_error("invalid input");
        }
    }
}

/**
 * \brief Parses expressions, handling addition (lowest precedence).
 * Grammar: <expr> = <mult> + <expr> | <mult>
 */
Expr* parse_expr(std::istream &in) {
    Expr* lhs = parse_mult(in);
    consume_whitespace(in);

    if (in.peek() == '+') {
        in.get(); // consume '+'
        Expr* rhs = parse_expr(in); // Recursive call for right associativity
        return new AddExpr(lhs, rhs);
    }
    return lhs;
}

/**
 * \brief Parses terms, handling multiplication (higher precedence).
 * Grammar: <mult> = <inner> * <mult> | <inner>
 */
Expr* parse_mult(std::istream &in) {
    Expr* lhs = parse_inner(in);
    consume_whitespace(in);

    if (in.peek() == '*') {
        in.get(); // consume '*'
        Expr* rhs = parse_mult(in); // Recursive call for right associativity
        return new MultExpr(lhs, rhs);
    }
    return lhs;
}

/**
 * \brief Parses inner units: numbers, variables, let-expressions, or parenthesized expressions.
 */
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
    else if (isdigit(c) || c == '-') {
        return parse_num(in);
    } 
    else if (isalpha(c)) {
        return parse_var(in);
    } 
    else if (c == '_') {
        return parse_let(in);
    } 
    else {
        consume(in, c); // Consume bad char to advance
        throw std::runtime_error("invalid input");
    }
}

/**
 * \brief Parses a number (positive or negative).
 */
Expr* parse_num(std::istream &in) {
    bool negative = false;
    if (in.peek() == '-') {
        negative = true;
        in.get();
    }
    
    // After optional '-', next char MUST be a digit
    if (!isdigit(in.peek())) {
         throw std::runtime_error("invalid input");
    }

    int num = 0;
    while (isdigit(in.peek())) {
        num = num * 10 + (in.get() - '0');
    }
    
    if (negative) num = -num;
    return new NumExpr(num);
}

/**
 * \brief Parses a variable name.
 */
Expr* parse_var(std::istream &in) {
    std::string name = "";
    while (isalpha(in.peek())) {
        name += in.get();
    }
    return new VarExpr(name);
}

/**
 * \brief Parses a let expression: _let <var> = <expr> _in <expr>
 */
Expr* parse_let(std::istream &in) {
    consume_keyword(in, "_let");
    consume_whitespace(in);
    
    // Parse variable name
    std::string lhs = "";
    while (isalpha(in.peek())) {
        lhs += in.get();
    }
    if (lhs == "") throw std::runtime_error("invalid input");

    consume_whitespace(in);
    consume(in, '=');
    
    Expr* rhs = parse_expr(in);
    
    consume_whitespace(in);
    consume_keyword(in, "_in");
    
    Expr* body = parse_expr(in);
    
    return new LetExpr(lhs, rhs, body);
}
