#include "ImageFile.h"

#include <iostream>
#include <string>

int main()
{
    constexpr char LENA_256[] = "lena256_PGM.pgm";

    std::string str(LENA_256);
    MKI::ImageFile lena(str);
}