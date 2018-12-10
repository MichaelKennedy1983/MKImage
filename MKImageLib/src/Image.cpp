#include "Image.h"

#include "Globals.h"

#include <chrono>
#include <iostream>
#include <thread>

namespace MKI
{
    Image::Image(ImagePixels&& pixels, size_t rows, size_t columns, uint8_t depth)
        : m_pixels(pixels),
          m_rows(rows),
          m_columns(columns),
          m_depth(depth)
    {

    }

    Image Image::resize(size_t new_height, size_t new_width,
                        ResizeAlgorithm algorithm) const
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        std::cout << "\nResizing with " << 
            Interpolation::ScalingFunct::algorithm_to_string.at(algorithm) <<
            ".\n";

        Image out_image(ImagePixels(new_height, std::vector<uint8_t>(new_width)),
                        new_height,
                        new_width,
                        this->m_depth);

        size_t num_threads = std::thread::hardware_concurrency();

        if (num_threads > 4) {
            num_threads -= 0;
        }
        Globals::threads_used = num_threads;

        std::vector<ImagePixels::iterator> iterators(num_threads + 1);
        iterators.front() = out_image.m_pixels.begin();
        iterators.back() = out_image.m_pixels.end();
        double thread_ratio = 1.0 / num_threads;
        size_t iterator_size = thread_ratio * std::distance(out_image.m_pixels.begin(),
                                                            out_image.m_pixels.end());
        for (int i = 1; i < num_threads; ++i) {
            iterators.at(i) = iterators.at(i - 1) + iterator_size;
        }

        std::vector<Interpolation::ScalingFunct> scaling_functs;
        scaling_functs.reserve(num_threads);
        for (int i = 0; i < num_threads; ++i) {
            scaling_functs.emplace_back(this,
                                        &out_image,
                                        iterators.at(i),
                                        iterators.at(i + 1),
                                        algorithm);
        }

        double ratio_height = m_rows / static_cast<double>(new_height);
        double ratio_width = m_columns / static_cast<double>(new_width);

        std::vector<std::thread> threads;
        threads.reserve(num_threads);
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(scaling_functs.at(i), ratio_height, ratio_width);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        std::chrono::duration<double> duration = 
            std::chrono::high_resolution_clock::now() - start_time;
        std::cout << "Resize completed in " << duration.count() << " seconds.\n";

        return out_image;
    }

    Image Image::singleResize(size_t new_height, size_t new_width,
                              ResizeAlgorithm algorithm) const
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        std::cout << "\nResizing with " << 
            Interpolation::ScalingFunct::algorithm_to_string.at(algorithm) <<
            ".\n";

        Image out_image(ImagePixels(new_height, std::vector<uint8_t>(new_width)),
                        new_height,
                        new_width,
                        this->m_depth);

        Interpolation::ScalingFunct scaling_funct(this,
                                                  &out_image,
                                                  out_image.m_pixels.begin(),
                                                  out_image.m_pixels.end(),
                                                  algorithm);

        double ratio_height = m_rows / static_cast<double>(new_height);
        double ratio_width = m_columns / static_cast<double>(new_width);

        scaling_funct(ratio_height, ratio_width);

        std::chrono::duration<double> duration = 
            std::chrono::high_resolution_clock::now() - start_time;
        std::cout << "Resize completed in " << duration.count() << " seconds.\n";

        return out_image;
    }
}