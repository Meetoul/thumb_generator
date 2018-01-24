#include <iostream>
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
#include <chrono>
#include <vector>
#include "ThreadPool.hpp"
#include "MResizer.hpp"

using namespace boost::filesystem;

const char *RESULT_DIR_NAME = "thumbs";

std::vector<char> read_raw(std::string &filename)
{
    std::ifstream ifs(filename, std::ios::binary);
    std::vector<char> result{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
    return result;
}

void decode_and_resize(std::vector<char> rawData, std::string dstName, int width, int height, int numberOfThreads)
{
    std::vector<char> dstRaw;
    resize(rawData, dstRaw, width, height, numberOfThreads);
    std::ofstream fout(dstName, std::ios::out | std::ios::binary);
    std::ostream_iterator<char> ofit(fout);
    std::copy(dstRaw.begin(), dstRaw.end(), ofit);
}

int main(int argc, char **argv)
{
    if (argc != 6)
    {
        std::cout << "Usage: ./ThumbGen [directory with .jpg images] [width] [height] "
                     "[number of workers] [number of resizing threads]"
                  << std::endl;
        return -1;
    }
    std::string sourceDirName = argv[1];
    int width = std::stoi(argv[2]);
    int height = std::stoi(argv[3]);
    int numOfWorkers = std::stoi(argv[4]);
    int numberOfThreads = std::stoi(argv[5]);
    path sourceDir(sourceDirName);
    path resultDir = sourceDir / RESULT_DIR_NAME;
    create_directory(resultDir);

    directory_iterator end_itr;

    auto begin = std::chrono::high_resolution_clock::now();
    {
        ThreadPool thread_pool(numOfWorkers);
        for (directory_iterator itr(sourceDir); itr != end_itr; ++itr)
        {
            if (extension(itr->path()) == ".jpg")
            {
                std::string src_name = itr->path().string();
                std::cout << "Processing: " << src_name << std::endl;
                path dstFilename = resultDir / itr->path().filename();
                std::vector<char> rawData = read_raw(src_name);
                thread_pool.add(decode_and_resize, rawData, dstFilename.string(), width, height, numberOfThreads);
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Generating time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
              << std::endl;

    return 0;
}