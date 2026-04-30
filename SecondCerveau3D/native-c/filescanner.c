#include "filescanner.h"
#include <openssl/sha.h>
#include <ftw.h>

static int file_count = 0;
static FileInfo temp_files[MAX_FILES];

char* compute_file_hash(const char *filepath) {
    static char hex_hash[65];
    unsigned char hash[SHA_DIGEST_LENGTH];
    FILE *fp = fopen(filepath, "rb");
    
    if (!fp) return NULL;
    
    SHA_CTX sha_ctx;
    SHA1_Init(&sha_ctx);
    
    char buf[4096];
    size_t bytes;
    while ((bytes = fread(buf, 1, sizeof(buf), fp)) > 0) {
        SHA1_Update(&sha_ctx, (unsigned char *)buf, bytes);
    }
    SHA1_Final(hash, &sha_ctx);
    
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(hex_hash + (i * 2), "%02x", hash[i]);
    }
    hex_hash[64] = '\0';
    fclose(fp);
    
    return hex_hash;
}

void analyze_file_threat(FileInfo *file) {
    /* Threat detection based on patterns and signatures */
    file->threat_level = 0;
    
    /* Check for suspicious extensions */
    const char *suspicious_ext[] = {".exe", ".dll", ".sh", ".bat", ".bin", NULL};
    for (int i = 0; suspicious_ext[i]; i++) {
        if (strstr(file->filename, suspicious_ext[i])) {
            file->threat_level += 2;
            break;
        }
    }
    
    /* Check for hidden files */
    if (file->filename[0] == '.') {
        file->threat_level += 1;
    }
    
    /* Check file permissions (if executable) */
    if (file->permissions[3] == 'x' || file->permissions[6] == 'x') {
        file->threat_level += 1;
    }
    
    /* Size anomalies */
    if (file->size > 100 * 1024 * 1024) {
        file->threat_level += 1;
    }
}

static int scan_callback(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    if (file_count >= MAX_FILES) return 0;
    
    if (typeflag == FTW_F) {
        FileInfo *file = &temp_files[file_count];
        strncpy(file->path, fpath, MAX_PATH_LEN - 1);
        strncpy(file->filename, fpath + ftwbuf->base, 255);
        file->size = sb->st_size;
        file->modified = sb->st_mtime;
        
        sprintf(file->permissions, "%o", sb->st_mode & 0777);
        
        if (!(file->hash = compute_file_hash(fpath))) {
            strcpy(file->hash, "N/A");
        }
        
        analyze_file_threat(file);
        file_count++;
    }
    
    return 0;
}

ScanResult* scan_directory(const char *root_path) {
    file_count = 0;
    memset(temp_files, 0, sizeof(temp_files));
    
    nftw(root_path, scan_callback, 20, FTW_PHYS);
    
    ScanResult *result = malloc(sizeof(ScanResult));
    memcpy(result->files, temp_files, sizeof(temp_files));
    result->count = file_count;
    
    time_t now = time(NULL);
    strftime(result->scan_timestamp, 32, "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    return result;
}

void free_scan_result(ScanResult *result) {
    if (result) free(result);
}

ScanResult* merge_scan_results(ScanResult *result1, ScanResult *result2) {
    ScanResult *merged = malloc(sizeof(ScanResult));
    merged->count = 0;
    
    memcpy(merged->files, result1->files, result1->count * sizeof(FileInfo));
    merged->count = result1->count;
    
    if (merged->count + result2->count <= MAX_FILES) {
        memcpy(&merged->files[merged->count], result2->files, result2->count * sizeof(FileInfo));
        merged->count += result2->count;
    }
    
    strcpy(merged->scan_timestamp, result1->scan_timestamp);
    
    return merged;
}

void send_scan_to_orchestrator(ScanResult *result) {
    /* Integration point with agent_orchestrator */
    printf("[FileScanner] Sending %d files to orchestrator\n", result->count);
    
    for (int i = 0; i < result->count && i < 5; i++) {
        printf("  [%s] Threat Level: %d\n", result->files[i].filename, result->files[i].threat_level);
    }
}

int check_suspicious_pattern(const char *filename, const char *path) {
    /* Pattern matching for suspicious files */
    const char *patterns[] = {"backdoor", "malware", "exploit", "payload", "shellcode", NULL};
    
    for (int i = 0; patterns[i]; i++) {
        if (strstr(filename, patterns[i]) || strstr(path, patterns[i])) {
            return 1;
        }
    }
    return 0;
}
