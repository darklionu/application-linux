/*
 * ANALYSE DE FICHIER - Extraction et Analyse
 * ================================================
 * Outils pour analyser des fichiers (texte, PDF, archives)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <magic.h>

#define MAX_BUFFER 4096

/* Couleurs */
#define GRN "\033[1;32m"
#define YEL "\033[1;33m"
#define RED "\033[1;31m"
#define CYN "\033[1;36m"
#define RST "\033[0m"

typedef struct {
    char type[64];
    char encoding[64];
    size_t size;
    char path[256];
} FileInfo;

/**
 * Détecte le type MIME d'un fichier
 */
int detect_file_type(const char *filepath, FileInfo *info) {
    magic_t magic = magic_open(MAGIC_MIME_TYPE);
    if (!magic) {
        fprintf(stderr, RED "[-]" RST " Erreur libmagic\n");
        return -1;
    }
    
    if (magic_load(magic, NULL) != 0) {
        fprintf(stderr, RED "[-]" RST " Impossible de charger la db libmagic\n");
        magic_close(magic);
        return -1;
    }
    
    const char *mime = magic_file(magic, filepath);
    if (mime) {
        strncpy(info->type, mime, sizeof(info->type) - 1);
    }
    
    magic_close(magic);
    
    /* Récupère taille */
    struct stat st;
    if (stat(filepath, &st) == 0) {
        info->size = st.st_size;
    }
    
    strncpy(info->path, filepath, sizeof(info->path) - 1);
    return 0;
}

/**
 * Analyse un fichier texte
 */
void analyze_text_file(const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if (!f) {
        fprintf(stderr, RED "[-]" RST " Impossible d'ouvrir: %s\n", filepath);
        return;
    }
    
    printf(GRN "[+]" RST " Analyse texte de: %s\n\n", filepath);
    
    char line[MAX_BUFFER];
    int lines = 0, words = 0, chars = 0;
    
    while (fgets(line, sizeof(line), f)) {
        lines++;
        chars += strlen(line);
        
        char *word = strtok(line, " \t\n");
        while (word) {
            words++;
            word = strtok(NULL, " \t\n");
        }
    }
    
    printf(CYN "Statistiques:\n" RST);
    printf("  Lignes: %d\n", lines);
    printf("  Mots:   %d\n", words);
    printf("  Chars:  %d\n", chars);
    
    rewind(f);
    printf(CYN "\nPremières 20 lignes:\n" RST);
    int count = 0;
    while (fgets(line, sizeof(line), f) && count < 20) {
        printf("  %s", line);
        count++;
    }
    
    fclose(f);
}

/**
 * Analyse un fichier PDF (extraction de texte simple)
 */
void analyze_pdf_file(const char *filepath) {
    printf(GRN "[+]" RST " Analyse PDF: %s\n\n", filepath);
    printf(YEL "[!]" RST " Extraction PDF nécessite libpoppler\n");
    printf(YEL "[!]" RST " Utilisez: pdftotext %s -\n\n", filepath);
    
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "pdftotext '%s' - 2>/dev/null | head -40", filepath);
    system(cmd);
}

/**
 * Analyse une archive
 */
void analyze_archive(const char *filepath) {
    printf(GRN "[+]" RST " Contenu archive: %s\n\n", filepath);
    
    char cmd[512];
    
    if (strstr(filepath, ".zip")) {
        snprintf(cmd, sizeof(cmd), "unzip -l '%s' 2>/dev/null | head -30", filepath);
    } else if (strstr(filepath, ".tar.gz") || strstr(filepath, ".tgz")) {
        snprintf(cmd, sizeof(cmd), "tar -tzf '%s' 2>/dev/null | head -30", filepath);
    } else if (strstr(filepath, ".tar")) {
        snprintf(cmd, sizeof(cmd), "tar -tf '%s' 2>/dev/null | head -30", filepath);
    } else if (strstr(filepath, ".7z")) {
        snprintf(cmd, sizeof(cmd), "7z l '%s' 2>/dev/null | head -30", filepath);
    } else {
        printf(RED "[-]" RST " Format d'archive non reconnu\n");
        return;
    }
    
    system(cmd);
}

/**
 * Affiche infos générales
 */
void print_file_info(FileInfo *info) {
    printf(CYN "═══════════════════════════════════════\n" RST);
    printf("Fichier: %s\n", info->path);
    printf("Type MIME: %s\n", info->type);
    printf("Taille: %zu bytes (%.2f KB)\n", 
           info->size, (float)info->size / 1024);
    printf(CYN "═══════════════════════════════════════\n\n" RST);
}

/**
 * Main
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf(GRN "Usage:" RST " %s <fichier>\n", argv[0]);
        printf(CYN "Formats supportés:" RST "\n");
        printf("  - Texte (txt, md, log, conf)\n");
        printf("  - PDF (pdftotext requis)\n");
        printf("  - Archives (zip, tar, tar.gz, 7z)\n");
        return 1;
    }
    
    FileInfo info = {0};
    
    /* Détecte le type */
    if (detect_file_type(argv[1], &info) != 0) {
        return 1;
    }
    
    print_file_info(&info);
    
    /* Analyse selon le type */
    if (strstr(info.type, "text")) {
        analyze_text_file(argv[1]);
    } else if (strstr(info.type, "pdf")) {
        analyze_pdf_file(argv[1]);
    } else if (strstr(info.type, "zip") || 
               strstr(info.type, "gzip") || 
               strstr(info.type, "x-tar") ||
               strstr(info.type, "7z")) {
        analyze_archive(argv[1]);
    } else {
        printf(YEL "[!]" RST " Type non analysable automatiquement\n");
        printf("Type détecté: %s\n", info.type);
    }
    
    return 0;
}
