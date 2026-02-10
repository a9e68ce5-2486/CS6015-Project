/**
 * \file parse.h
 * \brief Declaration of parsing functions for the MSDScript interpreter.
 *
 * This file contains the function prototypes for parsing strings and input streams
 * into Expression objects.
 *
 * \author MSDScript Student
 * \date 2024
 */

#ifndef PARSE_H
#define PARSE_H

#include "expr.h"
#include <string>
#include <iostream>

/**
 * \brief Parses a mathematical expression from a string.
 *
 * \param s The string containing the expression.
 * \return A pointer to the parsed Expr object.
 * \throws std::runtime_error if the string contains invalid syntax.
 */
Expr* parse_str(std::string s);

/**
 * \brief Parses a mathematical expression from an input stream.
 *
 * \param in The input stream to read from.
 * \return A pointer to the parsed Expr object.
 * \throws std::runtime_error if the stream contains invalid syntax.
 */
Expr* parse(std::istream &in);

#endif
