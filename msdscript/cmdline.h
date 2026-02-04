/**
 * \file cmdline.h
 * \brief Declaration of command-line argument handling functionality.
 *
 * This file contains the function prototype for parsing and processing
 * command-line arguments passed to the MSDScript program.
 *
 * \author MSDScript Student
 * \date 2024
 */

#ifndef CMDLINE_H
#define CMDLINE_H

/**
 * \brief Processes command-line arguments.
 *
 * This function iterates through the command-line arguments provided by the user
 * and performs actions based on specific flags:
 * - `--help`: Prints a help message and exits.
 * - `--test`: Runs the test suite (Catch2) and exits.
 *
 * If an unknown argument is encountered, it prints an error message and exits.
 *
 * \param argc The count of command-line arguments.
 * \param argv The array of command-line argument strings.
 */
void use_arguments(int argc, char **argv);

#endif // CMDLINE_H
