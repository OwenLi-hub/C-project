#include <malloc.h>
#include "stack.h"

struct stack_node {
    // TODO (task 1): what fields do we need here?
    struct stack_node *next;
    char value;
};

struct stack {
    // Null if the stack is empty.
    struct stack_node *head;
};

stack_ptr stack_new() {
    stack_ptr s = malloc(sizeof(struct stack));
    s->head = NULL;
    return s;
}

void stack_free(stack_ptr s) {
    // TODO (task 1): how do we make sure the nodes don't leak memory?
    while(s->head!=NULL){
        //the while loop will creat a temp that stores the new head an frees it
        struct stack_node *temp=s->head;
        s->head=s->head->next;
        free(temp);
    }
    free(s);
}

void stack_push(stack_ptr s, char c) {
    // TODO (task 1): how do we push an entry onto the stack?
    struct stack_node *new = malloc(sizeof(struct stack_node));
    //if new is NULL return the code
     if (new == NULL) {
        return;
    }
    //link the head value into the next one
    new->value = c;
    new->next= s->head;
    s->head = new;

}

bool stack_pop(stack_ptr s, char *out) {
    // TODO (task 1): how do we pop an entry from the stack?
    if(s->head==NULL){
        return false;
    }
    //assigned the head vlaue into out
    *out=s->head->value;
    struct stack_node *temp=s->head;
    //assign the head value to the next value
    s->head=s->head->next;
    //free the temp head value
    free(temp);
    return true;
}
