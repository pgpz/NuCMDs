// Configuration for NuCMDs.

// all of these ARE configurable.
// you will have to compile each command seperately if you want it to change in any way.
// i hope y ou like building shit from source 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#ifdef URL_COMMAND
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: url <url>\n");
        return 1;
    }
    char cmd[2048];
    snprintf(cmd, sizeof(cmd), "start %s", argv[1]);
    system(cmd);
    return 0;
}
#endif

#ifdef RUN_COMMAND
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: run <program or script>\n");
        return 1;
    }
    ShellExecuteA(NULL, "open", argv[1], NULL, NULL, SW_SHOW);
    return 0;
}
#endif

#ifdef GOTO_COMMAND
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: goto <directory>\n");
        return 1;
    }
    ShellExecuteA(NULL, "open", argv[1], NULL, NULL, SW_SHOW);
    return 0;
}
#endif

#ifdef WHO_COMMAND
int main() {
    char username[256];
    DWORD size = sizeof(username);
    if (GetUserNameA(username, &size)) {
        printf("You are: %s\n", username);
    } else {
        printf("Unable to determine user.\n");
    }
    return 0;
}
#endif

#ifdef SAY_COMMAND
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: say <text to echo>\n");
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        printf("%s", argv[i]);
        if (i < argc - 1) printf(" ");
    }
    printf("\n");
    return 0;
}
#endif

#ifdef CREATE_COMMAND
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: create <filename>\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "w");
    if (!f) {
        perror("failed to create file!!");
        return 1;
    }
    fclose(f);
    printf("created file: %s\n", argv[1]);
    return 0;
}
#endif

#ifdef TIMESTAMP_COMMAND
#include <time.h>

int main(int argc, char *argv[]) {
    time_t now = time(NULL);
    if (now == ((time_t)-1)) {
        printf("Failed to get current time\n");
        return 1;
    }

    struct tm *local = localtime(&now);
    if (!local) {
        printf("Failed to convert time\n");
        return 1;
    }

    char buf[64];
    if (strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local) == 0) {
        printf("Failed to format time\n");
        return 1;
    }

    printf("%s\n", buf);
    return 0;
}
#endif

#ifdef IPINFO_COMMAND
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        printf("Failed to get hostname.\n");
        WSACleanup();
        return 1;
    }
    printf("Hostname: %s\n", hostname);

    struct hostent *host = gethostbyname(hostname);
    if (!host) {
        printf("Failed to get host info.\n");
        WSACleanup();
        return 1;
    }

    printf("IPv4 Addresses:\n");
    for (int i = 0; host->h_addr_list[i] != NULL; i++) {
        struct in_addr addr;
        memcpy(&addr, host->h_addr_list[i], sizeof(struct in_addr));
        printf("  %s\n", inet_ntoa(addr));
    }

    WSACleanup();
    return 0;
}
#endif

#ifdef MEM_COMMAND
#include <windows.h>
#include <stdio.h>

int main() {
    MEMORYSTATUSEX memStatus = {0};
    memStatus.dwLength = sizeof(MEMORYSTATUSEX);

    if (GlobalMemoryStatusEx(&memStatus)) {
        printf("Total RAM: %llu MB\n", memStatus.ullTotalPhys / (1024 * 1024));
        printf("Available RAM: %llu MB\n", memStatus.ullAvailPhys / (1024 * 1024));
    } else {
        printf("Failed to get memory info.\n");
        return 1;
    }

    return 0;
}
#endif

#ifdef CPU_COMMAND
#include <windows.h>
#include <stdio.h>

int main() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    printf("CPU Architecture: ");
    switch (sysInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            printf("x64 (AMD or Intel)\n");
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            printf("x86\n");
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            printf("ARM\n");
            break;
        default:
            printf("Unknown (%d)\n", sysInfo.wProcessorArchitecture);
            break;
    }

    printf("Number of Logical Cores: %u\n", sysInfo.dwNumberOfProcessors);
    printf("Processor Type: %u\n", sysInfo.dwProcessorType); 

    return 0;
}
#endif

#ifdef FLIP_COMMAND
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand((unsigned int)time(NULL));
    int result = rand() % 2;

    if (result == 0) {
        printf("Heads\n");
    } else {
        printf("Tails\n");
    }

    return 0;
}
#endif

#ifdef DAY_COMMAND
#include <stdio.h>
#include <time.h>

int main() {
    time_t now = time(NULL);
    if (now == ((time_t)-1)) {
        printf("Failed to get current time.\n");
        return 1;
    }

    struct tm *local = localtime(&now);
    if (!local) {
        printf("Failed to convert time.\n");
        return 1;
    }

    char buf[128];
    if (strftime(buf, sizeof(buf), "%A, %B %d, %Y", local) == 0) {
        printf("Failed to format date.\n");
        return 1;
    }

    printf("Today is %s\n", buf);
    return 0;
}
#endif

#ifdef EIGHTBALL_COMMAND
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: 8ball <your question>\n");
        return 1;
    }

    const char *responses[] = {
        "Yes.",
        "No.",
        "Maybe.",
        "Ask again later.",
        "Definitely.",
        "I don't think so.",
        "Without a doubt.",
        "Highly unlikely.",
        "Possibly.",
        "Signs point to yes."
    };

    int num_responses = sizeof(responses) / sizeof(responses[0]);
    srand((unsigned int)time(NULL));
    int idx = rand() % num_responses;

    printf("The magic 8ball says: %s\n", responses[idx]);
    return 0;
}
#endif

#ifdef COUNTDOWN_COMMAND
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: countdown <seconds>\n");
        return 1;
    }

    int seconds = atoi(argv[1]);
    if (seconds <= 0) {
        printf("Please enter a positive number of seconds.\n");
        return 1;
    }

    for (int i = seconds; i >= 0; i--) {
        printf("%d\n", i);
        Sleep(1000);
    }

    printf("Done!\n");
    return 0;
}
#endif

#ifdef ROLL_COMMAND
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: roll <sides>\n");
        return 1;
    }

    int sides = atoi(argv[1]);
    if (sides <= 1) {
        printf("Number of sides must be greater than 1.\n");
        return 1;
    }

    srand((unsigned int)time(NULL));
    int roll = (rand() % sides) + 1;
    printf("You rolled a %d !!!! (1-%d)\n", roll, sides);

    return 0;
}
#endif

#ifdef PARTY_COMMAND
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void colorful_text(const char *text, WORD color) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(h, &info);

    SetConsoleTextAttribute(h, color);
    printf("%s", text);
    SetConsoleTextAttribute(h, info.wAttributes); 
}

int main() {
    const char *banner[] = {
        "=============================\n",
        "       PARTY  MODE  ON       \n",
        "=============================\n",
        "* * * * * * * * * * * * * * *\n",
        "   Let us have fun tonight! \n",
        "* * * * * * * * * * * * * * *\n",
        NULL
    };

    system("cls"); 
    for (int i = 0; banner[i]; i++) {
        colorful_text(banner[i], FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        Sleep(200);
    }

    return 0;
}
#endif

#ifdef PING_COMMAND
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ping <hostname_or_ip>\n");
        return 1;
    }

    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "ping %s", argv[1]);
    system(cmd);

    return 0;
}
#endif

#ifdef MORSE_COMMAND
#include <stdio.h>
#include <ctype.h>
#include <string.h>

const char *morse_table[36] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",     // A-I
    ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",   // J-R
    "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",          // S-Z
    "-----", ".----", "..---", "...--", "....-", ".....", "-....",     // 0-6
    "--...", "---..", "----."                                          // 7-9
};

void print_morse_char(char c) {
    if (c >= 'A' && c <= 'Z') {
        printf("%s ", morse_table[c - 'A']);
    } else if (c >= '0' && c <= '9') {
        printf("%s ", morse_table[26 + c - '0']);
    } else if (c == ' ') {
        printf("/ ");  
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: morse <text>\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        for (size_t j = 0; j < strlen(argv[i]); j++) {
            char c = toupper(argv[i][j]);
            print_morse_char(c);
        }
        printf("/ ");  
    }
    printf("\n");
    return 0;
}
#endif

#ifdef CLEAR_COMMAND
#include <stdlib.h>

int main() {
    system("cls");
    return 0;
}
#endif

#ifdef UPTIME_COMMAND
#include <stdio.h>
#include <windows.h>

int main() {
    DWORD ms = GetTickCount(); 

    unsigned long seconds = ms / 1000;
    unsigned long hours = seconds / 3600;
    unsigned long minutes = (seconds % 3600) / 60;
    unsigned long secs = seconds % 60;

    printf("System Uptime: %lu hours, %lu minutes, %lu seconds\n", hours, minutes, secs);
    return 0;
}
#endif

#ifdef DELETE_COMMAND
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: delete <file_or_directory>\n");
        return 1;
    }

    // Check if path is directory or file
    DWORD attr = GetFileAttributesA(argv[1]);
    if (attr == INVALID_FILE_ATTRIBUTES) {
        perror("File or directory not found");
        return 1;
    }

    BOOL success;
    if (attr & FILE_ATTRIBUTE_DIRECTORY) {
        success = RemoveDirectoryA(argv[1]);
        if (!success) {
            perror("Failed to delete directory (maybe not empty)");
            return 1;
        }
        printf("Deleted directory: %s\n", argv[1]);
    } else {
        success = DeleteFileA(argv[1]);
        if (!success) {
            perror("Failed to delete file");
            return 1;
        }
        printf("deleted file: %s\n", argv[1]);
    }

    return 0;
}
#endif

#ifdef READ_COMMAND
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: read <filename>\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }

    fclose(file);
    return 0;
}
#endif

#ifdef WRITE_COMMAND
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("usage: write <filename> <text to write>\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "w");
    if (!file) {
        perror("failed to open file for writing");
        return 1;
    }

    for (int i = 2; i < argc; i++) {
        fprintf(file, "%s", argv[i]);
        if (i < argc - 1) fprintf(file, " ");
    }
    fprintf(file, "\n");

    fclose(file);
    printf("wrote to file: %s\n", argv[1]);
    return 0;
}
#endif

#ifdef FIND_COMMAND
#include <stdio.h>
#include <windows.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: find <pattern>\n");
        return 1;
    }

    WIN32_FIND_DATAA findData;
    HANDLE hFind;

    char searchPattern[260];
    snprintf(searchPattern, sizeof(searchPattern), ".\\*%s*", argv[1]);

    hFind = FindFirstFileA(searchPattern, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("No files found matching '%s'\n", argv[1]);
        return 1;
    }

    do {
        printf("%s\n", findData.cFileName);
    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);
    return 0;
}
#endif

#ifdef CRYPTO_COMMAND
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

const char* symbol_to_id(const char* sym) {
    if (strcmp(sym, "BTC") == 0) return "bitcoin";
    if (strcmp(sym, "ETH") == 0) return "ethereum";
    if (strcmp(sym, "XMR") == 0) return "monero";
    if (strcmp(sym, "SOL") == 0) return "solana";
    if (strcmp(sym, "XRP") == 0) return "ripple";
    if (strcmp(sym, "LTC") == 0) return "litecoin";
    return NULL;
}

int fetch_price(const char* coin_id, char** response) {
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct hostent *remoteHost = NULL;
    struct sockaddr_in serverAddr;
    char sendbuf[512];
    char recvbuf[2048];
    int iResult;

    *response = NULL;

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 0;
    }

    remoteHost = gethostbyname("api.coingecko.com");
    if (!remoteHost) {
        printf("Failed to resolve hostname\n");
        WSACleanup();
        return 0;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(80);
    memcpy(&serverAddr.sin_addr, remoteHost->h_addr_list[0], remoteHost->h_length);

    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 0;
    }

    if (connect(ConnectSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Failed to connect\n");
        closesocket(ConnectSocket);
        WSACleanup();
        return 0;
    }

    snprintf(sendbuf, sizeof(sendbuf),
             "GET /api/v3/simple/price?ids=%s&vs_currencies=usd HTTP/1.0\r\n"
             "Host: api.coingecko.com\r\n"
             "Connection: close\r\n\r\n",
             coin_id);

    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("Send failed\n");
        closesocket(ConnectSocket);
        WSACleanup();
        return 0;
    }

    char *resp = malloc(1);
    int total_size = 0;
    if (!resp) {
        closesocket(ConnectSocket);
        WSACleanup();
        return 0;
    }

    while ((iResult = recv(ConnectSocket, recvbuf, sizeof(recvbuf)-1, 0)) > 0) {
        char *tmp = realloc(resp, total_size + iResult + 1);
        if (!tmp) {
            free(resp);
            closesocket(ConnectSocket);
            WSACleanup();
            return 0;
        }
        resp = tmp;
        memcpy(resp + total_size, recvbuf, iResult);
        total_size += iResult;
        resp[total_size] = 0;
    }

    closesocket(ConnectSocket);
    WSACleanup();

    if (iResult == SOCKET_ERROR) {
        free(resp);
        return 0;
    }

    *response = resp;
    return 1;
}

int main() {
    const char* symbols[] = {"BTC", "ETH", "XMR", "SOL", "XRP", "LTC"};
    const int n = sizeof(symbols) / sizeof(symbols[0]);

    for (int i = 0; i < n; i++) {
        const char *id = symbol_to_id(symbols[i]);
        if (!id) {
            printf("Unknown symbol: %s\n", symbols[i]);
            continue;
        }

        char *response = NULL;
        if (!fetch_price(id, &response)) {
            printf("%s: failed to fetch price\n", symbols[i]);
            continue;
        }

        char *json_start = strstr(response, "\r\n\r\n");
        if (json_start) json_start += 4;

        char *pos = strstr(json_start ? json_start : response, "\"usd\":");
        if (pos) {
            pos += 6; 
            double price = atof(pos);
            printf("%s price: $%.2f\n", symbols[i], price);
        } else {
            printf("%s: price not found\n", symbols[i]);
        }

        free(response);
    }

    return 0;
}
#endif



#ifdef YEARIN_COMMAND
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: year-in <years>\n");
        return 1;
    }
    int years = atoi(argv[1]);
    if (years < 0) {
        printf("Please enter a non-negative number.\n");
        return 1;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    int current_year = t->tm_year + 1900;

    int future_year = current_year + years;
    printf("In %d year%s, it will be %d.\n", years, (years == 1) ? "" : "s", future_year);

    return 0;
}
#endif



#ifdef CHECKSITE_COMMAND
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: checksite <domain>\n");
        return 1;
    }

    const char *host = argv[1];
    WSADATA wsa;
    SOCKET sock;
    struct hostent *he;
    struct sockaddr_in server;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    he = gethostbyname(host);
    if (!he) {
        printf("[!] Could not resolve hostname: %s\n", host);
        WSACleanup();
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        printf("[!] Socket creation failed.\n");
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    memcpy(&server.sin_addr, he->h_addr_list[0], he->h_length);

    printf("[*] Connecting to %s...\n", host);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("[X] %s is *offline* or unreachable on port 80\n", host);
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("[✓] %s is online and reachable on port 80\n", host);
    closesocket(sock);
    WSACleanup();
    return 0;
}
#endif

#ifdef RENAME_COMMAND
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: rename <old_filename> <new_filename>\n");
        return 1;
    }

    if (rename(argv[1], argv[2]) != 0) {
        perror("Rename failed");
        return 1;
    }

    printf("Renamed '%s' to '%s'\n", argv[1], argv[2]);
    return 0;
}
#endif



#ifdef MOVE_COMMAND
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: move <source> <destination>\n");
        return 1;
    }

    if (rename(argv[1], argv[2]) != 0) {
        perror("Move failed");
        return 1;
    }

    printf("Moved '%s' to '%s'\n", argv[1], argv[2]);
    return 0;
}
#endif


#ifdef COPY_COMMAND
#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: copy <source> <destination>\n");
        return 1;
    }

    if (!CopyFileA(argv[1], argv[2], FALSE)) {
        DWORD err = GetLastError();
        printf("Copy failed with error code: %lu\n", err);
        return 1;
    }

    printf("Copied '%s' to '%s'\n", argv[1], argv[2]);
    return 0;
}
#endif

#ifdef LINES_COMMAND
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: lines <filename>\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    int lines = 0;
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') lines++;
    }

    fclose(file);

    printf("%s has %d lines\n", argv[1], lines);
    return 0;
}
#endif


#ifdef TREE_COMMAND
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

void print_tree(const char *path, int level) {
    WIN32_FIND_DATAA fd;
    char search_path[MAX_PATH];
    snprintf(search_path, sizeof(search_path), "%s\\*", path);

    HANDLE hFind = FindFirstFileA(search_path, &fd);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0)
            continue;

        for (int i = 0; i < level; i++)
            printf("  ");

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            printf("[D] %s\n", fd.cFileName);

            // Build new path
            char new_path[MAX_PATH];
            snprintf(new_path, sizeof(new_path), "%s\\%s", path, fd.cFileName);
            print_tree(new_path, level + 1);
        } else {
            printf("%s\n", fd.cFileName);
        }
    } while (FindNextFileA(hFind, &fd));

    FindClose(hFind);
}

int main(int argc, char *argv[]) {
    const char *start_path = "."; 
    if (argc >= 2) {
        start_path = argv[1];
    }

    printf("%s\n", start_path);
    print_tree(start_path, 1);

    return 0;
}
#endif


#ifdef MKCD_COMMAND
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: mkcd <directory>\n");
        return 1;
    }

    const char *dir = argv[1];

    if (!CreateDirectoryA(dir, NULL)) {
        DWORD err = GetLastError();
        if (err != ERROR_ALREADY_EXISTS) {
            printf("Failed to create directory: %s\n", dir);
            return 1;
        }
    }

    if (!SetCurrentDirectoryA(dir)) {
        printf("Failed to change directory to: %s\n", dir);
        return 1;
    }

    printf("Created and changed directory to: %s\n", dir);
    return 0;
}
#endif

#ifdef B64_COMMAND
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

const char b64chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void b64_encode(const unsigned char *in, size_t len, char **out) {
    size_t olen = 4 * ((len + 2) / 3);
    *out = malloc(olen + 1);
    if (!*out) return;

    char *p = *out;
    for (size_t i = 0; i < len;) {
        uint32_t octet_a = i < len ? in[i++] : 0;
        uint32_t octet_b = i < len ? in[i++] : 0;
        uint32_t octet_c = i < len ? in[i++] : 0;

        uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;

        *p++ = b64chars[(triple >> 18) & 0x3F];
        *p++ = b64chars[(triple >> 12) & 0x3F];
        *p++ = (i > len + 1) ? '=' : b64chars[(triple >> 6) & 0x3F];
        *p++ = (i > len)     ? '=' : b64chars[triple & 0x3F];
    }

    *p = '\0';
}

int b64_decode(const char *in, unsigned char **out, size_t *outlen) {
    size_t len = strlen(in);
    if (len % 4 != 0) return 0;

    size_t olen = len / 4 * 3;
    if (in[len - 1] == '=') olen--;
    if (in[len - 2] == '=') olen--;

    *out = malloc(olen);
    if (!*out) return 0;

    unsigned char *p = *out;
    for (size_t i = 0; i < len;) {
        uint32_t sextet_a = in[i] == '=' ? 0 & i++ : strchr(b64chars, in[i++]) - b64chars;
        uint32_t sextet_b = in[i] == '=' ? 0 & i++ : strchr(b64chars, in[i++]) - b64chars;
        uint32_t sextet_c = in[i] == '=' ? 0 & i++ : strchr(b64chars, in[i++]) - b64chars;
        uint32_t sextet_d = in[i] == '=' ? 0 & i++ : strchr(b64chars, in[i++]) - b64chars;

        uint32_t triple = (sextet_a << 18) | (sextet_b << 12) |
                          (sextet_c << 6) | sextet_d;

        if (p - *out < olen) *p++ = (triple >> 16) & 0xFF;
        if (p - *out < olen) *p++ = (triple >> 8) & 0xFF;
        if (p - *out < olen) *p++ = triple & 0xFF;
    }

    *outlen = olen;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage:\n  b64 encode <text>\n  b64 decode <base64>\n");
        return 1;
    }

    if (strcmp(argv[1], "encode") == 0) {
        const char *input = argv[2];
        char *encoded = NULL;
        b64_encode((const unsigned char *)input, strlen(input), &encoded);
        if (encoded) {
            printf("Encoded: %s\n", encoded);
            free(encoded);
        } else {
            printf("Encoding failed.\n");
        }
    } else if (strcmp(argv[1], "decode") == 0) {
        const char *input = argv[2];
        unsigned char *decoded = NULL;
        size_t outlen;
        if (b64_decode(input, &decoded, &outlen)) {
            printf("Decoded: %.*s\n", (int)outlen, decoded);
            free(decoded);
        } else {
            printf("Decoding failed.\n");
        }
    } else {
        printf("Invalid mode. Use 'encode' or 'decode'.\n");
        return 1;
    }

    return 0;
}
#endif

#ifdef SAVEURL_COMMAND
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wininet.h>
#include <stdio.h>

#pragma comment(lib, "wininet.lib")

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: saveurl <url>\n");
        return 1;
    }

    const char *url = argv[1];
    HINTERNET hInternet = InternetOpenA("saveurl", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInternet) {
        printf("Failed to initialize WinINet.\n");
        return 1;
    }

    HINTERNET hFile = InternetOpenUrlA(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hFile) {
        printf("Failed to open URL: %s\n", url);
        InternetCloseHandle(hInternet);
        return 1;
    }

    FILE *fp = fopen("output.html", "wb");
    if (!fp) {
        printf("Failed to create file.\n");
        InternetCloseHandle(hFile);
        InternetCloseHandle(hInternet);
        return 1;
    }

    char buffer[4096];
    DWORD bytesRead;
    while (InternetReadFile(hFile, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        fwrite(buffer, 1, bytesRead, fp);
    }

    fclose(fp);
    InternetCloseHandle(hFile);
    InternetCloseHandle(hInternet);

    printf("Saved URL to output.html\n");
    return 0;
}
#endif

#ifdef MATRIX_COMMAND
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define WIDTH 80
#define HEIGHT 25

HANDLE hOut;

typedef struct {
    WORD shades[4];
    int shades_count;
} ColorScheme;

ColorScheme greenScheme = {
    { FOREGROUND_GREEN | FOREGROUND_INTENSITY, FOREGROUND_GREEN, FOREGROUND_GREEN >> 1, 0 },
    3
};
ColorScheme redScheme = {
    { FOREGROUND_RED | FOREGROUND_INTENSITY, FOREGROUND_RED, FOREGROUND_RED >> 1, 0 },
    3
};
ColorScheme blueScheme = {
    { FOREGROUND_BLUE | FOREGROUND_INTENSITY, FOREGROUND_BLUE, FOREGROUND_BLUE >> 1, 0 },
    3
};

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = {0};
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}

void gotoxy(int x, int y) {
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hOut, coord);
}

char matrix_char() {
    const char *chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@#$%^&*";
    return chars[rand() % (int)strlen(chars)];
}

int parse_int_arg(const char *arg, const char *key, int min, int max, int default_val) {
    if (strncmp(arg, key, strlen(key)) == 0 && arg[strlen(key)] == '=') {
        int val = atoi(arg + strlen(key) + 1);
        if (val >= min && val <= max) return val;
    }
    return default_val;
}

const char* parse_str_arg(const char *arg, const char *key) {
    if (strncmp(arg, key, strlen(key)) == 0 && arg[strlen(key)] == '=') {
        return arg + strlen(key) + 1;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    srand((unsigned int)time(NULL));
    hideCursor();

    
    ColorScheme *scheme = &greenScheme;
    int speed = 3;      
    int max_trail = 15;

    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];
        const char *color_val = parse_str_arg(arg, "color");
        if (color_val) {
            if (strcmp(color_val, "red") == 0) scheme = &redScheme;
            else if (strcmp(color_val, "blue") == 0) scheme = &blueScheme;
            else scheme = &greenScheme;
            continue;
        }
        speed = parse_int_arg(arg, "speed", 1, 10, speed);
        max_trail = parse_int_arg(arg, "trail", 5, 30, max_trail);
    }

    int positions[WIDTH];
    int speeds[WIDTH];
    int speed_counters[WIDTH];
    int trail_lengths[WIDTH];

    for (int i = 0; i < WIDTH; i++) {
        positions[i] = rand() % HEIGHT;
        speeds[i] = speed; // uniform speed for simplicity
        speed_counters[i] = 0;
        trail_lengths[i] = 5 + rand() % max_trail;
    }

    char screen[HEIGHT][WIDTH];
    memset(screen, ' ', sizeof(screen));

    while (1) {
        for (int col = 0; col < WIDTH; col++) {
            speed_counters[col]++;
            if (speed_counters[col] < speeds[col]) continue;
            speed_counters[col] = 0;

            int head = positions[col];

            for (int t = 0; t < trail_lengths[col]; t++) {
                int y = (head - t + HEIGHT) % HEIGHT;

                gotoxy(col, y);

                if (t == 0) {
                    SetConsoleTextAttribute(hOut, scheme->shades[0]);
                    char c = matrix_char();
                    putchar(c);
                    screen[y][col] = c;
                } else {
                    int shade_idx = (t * scheme->shades_count) / trail_lengths[col];
                    if (shade_idx >= scheme->shades_count) shade_idx = scheme->shades_count - 1;

                    SetConsoleTextAttribute(hOut, scheme->shades[shade_idx]);

                    char c = screen[y][col];
                    if (rand() % 5 == 0) c = matrix_char();

                    putchar(c);
                    screen[y][col] = c;
                }
            }

            int clear_y = (head - trail_lengths[col] + HEIGHT) % HEIGHT;
            gotoxy(col, clear_y);
            putchar(' ');
            screen[clear_y][col] = ' ';

            positions[col] = (head + 1) % HEIGHT;
        }

        Sleep(50 - (speed * 4));  
    }

    return 0;
}
#endif


#ifdef NUCMD_GUI_COMMAND
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define WIDTH 60
#define HEIGHT 15

void draw_frame() {
    for (int i = 0; i < WIDTH; i++) putchar('=');
    putchar('\n');

    for (int y = 0; y < HEIGHT - 2; y++) {
        putchar('=');
        for (int i = 0; i < WIDTH - 2; i++) putchar(' ');
        putchar('=');
        putchar('\n');
    }

    for (int i = 0; i < WIDTH; i++) putchar('=');
    putchar('\n');
}

void print_centered(int y, const char *text) {
    int len = (int)strlen(text);
    int start = (WIDTH - len) / 2;
    COORD coord = {start, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("%s", text);
}

void clear_screen() {
    system("cls");
}

void wait_for_key() {
    print_centered(HEIGHT - 2, "Press any key to return...");
    _getch();
}

void show_readme() {
    clear_screen();
    draw_frame();

    const char *readme[] = {
        "NuCMDs - The Ultimate Command-Line",
        "Linux friendly command line.",
        "NuCMDs is FREE!!!",
        "This is copyrighted by pgp/s & IPN.",
        "Visit https://pgpkey.net for more info.",
        "Adding new commands all the time!",
        "Monero is accepted for donations.",
        "And please support third world countries!",
        NULL
    };

    int y = 2;
    for (int i = 0; readme[i]; i++, y++) {
        print_centered(y, readme[i]);
    }

    wait_for_key();
}

int main() {
    clear_screen();
    draw_frame();

    print_centered(2, "NuCMDs Settings");
    print_centered(4, "1) Configuration (Open C:\\NuCMDs folder)");
    print_centered(6, "2) Donate (Monero)");
    print_centered(8, "3) ReadMe");
    print_centered(10, "Press number (1-3) or ESC to exit");

    while (1) {
        int ch = _getch();
        if (ch == 27) break; 

        switch (ch) {
            case '1':
                system("explorer C:\\NuCMDs");
                print_centered(HEIGHT - 2, "Opened C:\\NuCMDs folder. Press any key...");
                _getch();
                clear_screen();
                draw_frame();
                print_centered(2, "NuCMDs Settings");
                print_centered(4, "1) Configuration (Open C:\\NuCMDs folder)");
                print_centered(6, "2) Donate (Monero)");
                print_centered(8, "3) ReadMe");
                print_centered(10, "Press number (1-3) or ESC to exit");
                break;
            case '2':
                system("start https://www.pgpkey.net");
                print_centered(HEIGHT - 2, "Opened donation page. Press any key...");
                _getch();
                clear_screen();
                draw_frame();
                print_centered(2, "NuCMDs Settings");
                print_centered(4, "1) Configuration (Open C:\\NuCMDs folder)");
                print_centered(6, "2) Donate (Monero)");
                print_centered(8, "3) ReadMe");
                print_centered(10, "Press number (1-3) or ESC to exit");
                break;
            case '3':
                show_readme();
                clear_screen();
                draw_frame();
                print_centered(2, "NuCMDs Settings");
                print_centered(4, "1) Configuration (Open C:\\NuCMDs folder)");
                print_centered(6, "2) Donate (Monero) ");
                print_centered(8, "3) ReadMe");
                print_centered(10, "Press number (1-3) or ESC to exit");
                break;
            default:
                break;
        }
    }

    clear_screen();
    return 0;
}
#endif


#ifdef LSBLK_COMMAND
#define WIN32_LEAN_AND_MEAN
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <windows.h>
#include <winioctl.h>
#include <stdio.h>

void print_disk_info() {
    printf("note: if this does not work, let me know!\n");
    printf("NAME        SIZE (GB)    TYPE\n");
    printf("===============================\n");

    for (int disk_num = 0; disk_num < 16; disk_num++) {
        char device_path[64];
        sprintf(device_path, "\\\\.\\PhysicalDrive%d", disk_num);

        HANDLE hDevice = CreateFileA(device_path, GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL, OPEN_EXISTING, 0, NULL);

        if (hDevice == INVALID_HANDLE_VALUE) {
            continue;
        }

        GET_LENGTH_INFORMATION lengthInfo;
        DWORD bytesReturned;

        BOOL res = DeviceIoControl(hDevice, IOCTL_DISK_GET_LENGTH_INFO,
            NULL, 0,
            &lengthInfo, sizeof(lengthInfo),
            &bytesReturned, NULL);

        if (!res) {
            DWORD err = GetLastError();
            CloseHandle(hDevice);
            continue;
        }

        double size_gb = (double)lengthInfo.Length.QuadPart / (1024.0 * 1024.0 * 1024.0);
        printf("PhysicalDrive%-2d    %8.2f     Disk\n", disk_num, size_gb);

        CloseHandle(hDevice);
    }
}

int main() {
    print_disk_info();
    return 0;
}
#endif

#ifdef LOCUST_COMMAND
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LINES 8192
#define MAX_LINE_LENGTH 2048
#define UNDO_STACK_SIZE 256
#define MAX_CMD_LENGTH 256

typedef enum { MODE_NORMAL, MODE_INSERT, MODE_COMMAND, MODE_VISUAL } Mode;
typedef enum { HL_NORMAL, HL_KEYWORD, HL_STRING, HL_COMMENT, HL_NUMBER } HighlightType;

const char* keywords[] = {
    "int", "char", "return", "if", "else", "while", "for", "break", "continue",
    "void", "unsigned", "signed", "static", "struct", "typedef", "const", "enum",
    "switch", "case", "default", "sizeof", "long", "short", "float", "double",
    "do", "goto", "volatile", "register", NULL
};

char buffer[MAX_LINES][MAX_LINE_LENGTH];
HighlightType hl[MAX_LINES][MAX_LINE_LENGTH];
int num_lines = 0;

int cx = 0, cy = 0;         // Cursor pos in file
int row_offset = 0;         // Vertical scroll
int col_offset = 0;         // Horizontal scroll
Mode mode = MODE_NORMAL;

char filename[260] = {0};

HANDLE hConsole;
CONSOLE_SCREEN_BUFFER_INFO csbi;
int screenWidth, screenHeight;

bool dirty = false;

typedef struct {
    int line;
    char old_line[MAX_LINE_LENGTH];
} UndoItem;

UndoItem undo_stack[UNDO_STACK_SIZE];
int undo_top = -1;

typedef struct {
    int start_x, start_y;
    int end_x, end_y;
    bool active;
} Selection;

Selection selection = {0,0,0,0,false};

char cmdline[MAX_CMD_LENGTH] = {0};
int cmdlen = 0;

bool line_dirty[MAX_LINES];
bool status_dirty = true;
bool cmdline_dirty = false;

void draw_screen();
void draw_status();
void draw_command_line(const char *cmd);
void draw_line_with_highlight(int y, int file_line);
void set_cursor();
void scroll();
void load_file(const char *fname);
void save_file();
void insert_char(char c);
void delete_char();
void insert_newline();
void move_cursor(int key);
void process_normal_mode(int c);
void process_insert_mode(int c);
void process_command_mode();
void update_syntax(int line);
void update_all_syntax();
void push_undo(int line);
void undo_last();
bool is_word_char(char c);
bool match_keyword(const char *word);
void clear_selection();
bool is_selected(int x, int y);
void mark_line_dirty(int line);
void mark_all_dirty();

void set_console_color(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

void clear_screen() {
    COORD topLeft = {0,0};
    DWORD written;
    FillConsoleOutputCharacterA(hConsole, ' ', screenWidth * screenHeight, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, screenWidth * screenHeight, topLeft, &written);
}

void mark_line_dirty(int line) {
    if (line >= 0 && line < MAX_LINES) {
        line_dirty[line] = true;
    }
}

void mark_all_dirty() {
    for (int i = 0; i < screenHeight - 2; i++) {
        int file_line = i + row_offset;
        if (file_line < num_lines) line_dirty[file_line] = true;
    }
    status_dirty = true;
    cmdline_dirty = true;
}

void draw_screen() {
    int max_rows = screenHeight - 2; // status + cmd line
    for (int i = 0; i < max_rows; i++) {
        int file_line = i + row_offset;
        if (file_line >= num_lines) {
            if (line_dirty[file_line]) {
                COORD coord = {0, (SHORT)i};
                SetConsoleCursorPosition(hConsole, coord);
                set_console_color(8); // dark gray
                printf("~");
                for (int j = 1; j < screenWidth; j++) putchar(' ');
                set_console_color(csbi.wAttributes);
                line_dirty[file_line] = false;
            }
        } else {
            if (line_dirty[file_line]) {
                draw_line_with_highlight(i, file_line);
                line_dirty[file_line] = false;
            }
        }
    }

    if (status_dirty) {
        draw_status();
        status_dirty = false;
    }

    if (cmdline_dirty && (mode == MODE_COMMAND || mode == MODE_VISUAL)) {
        draw_command_line(cmdline);
        cmdline_dirty = false;
    } else if (mode == MODE_NORMAL || mode == MODE_INSERT) {
        
        COORD coord = {0, (SHORT)(screenHeight - 1)};
        SetConsoleCursorPosition(hConsole, coord); // literally fuck this part btw
        printf("%-*s", screenWidth, "");
    }

    set_cursor();
}

void draw_status() {
    COORD coord = {0, (SHORT)(screenHeight - 2)};
    SetConsoleCursorPosition(hConsole, coord);

    char status[512];
    char mode_str[16];
    switch(mode) {
        case MODE_NORMAL: strcpy(mode_str, "NORMAL"); break;
        case MODE_INSERT: strcpy(mode_str, "INSERT"); break;
        case MODE_COMMAND: strcpy(mode_str, "COMMAND"); break;
        case MODE_VISUAL: strcpy(mode_str, "VISUAL"); break;
    }

    char dirty_mark = dirty ? '*' : ' ';
    snprintf(status, sizeof(status),
        "%s %c - %s | Line %d/%d Col %d   Press :q to quit",
        filename[0] ? filename : "[No Name]", dirty_mark,
        mode_str,
        cy + 1, num_lines,
        cx + 1);

    set_console_color(112); // white bg black fg
    printf("%-*s", screenWidth, status);
    set_console_color(csbi.wAttributes);
}

void draw_command_line(const char *cmd) {
    COORD coord = {0, (SHORT)(screenHeight - 1)};
    SetConsoleCursorPosition(hConsole, coord);
    set_console_color(240); // light gray background
    printf(":%s", cmd);
    int len = (int)strlen(cmd) + 1;
    for (int i = len; i < screenWidth; i++) putchar(' ');
    set_console_color(csbi.wAttributes);
}

void draw_line_with_highlight(int y, int file_line) {
    COORD coord = {0, (SHORT)y};
    SetConsoleCursorPosition(hConsole, coord);

    int len = (int)strlen(buffer[file_line]);
    int max_cols = screenWidth;

    for (int i = 0; i < max_cols; i++) {
        if (i >= len) {
            putchar(' ');
            continue;
        }

        bool selected = is_selected(i, file_line);

        if (selected) {
            set_console_color(240); 
        } else {
            switch(hl[file_line][i]) {
                case HL_NORMAL: set_console_color(7); break;      // White
                case HL_KEYWORD: set_console_color(11); break;    // Cyan
                case HL_STRING: set_console_color(10); break;     // Green
                case HL_COMMENT: set_console_color(8); break;     // Dark Gray
                case HL_NUMBER: set_console_color(13); break;     // Magenta
            }
        }
        putchar(buffer[file_line][i]);
        set_console_color(csbi.wAttributes);
    }
}

void set_cursor() {
    int screen_x = cx - col_offset;
    int screen_y = cy - row_offset;

    if (screen_x < 0) screen_x = 0;
    if (screen_y < 0) screen_y = 0;

    if (screen_x >= screenWidth) screen_x = screenWidth - 1;
    if (screen_y >= screenHeight - 2) screen_y = screenHeight - 3;

    COORD coord = {(SHORT)screen_x, (SHORT)screen_y};
    SetConsoleCursorPosition(hConsole, coord);
}

void scroll() {
    if (cy < row_offset) {
        row_offset = cy;
        mark_all_dirty();
    }
    if (cy >= row_offset + screenHeight - 2) {
        row_offset = cy - (screenHeight - 3);
        mark_all_dirty();
    }
    if (cx < col_offset) {
        col_offset = cx;
        mark_all_dirty();
    }
    if (cx >= col_offset + screenWidth) {
        col_offset = cx - screenWidth + 1;
        mark_all_dirty();
    }
}

void load_file(const char *fname) {
    FILE *f = fopen(fname, "r");
    if (!f) return;

    num_lines = 0;
    while (fgets(buffer[num_lines], MAX_LINE_LENGTH, f) && num_lines < MAX_LINES) {
        size_t len = strlen(buffer[num_lines]);
        while (len > 0 && (buffer[num_lines][len - 1] == '\n' || buffer[num_lines][len - 1] == '\r')) {
            buffer[num_lines][--len] = 0;
        }
        update_syntax(num_lines);
        num_lines++;
    }
    fclose(f);
    strncpy(filename, fname, sizeof(filename)-1);
    if (num_lines == 0) {
        num_lines = 1;
        buffer[0][0] = 0;
    }
    mark_all_dirty();
}

void save_file() {
    if (!filename[0]) return;
    FILE *f = fopen(filename, "w");
    if (!f) {
        COORD coord = {0, (SHORT)(screenHeight - 1)};
        SetConsoleCursorPosition(hConsole, coord);
        printf("Failed to save file.");
        _getch();
        return;
    }
    for (int i = 0; i < num_lines; i++) {
        fprintf(f, "%s\n", buffer[i]);
    }
    fclose(f);
    dirty = false;
    status_dirty = true;
}

void push_undo(int line) {
    if (undo_top >= UNDO_STACK_SIZE - 1) {
        memmove(&undo_stack[0], &undo_stack[1], sizeof(UndoItem)*(UNDO_STACK_SIZE -1));
        undo_top--;
    }
    undo_top++;
    undo_stack[undo_top].line = line;
    strcpy(undo_stack[undo_top].old_line, buffer[line]);
}

void undo_last() {
    if (undo_top < 0) return;
    int line = undo_stack[undo_top].line;
    strcpy(buffer[line], undo_stack[undo_top].old_line);
    update_syntax(line);
    undo_top--;
    dirty = true;
    mark_line_dirty(line);
    status_dirty = true;
}

bool is_word_char(char c) {
    return (isalnum(c) || c == '_');
}

bool match_keyword(const char *word) {
    for (int i=0; keywords[i]; i++) {
        if (strcmp(word, keywords[i]) == 0) return true;
    }
    return false;
}


void update_syntax(int line) {
    const char *s = buffer[line];
    int i = 0;
    bool in_string = false;
    bool in_comment = false;

    memset(hl[line], HL_NORMAL, sizeof(hl[line]));

    while (s[i]) {
        if (in_comment) {
            hl[line][i] = HL_COMMENT;
            i++;
            continue;
        }
        if (in_string) {
            hl[line][i] = HL_STRING;
            if (s[i] == '"' && s[i-1] != '\\') {
                in_string = false;
            }
            i++;
            continue;
        }
        if (s[i] == '/' && s[i+1] == '/') {
            for (int j = i; s[j]; j++) hl[line][j] = HL_COMMENT;
            break;
        }
        if (s[i] == '"') {
            hl[line][i] = HL_STRING;
            in_string = true;
            i++;
            continue;
        }
        if (isdigit(s[i])) {
            hl[line][i] = HL_NUMBER;
            i++;
            continue;
        }
        if (isalpha(s[i]) || s[i] == '_') {
            int start = i;
            while (isalnum(s[i]) || s[i] == '_') i++;
            char word[64];
            int len = i - start;
            if (len >= (int)sizeof(word)) len = sizeof(word)-1;
            strncpy(word, &s[start], len);
            word[len] = 0;

            if (match_keyword(word)) {
                for (int k = start; k < i; k++) hl[line][k] = HL_KEYWORD;
            }
            continue;
        }
        i++;
    }
}

void update_all_syntax() {
    for (int i = 0; i < num_lines; i++) {
        update_syntax(i);
    }
}

void insert_char(char c) {
    if (cy >= MAX_LINES) return;
    int len = (int)strlen(buffer[cy]);
    if (len >= MAX_LINE_LENGTH - 1) return;

    push_undo(cy);

    for (int i = len; i >= cx; i--) {
        buffer[cy][i+1] = buffer[cy][i];
    }
    buffer[cy][cx] = c;
    cx++;
    dirty = true;
    update_syntax(cy);
    mark_line_dirty(cy);
    status_dirty = true;
}

void delete_char() {
    if (cy >= MAX_LINES) return;
    int len = (int)strlen(buffer[cy]);
    if (cx == 0 && cy > 0) {
        push_undo(cy-1);
        int prev_len = (int)strlen(buffer[cy-1]);
        if (prev_len + len >= MAX_LINE_LENGTH - 1) return;

        strcat(buffer[cy-1], buffer[cy]);
        for (int i = cy; i < num_lines - 1; i++) {
            strcpy(buffer[i], buffer[i+1]);
        }
        num_lines--;
        cy--;
        cx = prev_len;
        dirty = true;
        update_syntax(cy);
        mark_line_dirty(cy);
        status_dirty = true;
        return;
    }
    if (cx == 0) return;

    push_undo(cy);
    for (int i = cx-1; i < len; i++) {
        buffer[cy][i] = buffer[cy][i+1];
    }
    cx--;
    dirty = true;
    update_syntax(cy);
    mark_line_dirty(cy);
    status_dirty = true;
}

void insert_newline() {
    if (num_lines >= MAX_LINES - 1) return;

    push_undo(cy);

    for (int i = num_lines; i > cy + 1; i--) {
        strcpy(buffer[i], buffer[i-1]);
    }
    int len = (int)strlen(buffer[cy]);
    char new_line[MAX_LINE_LENGTH];
    strcpy(new_line, &buffer[cy][cx]);
    buffer[cy][cx] = 0;
    strcpy(buffer[cy+1], new_line);
    cy++;
    cx = 0;
    num_lines++;
    dirty = true;
    update_syntax(cy-1);
    update_syntax(cy);
    mark_line_dirty(cy-1);
    mark_line_dirty(cy);
    status_dirty = true;
}

void clear_selection() {
    selection.active = false;
}

bool is_selected(int x, int y) {
    if (!selection.active) return false;

    int start_line = selection.start_y;
    int end_line = selection.end_y;
    int start_col = selection.start_x;
    int end_col = selection.end_x;

    if (start_line > end_line) {
        int t = start_line; start_line = end_line; end_line = t;
        t = start_col; start_col = end_col; end_col = t;
    }

    if (y < start_line || y > end_line) return false;

    if (y == start_line && y == end_line) {
        if (x >= start_col && x < end_col) return true;
    } else if (y == start_line) {
        if (x >= start_col) return true;
    } else if (y == end_line) {
        if (x < end_col) return true;
    } else {
        return true;
    }
    return false;
}

void move_cursor_visual(int key) {
    switch(key) {
        case 'h': if (cx > 0) cx--; break;
        case 'l': if (cx < (int)strlen(buffer[cy])) cx++; break;
        case 'j': if (cy < num_lines - 1) {
            cy++;
            if (cx > (int)strlen(buffer[cy])) cx = (int)strlen(buffer[cy]);
        } break;
        case 'k': if (cy > 0) {
            cy--;
            if (cx > (int)strlen(buffer[cy])) cx = (int)strlen(buffer[cy]);
        } break;
    }
}

void move_cursor(int key) {
    switch(key) {
        case 'h': if (cx > 0) cx--; break;
        case 'l': if (cx < (int)strlen(buffer[cy])) cx++; break;
        case 'j': if (cy < num_lines - 1) {
            cy++;
            if (cx > (int)strlen(buffer[cy])) cx = (int)strlen(buffer[cy]);
        } break;
        case 'k': if (cy > 0) {
            cy--;
            if (cx > (int)strlen(buffer[cy])) cx = (int)strlen(buffer[cy]);
        } break;
        case '0': cx = 0; break;
        case '$': cx = (int)strlen(buffer[cy]); break;
    }
}

void process_normal_mode(int c) {
    switch (c) {
        case 'i': mode = MODE_INSERT; status_dirty = true; break;
        case 'I': cx = 0; mode = MODE_INSERT; status_dirty = true; break;
        case 'a': if (cx < (int)strlen(buffer[cy])) cx++; mode = MODE_INSERT; status_dirty = true; break;
        case 'A': cx = (int)strlen(buffer[cy]); mode = MODE_INSERT; status_dirty = true; break;
        case 'x': delete_char(); break;
        case 'u': undo_last(); break;
        case ':': mode = MODE_COMMAND; cmdlen = 0; cmdline[0] = 0; cmdline_dirty = true; break;
        case 'v': mode = MODE_VISUAL; selection.start_x = cx; selection.start_y = cy; selection.active = true; status_dirty = true; break;
        case 'h': move_cursor('h'); break;
        case 'j': move_cursor('j'); break;
        case 'k': move_cursor('k'); break;
        case 'l': move_cursor('l'); break;
        case '0': move_cursor('0'); break;
        case '$': move_cursor('$'); break;
        case 27:
            clear_selection();
            break;
    }
}

void process_insert_mode(int c) {
    if (c == 27) { // ESC
        mode = MODE_NORMAL;
        status_dirty = true;
        clear_selection();
        return;
    }
    if (c == 8) { // Backspace
        delete_char();
        return;
    }
    if (c == 13) { // Enter
        insert_newline();
        return;
    }
    if (isprint(c)) {
        insert_char((char)c);
    }
}

void process_command_mode() {
    draw_command_line(cmdline);
    int c = _getch();
    if (c == 13) { // ???? LOL
        if (strcmp(cmdline, "q") == 0) {
            exit(0);
        } else if (strcmp(cmdline, "w") == 0) {
            save_file();
        } else if (strcmp(cmdline, "wq") == 0) {
            save_file();
            exit(0);
        } else if (strcmp(cmdline, "u") == 0) {
            undo_last();
        }
        mode = MODE_NORMAL;
        status_dirty = true;
        cmdline_dirty = false;
        cmdlen = 0;
        cmdline[0] = 0;
    } else if (c == 27) { 
        mode = MODE_NORMAL;
        status_dirty = true;
        cmdlen = 0;
        cmdline[0] = 0;
        cmdline_dirty = false;
    } else if (c == 8) { 
        if (cmdlen > 0) {
            cmdlen--;
            cmdline[cmdlen] = 0;
            cmdline_dirty = true;
        }
    } else if (isprint(c) && cmdlen < MAX_CMD_LENGTH - 1) {
        cmdline[cmdlen++] = (char)c;
        cmdline[cmdlen] = 0;
        cmdline_dirty = true;
    }
}

int main(int argc, char **argv) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

   
    if (argc > 1) {
        load_file(argv[1]);
    } else {
        num_lines = 1;
        buffer[0][0] = 0;
    }

    mark_all_dirty();
    update_all_syntax();

    while (1) {
        scroll();
        draw_screen();

        int c = _getch();

        if (mode == MODE_NORMAL) {
            if (c == 0 || c == 224) { 
                c = _getch(); // ignore arrows for now or you can add here
            }
            process_normal_mode(c);
        } else if (mode == MODE_INSERT) {
            process_insert_mode(c);
        } else if (mode == MODE_COMMAND || mode == MODE_VISUAL) {
            process_command_mode();
        }
    }
    return 0;
}
#endif


#ifdef NANO_COMMAND
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LINES 8192
#define MAX_LINE_LENGTH 1024

char buffer[MAX_LINES][MAX_LINE_LENGTH];
int num_lines = 1;
// cursor position in file (x = col, y = row)
int row_offset = 0;  
int col_offset = 0;  

bool dirty = false;
bool quit = false;

char filename[260] = {0};

HANDLE hConsole;
CONSOLE_SCREEN_BUFFER_INFO csbi;
int screenWidth, screenHeight;

char status_msg[256] = "";
DWORD status_msg_time = 0;

char search_query[256] = "";
bool searching = false;
int last_search_x = -1, last_search_y = -1;


void draw_screen();
void draw_status_bar();
void draw_message_bar();
void scroll();
void set_cursor();
void load_file(const char *fname);
void save_file();
void process_keypress();
void insert_char(char c);
void delete_char();
void insert_newline();
void move_cursor(int key);
void prompt_save_on_exit();
void search_forward();

void clear_screen() {
    COORD topLeft = {0,0};
    DWORD written;
    FillConsoleOutputCharacterA(hConsole, ' ', screenWidth * screenHeight, topLeft, &written);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, screenWidth * screenHeight, topLeft, &written);
    SetConsoleCursorPosition(hConsole, topLeft);
}

void draw_screen() {
    clear_screen();

    int max_rows = screenHeight - 2; // 1 status bar + 1 message bar

    for (int y = 0; y < max_rows; y++) {
        int file_line = y + row_offset;
        COORD coord = {0, (SHORT)y};
        SetConsoleCursorPosition(hConsole, coord);

        if (file_line >= num_lines) {
            // draw ~ for empty lines
            SetConsoleTextAttribute(hConsole, 8); // dark gray
            printf("~\n");
            SetConsoleTextAttribute(hConsole, csbi.wAttributes);
        } else {
            // draw line with horizontal scroll
            int len = (int)strlen(buffer[file_line]);
            if (len > col_offset) {
                printf("%.*s\n", screenWidth, buffer[file_line] + col_offset);
            } else {
                putchar('\n');
            }
        }
    }

    draw_status_bar();
    draw_message_bar();
    set_cursor();
}

void draw_status_bar() {
    COORD coord = {0, (SHORT)(screenHeight - 2)};
    SetConsoleCursorPosition(hConsole, coord);
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    char status[512];
    char file_display[260];
    if (filename[0]) strncpy(file_display, filename, sizeof(file_display)-1);
    else strcpy(file_display, "[No Name]");
    char dirty_mark = dirty ? '*' : ' ';

    snprintf(status, sizeof(status), " %s %c | Ln %d, Col %d ", file_display, dirty_mark, cy + 1, cx + 1);
    int len = (int)strlen(status);

    printf("%s", status);

    
    for (int i = len; i < screenWidth; i++) putchar(' ');

    SetConsoleTextAttribute(hConsole, csbi.wAttributes);
}

void draw_message_bar() {
    COORD coord = {0, (SHORT)(screenHeight - 1)};
    SetConsoleCursorPosition(hConsole, coord);
    DWORD now = GetTickCount();

    if (status_msg[0] && now - status_msg_time < 5000) {
        printf("%-*s", screenWidth, status_msg);
    } else {
        printf("%-*s", screenWidth, "");
    }
}

void set_status_message(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(status_msg, sizeof(status_msg), fmt, ap);
    va_end(ap);
    status_msg_time = GetTickCount();
}

void set_cursor() {
    int screen_x = cx - col_offset;
    int screen_y = cy - row_offset;

    if (screen_x < 0) screen_x = 0;
    if (screen_y < 0) screen_y = 0;

    if (screen_x >= screenWidth) screen_x = screenWidth - 1;
    if (screen_y >= screenHeight - 2) screen_y = screenHeight - 3;

    COORD coord = {(SHORT)screen_x, (SHORT)screen_y};
    SetConsoleCursorPosition(hConsole, coord);
}

void scroll() {
    if (cy < row_offset) {
        row_offset = cy;
    }
    if (cy >= row_offset + screenHeight - 2) {
        row_offset = cy - (screenHeight - 3);
    }
    if (cx < col_offset) {
        col_offset = cx;
    }
    if (cx >= col_offset + screenWidth) {
        col_offset = cx - screenWidth + 1;
    }
}

void load_file(const char *fname) {
    FILE *f = fopen(fname, "r");
    if (!f) {
        set_status_message("Could not open file: %s", fname);
        return;
    }

    num_lines = 0;
    while (fgets(buffer[num_lines], MAX_LINE_LENGTH, f) && num_lines < MAX_LINES) {
        size_t len = strlen(buffer[num_lines]);
        while (len > 0 && (buffer[num_lines][len - 1] == '\n' || buffer[num_lines][len - 1] == '\r')) {
            buffer[num_lines][--len] = 0;
        }
        num_lines++;
    }
    fclose(f);

    strncpy(filename, fname, sizeof(filename)-1);
    dirty = false;
    set_status_message("Opened file: %s", filename);
}

void save_file() {
    if (!filename[0]) {
        set_status_message("No file name specified.");
        return;
    }
    FILE *f = fopen(filename, "w");
    if (!f) {
        set_status_message("Could not save file: %s", filename);
        return;
    }
    for (int i = 0; i < num_lines; i++) {
        fprintf(f, "%s\n", buffer[i]);
    }
    fclose(f);
    dirty = false;
    set_status_message("File saved: %s", filename);
}

void insert_char(char c) {
    if (cy >= MAX_LINES) return;
    int len = (int)strlen(buffer[cy]);
    if (len >= MAX_LINE_LENGTH - 1) return;
    for (int i = len; i >= cx; i--) {
        buffer[cy][i+1] = buffer[cy][i];
    }
    buffer[cy][cx] = c;
    cx++;
    dirty = true;
}

void delete_char() {
    if (cy >= MAX_LINES) return;
    int len = (int)strlen(buffer[cy]);
    if (cx == 0 && cy > 0) {
        // Join this line with previous
        int prev_len = (int)strlen(buffer[cy-1]);
        if (prev_len + len >= MAX_LINE_LENGTH - 1) return;

        strcat(buffer[cy-1], buffer[cy]);
        for (int i = cy; i < num_lines -1; i++) {
            strcpy(buffer[i], buffer[i+1]);
        }
        num_lines--;
        cy--;
        cx = prev_len;
        dirty = true;
        return;
    }
    if (cx == 0) return;

    for (int i = cx -1; i < len; i++) {
        buffer[cy][i] = buffer[cy][i+1];
    }
    cx--;
    dirty = true;
}
void insert_newline() {
    if (num_lines >= MAX_LINES - 1) return;
    int len = (int)strlen(buffer[cy]);
    char new_line[MAX_LINE_LENGTH];

    strcpy(new_line, &buffer[cy][cx]);
    buffer[cy][cx] = 0;
    for (int i = num_lines; i > cy + 1; i--) {
        strcpy(buffer[i], buffer[i-1]);
    }
    strcpy(buffer[cy + 1], new_line);
    num_lines++;
    cy++;
    cx = 0;
    dirty = true;
}

void move_cursor(int key) {
    switch(key) {
        case 0x4B: // left arrow
            if (cx > 0) cx--;
            break;
        case 0x4D: // right arrow
            if (cx < (int)strlen(buffer[cy])) cx++;
            break;
        case 0x48: // up arrow
            if (cy > 0) {
                cy--;
                int len = (int)strlen(buffer[cy]);
                if (cx > len) cx = len;
            }
            break;
        case 0x50: // down arrow
            if (cy < num_lines - 1) {
                cy++;
                int len = (int)strlen(buffer[cy]);
                if (cx > len) cx = len;
            }
            break;
    }
}

void prompt_save_on_exit() {
    while (1) {
        set_status_message("Save modified buffer? (Y)es, (N)o, (C)ancel");
        draw_screen();
        int c = _getch();
        if (c == 'y' || c == 'Y') {
            save_file();
            quit = true;
            break;
        } else if (c == 'n' || c == 'N') {
            quit = true;
            break;
        } else if (c == 'c' || c == 'C') {
            set_status_message("");
            break;
        }
    }
}

void search_forward() {
    if (!search_query[0]) {
        set_status_message("No search query.");
        return;
    }

    int start_y = (last_search_y == -1) ? cy : last_search_y;
    int start_x = (last_search_y == -1) ? cx + 1 : last_search_x + 1;

    for (int y = start_y; y < num_lines; y++) {
        int len = (int)strlen(buffer[y]);
        int x_start = (y == start_y) ? start_x : 0;
        for (int x = x_start; x < len; x++) {
            if (strncmp(&buffer[y][x], search_query, strlen(search_query)) == 0) {
                cy = y;
                cx = x;
                last_search_y = y;
                last_search_x = x;
                scroll();
                set_status_message("Found '%s'", search_query);
                return;
            }
        }
    }
    set_status_message("Search hit BOTTOM, no more matches.");
    last_search_y = -1;
    last_search_x = -1;
}

void process_keypress() {
    int c = _getch();

    if (searching) {
        if (c == 27) { 
            searching = false;
            set_status_message("");
            return;
        } else if (c == 13) { 
            searching = false;
            search_forward();
            return;
        } else if (c == 8) { 
            int len = (int)strlen(search_query);
            if (len > 0) {
                search_query[len - 1] = 0;
                set_status_message("Search: %s", search_query);
            }
            return;
        } else if (isprint(c) && strlen(search_query) < sizeof(search_query) - 1) {
            int len = (int)strlen(search_query);
            search_query[len] = (char)c;
            search_query[len+1] = 0;
            set_status_message("Search: %s", search_query);
            return;
        }
    }

    if (c == 0 || c == 224) {
        c = _getch(); 
        move_cursor(c);
        return;
    }

    if (c == 17) { 
        if (dirty) {
            prompt_save_on_exit();
        } else {
            quit = true;
        }
        return;
    }

    if (c == 15) { 
        save_file();
        return;
    }

    if (c == 23) { 
        searching = true;
        search_query[0] = 0;
        set_status_message("Search: ");
        return;
    }

    if (c == 11) { 
        if (num_lines <= 1) return;
        for (int i = cy; i < num_lines -1; i++) {
            strcpy(buffer[i], buffer[i+1]);
        }
        num_lines--;
        if (cy >= num_lines) cy = num_lines -1;
        cx = 0;
        dirty = true;
        return;
    }

    if (c == 21) { 
        set_status_message("Paste not implemented.");
        return;
    }

    if (c == 13) { 
        insert_newline();
        return;
    }

    if (c == 8) { 
        delete_char();
        return;
    }

    if (isprint(c)) {
        insert_char((char)c);
        return;
    }
}

int main(int argc, char *argv[]) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    
    if (argc >= 2) load_file(argv[1]);
    else {
        buffer[0][0] = 0;
        num_lines = 1;
    }

    set_status_message("Ctrl+O = save | Ctrl+Q = quit | Ctrl+W = search | Ctrl+K = cut line | Ctrl+U = paste line");

    while (!quit) {
        scroll();
        draw_screen();
        process_keypress();
    }

    
    clear_screen();
    SetConsoleTextAttribute(hConsole, csbi.wAttributes);
    COORD coord = {0,0};
    SetConsoleCursorPosition(hConsole, coord);
    return 0;
}
#endif
