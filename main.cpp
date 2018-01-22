#include <iostream>
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
#include <chrono>
#include <vector>
#include "ThreadPool.hpp"
#include "MResizer.hpp"

using namespace boost::filesystem;

std::vector<char> read_raw(std::string &filename)
{
    std::ifstream ifs(filename, std::ios::binary);
    std::vector<char> result{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
    return result;
}

void decode_and_resize(std::vector<char> rawData, std::string dstName, double scaleFactor, int numberOfThreads)
{
    std::vector<char> dstRaw;
    resize(rawData, dstRaw, scaleFactor, numberOfThreads);
    std::ofstream fout(dstName, std::ios::out | std::ios::binary);
    std::ostream_iterator<char> ofit(fout);
    std::copy(dstRaw.begin(), dstRaw.end(), ofit);
}

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        std::cout << "Usage: ./ThumbGen [directory with .jpg images] [scale factor] "
                     "[number of workers] [number of resizing threads]"
                  << std::endl;
        return -1;
    }
    std::string folder = argv[1];
    std::string dst_path = folder + std::string("/thumbs/");
    double scaleFactor = std::stod(argv[2]);
    int numOfWorkers = std::stoi(argv[3]);
    int numberOfThreads = std::stoi(argv[4]);

    path p(folder.c_str());

    directory_iterator end_itr;

    auto begin = std::chrono::high_resolution_clock::now();
    {
        ThreadPool thread_pool(numOfWorkers);
        for (directory_iterator itr(p); itr != end_itr; ++itr)
        {
            if (extension(itr->path()) == ".jpg")
            {
                std::string src_name = itr->path().string();
                std::cout << "Processing: " << src_name << std::endl;
                std::string dstName = dst_path + itr->path().filename().string();
                std::vector<char> rawData = read_raw(src_name);
                thread_pool.add(decode_and_resize, rawData, dstName, scaleFactor, numberOfThreads);
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Generating time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
              << std::endl;

    return 0;
}