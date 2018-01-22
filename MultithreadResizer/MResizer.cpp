#include <opencv2/opencv.hpp>
#include <thread>
#include <future>

void resize_wrapper(cv::Mat *src, cv::Mat *dst, int y, int h, double scaleFactor)
{
    int dst_y = y * scaleFactor;
    int dst_h = h * scaleFactor;
    cv::resize((*src)(cv::Rect{0, y, src->cols, h}), (*dst)(cv::Rect{0, dst_y, dst->cols, dst_h}), cv::Size(), scaleFactor, scaleFactor);
}

void mt_resize(cv::Mat &src, cv::Mat &dst, double scaleFactor, int threadNum = 0)
{
    if (threadNum == 0)
        threadNum = std::thread::hardware_concurrency();
    int partHeight = src.rows / threadNum;
    std::vector<std::future<void>> futures;
    dst = cv::Mat(cv::Size(src.cols * scaleFactor, src.rows * scaleFactor), CV_8UC3, cv::Scalar(0, 0, 0));
    for (int i = 0; i < threadNum; i++)
    {
        futures.emplace_back(std::async(resize_wrapper, &src, &dst, partHeight * i, partHeight, scaleFactor));
    }
    for (auto &f : futures)
    {
        f.get();
    }
}

void resize(std::vector<char> &srcRaw, std::vector<char> &dstRaw, double scaleFactor, int numberOfThreads = 0)
{
    cv::Mat src = cv::imdecode(srcRaw, CV_LOAD_IMAGE_COLOR);
    cv::Mat dst;
    mt_resize(src, dst, scaleFactor, numberOfThreads);
    std::vector<unsigned char> udstRaw;
    cv::imencode("*.jpg", dst, udstRaw);
    dstRaw = std::vector<char>(udstRaw.begin(), udstRaw.end());
}
