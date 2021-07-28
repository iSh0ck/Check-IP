// Includes
#include <stdio.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Variable declaration
struct addrIP {
    char* val1;
    char* val2;
    char* val3;
    char* val4;
    char* mask;
};

bool isPrivate;
bool isLocalhost;
char classIP;

// Fonction declaration
bool checkIP(char *ip);
struct addrIP* extract(char *ip);
int* convert(struct addrIP* pt_ip);
void decode(struct addrIP* pt_ip);
void calc(struct addrIP* pt_ip);
void save(int* ipAddr, char* ntwAddr, char* sbnMask, char* brdAddr, char* fstIP, char* lstIP, int* nbSubnet, int* nbHost);