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
Mat img, showImg;
stack<Mat*> img_stack;//��ջ
stack<Rect*> rect_stack;// ���ڴ洢��ÿ�����ζ�Ӧ�� Rect������
Point last_p1, last_p2; // �� Mat ���Ͳ�һ����Mat����ʹ�õȺŽ��и�ֵ������Ļ��Ⱥ����˹���洢�ռ䣬����Point���ԣ�

#define output_path  "E:\\keti_data\\position_data\\"  // �洢 λ�������ļ�
#define fig_output_path  "E:\\keti_data\\rect_fig\\"  // �洢�ü��õ�ͼƬ
#define raw_fig_path  "E:\\keti_data\\decompose_fig_2\\"    // ԭʼͼƬ·��
#define decompose_path "E:\\keti_data\\decompose_fig_2\\"
#define DECOMPOSE_FLAG 0  //���������� ִ�� ��Ƶת����֡�����Ƕ���ÿ֡�Ĵ���
#define VIDEO_PATH "E:\\FFOutput\\VID_20171109_16414_2.mp4"
#define file_name_str_path "E:\\keti_data\\file_name_str.txt"


int obj2file(Rect* obj_reference, int rect_count, string file_name);
int file2obj(Rect* rec_arr,int* count);
void store_capture(Rect* rec_arr, int capture_num, string file_name);
int video_decompose();
