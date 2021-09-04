#include <functional>
#include <iostream>

#include <spdlog/spdlog.h>
#include <docopt/docopt.h>
#include <opencv2/opencv.hpp>
#include <cmath>

static constexpr auto USAGE =
  R"(mvg2

    Usage:
          naval_fate ship new <name>...
          naval_fate ship <name> move <x> <y> [--speed=<kn>]
          naval_fate ship shoot <x> <y>
          naval_fate mine (set|remove) <x> <y> [--moored | --drifting]
          naval_fate (-h | --help)
          naval_fate --version
 Options:
          -h --help     Show this screen.
          --version     Show version.
          --speed=<kn>  Speed in knots [default: 10].
          --moored      Moored (anchored) mine.
          --drifting    Drifting mine.
)";

static std::pair<cv::Mat,cv::Mat> edges_and_morphing(cv::Mat &frame)
{
  cv::Mat edges;
  cv::Mat morphed;
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size{3,3});
  cv::Canny(frame, edges, 100, 200);
  cv::copyMakeBorder(edges, edges, 1, 1, 1, 1, cv::BORDER_CONSTANT);
  cv::morphologyEx(edges, morphed, cv::MORPH_CLOSE, kernel);
  cv::Mat disp;
  cv::hconcat(edges, morphed, disp);
  cv::imshow("Win", disp);
  cv::waitKey();
  return std::make_pair(edges, morphed);
}

static void draw_edged_morphed_side_by_side(cv::Mat& edges, cv::Mat& morphed, int delay = 0)
{
  cv::Mat disp;
  cv::hconcat(edges, morphed, disp);
  cv::imshow("Win", disp);
  cv::waitKey();
}

int main(int argc, const char **argv)
{
  std::string filename1 = ASSET_DIR "/IMG_3045.mov";
  cv::VideoCapture cap{filename1};

  cv::Mat frame = cv::imread(ASSET_DIR "/checkerboard.png", cv::IMREAD_GRAYSCALE);
//  cap >> frame;
  cv::Mat H = cv::Mat::zeros(cv::Size{3,3}, CV_32F);
  cv::Mat Hs = H(cv::Rect{0, 0, 2, 2});
  cv::Mat Ht = H(cv::Rect{2,0,1,2});
  cv::Mat Hv = H(cv::Rect{0, 2, 3, 1});
  Hs = cv::Mat::eye(cv::Size{2,2}, CV_32F);
  Ht = cv::Mat::zeros(cv::Size{1,2}, CV_32F);

  Hs.at<float>(0,0) = cos(M_PI_4);
  Hs.at<float>(0,1) = -sin(M_PI_4);
  Hs.at<float>(1,0) = sin(M_PI_4);
  Hs.at<float>(1,1) = cos(M_PI_4);

  Ht.at<float>(0, 0) = static_cast<float>(frame.rows / 2);
  Ht.at<float>(1, 0) = -static_cast<float>(frame.cols / 2);

  Hv.at<float>(0, 0) = 0;
  Hv.at<float>(0,1) = 0;
  Hv.at<float>(0, 2) = 1;

  std::cout << H << std::endl;
  cv::Mat filtered;
  cv::Mat disp;
  cv::warpPerspective(frame, filtered, H, cv::Size{-1,-1});
  cv::hconcat(frame, filtered, disp);
  cv::imshow("win", filtered);
  cv::waitKey();
//  std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
//    { std::next(argv), std::next(argv, argc) },
//    true,// show help if requested
//    "Naval Fate 2.0");// version string
//
//  for (auto const &arg : args) {
//    std::cout << arg.first << "=" << arg.second << std::endl;
//  }
  (void)argc;
  (void)argv;
  (void)USAGE;

  //Use the default logger (stdout, multi-threaded, colored)
  spdlog::info("Hello, {}!", "World");
}
