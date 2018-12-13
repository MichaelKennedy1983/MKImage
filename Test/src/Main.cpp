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
    constexpr char SHUTTLE[] = "shuttle_pgm.pgm";

    constexpr char LENA_256_SAVE[] = "lena256_PGM_copy.pgm";
    constexpr char LENA_256_UPSIZE_SAVE[] = "lena256_PGM_upsize.pgm";
    constexpr char LENA_256_DOWNSIZE_SAVE[] = "lena256_PGM_downsize.pgm";
    constexpr char LENA_512_SAVE[] = "lena512_PGM_copy.pgm";
    constexpr char LENA_512_BLUPSIZE_SAVE[] = "lena512_PGM_blupsize.pgm";
    constexpr char LENA_512_BLDOWNSIZE_SAVE[] = "lena512_PGM_bldownsize.pgm";
    constexpr char LENA_512_BCUPSIZE_SAVE[] = "lena512_PGM_bcupsize.pgm";
    constexpr char LENA_512_BCDOWNSIZE_SAVE[] = "lena512_PGM_bcdownsize.pgm";
    constexpr char SHUTTLE_UPSIZE_SAVE[] = "shuttle_pgm_upsize.pgm";
    constexpr char SHUTTLE_DOWNSIZE_SAVE[] = "shuttle_pgm_downsize.pgm";

    constexpr char COMMENT[] = "File edited with MKImage";

    constexpr double RATIO = 2.0;
    constexpr double REV_RATIO = 0.5;
    constexpr size_t REPS = 500;

    auto start_time = std::chrono::high_resolution_clock::now();

    // MKI::ImageFile shuttle_file(SHUTTLE);
    // const MKI::Image& shuttle = shuttle_file.image();
    // shuttle_file.add(shuttle.resize(shuttle.rows() * RATIO,
    //                                 shuttle.columns() * RATIO,
    //                                 MKI::ResizeAlgorithm::bilinear));
    // shuttle_file.save(SHUTTLE_UPSIZE_SAVE, COMMENT);

    MKI::ImageFile lena512_file(LENA_256);
    const MKI::Image& lena512 = lena512_file.image();
    
    auto l512_bl_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < REPS; ++i) {
        lena512.resize(lena512.rows() * RATIO,
                       lena512.columns() * RATIO,
                       MKI::ResizeAlgorithm::bilinear);
    }
    std::chrono::duration<double> l512_bl_dur = std::chrono::high_resolution_clock::now() - l512_bl_start;

    auto l512_bc_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < REPS; ++i) {
        lena512.resize(lena512.rows() * RATIO,
                       lena512.columns() * RATIO,
                       MKI::ResizeAlgorithm::bicubic);
    }
    std::chrono::duration<double> l512_bc_dur = std::chrono::high_resolution_clock::now() - l512_bc_start;

    double l512_bl_avg = l512_bl_dur.count() / REPS;
    double l512_bc_avg = l512_bc_dur.count() / REPS;

    std::cout << "\nBilinear interpolation " << RATIO <<
        "X average: " << l512_bl_avg << " seconds.\n";
    std::cout << "Bicubic interpolation " << RATIO << 
        "X average: " << l512_bc_avg << " seconds.\n";


    size_t num_threads = std::thread::hardware_concurrency();
    std::cout << "\nLogical threads: " << num_threads << '\n';
    std::cout << "Threads used: " << MKI::Globals::threads_used << '\n';

    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now()
        - start_time;
    std::cout << "\nProgram finished in " << duration.count() << " seconds\n\n";

    return 0;
}