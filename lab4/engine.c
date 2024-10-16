#include "engine.h"
#include "util.h"

bool return_error(const char **input, const char *desc, struct error *err){
    err->pos = *input;
    err->desc = desc;
    return false;
}

bool expect_char(const char **input, char expected, struct error *err) {
    // Skip past whitespace
    skip_whitespace(input);

    // Check if the statement has the expeted character, if not then return false and store the error message
    if (**input != expected) {
        static char error_desc[30]; // Adjust size as necessary
        const char *error_message = "Error caused due to missing ";
        // Copy the error message to error_desc
        int i = 0;
        while (error_message[i] != '\0') {
            error_desc[i] = error_message[i];
            i++;
        }
        // Append the expected character and end the string
        error_desc[i++] = expected;
        error_desc[i] = '\0';

        if(!return_error(input, error_desc, err)) return false;
    }

    // Move past the expected character
    (*input)++;  
    return true;
}

bool run_statements(struct context *ctx, const char **input, struct error *err) {
    skip_whitespace(input);

    if (**input == '\0' || **input == '}')
        return true;

    return run_statement(ctx, input, err) && run_statements(ctx, input, err);
}

bool run_statement(struct context *ctx, const char **input, struct error *err) {
    skip_whitespace(input);

    // Check if input starts with ENQ, ASSERT, x or y, or WHILE. 
    // Then move past the function in the sentence, check for and move past and expected characters like (, { ;
    // Get the expression / condition, and evalulate it using the appropriate function
    // Return true if everything went fine. If not then make an error in the struct err, and return false
    // Else if nothing matches then return an error. 

    if (starts_with(*input, "ENQ")) {
        (*input) += 3;

        if (!expect_char(input, '(', err)) return false;
        int value;
        if (!eval_expression(ctx, input, err, &value)) return false;

        // Enqueue the value
        enqueue(&ctx->q, value);

        if (!expect_char(input, ')', err)) return false;
        if (!expect_char(input, ';', err)) return false;
        return true;
    } else if (starts_with(*input, "ASSERT")) {
        (*input) += 6;
        if (!expect_char(input, '(', err)) return false;

        // Evaluate the condition within ASSERT, and check if it is false
        bool condition;
        if (!eval_condition(ctx, input, err, &condition)) return false;
        if (!condition) {
            if(!return_error(input, "Assertion failed", err)) return false;
        }
        if (!expect_char(input, ')', err)) return false;
        if (!expect_char(input, ';', err)) return false;
        return true;
    } else if (starts_with(*input, "x") || starts_with(*input, "y")) {
        char var = **input;
        (*input)++;
        if (!expect_char(input, '=', err)) return false;

        // Evaluate the expression on the right-hand side
        int exp;
        if (!eval_expression(ctx, input, err, &exp)) return false;

        // Assign values
        if (var == 'x') ctx->x = exp;
        else ctx->y = exp;

        if (!expect_char(input, ';', err)) return false;
        return true;
    } else if (starts_with(*input, "WHILE")) {
        (*input) += 6;
        if (!expect_char(input, '(', err)) return false;

        // Setting the starting point of condition, and declaring temporary variable to be used later
        const char *cond_start = *input, *cond;
        bool condition = false;

        // Evaluate the condition
        if (!eval_condition(ctx, input, err, &condition)) return false;
        if (!expect_char(input, ')', err)) return false;
        if (!expect_char(input, '{', err)) return false;

        // Set the starting character of loop
        const char *start = *input;
        int bracket = 1;

        // Find the end of the while loop
        while (*input && bracket > 0) {
            if (starts_with(*input, "{")) bracket++;
            else if (starts_with(*input, "}")) bracket--;
            (*input)++;
        }

        const char *end = *input - 1;

        // Run the loop senetences, and after every iteration evaluate the WHILE condition
        do {
            // The loop runs from start to finish, and temp variable loop is used to keep track of the start and finish
            const char *loop = start;
            if (!run_statements(ctx, &loop, err) || loop != end) return false;
            // Allows us to re-evaluate the condition from start each time
            cond = cond_start;
            if (!eval_condition(ctx, &cond, err, &condition)) return false;
        } while (condition);
        return true;
    } else {
        if(!return_error(input, "Unexpected function", err)) return false;
        return false;
    }
}

bool eval_expression(struct context *ctx, const char **input, struct error *err, int *out) {
    skip_whitespace(input);
    // Initialize a variable to store the value of the expression
    int part1;

    // Check if the input starts with x, y, DEQ, or a number. Else return an error
    if (starts_with(*input, "x")) {
        (*input)++;
        part1 = ctx->x;
    } else if (starts_with(*input, "y")) {
        (*input)++;
        part1 = ctx->y;
    } else if (starts_with(*input, "DEQ")) {
        (*input) += 3;
        // Try to dequeue from the queue.
        if (!dequeue(&ctx->q, &part1)) {
            if(!return_error(input, "Empty Queue", err)) return false;
        }
    } else if ((**input >= '0' && **input <= '9') || starts_with(*input, "-") || starts_with(*input, "+")) {
        if (!parse_integer(input, err, &part1)) return false;
    } else {
        if(!return_error(input, "Invalid character", err)) return false;
    }

    skip_whitespace(input);

    // The only operator that can be used is a +. Use a while loop since multiple + can be used in one expression. 
    while (**input == '+') {
        (*input)++;
        skip_whitespace(input);
        // Initialize a variable for the rest of the expression
        int part2;

        // Check if the input starts with x, y, DEQ, or a digit. Else return an error
        if (starts_with(*input, "x")) {
            (*input)++;
            part2 = ctx->x;
        } else if (starts_with(*input, "y")) {
            (*input)++;
            part2 = ctx->y;
        } else if (starts_with(*input, "DEQ")) {
            (*input) += 3;
            // Try to dequeue from the queue.
            if (!dequeue(&ctx->q, &part2)) {
                if(!return_error(input, "Empty Queue", err)) return false;
            }
        } else if ((**input >= '0' && **input <= '9') || **input == '-' || **input == '+') {
            if (!parse_integer(input, err, &part2)) return false;
        } else if (starts_with(*input, "(")) {
            (*input)++;
            // Recursivly evaluate the expression.
            if (!eval_expression(ctx, input, err, &part2)) return false;
            if (!expect_char(input, ')', err)) return false;
        } else {
            if(!return_error(input, "Invalid character", err)) return false;
        }
        // Add the two parts together, return part1
        part1 += part2;
        skip_whitespace(input);
    }
    *out = part1;
    return true;
}

bool eval_condition(struct context *ctx, const char **input, struct error *err, bool *out) {
    skip_whitespace(input);
    // Check for ! or EMPTY. If none of them are there then just evaluate the expression
    if (**input == '!') {
        (*input)++;
        bool result;
        // Recursivly analyse the condition after the NOT
        if (!eval_condition(ctx, input, err, &result)) return false;
        *out = !result;
        return true;
    } else if (starts_with(*input, "EMPTY")) {
        (*input) += 5;
        // Use the queue_empty function to see if it is empty, then return the result
        *out = queue_empty(&ctx->q);
        return true;
    } else {
        int part1, part2;
        // Evaluate part 1
        if (!eval_expression(ctx, input, err, &part1)) {
            if(!return_error(input, "Invalid expression", err)) return false;
        }
        skip_whitespace(input);
        char oper = **input;
        (*input)++;
        // Evaluate part2
        if (!eval_expression(ctx, input, err, &part2)) return false;
        // Check for =, or <. Then evaluate part2 and perform the comparison
        if (oper == '=') *out = (part1 == part2);
        else if (oper == '<') *out = (part1 < part2);
        else {
            if(!return_error(input, "Invalid operator", err)) return false;
        }
        return true;
    }
}

bool parse_integer(const char **input, struct error *err, int *out) {
    // Try to parse an integer at the current position, advancing '*input' to after it.
    const char *before = *input;
    int result = (int) strtol(before, (char **) input, 10);
    // If no characters were used, then the input was invalid.
    if (*input == before) {
        err->pos = before;
        err->desc = "Expected an integer";
        return false;
    } else {
        *out = result;
        return true;
    }
}