#include <stdio.h>
#ifdef _WIN32
    #include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt); // Simpan pengaturan terminal lama
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Matikan echo dan input line-buffered
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Terapkan pengaturan baru
    ch = getchar(); // Ambil satu karakter
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Kembalikan pengaturan lama
    return ch;
}
#endif

