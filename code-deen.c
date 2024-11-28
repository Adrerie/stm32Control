#include "code-deen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <dirent.h>
#include <time.h>
//#include <sys/stat.h>
#include <zlib.h>
#include "libb64/include/b64/cencode.h"
//#include "libb64/include/b64/cdecode.h"

#include "code-deen.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void encode_image_to_base64(const uint8_t *image_data, size_t image_size, char *encoded_data, size_t *encoded_size) {
    base64_encodestate state;
    base64_init_encodestate(&state);
    *encoded_size = base64_encode_block((const char *)image_data, image_size, encoded_data, &state);
    *encoded_size += base64_encode_blockend(encoded_data + *encoded_size, &state);

    // Compress the encoded data
    uLongf compressed_size = compressBound(*encoded_size);
    unsigned char *compressed_data = malloc(compressed_size);
    compress(compressed_data, &compressed_size, (const Bytef *)encoded_data, *encoded_size);

    // Copy compressed data back to encoded_data
    memcpy(encoded_data, compressed_data, compressed_size);
    *encoded_size = compressed_size;

    free(compressed_data);
}


// void encode_image_to_base64(const char *image_path, const char *output_path) {
//     FILE *image_file = fopen(image_path, "rb");
//     if (!image_file) {
//         perror("Failed to open image file");
//         return;
//     }
//
//     fseek(image_file, 0, SEEK_END);
//     const long image_size = ftell(image_file);
//     fseek(image_file, 0, SEEK_SET);
//
//     unsigned char *image_data = malloc(image_size);
//     fread(image_data, 1, image_size, image_file);
//     fclose(image_file);
//
//     char *encoded_data = malloc(image_size * 2); // Base64编码后的数据会比原数据大
//     base64_encodestate state;
//     base64_init_encodestate(&state);
//     int encoded_size =(int) base64_encode_block((const char *)image_data, image_size, encoded_data, &state);
//     encoded_size += (int) base64_encode_blockend(encoded_data + encoded_size, &state);
//
//     uLongf compressed_size = compressBound(encoded_size);
//     unsigned char *compressed_data = malloc(compressed_size);
//     compress(compressed_data, &compressed_size, (const Bytef *)encoded_data, encoded_size);
//
//     FILE *output_file = fopen(output_path, "wb");
//     fwrite(compressed_data, 1, compressed_size, output_file);
//     fclose(output_file);
//
//     free(image_data);
//     free(encoded_data);
//     free(compressed_data);
//
//
//     // 标记图片为已发送
//     char mark_path[1024];
//     snprintf(mark_path, sizeof(mark_path), "%s.sent", image_path);
//     FILE *mark_file = fopen(mark_path, "w");
//     if (mark_file) {
//         fprintf(mark_file, "Sent at %lld\n", time(NULL));
//         fclose(mark_file);
//     }
//
//     printf("Encoded and compressed content of %s saved to %s\n", image_path, output_path);
// }


// void decode_base64_to_image(const char *encoded_path, const char *output_path) {
//     FILE *encoded_file = fopen(encoded_path, "rb");
//     if (!encoded_file) {
//         perror("Failed to open encoded file");
//         return;
//     }
//
//     fseek(encoded_file, 0, SEEK_END);
//     const long encoded_size = ftell(encoded_file);
//     fseek(encoded_file, 0, SEEK_SET);
//
//     unsigned char *compressed_data = malloc(encoded_size);
//     fread(compressed_data, 1, encoded_size, encoded_file);
//     fclose(encoded_file);
//
//     uLongf decoded_size = encoded_size * 2; // 解压后的数据会比压缩数据大
//     unsigned char *decoded_data = malloc(decoded_size);
//     uncompress(decoded_data, &decoded_size, compressed_data, encoded_size);
//
//     char *image_data = malloc(decoded_size);
//     base64_decodestate state;
//     base64_init_decodestate(&state);
//     const int image_size = base64_decode_block((const char *)decoded_data, decoded_size, image_data, &state);
//
//     FILE *output_file = fopen(output_path, "wb");
//     fwrite(image_data, 1, image_size, output_file);
//     fclose(output_file);
//
//     free(compressed_data);
//     free(decoded_data);
//     free(image_data);
//
//     printf("Decoded image saved to %s\n", output_path);
// }
//
// void encode_images_in_folder(const char *image_folder, const char *output_folder) {
//     struct stat st = {0};
//     if (stat(output_folder, &st) == -1) {
//         mkdir(output_folder);
//     }
//
//     DIR *dir = opendir(image_folder);
//     if (!dir) {
//         perror("Failed to open image folder");
//         return;
//     }
//
//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL) {
//         char filepath[1024];
//         snprintf(filepath, sizeof(filepath), "%s/%s", image_folder, entry->d_name);
//         struct stat path_stat;
//         stat(filepath, &path_stat);
//         if (S_ISREG(path_stat.st_mode)) {
//             const char *filename = entry->d_name;
//             const char *ext = strrchr(filename, '.');
//             if (ext && (strcmp(ext, ".png") == 0 || strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)) {
//                 char image_path[1024];
//                 snprintf(image_path, sizeof(image_path), "%s/%s", image_folder, filename);
//
//                 char output_path[1024];
//                 snprintf(output_path, sizeof(output_path), "%s/%s.txt", output_folder, filename);
//
//                 encode_image_to_base64(image_path, output_path);
//             }
//         }
//     }
//
//     closedir(dir);
// }
//
// void decode_images_in_folder(const char *encoded_folder, const char *output_folder) {
//     struct stat st = {0};
//     if (stat(output_folder, &st) == -1) {
//         mkdir(output_folder);
//     }
//
//     DIR *dir = opendir(encoded_folder);
//     if (!dir) {
//         perror("Failed to open encoded folder");
//         return;
//     }
//
//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL) {
//         char filepath[1024];
//         snprintf(filepath, sizeof(filepath), "%s/%s", encoded_folder, entry->d_name);
//         struct stat path_stat;
//         stat(filepath, &path_stat);
//         if (S_ISREG(path_stat.st_mode)) {
//             const char *filename = entry->d_name;
//             const char *ext = strrchr(filename, '.');
//             if (ext && strcmp(ext, ".txt") == 0) {
//                 char encoded_path[1024];
//                 snprintf(encoded_path, sizeof(encoded_path), "%s/%s", encoded_folder, filename);
//
//                 char output_path[1024];
//                 snprintf(output_path, sizeof(output_path), "%s/%.*s", output_folder, (int)(ext - filename), filename);
//
//                 decode_base64_to_image(encoded_path, output_path);
//             }
//         }
//     }
//
//     closedir(dir);
// }

// int main() {
//     const char *image_folder = "D:/Adrerie/Desktop/sample/test/encode";  // 替换为你的图片文件夹路径
//     const char *encoded_folder = "D:/Adrerie/Desktop/sample/test/encoded";  // 替换为你的Base64编码文件夹路径
//     const char *output_folder = "D:/Adrerie/Desktop/sample/test/decode";  // 替换为你的输出文件夹路径
//
//     // 编码图片
//     encode_images_in_folder(image_folder, encoded_folder);
//
//     // 解码图片
//     //decode_images_in_folder(encoded_folder, output_folder);
//
//     return 0;
// }
