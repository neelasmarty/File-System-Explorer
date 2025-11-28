#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

void show_file_info(const char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        perror("stat");
        return;
    }

    // Permissions
    printf( (S_ISDIR(st.st_mode)) ? "d" : "-");
    printf( (st.st_mode & S_IRUSR) ? "r" : "-");
    printf( (st.st_mode & S_IWUSR) ? "w" : "-");
    printf( (st.st_mode & S_IXUSR) ? "x" : "-");
    printf( (st.st_mode & S_IRGRP) ? "r" : "-");
    printf( (st.st_mode & S_IWGRP) ? "w" : "-");
    printf( (st.st_mode & S_IXGRP) ? "x" : "-");
    printf( (st.st_mode & S_IROTH) ? "r" : "-");
    printf( (st.st_mode & S_IWOTH) ? "w" : "-");
    printf( (st.st_mode & S_IXOTH) ? "x" : "-");

    // Owner info
    struct passwd *pw = getpwuid(st.st_uid);
    struct group  *gr = getgrgid(st.st_gid);

    printf(" %s %s %ld %s\n", pw->pw_name, gr->gr_name, st.st_size, path);
}

void list_files(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    printf("Files in %s:\n", path);
    while ((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        show_file_info(full_path);
    }
    closedir(dir);
}

void print_tree(const char *base_path, int level) {
    DIR *dir = opendir(base_path);
    if (!dir) return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        for (int i = 0; i < level; i++) printf("  ");
        printf("|-- %s\n", entry->d_name);

        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", base_path, entry->d_name);

        struct stat st;
        if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
            print_tree(path, level + 1);
        }
    }
    closedir(dir);
}

long get_dir_size(const char *path) {
    long total = 0;
    DIR *dir = opendir(path);
    if (!dir) return 0;

    struct dirent *entry;
    struct stat st;
    char full_path[1024];

    while ((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if(stat(full_path, &st) == 0) {
            if(S_ISDIR(st.st_mode)) {
                total += get_dir_size(full_path);
            } else {
                total += st.st_size;
            }
        }
    }
    closedir(dir);
    return total;
}

void search_file(const char *base_path, const char *target) {
    DIR *dir = opendir(base_path);
    if (!dir) return;

    struct dirent *entry;
    char path[1024];

    while ((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", base_path, entry->d_name);

        if(strstr(entry->d_name, target)) {
            printf("Found: %s\n", path);
        }

        struct stat st;
        if(stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
            search_file(path, target);
        }
    }
    closedir(dir);
}

int main() {
    char path[1024];
    printf("Enter directory path: ");
    scanf("%1023s", path);

    printf("\n--- Listing Files ---\n");
    list_files(path);

    printf("\n--- Directory Tree ---\n");
    print_tree(path, 0);

    printf("\n--- Folder Size ---\n");
    printf("%ld bytes\n", get_dir_size(path));

    char target[256];
    printf("\nEnter filename to search: ");
    scanf("%255s", target);

    printf("\n--- Search Results ---\n");
    search_file(path, target);

    return 0;
}
