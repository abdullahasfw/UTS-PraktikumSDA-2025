#include <stdio.h>
#include <string.h>
#define max 100

struct stack {
    int top;
    char data[max][max];
};

void initStack(struct stack *stc) {
    stc->top = -1;
}

int isEmpty(struct stack *stc) {
    return stc->top == -1;
}

int isFull(struct stack *stc) {
    return stc->top == max - 1;
}

void push(struct stack *stc, char *str) {
    if(!isFull(stc))
        strcpy(stc->data[++(stc->top)], str);
    else
        printf("Stack penuh...\nPush gagal\n");
}

void pop(struct stack *stc, char *str) {
    if (!isEmpty(stc)) {
        strcpy(str, stc->data[(stc->top)--]);
    } else {
        printf("Stack kosong...\nPop gagal\n");
    }
}
