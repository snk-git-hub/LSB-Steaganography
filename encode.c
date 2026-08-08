#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo) {
	if(strcmp(strchr(argv[2],'.'),".bmp")==0) {
       encInfo->src_image_fname = argv[2];
	}
	else {
		return e_failure;
	}
	if(strcmp(strchr(argv[3],'.'),".txt")==0) {
		encInfo->secret_fname = argv[3];
	}
	else {
		return e_failure;
	}
	if (argv[4]==NULL) {
		encInfo->stego_image_fname = "stego.bmp";
	}
	else {
		encInfo->stego_image_fname = argv[4];
	}
   return e_success;
}

Status do_encoding(EncodeInfo *encInfo) {
	if (open_files(encInfo) == e_success) {
		printf("Opened all required files in required mode.\n");
	}
	else {
		printf("Error opening file \n");
		return e_failure;
	}
	if (check_capacity(encInfo)==e_success) {
		printf("The file %s is suitable for encoding\n", encInfo->src_image_fname);
	}
	else {
		return e_failure;
	}
	if (copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success) {
     printf("copy bmp header as it is from %s,  %s\n", encInfo->src_image_fname, encInfo->stego_image_fname);
	}
	else {
		printf("failed copy bmp header as it is from %s , %s \n", encInfo->src_image_fname, encInfo->src_image_fname);
        return e_failure;
	}
	if (encode_magic_string(MAGIC_STRING,encInfo)==e_success) {
		printf("encoded the magic string sucessfully \n");
	}
	else {
		printf("failed encode magic string sucessfully \n");
		return e_failure;
	}
	strcpy(encInfo->extn_secret_file , (strchr(encInfo->secret_fname,'.')));
	if (encode_size_of_extension(4,encInfo)==e_success) {
     printf("encode the extension size sucessfully \n");
	}
	else {
		printf("failed encode extension size sucessfully \n");
		return e_failure;
	}
return e_success;
}
uint get_file_size(FILE *fptr) {
	long size;
	fseek(fptr, 0, SEEK_END);
	size = ftell(fptr);
	if (size < 0) return 0;
	return (uint)size;
}
Status check_capacity(EncodeInfo *encInfo) {
encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
	if (encInfo->image_capacity>(2+4+4+4+encInfo->size_secret_file)*8) {
		return e_success;
	}
	else {
		return e_failure;
	}
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){
	char str[54];
	fseek(fptr_src_image,0,SEEK_SET);
	fread(str,sizeof(char),54,fptr_src_image);
	fwrite(str,sizeof(char),54,fptr_dest_image);
	return  e_success;
}
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo) {
    encode_data_to_image((char *)magic_string,2, encInfo);
	return e_success;
}
Status encode_data_to_image(char *data, int size, EncodeInfo *encInfo) {
	for (int i =0; i < size; i++) {
	    fread(encInfo->image_data,8,1,encInfo->fptr_src_image);
		encode_byte_to_lsb(data[i],encInfo->image_data);
        fwrite(encInfo->image_data,8,1,encInfo->fptr_stego_image);
	}
	return e_success;
}
Status encode_byte_to_lsb(char data, char *image_buffer) {
	for (int i = 0; i < 8; i++) {
		image_buffer[i] = (char)((image_buffer[i] & 0xFE) | ((data >> (7 - i)) & 1));
	}
	return e_success;
}
Status encode_size_of_extension(int size, EncodeInfo *encInfo) {
	encode_integer_to_lsb(size,encInfo);
	return e_success;
}
Status encode_integer_to_lsb(int size, EncodeInfo *encInfo) {

	return e_success;
}