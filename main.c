#include "main.h"

void main() {
    struct addrIP* pt_ip;
    int* i_ip;
    char ipAddr[18] = {};
    printf("Entrez une IP (Format: xxx.xxx.xxx.xxx/xx) : ");
    scanf("%s", ipAddr);

    if (checkIP(ipAddr) == 1) {
        pt_ip = extract(ipAddr);
        i_ip = convert(pt_ip);

        printf("Adresse IP rentrée : %d.%d.%d.%d/%d\n", i_ip[0], i_ip[1], i_ip[2], i_ip[3], i_ip[4]);
        decode(pt_ip);
        calc(pt_ip);
    }
}

bool checkIP(char *ip) {
    if (strlen(ip) <= 18 && strlen(ip) >= 8) { // Check size of the char*
        bool isCharacterAllowed;
        int i = 0;
        while (i < strlen(ip)) // Check if character is allowed (1234567890./)
        {
            if (ip[i] == '1' || ip[i] == '2' || ip[i] == '3' || ip[i] == '4' || ip[i] == '5'
                || ip[i] == '6' || ip[i] == '7' || ip[i] == '8' || ip[i] == '9' || ip[i] == '0'
                || ip[i] == '.' || ip[i] == '/')
            {
                isCharacterAllowed = true;
            } else {
                isCharacterAllowed = false;
            }
            i++;
        }

        bool testCompositionIP = true;
        int compteur = 0; // Check nb of digit by block of the IP
        for (int i = 0; i <= strlen(ip) && testCompositionIP != false; i++)
        {
            if (ip[i] == '1' || ip[i] == '2' || ip[i] == '3' || ip[i] == '4' || ip[i] == '5'
                || ip[i] == '6' || ip[i] == '7' || ip[i] == '8' || ip[i] == '9' || ip[i] == '0')
            {
                compteur++;
                if (compteur <= 3)
                {
                    testCompositionIP = true;
                } else {
                    testCompositionIP = false;
                }
            } 
            if (ip[i] == '.' || ip[i] == '/') {
                compteur = 0;
            }
        }
        // Check number of block
        if (isCharacterAllowed == true) {
            int nbDot = 0;
            for (int i = 0; i <= strlen(ip); i++) {
                if (ip[i] == '.') {
                    nbDot++;
                }
            }
            if (nbDot == 3) {
                if (testCompositionIP == true) { // Check if mask is set
                    int nbSlash = 0;
                    for (int i = 0; i <= strlen(ip); i++) {
                        if (ip[i] == '/') {
                            nbSlash++;
                        }
                    }
                    if (nbSlash == 1) {
                        return true;
                    } else {
                        printf("Mauvais format. L'adresse doit contenir un masque.\n");
                        return false;
                    }
                } else {
                    printf("Mauvais format. L'adresse ne peut pas contenir plus de 3 digits par membres\n");
                    return false;
                }
            } else {
                printf("Mauvais format. Une adresse IP est au format xxx.xxx.xxx\n");
                return false;
            }
        } else {
            printf("Mauvais format. Les caractères autorisés sont: 123456789./\n");
            return false;
        }
    } else {
        printf("Mauvais format. L'adresse doit contenir entre 8 et 18 caractères.\n");
        return false;
    }
}

struct addrIP* extract(char *ip) {
    const char * separators = "./"; // Caracter detected by the parser
    struct addrIP *pt_ip;
    pt_ip = malloc(sizeof(struct addrIP));

    char *strToken = strtok (ip, separators);
    int i = 0;
    while (strToken != NULL) { // Store wich block into different var
        if (i == 0) {
            pt_ip->val1 = strToken;
        } else if (i == 1) {
            pt_ip->val2 = strToken;
        } else if (i == 2) {
            pt_ip->val3 = strToken;
        } else if (i == 3) {
            pt_ip->val4 = strToken;
        } else {
            pt_ip->mask = strToken;
        }
        i++;
        strToken = strtok(NULL, separators);
    }
    return pt_ip;
}

int* convert(struct addrIP* pt_ip) {

    int* i_ip = malloc(sizeof(int) *5);
    i_ip[0] = atoi(pt_ip->val1); // Convert char* to int
    i_ip[1] = atoi(pt_ip->val2);
    i_ip[2] = atoi(pt_ip->val3);
    i_ip[3] = atoi(pt_ip->val4);
    i_ip[4] = atoi(pt_ip->mask);

    if (i_ip[0] > 255 || i_ip[1] > 255 || i_ip[2] > 255 || i_ip[3] > 255) { // Check if the IP is correct
        printf("Adresse IP invalide.\n");
        exit(0);        
    }
    if (i_ip[4] > 32) { // Check if the mask is correct
        printf("Masque invalide.\n");
        exit(0);
    }
    return i_ip;
}

void decode(struct addrIP* pt_ip) {
    printf("\n|--- INFORMATIONS ---|\n\n");
    int* i_ip = convert(pt_ip);

    // Search the class of the IP
    // Check if it's private or public
    if (i_ip[0] >= 0 && i_ip[0] <= 127) {
        printf("Classe: A\n");
        classIP = 'A';
        if (i_ip[0] == 10) {
            printf("Type: Privée\n");
            isPrivate = true;
        } else if ((i_ip[0] == 127 && i_ip[1] == 0 && i_ip[2] == 0) ||
                    (i_ip[0] == 0 && i_ip[1] == 0 && i_ip[2] == 0 && i_ip[3] == 0)) {
            printf("Type: Réservée (localhost)\n");
            isPrivate = true;
            isLocalhost = true;
        } else {
            printf("Type: Publique\n");
            isPrivate = false;
        }
    } else if (i_ip[0] >= 128 && i_ip[0] <= 191) {
        printf("Classe: B\n");
        classIP = 'B';
        if (i_ip[0] == 172 && i_ip[1] >= 16 && i_ip[1] <= 31) {
            printf("Type: Privée\n");
            isPrivate = true;
        } else {
            printf("Type: Publique\n");
            isPrivate = false;
        }
    } else if (i_ip[0] >= 192 && i_ip[0] <= 223) {
        printf("Classe: C\n");
        classIP = 'C';
        if (i_ip[0] == 192 && i_ip[1] == 168 && i_ip[2] >= 1) {
            printf("Type: Privée\n");
            isPrivate = true;
        } else {
            printf("Type: Publique\n");
            isPrivate = false;
        }
    } else if (i_ip[0] >= 224 && i_ip[0] <= 239) {
        printf("Classe: D\n");
        classIP = 'D';
        printf("Type: Multicast\n");
    } else if (i_ip[0] >= 240 && i_ip[0] <= 255) {
        printf("Classe: E\n");
        classIP = 'E';
        printf("Type: Réservée\n");
        isPrivate = true;
    } else {
        printf("Adresse non valide\n");
    }
}

void calc(struct addrIP* pt_ip) {
    int* i_ip = convert(pt_ip);
    int bitb, bitr, nbSubnet, nbHost, bsize;

    // Calc for default netmask (32,24,16 & 8)
    if (i_ip[4] >= 24) {
        bitb = i_ip[4] - 24;
        if (bitb == 0) {
            bitb = i_ip[4];
        } else {
            bitb += 24;
        }
    } else {
        if (i_ip[4] >= 16) {
            bitb = i_ip[4] - 16;
            if (bitb == 0) {
                bitb = i_ip[4];
            } else {
                bitb += 16;
            }
        } else {
            if (i_ip[4] >= 8) {
                bitb = i_ip[4] - 8;
                if (bitb == 0) {
                    bitb = i_ip[4];
                } else {
                    bitb += 8;
                }
            } else {
                bitb = 8 - i_ip[4];
            }
        }
    }
    bitr = 32 - i_ip[4];

    if (i_ip[4] == 32) {
        nbSubnet = 1;
        nbHost = 0;
    } else {
        nbSubnet = pow(2, bitb);
        nbHost = pow(2, bitr) -2;
    }
    
    // Store informations into var to save it later
    char *networkAddress, *subnetMask, *broadcastAddress, *firstIP, *lastIP;
    size_t size;

    if (bitb == 32) {
        // Sauvegarde de l'adresse réseau
        size = snprintf(NULL, 0, "%d.%d.%d.%d/%d", i_ip[0], i_ip[1], i_ip[2], i_ip[3], i_ip[4]);
        networkAddress = (char *) malloc(size + 1);
        snprintf(networkAddress, size+1, "%d.%d.%d.%d/%d", i_ip[0], i_ip[1], i_ip[2], i_ip[3], i_ip[4]);

        // Sauvegarde du masque
        size = snprintf(NULL, 0, "255.255.255.255");
        subnetMask = (char *) malloc(size + 1);
        snprintf(subnetMask, size+1, "255.255.255.255");

        // Sauvegarde de l'adresse de diffusion
        size = snprintf(NULL, 0, "%d.%d.%d.%d", i_ip[0], i_ip[1], i_ip[2], i_ip[3]);
        broadcastAddress = (char *) malloc(size + 1);
        snprintf(broadcastAddress, size+1, "%d.%d.%d.%d", i_ip[0], i_ip[1], i_ip[2], i_ip[3]);

        // Sauvegarde de la 1ère IP possible
        size = snprintf(NULL, 0, "%d.%d.%d.%d", i_ip[0], i_ip[1], i_ip[2], i_ip[3]);
        firstIP = (char *) malloc(size + 1);
        snprintf(firstIP, size+1, "%d.%d.%d.%d", i_ip[0], i_ip[1], i_ip[2], i_ip[3]);

        // Sauvegarde de la dernière IP possible
        size = snprintf(NULL, 0, "%d.%d.%d.%d", i_ip[0], i_ip[1], i_ip[2], i_ip[3]);
        lastIP = (char *) malloc(size + 1);
        snprintf(lastIP, size+1, "%d.%d.%d.%d", i_ip[0], i_ip[1], i_ip[2], i_ip[3]);

    } else if (bitb == 24) {
        size = snprintf(NULL, 0, "%d.%d.%d.0/%d", i_ip[0], i_ip[1], i_ip[2], i_ip[4]);
        networkAddress = (char *) malloc(size + 1);
        snprintf(networkAddress, size+1, "%d.%d.%d.0/%d", i_ip[0], i_ip[1], i_ip[2], i_ip[4]);

        size = snprintf(NULL, 0, "255.255.255.0");
        subnetMask = (char *) malloc(size + 1);
        snprintf(subnetMask, size+1, "255.255.255.0");

        size = snprintf(NULL, 0, "%d.%d.%d.255", i_ip[0], i_ip[1], i_ip[2]);
        broadcastAddress = (char *) malloc(size + 1);
        snprintf(broadcastAddress, size+1, "%d.%d.%d.255", i_ip[0], i_ip[1], i_ip[2]);

        size = snprintf(NULL, 0, "%d.%d.%d.1", i_ip[0], i_ip[1], i_ip[2]);
        firstIP = (char *) malloc(size + 1);
        snprintf(firstIP, size+1, "%d.%d.%d.1", i_ip[0], i_ip[1], i_ip[2]);

        size = snprintf(NULL, 0, "%d.%d.%d.254", i_ip[0], i_ip[1], i_ip[2]);
        lastIP = (char *) malloc(size + 1);
        snprintf(lastIP, size+1, "%d.%d.%d.254", i_ip[0], i_ip[1], i_ip[2]);

    } else if (bitb == 16) {
        size = snprintf(NULL, 0, "%d.%d.0.0/%d", i_ip[0], i_ip[1], i_ip[4]);
        networkAddress = (char *) malloc(size + 1);
        snprintf(networkAddress, size+1, "%d.%d.0.0/%d", i_ip[0], i_ip[1], i_ip[4]);

        size = snprintf(NULL, 0, "255.255.0.0");
        subnetMask = (char *) malloc(size + 1);
        snprintf(subnetMask, size+1, "255.255.0.0");

        size = snprintf(NULL, 0, "%d.%d.255.255", i_ip[0], i_ip[1]);
        broadcastAddress = (char *) malloc(size + 1);
        snprintf(broadcastAddress, size+1, "%d.%d.255.255", i_ip[0], i_ip[1]);

        size = snprintf(NULL, 0, "%d.%d.0.1", i_ip[0], i_ip[1]);
        firstIP = (char *) malloc(size + 1);
        snprintf(firstIP, size+1, "%d.%d.0.1", i_ip[0], i_ip[1]);

        size = snprintf(NULL, 0, "%d.%d.255.254", i_ip[0], i_ip[1]);
        lastIP = (char *) malloc(size + 1);
        snprintf(lastIP, size+1, "%d.%d.255.254", i_ip[0], i_ip[1]);

    } else if (bitb == 8) {
        size = snprintf(NULL, 0, "%d.0.0.0/%d", i_ip[0], i_ip[4]);
        networkAddress = (char *) malloc(size + 1);
        snprintf(networkAddress, size+1, "%d.0.0.0/%d", i_ip[0], i_ip[4]);

        size = snprintf(NULL, 0, "255.0.0.0");
        subnetMask = (char *) malloc(size + 1);
        snprintf(subnetMask, size+1, "255.0.0.0");

        size = snprintf(NULL, 0, "%d.255.255.255", i_ip[0]);
        broadcastAddress = (char *) malloc(size + 1);
        snprintf(broadcastAddress, size+1, "%d.255.255.255", i_ip[0]);

        size = snprintf(NULL, 0, "%d.0.0.1", i_ip[0]);
        firstIP = (char *) malloc(size + 1);
        snprintf(firstIP, size+1, "%d.0.0.1", i_ip[0]);

        size = snprintf(NULL, 0, "%d.255.255.254", i_ip[0]);
        lastIP = (char *) malloc(size + 1);
        snprintf(lastIP, size+1, "%d.255.255.254", i_ip[0]);

    } else {
        size = snprintf(NULL, 0, "0.0.0.0/0");
        networkAddress = (char *) malloc(size + 1);
        snprintf(networkAddress, size+1, "0.0.0.0/0");

        size = snprintf(NULL, 0, "0.0.0.0");
        subnetMask = (char *) malloc(size + 1);
        snprintf(subnetMask, size+1, "0.0.0.0");

        size = snprintf(NULL, 0, "255.255.255.255");
        broadcastAddress = (char *) malloc(size + 1);
        snprintf(broadcastAddress, size+1, "255.255.255.255");

        size = snprintf(NULL, 0, "0.0.0.1");
        firstIP = (char *) malloc(size + 1);
        snprintf(firstIP, size+1, "0.0.0.1");

        size = snprintf(NULL, 0, "255.255.255.254");
        lastIP = (char *) malloc(size + 1);
        snprintf(lastIP, size+1, "255.255.255.254");
    }

    printf("Réseau: %s\n", networkAddress);
    printf("Masque: %s\n", subnetMask);
    printf("Broadcast: %s\n", broadcastAddress);
    printf("Première IP: %s\n", firstIP);
    printf("Dernière IP: %s\n", lastIP);
    printf("Max. de réseaux: %d\n", nbSubnet);
    printf("Max. de machines: %d\n", nbHost);

    save(i_ip, networkAddress, subnetMask, broadcastAddress, firstIP, lastIP, &nbSubnet, &nbHost);
}

void save(int* ipAddr, char* ntwAddr, char* sbnMask, char* brdAddr, char* fstIP, char* lstIP, int* nbSubnet, int* nbHost) {
    FILE *p_file = fopen("Save.txt", "a");
    fprintf(p_file, "Adresse IP: %d.%d.%d.%d/%d\n", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3], ipAddr[4]);
    fprintf(p_file, "\tClasse: %c\n", classIP);

    if (isPrivate == true) {
        if (isLocalhost == true) {
            fprintf(p_file, "\tType: Réservée (localhost)\n");
        } else {
            fprintf(p_file, "\tType: Privée\n");
        }

        fprintf(p_file, "\tRéseau: %s\n", ntwAddr);
        fprintf(p_file, "\tMasque: %s\n", sbnMask);
        fprintf(p_file, "\tBroadcast: %s\n", brdAddr);
        fprintf(p_file, "\t\tPremière IP: %s\n", fstIP);
        fprintf(p_file, "\t\tDernière IP: %s\n", lstIP);
        fprintf(p_file, "\t\t\tMax. de réseaux: %d\n", *nbSubnet);
        fprintf(p_file, "\t\t\tMax. de machines: %d\n", *nbHost);
    } else {
        fprintf(p_file, "\tType: Publique\n");
        fprintf(p_file, "\tRéseau: %s\n", ntwAddr);
        fprintf(p_file, "\tMasque: %s\n", sbnMask);
        fprintf(p_file, "\tBroadcast: %s\n", brdAddr);
        fprintf(p_file, "\t\tPremière IP: %s\n", fstIP);
        fprintf(p_file, "\t\tDernière IP: %s\n", lstIP);
        fprintf(p_file, "\t\t\tMax. de réseaux: %d\n", *nbSubnet);
        fprintf(p_file, "\t\t\tMax. de machines: %d\n", *nbHost);
    }
    fprintf(p_file, "\n\n");
    fclose(p_file);
}