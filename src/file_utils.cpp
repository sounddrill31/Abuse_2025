#include "file_utils.h"
#include <filesystem>
#include <string>
#include <iostream>
#include <fcntl.h>
#include <cstring> // for strcpy, strchr

static char *filename_prefix = nullptr;
static char *save_filename_prefix = nullptr;

namespace {
    /**
     * @brief Ensures a path ends with a directory separator
     * @param prefix The path to normalize
     * @return Newly allocated string with normalized path
     */
    char *normalize_path(const char *prefix) {
        try {
            std::string path(prefix);
            if (!path.empty() && path.back() != '/' && path.back() != '\\') {
                path += PATH_SEPARATOR_CHAR;
            }

            char *result = static_cast<char *>(malloc(path.length() + 1));
            if (!result) {
                return nullptr;
            }

            strcpy(result, path.c_str());
            return result;
        } catch (...) {
            return nullptr;
        }
    }

    /**
     * @brief Safely combines prefix and filename
     * @return Newly allocated string with combined path or nullptr on error
     */
    std::string combine_paths(const char *prefix, const char *filename) {
        try {
            const std::filesystem::path base(prefix);
            const std::filesystem::path file(filename);
            return (base / file).string();
        } catch (...) {
            return filename;
        }
    }
}

void set_filename_prefix(char const *prefix) {
    // Free existing prefix if any
    free(filename_prefix);
    filename_prefix = nullptr;

    // Set new prefix if provided
    if (prefix) {
        filename_prefix = normalize_path(prefix);
    }
}

char *get_filename_prefix() {
    return filename_prefix;
}

void set_save_filename_prefix(char const *prefix) {
    // Free existing prefix if any
    free(save_filename_prefix);
    save_filename_prefix = nullptr;

    // Set new prefix if provided
    if (prefix) {
        save_filename_prefix = normalize_path(prefix);
    }
}

char *get_save_filename_prefix() {
    return save_filename_prefix;
}

FILE *prefix_fopen(const char *filename, const char *mode) {
    if (!filename || !mode) {
        return nullptr;
    }

    // Handle absolute paths directly
    if (filename[0] == '/' || filename[0] == '\\') {
        return fopen(filename, mode);
    }

    try {
        const bool is_write = strchr(mode, 'w') || strchr(mode, 'a');
        const bool is_read = strchr(mode, 'r');
        FILE *fp = nullptr;

        // For write/append modes, always try save prefix first
        if (is_write && save_filename_prefix) {
            const std::string path = combine_paths(save_filename_prefix, filename);
            // Create directories if writing
            if (is_write) {
                std::filesystem::path filepath(path);
                if (filepath.has_parent_path()) {
                    std::filesystem::create_directories(filepath.parent_path());
                }
            }
            fp = fopen(path.c_str(), mode);
            if (fp) {
                // std::cerr << "prefix_fopen: opened for writing: " << path << std::endl;
                return fp;
            }
        }

        // For read mode, try save prefix then data prefix
        if (is_read) {
            if (save_filename_prefix) {
                const std::string path = combine_paths(save_filename_prefix, filename);
                fp = fopen(path.c_str(), mode);
                if (fp) {
                    // std::cerr << "prefix_fopen: opened for reading (save): " << path << std::endl;
                    return fp;
                }
            }

            if (filename_prefix) {
                const std::string path = combine_paths(filename_prefix, filename);
                fp = fopen(path.c_str(), mode);
                if (fp) {
                    return fp;
                }
            }
        }

        // Fall back to direct file open
        // Create directories for direct path if writing
        if (is_write) {
            std::filesystem::path filepath(filename);
            if (filepath.has_parent_path()) {
                std::filesystem::create_directories(filepath.parent_path());
            }
        }
        fp = fopen(filename, mode);
        
        return fp;
    } catch (const std::exception &e) {
        std::cerr << "prefix_fopen error: " << e.what() << std::endl;
        return nullptr;
    }
}

int prefix_open(const char *filename, const int flags, const mode_t mode) {
    if (!filename) {
        return -1;
    }

    // Handle absolute paths directly
    if (filename[0] == '/' || filename[0] == '\\') {
        return open(filename, flags, mode);
    }

    try {
        const bool is_write = (flags & (O_WRONLY | O_RDWR | O_CREAT | O_APPEND)) != 0;
        const bool is_read = (flags & O_ACCMODE) == O_RDONLY;
        int fd;

        // For write modes, always try save prefix first
        if (is_write && save_filename_prefix) {
            const std::string path = combine_paths(save_filename_prefix, filename);
            // Create directories if writing
            if (is_write) {
                std::filesystem::path filepath(path);
                if (filepath.has_parent_path()) {
                    std::filesystem::create_directories(filepath.parent_path());
                }
            }
            fd = open(path.c_str(), flags, mode);
            if (fd != -1) {
                return fd;
            }
        }

        // For read mode, try save prefix then data prefix
        if (is_read) {
            if (save_filename_prefix) {
                const std::string path = combine_paths(save_filename_prefix, filename);
                fd = open(path.c_str(), flags, mode);
                if (fd != -1) {
                    return fd;
                }
            }

            if (filename_prefix) {
                const std::string path = combine_paths(filename_prefix, filename);
                fd = open(path.c_str(), flags, mode);
                if (fd != -1) {
                    return fd;
                }
            }
        }

        // Fall back to direct file open
        // Create directories for direct path if writing
        if (is_write) {
            std::filesystem::path filepath(filename);
            if (filepath.has_parent_path()) {
                std::filesystem::create_directories(filepath.parent_path());
            }
        }
        return open(filename, flags, mode);
    } catch (const std::exception &e) {
        std::cerr << "prefix_open error: " << e.what() << std::endl;
        return -1;
    }
}
