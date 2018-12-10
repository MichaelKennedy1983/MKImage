#include "ImageFile.h"
#include "Image.h"
#include "Globals.h"

#include <iostream>
#include <chrono>
#include <thread>

int main()
{
    constexpr char LENA_256[] = "lena256_PGM.pgm";
    constexpr char LENA_512[] = "lena512_PGM.pgm";

    constexpr char LENA_256_SAVE[] = "lena256_PGM_copy.pgm";
    constexpr char LENA_256_UPSIZE_SAVE[] = "lena256_PGM_upsize.pgm";
    constexpr char LENA_256_DOWNSIZE_SAVE[] = "lena256_PGM_downsize.pgm";
    constexpr char LENA_512_SAVE[] = "lena512_PGM_copy.pgm";
    constexpr char LENA_512_UPSIZE_SAVE[] = "lena512_PGM_upsize.pgm";
    constexpr char LENA_512_DOWNSIZE_SAVE[] = "lena512_PGM_downsize.pgm"; 

    constexpr char COMMENT[] = "File edited with MKImage";

    constexpr double RATIO = 2.0;
    constexpr double REV_RATIO = 0.5;
    constexpr size_t REPS = 1'000;

    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> l256_dur(0), l512_dur(0);

    auto l256_start = std::chrono::high_resolution_clock::now();
    MKI::ImageFile lena256(LENA_256);
    for (int i = 0; i < REPS; ++i) {
        l256_start = std::chrono::high_resolution_clock::now();
        lena256.image().resize(lena256.image().rows() * RATIO,
                               lena256.image().columns() * RATIO,
                               MKI::ResizeAlgorithm::bicubic);
        l256_dur += std::chrono::high_resolution_clock::now() - l256_start;
    }

    auto l512_start = std::chrono::high_resolution_clock::now();
    MKI::ImageFile lena512(LENA_512);
    for (int i = 0; i < REPS; ++i) {
        l512_start = std::chrono::high_resolution_clock::now();
        lena512.image().resize(lena512.image().rows() * RATIO,
                               lena512.image().columns() * RATIO,
                               MKI::ResizeAlgorithm::bicubic);
        l512_dur += std::chrono::high_resolution_clock::now() - l512_start;
    }

    double l256_avg = l256_dur.count() / REPS;
    double l512_avg = l512_dur.count() / REPS;

    std::cout << '\n' << LENA_256 << " average " << RATIO <<
        "X bicubic time: " << l256_avg << '\n';
    std::cout << LENA_512 << " average " << RATIO <<
        "X bicubic time: " << l512_avg << '\n';

    size_t num_threads = std::thread::hardware_concurrency();
    std::cout << "\nLogical threads: " << num_threads << '\n';
    std::cout << "Threads used: " << MKI::Globals::threads_used << '\n';

    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now()
        - start_time;
    std::cout << "\nProgram finished in " << duration.count() << " seconds\n\n";

    return 0;
}