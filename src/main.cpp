/*
 * @Author: WJM
 * @Date: 2024-03-08 15:05:07
 * @LastEditors: WJM
 * @LastEditTime: 2024-03-08 18:03:29
 * @Description:
 * @FilePath: /rknn_yolov8_detector_v2/src/main.cpp
 * @custom_string: http://www.aiar.xjtu.edu.cn/
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <sys/time.h>
#include <model_inference.h>

int main(int argc, char **argv)
{
    char model_path[256] = "/home/rpdzkj/wjm/pinlingv2.3.1/pinlingv2.3/rknn_yolov8_detector/build/model/besti8.rknn";
    CModelInference modelInference(model_path, 1, 10, 0.45, 0.2);
    cv::VideoWriter writer;
    int codec = cv::VideoWriter::fourcc('M', 'P', 'E', 'G');         // 选择合适的编解码器
    writer.open("output.avi", codec, 30, cv::Size(1280, 720), true); // 假设我们想保存彩色视频
    cv::Mat frame;
    // 打开视频文件
    cv::VideoCapture cap("/home/rpdzkj/video/2_1.mp4");
    std::cout << "start inference" << std::endl;
    if (!cap.isOpened())
    {
        std::cout << "无法打开视频文件" << std::endl;
        return -1;
    }
    int frame_count = 0;

    while (cap.read(frame) && frame_count < 400)
    {
        if (frame.empty())
        {
            printf("input img empty, quit\n");
            continue;
        }
        modelInference.InferenceModel(frame);
        for (int i = 0; i < modelInference.detectResultGroup.count; i++)
        {
            printf("%d @ (%d %d %d %d) %.3f\n", modelInference.detectResultGroup.results[i].obj_id,
                   modelInference.detectResultGroup.results[i].x, modelInference.detectResultGroup.results[i].y,
                   modelInference.detectResultGroup.results[i].w, modelInference.detectResultGroup.results[i].h,
                   modelInference.detectResultGroup.results[i].prop);
            int x = modelInference.detectResultGroup.results[i].x;
            int y = modelInference.detectResultGroup.results[i].y;
            int w = modelInference.detectResultGroup.results[i].w;
            int h = modelInference.detectResultGroup.results[i].h;
            cv::rectangle(frame, cv::Rect(x, y, w, h), cv::Scalar(255, 255, 0), 2);
        }
        // cv::imwrite("img/"+std::to_string(frame_count)+".jpg",frameQueue.front());
        frame_count++;
        // std::cout << frame_count << std::endl;
        writer.write(frame);
    }
    writer.release();
    std::cout << "main exit" << std::endl;

    cap.release();
    std::cout << "main exit" << std::endl;
    return 0;
}