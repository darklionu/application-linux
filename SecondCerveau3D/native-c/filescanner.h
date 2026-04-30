#ifndef FILESCANNER_H
#define FILESCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_PATH_LEN 4096
#define MAX_FILES 10000
#define MAX_SIGNATURE 256

typedef struct {
    char path[MAX_PATH_LEN];
    char filename[256];
    size_t size;
    time_t modified;
    char permissions[16];
    char hash[65];
    int threat_level;
} FileInfo;

typedef struct {
    FileInfo files[MAX_FILES];
    int count;
    char scan_timestamp[32];
} ScanResult;

/* Scanner functions */
ScanResult* scan_directory(const char *root_path);
void analyze_file_threat(FileInfo *file);
char* compute_file_hash(const char *filepath);
void free_scan_result(ScanResult *result);
ScanResult* merge_scan_results(ScanResult *result1, ScanResult *result2);

/* Integration with agent system */
void send_scan_to_orchestrator(ScanResult *result);
int check_suspicious_pattern(const char *filename, const char *path);

#endif
