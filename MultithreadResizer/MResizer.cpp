#include <opencv2/opencv.hpp>
#include <thread>
#include <future>

void resize_wrapper(cv::Mat *src, cv::Mat *dst, int srcPH, int dstPH, int i)
{
    int src_y = i * srcPH;
    int dst_y = i * dstPH;
    cv::resize((*src)(cv::Rect{0, src_y, src->cols, srcPH}),
               (*dst)(cv::Rect{0, dst_y, dst->cols, dstPH}), cv::Size(dst->cols, dstPH));
}

void mt_resize(cv::Mat &src, cv::Mat &dst, int width, int height, int threadNum = 0)
{
    if (threadNum == 0)
        threadNum = std::thread::hardware_concurrency();
    int srcPartHeight = src.rows / threadNum;
    int dstPartHeight = height / threadNum;
    std::vector<std::future<void>> futures;
    dst = cv::Mat(cv::Size(width, height), CV_8UC3, cv::Scalar(0, 0, 0));
    for (int i = 0; i < threadNum; i++)
    {
        futures.emplace_back(std::async(resize_wrapper, &src, &dst, srcPartHeight, dstPartHeight, i));
    }
    for (auto &f : futures)
    {
        f.get();
    }
}

void resize(std::vector<char> &srcRaw, std::vector<char> &dstRaw, int width, int height, int numberOfThreads = 0)
{
    cv::Mat src = cv::imdecode(srcRaw, CV_LOAD_IMAGE_COLOR);
    cv::Mat dst;
    mt_resize(src, dst, width, height, numberOfThreads);
    std::vector<unsigned char> udstRaw;
    cv::imencode("*.jpg", dst, udstRaw);
    dstRaw = std::vector<char>(udstRaw.begin(), udstRaw.end());
}
