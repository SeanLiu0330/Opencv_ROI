#pragma once
#include <iostream>
#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>  
#include <cmath>
#include <stack>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>  
#include <io.h>
#include <iostream>  
#include <fstream>  
#include <cstring>

using namespace std;
using namespace cv;

#define MAX_NUM 15

Rect select;
bool select_flag = false;
Mat img, showImg,img4roi;
stack<Mat*> img_stack;//空栈
stack<Rect*> rect_stack;// 用于存储跟每个矩形对应的 Rect变量；
Point last_p1, last_p2; // 和 Mat 类型不一样，Mat不能使用等号进行赋值，否则的话等号两端共享存储空间，但是Point可以；
const string date_string="1207_";

#define output_path  "E:\\keti_data\\position_data\\"  // 存储 位置数据文件
#define fig_output_path  "E:\\keti_data\\rect_fig_3\\"  // 存储裁剪好的图片
#define raw_fig_path  "E:\\keti_data\\decompose_fig_3\\"    // 原始图片路径
#define decompose_path "E:\\keti_data\\decompose_fig_3\\"  // decompose生成图片的位置
#define DECOMPOSE_FLAG 0  //用来控制是 执行 视频转换成帧，还是对于每帧的处理；
#define VIDEO_PATH "E:\\FFOutput\\2017-12-05-data\\VID_20171205_141956_1.mp4"
#define file_name_str_path "E:\\keti_data\\file_name_str.txt" //图像名称 文件 路径
#define VECTOR_SIZE 300  // python 处理图像名称字符串时，往文件中每行写入的 图像名称的数量；
#define IMAGE_POSITION_FILE_PATH  "E:\\keti_data\\img_position.txt"
#define READ_OUTER_POSITION false
#define MANUAL_SETTING_START_POSITION 46809
#define FRAME_TO_START 59546    //decompose中开始读取的位置
#define DATE_PREFIX  "12_05"


int obj2file(Rect* obj_reference, int rect_count, string file_name);
int file2obj(Rect* rec_arr,int* count);
void store_capture(Rect* rec_arr, int capture_num, string file_name);
int video_decompose();
