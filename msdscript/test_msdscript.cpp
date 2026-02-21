#include <algorithm>
#include <cctype>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include "exec.h"

static const int MAX_DEPTH = 6;
static const int NUM_TRIALS = 2000;

static std::string trim_trailing_ws(const std::string &s) {
    size_t end = s.size();
    while (end > 0 && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        end--;
    }
    return s.substr(0, end);
}

static std::string random_var(std::mt19937 &rng, int salt) {
    std::uniform_int_distribution<int> len_dist(2, 4);
    std::uniform_int_distribution<int> char_dist(0, 25);
    std::string name = "v";
    int salt_copy = salt;
    while (salt_copy > 0) {
        name.push_back(static_cast<char>('a' + (salt_copy % 26)));
        salt_copy /= 26;
    }
    int len = len_dist(rng);
    for (int i = 0; i < len; i++) {
        name.push_back(static_cast<char>('a' + char_dist(rng)));
    }
    return name;
}

static std::string random_num(std::mt19937 &rng) {
    std::uniform_int_distribution<int> num_dist(-50, 50);
    return std::to_string(num_dist(rng));
}

static std::string random_expr(std::mt19937 &rng, int depth, std::vector<std::string> env, int &name_id) {
    if (depth <= 0) {
        if (!env.empty()) {
            std::uniform_int_distribution<int> leaf_choice(0, 2);
            if (leaf_choice(rng) == 0) {
                std::uniform_int_distribution<int> var_choice(0, static_cast<int>(env.size()) - 1);
                return env[var_choice(rng)];
            }
        }
        return random_num(rng);
    }

    int max_kind = env.empty() ? 2 : 3;
    std::uniform_int_distribution<int> kind_dist(0, max_kind);
    int kind = kind_dist(rng);

    if (kind == 0) {
        return random_num(rng);
    }
    if (kind == 1 && !env.empty()) {
        std::uniform_int_distribution<int> var_choice(0, static_cast<int>(env.size()) - 1);
        return env[var_choice(rng)];
    }
    if (kind == 2) {
        std::string lhs = random_expr(rng, depth - 1, env, name_id);
        std::string rhs = random_expr(rng, depth - 1, env, name_id);
        std::uniform_int_distribution<int> op_dist(0, 1);
        char op = op_dist(rng) == 0 ? '+' : '*';
        return "(" + lhs + " " + op + " " + rhs + ")";
    }

    std::string var = random_var(rng, name_id++);
    std::string rhs = random_expr(rng, depth - 1, env, name_id);
    env.push_back(var);
    std::string body = random_expr(rng, depth - 1, env, name_id);
    return "(_let " + var + " = " + rhs + " _in " + body + ")";
}

static ExecResult run_mode(const std::string &program,
                           const std::string &mode,
                           const std::string &input) {
    const char *argv[] = {program.c_str(), mode.c_str()};
    return exec_program(2, argv, input);
}

static void print_failure(const std::string &reason,
                          const std::string &input,
                          const std::string &mode,
                          const ExecResult &a,
                          const ExecResult *b = nullptr,
                          const std::string &prog_a = "",
                          const std::string &prog_b = "") {
    std::cerr << "Found failing case\n";
    std::cerr << "Reason: " << reason << "\n";
    if (!mode.empty()) {
        std::cerr << "Mode: " << mode << "\n";
    }
    std::cerr << "Input:\n" << input << "\n";
    if (!prog_a.empty()) {
        std::cerr << "Program A: " << prog_a << "\n";
    }
    std::cerr << "A exit=" << a.exit_code
              << " out=[" << trim_trailing_ws(a.out) << "]"
              << " err=[" << trim_trailing_ws(a.err) << "]\n";
    if (b != nullptr) {
        if (!prog_b.empty()) {
            std::cerr << "Program B: " << prog_b << "\n";
        }
        std::cerr << "B exit=" << b->exit_code
                  << " out=[" << trim_trailing_ws(b->out) << "]"
                  << " err=[" << trim_trailing_ws(b->err) << "]\n";
    }
}

static int test_single_program(const std::string &program) {
    std::mt19937 rng(std::random_device{}());

    for (int i = 0; i < NUM_TRIALS; i++) {
        int name_id = 0;
        std::string expr = random_expr(rng, MAX_DEPTH, {}, name_id);

        ExecResult interp = run_mode(program, "--interp", expr);
        ExecResult printed = run_mode(program, "--print", expr);
        ExecResult pretty = run_mode(program, "--pretty-print", expr);

        if (interp.exit_code != 0 || printed.exit_code != 0 || pretty.exit_code != 0) {
            print_failure("single-program: one mode failed on valid random expression", expr, "all", interp, nullptr, program);
            std::cerr << "print exit=" << printed.exit_code << " pretty exit=" << pretty.exit_code << "\n";
            return 1;
        }

        std::string interp_out = trim_trailing_ws(interp.out);

        ExecResult interp_from_print = run_mode(program, "--interp", printed.out);
        if (interp_from_print.exit_code != 0 || trim_trailing_ws(interp_from_print.out) != interp_out) {
            print_failure("single-program: --print output changes meaning under --interp",
                          expr,
                          "--interp < print-output",
                          interp_from_print,
                          nullptr,
                          program);
            std::cerr << "Expected interp: [" << interp_out << "]\n";
            std::cerr << "Printed form:\n" << printed.out << "\n";
            return 1;
        }

        ExecResult interp_from_pretty = run_mode(program, "--interp", pretty.out);
        if (interp_from_pretty.exit_code != 0 || trim_trailing_ws(interp_from_pretty.out) != interp_out) {
            print_failure("single-program: --pretty-print output changes meaning under --interp",
                          expr,
                          "--interp < pretty-output",
                          interp_from_pretty,
                          nullptr,
                          program);
            std::cerr << "Expected interp: [" << interp_out << "]\n";
            std::cerr << "Pretty form:\n" << pretty.out << "\n";
            return 1;
        }
    }

    std::cout << "No issues found in " << NUM_TRIALS << " random trials for " << program << "\n";
    return 0;
}

static int test_two_programs(const std::string &program_a, const std::string &program_b) {
    std::mt19937 rng(std::random_device{}());
    const std::vector<std::string> modes = {"--interp", "--print", "--pretty-print"};

    for (int i = 0; i < NUM_TRIALS; i++) {
        int name_id = 0;
        std::string expr = random_expr(rng, MAX_DEPTH, {}, name_id);

        for (const std::string &mode : modes) {
            ExecResult a = run_mode(program_a, mode, expr);
            ExecResult b = run_mode(program_b, mode, expr);

            bool mismatch = false;
            if (a.exit_code != b.exit_code) {
                mismatch = true;
            }
            if (trim_trailing_ws(a.out) != trim_trailing_ws(b.out)) {
                mismatch = true;
            }
            if (trim_trailing_ws(a.err) != trim_trailing_ws(b.err)) {
                mismatch = true;
            }

            if (mismatch) {
                print_failure("two-program: outputs differ", expr, mode, a, &b, program_a, program_b);
                return 1;
            }
        }
    }

    std::cout << "No differences found in " << NUM_TRIALS
              << " random trials between " << program_a
              << " and " << program_b << "\n";
    return 0;
}

int main(int argc, char **argv) {
    try {
        if (argc == 2) {
            return test_single_program(argv[1]);
        }
        if (argc == 3) {
            return test_two_programs(argv[1], argv[2]);
        }

        std::cerr << "Usage: ./test_msdscript <msdscript-path> [other-msdscript-path]\n";
        return 1;
    } catch (std::runtime_error &exn) {
        std::cerr << exn.what() << "\n";
        return 1;
    }
}
