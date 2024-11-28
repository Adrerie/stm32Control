#pragma once
#include <stdint.h>


extern void  encode_image_to_base64(const uint8_t *image_data, size_t image_size, char *encoded_data, size_t *encoded_size);

