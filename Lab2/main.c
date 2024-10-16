#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct list_node {
    int value;
    struct list_node *next;
};

void print_list(struct list_node *head) {
// TODO: implement this function for printing all nodes(task 1)
struct list_node *ptr= head;
while(ptr!=NULL){
    printf("%d,", ptr->value);
    ptr=ptr->next;
}

}

struct list_node* insertFirst(struct list_node *head, int data) {
// TODO: implement this function for inserting the first node in an empty linked list(task 1).
struct list_node *new = (struct list_node*)malloc(sizeof(struct list_node));
new->value=data;
new->next=head;
head=new;
return head;

}

void insert_at_tail(struct list_node **head, int value) {
// TODO: implement this function for inserting a node at the tail of the linked list  (task 1).

struct list_node *new = (struct list_node*)malloc(sizeof(struct list_node));
new->value=value;
new->next=NULL;

struct list_node *tail;
tail=*head;

while ((tail->next)!=NULL){
    tail=tail->next;
    } 

tail->next=new;

}


void swap_adjacent(struct list_node *head) {
    // TODO: implement this (task 2).

    struct list_node *curr = head;
    while (curr!= NULL && curr->next != NULL){
        int n = curr->value;
        curr->value = curr->next->value;
        curr->next->value=n;
        curr=curr->next->next;
    }


}

void double_list(struct list_node *head) {
    // TODO: implement this (task 3).

     struct list_node  *curr=head;
     struct list_node *new = (struct list_node*)malloc(sizeof(struct list_node));
     struct list_node *new_head=NULL;
     struct list_node *new_tail=NULL;

     curr = head->next;

     while(curr!=NULL){
       new->value=curr->value;
       new->next=NULL;

       if(new_tail==NULL){
        new_head=new;
        new_tail=new;
       } 

       else{
        new_tail->next=new;
        new_tail=new;
       }

       curr=curr->next;
    }

    if(head==NULL){
        head=new_head;}
         else{
        struct list_node* old_tail = head;

        while(old_tail->next != NULL){
            old_tail = old_tail->next;
        }
        old_tail->next = new_head;
    }

     }



void remove_adjacent_duplicates(struct list_node *head) {
    // TODO: implement this (task 4).
    struct list_node *curr = head, *temp;

    while (curr != NULL&&curr->next!=NULL) {
    // curr=curr->next;
    if(curr->value == curr->next->value){
        // printf("%d", curr->value);
        temp=curr->next;
        curr -> next=temp->next;
        free(temp);
    }
    curr=curr->next;
    }
    }




int main() {
    // List is initially empty.
    struct list_node *head = NULL;

    // insert head
    head = insertFirst(head, 1);
    insert_at_tail(&head, 2);
    insert_at_tail(&head, 3);
    insert_at_tail(&head, 3);
    insert_at_tail(&head, 4);
    insert_at_tail(&head, 5);
    insert_at_tail(&head, 6);


    printf("Original list: ");
    print_list(head);

    printf("\nSwapping adjacent items: ");
    swap_adjacent(head);
    print_list(head);

    printf("\nDuplicating all items: ");
    double_list(head);
    print_list(head);

    printf("\nRemoving adjacent duplicates: ");
    remove_adjacent_duplicates(head);
    print_list(head);

    return 0;
}
