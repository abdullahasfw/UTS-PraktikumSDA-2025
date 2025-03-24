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
void reverse(char* str);

int menuSelect(char **menu, int len) {
    char ch;
    int pilihan = 1;
    while(1) {
        for(int i = 0; i < len; i++) {
            if(pilihan == i+1)
                printf("\e[33m\e[1m");
            printf("%s\n", menu[i]);
            printf("\e[0m");
        }
        ch = getch();
        if(ch == 'w' && pilihan != 1)
            pilihan--;
        else if(ch == 's' && pilihan != len)
            pilihan++;
        else if(ch == 224) {
            if(ch == 72 && pilihan != 0)
                pilihan--;
            else if(ch == 80 && pilihan != 5)
                pilihan++;
        }
        else if(ch == ' ' || ch == 13)
            return pilihan;
        else if(ch == 127 || ch == 27 || ch == '\b')
            return 0;

        for(int j = 0; j < len; j++) 
            printf("\e[F");
    }
}

int main() {
    char input[max], output[max];
    char *convertion[] = {"Infix to Postfix", "Postfix to Infix", "Infix to Prefix", "Prefix to Infix", "Prefix to Postfix", "Postfix to Prefix"};

    printf("\033[H\033[J");
    printf("\nPilih Konversi\n\n");
    int pilihan = menuSelect(convertion, 6);

    char asal[8], tujuan[8];
    sscanf(convertion[pilihan-1], "%s to %s", asal, tujuan);
    
    printf("\033[H\033[J");
    printf("Masukkan ekspresi %s: ", asal);

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
        #ifdef _WIN32
            Sleep(100);
        #else
            usleep(150000);
        #endif
        printf("\b");
    }
    printf("\n");
    printf("\033[H\033[J");

    printf("Hasil Konversi dari %s ke %s adalah\n\n%s\t-->\t%s\n\n", asal, tujuan, input, output);

}

void reverse(char* str) {
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
    reverse(infix);
    
    for (int i = 0; i < strlen(infix); i++) {
        if (infix[i] == '(') infix[i] = ')';
        else if (infix[i] == ')') infix[i] = '(';
    }
    
    infixToPostfix(infix, prefix);
    reverse(prefix);

    reverse(infix);

}

void prefixToInfix(char *prefix, char *infix) {
    struct stack s;
    initStack(&s);
    reverse(prefix);
    
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
    reverse(prefix);

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
