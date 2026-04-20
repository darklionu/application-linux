/*
 * JOHN - John the Ripper Integration & Hash Cracking
 * ===================================================
 * Audit de mots de passe et cracking de hashes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

#define GRN "\033[1;32m"
#define RED "\033[1;31m"
#define YEL "\033[1;33m"
#define CYN "\033[1;36m"
#define RST "\033[0m"

/**
 * Détecte le type de hash
 */
const char *detect_hash_type(const char *hash) {
    size_t len = strlen(hash);
    
    if (len == 32 && strspn(hash, "0123456789abcdefABCDEF") == len) {
        return "MD5";
    }
    if (len == 40 && strspn(hash, "0123456789abcdefABCDEF") == len) {
        return "SHA1";
    }
    if (len == 64 && strspn(hash, "0123456789abcdefABCDEF") == len) {
        return "SHA256";
    }
    if (len == 128 && strspn(hash, "0123456789abcdefABCDEF") == len) {
        return "SHA512";
    }
    if (hash[0] == '$') {
        if (strncmp(hash, "$1$", 3) == 0) return "MD5-CRYPT";
        if (strncmp(hash, "$5$", 3) == 0) return "SHA256-CRYPT";
        if (strncmp(hash, "$6$", 3) == 0) return "SHA512-CRYPT";
        if (strncmp(hash, "$2a$", 4) == 0 || strncmp(hash, "$2y$", 4) == 0) return "BCRYPT";
    }
    
    return "UNKNOWN";
}

/**
 * Extrait les hashes d'un fichier shadow/passwd
 */
void extract_hashes(const char *filepath) {
    printf(GRN "[+]" RST " Extraction des hashes de %s\n\n", filepath);
    
    FILE *f = fopen(filepath, "r");
    if (!f) {
        fprintf(stderr, RED "[-]" RST " Impossible d'ouvrir: %s\n", filepath);
        return;
    }
    
    char line[512];
    int count = 0;
    
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        
        /* Format shadow: user:hash:... */
        char *colon = strchr(line, ':');
        if (!colon) continue;
        
        char *user = line;
        *colon = '\0';
        char *hash = colon + 1;
        
        /* Saute les hashes vides ou verrouillés */
        if (!hash[0] || hash[0] == '*' || hash[0] == '!') continue;
        
        const char *type = detect_hash_type(hash);
        printf(CYN "[%d]" RST " User: %-15s Hash: %s... Type: %s\n",
               ++count, user, hash[0] ? "••••" : "(vide)", type);
    }
    
    fclose(f);
    printf("\n" GRN "[✓]" RST " %d hash(es) trouvé(s)\n\n", count);
}

/**
 * Cracking simple avec wordlist
 */
void crack_hash(const char *hash, const char *wordlist_path) {
    printf(GRN "[+]" RST " Cracking hash: %s\n", hash);
    printf("Wordlist: %s\n\n", wordlist_path);
    
    const char *hash_type = detect_hash_type(hash);
    printf("Type détecté: %s\n\n", hash_type);
    
    FILE *wf = fopen(wordlist_path, "r");
    if (!wf) {
        fprintf(stderr, RED "[-]" RST " Impossible d'ouvrir wordlist: %s\n", wordlist_path);
        return;
    }
    
    char word[256];
    long tries = 0;
    
    printf(CYN "Tentatives de cracking:\n" RST);
    
    while (fgets(word, sizeof(word), wf)) {
        word[strcspn(word, "\n\r")] = 0;
        if (!word[0]) continue;
        
        tries++;
        
        /* Test simple avec crypt() pour les hashes UNIX */
        if (strcmp(hash_type, "MD5-CRYPT") == 0 ||
            strcmp(hash_type, "SHA256-CRYPT") == 0 ||
            strcmp(hash_type, "SHA512-CRYPT") == 0) {
            
            char *result = crypt(word, hash);
            if (result && strcmp(result, hash) == 0) {
                printf(GRN "[+] PASSWORD TROUVÉ: %s\n\n" RST, word);
                fclose(wf);
                return;
            }
        }
        
        if (tries % 100 == 0) {
            printf(YEL "  [%ld] Essais...\r" RST, tries);
            fflush(stdout);
        }
    }
    
    fclose(wf);
    printf("\n" YEL "[!]" RST " Aucun mot trouvé après %ld essais\n", tries);
}

/**
 * Génère MD5 d'une string
 */
void hash_md5(const char *input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_CTX md5;
    
    MD5_Init(&md5);
    MD5_Update(&md5, (unsigned char*)input, strlen(input));
    MD5_Final(digest, &md5);
    
    printf(CYN "MD5:" RST " ");
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        printf("%02x", digest[i]);
    }
    printf("\n");
}

/**
 * Génère SHA256 d'une string
 */
void hash_sha256(const char *input) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, (unsigned char*)input, strlen(input));
    SHA256_Final(digest, &sha256);
    
    printf(CYN "SHA256:" RST " ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", digest[i]);
    }
    printf("\n");
}

/**
 * Énumère les comptes du fichier passwd
 */
void list_users(const char *filepath) {
    printf(GRN "[+]" RST " Comptes système de %s\n\n", filepath);
    
    FILE *f = fopen(filepath, "r");
    if (!f) {
        fprintf(stderr, RED "[-]" RST " Impossible d'ouvrir: %s\n", filepath);
        return;
    }
    
    char line[512];
    printf(CYN "%-15s %-5s %-5s %-35s\n" RST, "User", "UID", "GID", "Home");
    printf("────────────────────────────────────────────────────────\n");
    
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        
        /* Format: user:x:uid:gid:comment:home:shell */
        char *fields[7];
        char *copy = strdup(line);
        char *tok = copy;
        
        for (int i = 0; i < 7 && tok; i++) {
            fields[i] = tok;
            tok = strchr(tok, ':');
            if (tok) *tok++ = '\0';
        }
        
        /* Affiche utilisateurs avec shell (non-système) */
        if (fields[6] && (strstr(fields[6], "bash") || strstr(fields[6], "sh"))) {
            printf("%-15s %-5s %-5s %-35s\n", 
                   fields[0], fields[2], fields[3], fields[5]);
        }
        
        free(copy);
    }
    
    fclose(f);
    printf("\n");
}

/**
 * Main
 */
int main(int argc, char *argv[]) {
    printf(CYN "╔═══════════════════════════════════════╗\n");
    printf("║  JOHN - Hash Cracking & Audit Tool  ║\n");
    printf("╚═══════════════════════════════════════╝\n\n" RST);
    
    if (argc < 2) {
        printf("Usage: %s <action> <args...>\n\n", argv[0]);
        printf("Actions:\n");
        printf("  list <passwd>           - Énumère les utilisateurs\n");
        printf("  extract <shadow>        - Extrait les hashes\n");
        printf("  crack <hash> <wordlist> - Cracking par wordlist\n");
        printf("  detect <hash>           - Détecte le type de hash\n");
        printf("  md5 <string>            - Génère MD5\n");
        printf("  sha256 <string>         - Génère SHA256\n");
        return 1;
    }
    
    const char *action = argv[1];
    
    if (strcmp(action, "list") == 0) {
        if (argc < 3) { fprintf(stderr, "Usage: %s list <passwd>\n", argv[0]); return 1; }
        list_users(argv[2]);
        
    } else if (strcmp(action, "extract") == 0) {
        if (argc < 3) { fprintf(stderr, "Usage: %s extract <shadow>\n", argv[0]); return 1; }
        extract_hashes(argv[2]);
        
    } else if (strcmp(action, "crack") == 0) {
        if (argc < 4) { fprintf(stderr, "Usage: %s crack <hash> <wordlist>\n", argv[0]); return 1; }
        crack_hash(argv[2], argv[3]);
        
    } else if (strcmp(action, "detect") == 0) {
        if (argc < 3) { fprintf(stderr, "Usage: %s detect <hash>\n", argv[0]); return 1; }
        printf("Hash: %s\n", argv[2]);
        printf("Type: %s\n", detect_hash_type(argv[2]));
        
    } else if (strcmp(action, "md5") == 0) {
        if (argc < 3) { fprintf(stderr, "Usage: %s md5 <string>\n", argv[0]); return 1; }
        printf("Input: %s\n", argv[2]);
        hash_md5(argv[2]);
        
    } else if (strcmp(action, "sha256") == 0) {
        if (argc < 3) { fprintf(stderr, "Usage: %s sha256 <string>\n", argv[0]); return 1; }
        printf("Input: %s\n", argv[2]);
        hash_sha256(argv[2]);
        
    } else {
        fprintf(stderr, RED "[-]" RST " Action inconnue: %s\n", action);
        return 1;
    }
    
    return 0;
}
