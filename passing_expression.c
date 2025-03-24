#include <stdio.h>
#include "stack.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "func.h"
#ifdef _WIN32
    #include <windows.h>
#else 
    #include <unistd.h>
#endif

#define max 100

int precedence(char c);
void infixToPostfix(char *src, char* dest);     
void postfixToInfix(char *src, char* dest);
void infixToPrefix(char *src, char* dest);
void prefixToInfix(char *src, char* dest);
void prefixToPostfix(char *src, char* dest);    
void postfixToPrefix(char *src, char* dest);
void strrev(char* str);        

int main() {
    int pilihan;
    char input[max], output[max];

    printf("\033[H\033[J");
    printf("\n--- Selamat Datang! ---\n");
    printf("1. Infix to Postfix\n");
    printf("2. Postfix to Infix\n");
    printf("3. Infix to Prefix\n");
    printf("4. Prefix to Infix\n");
    printf("5. Prefix to Postfix\n");
    printf("6. Postfix to Prefix\n");
    printf("Pilih konversi: ");
    scanf("%d", &pilihan);


    printf("\033[H\033[J");

    printf("Masukkan ekspresi ");
    if(pilihan == 1 || pilihan == 3) {
        printf("Infix: ");
    } else if(pilihan == 2 || pilihan == 6) {
        printf("Postfix: ");
    } else if(pilihan == 4 || pilihan == 5) {
        printf("Prefix: ");
    }


    scanf("%s", input);
    switch(pilihan) {
        case 1:
            infixToPostfix(input, output);
            break;
        case 2:
            postfixToInfix(input, output);
            break;
        case 3:
            infixToPrefix(input, output);
            break;
        case 4:
            prefixToInfix(input, output);
            break;
        case 5:
            prefixToPostfix(input, output);
            break;
        case 6:
            postfixToPrefix(input, output);
            break;
    }


    printf("Loading... \t\t");
    char *spin = "/-\\|";
    for(int i=0; i < 20; i++) {
        printf("%c", spin[i%4]);
        fflush(stdout);
        usleep(150000);
        printf("\b");
    }
    printf("\n");
    printf("\033[H\033[J");

    printf("Hasil Konversi adalah \033[33m%s\n", output);

}

void strrev(char* str) {
    int length = strlen(str);
    int i, j;
    char temp;

    for(i = 0, j = length - 1; i < j; i++, j--) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

int precedence(char c) {
    if(c == '^')
        return 3;
    else if(c == '*' || c == '/')
        return 2;
    else if(c == '+' || c == '-')
        return 1;
    return 0;
}

void infixToPostfix(char *infix, char *postfix) {
    struct stack s;
    initStack(&s);
    int j = 0;
    
    for (int i = 0; i < strlen(infix); i++) {
        if (isalnum(infix[i])) {
            postfix[j++] = infix[i];
        } else if (infix[i] == '(') {
            char temp[2] = {infix[i], '\0'};
            push(&s, temp);
        } else if (infix[i] == ')') {
            char temp[max];
            while (!isEmpty(&s)) {
                pop(&s, temp);
                if (temp[0] == '(') break;
                postfix[j++] = temp[0];
            }
        } else {
            while (!isEmpty(&s)) {
                char temp[max];
                pop(&s, temp);
                if (precedence(temp[0]) < precedence(infix[i])) {
                    push(&s, temp);
                    break;
                }
                postfix[j++] = temp[0];
            }
            char op[2] = {infix[i], '\0'};
            push(&s, op);
        }
    }
    
    while (!isEmpty(&s)) {
        char temp[max];
        pop(&s, temp);
        postfix[j++] = temp[0];
    }
    
    postfix[j] = '\0';
}

void postfixToInfix(char *postfix, char *infix) {
    struct stack s;
    initStack(&s);
    
    for (int i = 0; i < strlen(postfix); i++) {
        if (isalnum(postfix[i])) {
            char temp[2] = {postfix[i], '\0'};
            push(&s, temp);
        } else {
            char op1[max], op2[max], expr[max];
            pop(&s, op2);
            pop(&s, op1);
            sprintf(expr, "(%s%c%s)", op1, postfix[i], op2);
            push(&s, expr);
        }
    }
    
    pop(&s, infix);
}

void infixToPrefix(char *infix, char *prefix) {
    strrev(infix);
    
    for (int i = 0; i < strlen(infix); i++) {
        if (infix[i] == '(') infix[i] = ')';
        else if (infix[i] == ')') infix[i] = '(';
    }
    
    infixToPostfix(infix, prefix);
    strrev(prefix);
}

void prefixToInfix(char *prefix, char *infix) {
    struct stack s;
    initStack(&s);
    strrev(prefix);
    
    for (int i = 0; i < strlen(prefix); i++) {
        if (isalnum(prefix[i])) {
            char temp[2] = {prefix[i], '\0'};
            push(&s, temp);
        } else {
            char op1[max], op2[max], expr[max];
            pop(&s, op1);
            pop(&s, op2);
            sprintf(expr, "(%s%c%s)", op1, prefix[i], op2);
            push(&s, expr);
        }
    }
    
    pop(&s, infix);
}

void prefixToPostfix(char *prefix, char *postfix) {
    char infix[max];
    prefixToInfix(prefix, infix);
    infixToPostfix(infix, postfix);
}

void postfixToPrefix(char *postfix, char *prefix) {
    char infix[max];
    postfixToInfix(postfix, infix);
    infixToPrefix(infix, prefix);
}
