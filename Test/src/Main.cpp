#include "ImageFile.h"

#include <iostream>
#include <string>

int main()
{
    constexpr char LENA_256[] = "lena256_PGM.pgm";
    constexpr char LENA_512[] = "lena512_PGM.pgm";

    constexpr char LENA_256_SAVE[] = "lena256_PGM_copy.pgm";
    constexpr char LENA_512_SAVE[] = "lena512_PGM_copy.pgm";

    constexpr char COMMENT[] = "File edited with MKImage";

    MKI::ImageFile lena256(LENA_256);
    lena256.save(LENA_256_SAVE, COMMENT);

    MKI::ImageFile lena512(LENA_512);
    lena512.save(LENA_512_SAVE, COMMENT);

    int i = 0;
}