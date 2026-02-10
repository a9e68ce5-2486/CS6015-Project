/**
 * \file cmdline.h
 * \brief Declaration of command-line argument handling functionality.
 *
 * This file defines the operational modes of the program and the function
 * prototype for parsing command-line arguments.
 *
 * \author MSDScript Student
 * \date 2024
 */

#ifndef CMDLINE_H
#define CMDLINE_H

/**
 * \brief Defines the operation mode based on command-line arguments.
 */
typedef enum {
  do_nothing,      ///< No specific action requested.
  do_interp,       ///< Parse and interpret input expression.
  do_print,        ///< Parse and print input expression (strict format).
  do_pretty_print  ///< Parse and pretty-print input expression.
} run_mode_t;

/**
 * \brief Processes command-line arguments.
 *
 * \param argc The count of command-line arguments.
 * \param argv The array of command-line argument strings.
 * \return The requested run mode.
 */
run_mode_t use_arguments(int argc, char **argv);

#endif // CMDLINE_H
