#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
//validate if you have passed atleast 3 CLA
    if (argc < 3) {
        fprintf(stderr, "\033[31mnot enough arguments\033[0m\n");
        return 1;
    }
 if(check_operation_type(argv)==e_encode) {
     printf("User selected encode operation\n");
     EncodeInfo stegnography;
     if (read_and_validate_encode_args(argv, &stegnography)==e_success) {
         printf("Encode operation completed successfully\n");
         printf("<----------------Started encoding ------------------------->");
         if (do_encoding(&stegnography)==e_success) {
             printf("Encode operation completed successfully\n");
         }
         else {
             printf("Encode operation failed\n");
             return -1;
         }
     }


     else {
         printf("Encode operation failed\n");
         return -1;
     }

 }
    else if (check_operation_type(argv)==e_decode) {
        printf("User selected decode operation\n");
    }
    else {
        printf("Invalid operation\n Please pass\n");
        printf("For encoading: ./a.out -e beautyful.bmp secret.txt [stego.bmp]\n");
        printf("For decoding:   ./a.out  -d stego.bmp  [output.txt]");
    }
    return 0;
}
OperationType  check_operation_type(char *argv[]) {
    if(strcmp(argv[1],"-e")==0){
        return e_encode;
    }
    else if (strcmp(argv[1],"-d")==0) {
        return e_decode;
    }
    else {
        return  e_unsupported;
    }

}
