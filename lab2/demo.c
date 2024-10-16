#include "demo.h"
#include "stack.h"

bool check_brackets(const char *str) {
    stack_ptr s = stack_new();
    // TODO (task 3): using the stack 's', check the brackets in the strings.

    int i=0;
    char out;

    while (str[i] != '\0') 
    {
        switch(str[i]){
        case '(':
            stack_push(s, str[i]);
            break;

        case '{':
            stack_push(s, str[i]);
            break;

        case '[':
            stack_push(s, str[i]);
            break;

        case ')':
            stack_pop(s, &out);
            if (out != '(') return false;
            break;

        case '}':
            stack_pop(s, &out);
            if (out != '{') return false;
            break;

        case ']':
            stack_pop(s, &out); 
            if (out != '[') return false;
            break;
        }
        out='\n';
        i++;
    }    
    if(stack_pop(s, &out)) return false;
    stack_free(s);
    return true;
}
