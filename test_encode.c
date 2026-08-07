#include <stdio.h>
#include "encode.h"
#include "types.h"

int main(int argc, char **argv)
{
//validate if you have passed atleast 3 CLA
    if (argc<3) {
        printf("not enough arguments\n");
    }
 if(check_operation_type(argv)==e_encode) {
     printf("User selected encode operation\n");
 }
    else if (check_operation_type(argv)==e_decode) {
        printf("User selected decode operation\n");
    }
    else {
        printf("Invalid operation\n Please pass\n");
        printf("For encoading:./a.out -e beautyful.bmp secret.txt [stego.bmp]");
        printf("For decoding:./a.out  -d stego.bmp  [output.txt]");
    }
    return 0;
}
OperationType  check_operation_type(char *argv[]) {

}
