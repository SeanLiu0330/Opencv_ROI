#pragma once
#include<iostream>
#include<core/core.hpp>
#include<highgui/highgui.hpp>
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
#include<iostream>  
#include<fstream>  


using namespace std;
using namespace cv;

#define MAX_NUM 15

Rect select;
bool select_flag = false;
Mat img, showImg;
stack<Mat*> img_stack;//空栈
stack<Rect*> rect_stack;// 用于存储跟每个矩形对应的 Rect变量；
Point last_p1, last_p2; // 和 Mat 类型不一样，Mat不能使用等号进行赋值，否则的话等号两端共享存储空间，但是Point可以；

#define output_path  "E:\\keti_data\\position_data\\"  // 存储 位置数据文件
#define fig_output_path  "E:\\keti_data\\rect_fig\\"  // 存储裁剪好的图片
#define raw_fig_path  "E:\\keti_data\\decompose_fig\\"    // 原始图片路径
#define decompose_path "E:\\keti_data\\decompose_fig\\"
#define DECOMPOSE_FLAG 0

int obj2file(Rect* obj_reference, int rect_count, string file_name);
int file2obj(Rect* rec_arr,int* count);
void store_capture(Rect* rec_arr, int capture_num, string file_name);
int video_decompose();
