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

char input[max], output[max];
int precedence(char c);
void infixToPostfix(char *src, char* dest);     
void postfixToInfix(char *src, char* dest);
void infixToPrefix(char *src, char* dest);
void prefixToInfix(char *src, char* dest);
void prefixToPostfix(char *src, char* dest);    
void postfixToPrefix(char *src, char* dest);
void reverse(char* str);

void clearScr() {
    #ifdef _WIN32
        system("cls");
    #else 
        system("clear");
    #endif
}

void printTitle(char *str) {
    int len = strlen(str);
    for(int i = 0; i < len + 10; i++)
        printf("=");
    printf("\n");
    printf("\e[1m---  %s  ---\e[0m\n", str);
    for(int i = 0; i < len + 10; i++)
        printf("=");
    printf("\n\n");
}

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
        if((ch == 'w' || ch == 'A') && pilihan != 1)
            pilihan--;
        else if((ch == 's' || ch == 'B') && pilihan != len)
            pilihan++;
        else if(ch == ' ' || ch == 13)
            return pilihan;
        else if(ch == 127 || ch == 27 || ch == '\b')
            return 0;

        for(int j = 0; j < len; j++) 
            printf("\e[A");
    }
}

int expressionCheck(char *str, char *type) {
    int len = strlen(str);
    if(strcmp(type, "Infix") == 0) {
        if(precedence(str[0]) || precedence(str[len-1]))     
            return 0;

    } else if(strcmp(type, "Postfix") == 0) {
        if(precedence(str[0]) || !precedence(str[len-1]))
            return 0;

    } else if(strcmp(type, "Prefix") == 0) {
        if(!precedence(str[0]) || precedence(str[len-1]))
            return 0;
    
    } else {
        return 0;
    }
    return 1;
}

void convertMenu() {
    char *convertion[] = {"Infix to Postfix", "Postfix to Infix", "Infix to Prefix", "Prefix to Infix", "Prefix to Postfix", "Postfix to Prefix"};
    int pilihan;

while(1) {
    clearScr();
    printTitle("Pilih Konversi");
    pilihan = menuSelect(convertion, 6);
    if(pilihan == 0)
        return;

    char asal[8], tujuan[8];
    sscanf(convertion[pilihan-1], "%s to %s", asal, tujuan);
    
    clearScr();
    printf("Masukkan ekspresi %s:\n--->   ", asal);
    scanf("%s", input);
    if(!expressionCheck(input, asal)) {
        printf("\e[30mInvalid Expression!\e[0m\n");
        goto akhir;
    }

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
            Sleep(150);
        #else
            usleep(150000);
        #endif
        printf("\b");
    }
    printf("\n");
    clearScr();

    printf("Hasil Konversi dari %s ke %s adalah\n\n%s\t-->\t%s\n\n", asal, tujuan, input, output);
    akhir:
    printf("Tekan BACKSPACE untuk kembali!");

    char ch;
    while(1) {
        ch = getch();
        if(ch == 27 || ch == 127 || ch == '\b')
            break;
    }
}

}

int main() {
    
    char *menu[] = {"MULAI", "ABOUT", "HOW TO START", "EXIT"};
    int select;
while(1) {
    clearScr();
    printTitle("SELAMAT DATANG");
    select = menuSelect(menu, 4);
        
    if(select == 1)
        convertMenu();
    else if(select == 2) {
        clearScr();
        printTitle("Tentang");
        printf("Rumus matematika sering kali lebih mudah dipahami dengan menggunakan tanda kurung. Namun, dalam komputer,  ekspresi dengan beberapa tanda kurung bisa jadi tidak efisien. Jadi, matematikawan telah menciptakan notasi yang berbeda seperti ekspresi infiks, prefiks, dan postfiks untuk mengurangi pekerjaan komputasi.\n\n");
        printf("\e[1mINFIX\e[0m\n");
        printf("Ekspresi infiks merupakan jenis ekspresi yang paling umum.  Notasi ini biasanya digunakan saat menulis ekspresi aritmatika secara manual. Selain itu, dalam ekspresi infiks, kita menempatkan operator di antara dua operan yang dioperasikannya. Misalnya, operator + muncul di antara operan A dan B dalam ekspresi A + B.\n\n");
        printf("\e[1mPREFIX\e[0m\n");
        printf("Ekspresi prefix, juga dikenal sebagai notasi Polandia, menempatkan operator sebelum operan. Misalnya, pada ekspresi + AB , kita menempatkan operator + di depan operan A dan B \n\n");
        printf("\e[1mPOSTFIX\e[0m\n");
        printf("Ekspresi postfix, juga dikenal sebagai notasi Polandia terbalik, di mana kita menempatkan operator setelah operan. Misalnya, dalam ekspresi AB + , operator + kita tempatkan setelah operan A dan B.\n\n");

        printf("Tekan BACKSPACE untuk kembali!");
        char ch;
        while(1) {
            ch = getch();
            if(ch == 127 || ch == 27 || ch == '\b')
                break;
        }
    } else if(select == 3) {
        clearScr();
        printTitle("How To Start");
        printf("Gunakan tanda panah atau W dan S untuk memilih menu.\n");
        printf("Gunakan tombol SPACE atau ENTER untuk masuk ke menu.\n");
        printf("Gunakan tombol BACKSPACE atau ESC untuk kembali ke menu sebelumnya.\n\n");
        printf("Tekan BACKSPACE untuk kembali!");
        char ch;
        while(1) {
            ch = getch();
            if(ch == 127 || ch == 27 || ch == '\b')
                break;
        }
    } else if(select == 4)
        exit(0);
}
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
