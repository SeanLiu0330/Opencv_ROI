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



using namespace std;
using namespace cv;
//using namespace System;
//using namespace std::IO;

#define MAX_NUM 15

Rect select;
bool select_flag = false;
Mat img, showImg;
stack<Mat*> img_stack;//��ջ
stack<Rect*> rect_stack;// ���ڴ洢��ÿ�����ζ�Ӧ�� Rect������
Point last_p1, last_p2; // �� Mat ���Ͳ�һ����Mat����ʹ�õȺŽ��и�ֵ������Ļ��Ⱥ����˹���洢�ռ䣬����Point���ԣ�

#define output_path  "E:\\keti_data\\position_data\\"  // �洢 λ�������ļ�
#define fig_output_path  "E:\\keti_data\\rect_fig\\"  // �洢�ü��õ�ͼƬ
#define raw_fig_path  "E:\\keti_data\\raw_fig\\"    // ԭʼͼƬ·��
int obj2file(string file_url, Rect* obj_reference, int rect_count);

//Ѱ�� ����ֵ ��С�� ��ֵ
int mini_abs(int a, int b) {
	return abs(a) <= abs(b) ? a : b;
}
void A_on_Mouse(int event, int x, int y, int flags, void*param)//ʵ�ֻ����ο�  
{
	Point p1, p2;
	if (event == EVENT_LBUTTONDOWN)
	{
		select.x = x;
		select.y = y;
		//select.
		select_flag = true;
	}
	else if (select_flag &&event == EVENT_MOUSEMOVE)
	{
		img.copyTo(showImg);
		p1 = Point(select.x, select.y);
		// ��ȡ �����ε� �߿�
		int x_minus = x - select.x;
		int y_minus = y - select.y;
		int mini_minus = mini_abs(x_minus, y_minus);
		//p2 = Point(x, y);
		p2 = Point(select.x + mini_minus, select.y + mini_minus);
		last_p1 = p1;
		last_p2 = p2;
		rectangle(showImg, p1, p2, Scalar(0, 255, 0), 2);
		imshow("img", showImg);

	}
	else if (select_flag && event == EVENT_LBUTTONUP)
	{
		//=========
		//��Ҫ����һ��ջ��ԭ����img�洢��ջ�У����ں���ɾ��������p1,p2�Ǿֲ����������ܴ��ݵ�buttonup
		rectangle(showImg, last_p1, last_p2, Scalar(255, 255, 0), 2);
		imshow("img", showImg);
		// === ��� ���� rect_stack�Ĳ�����
		Rect* temp_rect = new Rect;
		*temp_rect = Rect(last_p1, last_p2);
		rect_stack.push(temp_rect);
		// ==== 
		Mat* temp_img = new Mat;
		*temp_img = showImg.clone();
		img_stack.push(temp_img);
		showImg.copyTo(img);
		//===========
		select_flag = false;

	}
	else if (event == EVENT_RBUTTONDOWN)
	{
		if (img_stack.size() > 1) {
			delete img_stack.top();
			delete rect_stack.top();
			rect_stack.pop();
			img_stack.pop();
		}
		Mat* temp = img_stack.top();
		(*temp).copyTo(img);
		imshow("img", img);
	}
}
void B_on_Mouse(int event, int x, int y, int flags, void*param)//ʵ�ֻ����ο򲢽�ͼ  
{

	Point p1, p2;
	switch (event)
	{

	case  EVENT_LBUTTONDOWN:
	{
		select.x = x;
		select.y = y;
		select_flag = true;
	}
	break;
	case EVENT_MOUSEMOVE:
	{
		if (select_flag)
		{

			img.copyTo(showImg);
			p1 = Point(select.x, select.y);
			p2 = Point(x, y);
			rectangle(showImg, p1, p2, Scalar(0, 255, 0), 2);
			imshow("img", showImg);
		}
	}
	break;
	case EVENT_LBUTTONUP:
	{
		//��ʾ�����ROI  
									//�ĳ�Rect roi = Rect(p1, p2);Ϊʲô���ԣ�  
		Rect roi = Rect(Point(select.x, select.y), Point(x, y));
		if (roi.width && roi.height)//��һ��ʱ��û�з�Ӧ  
		{
			Mat roiImg = img(roi);
			imshow("roi", roiImg);
		}
		select_flag = false;

	}
	break;
	}
}

int main()
{	
	//get the file_name_list
	// get the file_url in a loop
	string file_list[1000]; // ����ǽ��� python��õ� �ļ��� Ŀ¼����һ���������ʽ�ṩ���������
	vector<string> file_vector = {"1.jpg","2.jpg","3.jpg"};
	for (vector<string>::iterator iter = file_vector.begin(); iter != file_vector.end(); iter++) {
		//Mat img, showImg;
		string current_file_name = *iter;
		string current_file_out_path = fig_output_path + current_file_name;
		string current_file_path = raw_fig_path + current_file_name;
		int loop_stop_flag = 0;
		img = imread(current_file_path, 1);
		Mat* raw_img = new Mat;
		*raw_img = img.clone();
		img_stack.push(raw_img);
		showImg = img.clone();
		//showImg = img_stack.top().clone();
		select.x = select.y = 0;
		imshow("img", showImg);

		while (1)
		{
			int key = waitKey(10);
			setMouseCallback("img", A_on_Mouse, 0);
			//switch (key)
			//{  // һ������ֻ���趨һ�� ��������𣿣� ����a����b��key��Ϊָ����ֵ��Ȼ��任 ���ڵ� ���������������key��һֱ����-1��ֱ����һ�ΰ�����
			//   // ���ԣ����������֮��ֻҪ����л�ͻ�һֱ���øú�����ֱ���Ƴ���
			//case 'a':
			//	setMouseCallback("img", A_on_Mouse, 0);
			//	break;
			//case 'b':
			//	setMouseCallback("img", B_on_Mouse, 0);
			//	break;
			//}
			if (key == ' ') {
				Rect rec_arr[MAX_NUM];
				int temp_count = 0;
				while (!rect_stack.empty()) {
					rec_arr[temp_count++] = *rect_stack.top();
					delete rect_stack.top();
					rect_stack.pop();
				}
				// ���� �洢�ļ�·�����ļ��������ã�
				string output_url = output_path + current_file_name;
				obj2file(output_url, rec_arr, temp_count);
				break;
			}
			if (key == 27 || key == 'q') {
				loop_stop_flag = 1;
				break;
			}				
		}
		if (loop_stop_flag) {
			break;
		}
	}
	//img = imread("C:\\Users\\sean\\Desktop\\test.jpg", 1);
	//Mat* raw_img = new Mat;
	//*raw_img = img.clone();
	//img_stack.push(raw_img);
	//showImg = img.clone();
	////showImg = img_stack.top().clone();
	//select.x = select.y = 0;
	//imshow("img", showImg);


	//while (1)
	//{
	//	int key = waitKey(10);
	//	switch (key)
	//	{  // һ������ֻ���趨һ�� ��������𣿣� ����a����b��key��Ϊָ����ֵ��Ȼ��任 ���ڵ� ���������������key��һֱ����-1��ֱ����һ�ΰ�����
	//	// ���ԣ����������֮��ֻҪ����л�ͻ�һֱ���øú�����ֱ���Ƴ���
	//	case 'a':
	//		setMouseCallback("img", A_on_Mouse, 0);
	//		break;
	//	case 'b':
	//		setMouseCallback("img", B_on_Mouse, 0);
	//		break;
	//	}
	//	if (key == ' ') {
	//		Rect rec_arr[MAX_NUM];
	//		int temp_count = 0;
	//		while (!rect_stack.empty()) {
	//			rec_arr[temp_count++] = *rect_stack.top();
	//			delete rect_stack.top();
	//			rect_stack.pop();
	//		}
	//		// ���� �洢�ļ�·�����ļ��������ã�
	//		string output_url = output_path + "test.txt";
	//		obj2file(output_url, rec_arr, temp_count);

	//	}
	//	if (key == 27 || key == 'q')
	//		break;
	//}
	waitKey(0);
	return 0;
}


int main2()
{
	//����Ƶ�ļ�����ʵ���ǽ���һ��VideoCapture�ṹ  
	VideoCapture capture("E:\\FFOutput\\VID_20171109_163650 - 2.avi");
	//����Ƿ�������:�ɹ���ʱ��isOpened����ture  
	if (!capture.isOpened())
		cout << "fail to open!" << endl;
	//��ȡ����֡��  
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "������Ƶ��" << totalFrameNumber << "֡" << endl;


	//���ÿ�ʼ֡()  
	long frameToStart = 300;
	capture.set(CV_CAP_PROP_POS_FRAMES, frameToStart);
	cout << "�ӵ�" << frameToStart << "֡��ʼ��" << endl;


	//���ý���֡  
	int frameToStop = 400;

	if (frameToStop < frameToStart)
	{
		cout << "����֡С�ڿ�ʼ֡��������󣬼����˳���" << endl;
		return -1;
	}
	else
	{
		cout << "����֡Ϊ����" << frameToStop << "֡" << endl;
	}


	//��ȡ֡��  
	double rate = capture.get(CV_CAP_PROP_FPS);
	cout << "֡��Ϊ:" << rate << endl;



	//����һ���������ƶ�ȡ��Ƶѭ�������ı���  
	bool stop = false;
	//����ÿһ֡��ͼ��  
	Mat frame;
	//��ʾÿһ֡�Ĵ���  
	namedWindow("Extracted frame");
	//��֡��ļ��ʱ��:  
	//int delay = 1000/rate;  
	int delay = 1000 / rate;


	//����whileѭ����ȡ֡  
	//currentFrame����ѭ�����п��ƶ�ȡ��ָ����֡��ѭ�������ı���  
	long currentFrame = frameToStart;


	//�˲����ĺ�  
	int kernel_size = 3;
	Mat kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size*kernel_size);

	while (!stop)
	{
		//��ȡ��һ֡  
		if (!capture.read(frame))
		{
			cout << "��ȡ��Ƶʧ��" << endl;
			return -1;
		}

		//������˲�����  
		// ���� ����һ�����⣺  capture.read ��ȡ�� ��Ƶ��ÿ֡���洢�� frame��������У�����˵����ָ������������ǲ��� release������ modify�ģ����ԣ������������ʹ���˲����ǣ�������
		imshow("Extracted frame", frame);
		filter2D(frame, frame, -1, kernel);

		imshow("after filter", frame);
		cout << "���ڶ�ȡ��" << currentFrame << "֡" << endl;
		//waitKey(int delay=0)��delay �� 0ʱ����Զ�ȴ�����delay>0ʱ��ȴ�delay����  
		//��ʱ�����ǰû�а�������ʱ������ֵΪ-1�����򷵻ذ���  


		int c = waitKey(delay);
		//����ESC���ߵ���ָ���Ľ���֡���˳���ȡ��Ƶ  
		if ((char)c == 27 || currentFrame > frameToStop)
		{
			stop = true;
		}
		//���°������ͣ���ڵ�ǰ֡���ȴ���һ�ΰ���  
		if (c >= 0)
		{
			waitKey(0);
		}
		currentFrame++;

	}
	//�ر���Ƶ�ļ�  
	capture.release();
	waitKey(0);
	return 0;
}
int main1()

{

	//����ͼƬ��ע��ͼƬ·��

	Mat image = imread("C:\\Users\\sean\\Desktop\\test.PNG");

	//ͼƬ����ɹ�����ж�

	if (!image.data)

	{

		cout << "you idiot��where did you hide lena��" << endl;

		//�ȴ�����

		system("pause");

		return -1;

	}

	//����һ������Ϊ��Lena����ͼ����ʾ���ڣ�������ǰ����Ҳ���ԣ�

	namedWindow("Lena", 1);

	//��ʾͼ��

	imshow("Lena", image);

	//�ȴ�����

	waitKey();

	return 0;

}


#include<iostream>  
#include<fstream>  
#include<string.h>  
int obj2file(string file_url, Rect* obj_reference, int rect_count) {
	// ����  c++ ���ݶ������ļ����д洢�ķ�����
	// ֱ�ӽ� ���ݶ����ָ�봫�ݸ� �������ָ����С֮��ֱ��д���ļ����ɣ�
	// ����Ĺ���ͬ��ֻ��Ҫ ���ݸ� ��ȡ����һ�� ����ָ�룬Ȼ��ָ���� ��Ҫ��ȡ�ı����Ĵ�С��
	// �������̾��൱�ڽ��ڴ��е� ����ԭԭ�������ƶ����� �ļ��У�
	//Rect select = Rect(Point(1, 1), Point(4, 4));
	ofstream fout;
	fout.open(file_url, ofstream::out | ofstream::trunc);
	fout.write((char *)&rect_count, sizeof(rect_count));
	fout.close();
	fout.open(file_url, ofstream::out | ofstream::app);
	for (int i = 0; i < rect_count; i++) {
		fout.write((char *)obj_reference + i, sizeof(*obj_reference));
	}
	fout.close();
	/*Rect select2;
	ifstream fin("C:\\Users\\sean\\Desktop\\output.txt");
	fin.read((char *)&select2, sizeof(select2));
	fin.close();
	cout << "select2 ��ȡ���" << endl;
	cout << "select2.x: "<< select2.x<< endl;
	cout << "select2.y: " << select2.y << endl;
	cout << "select2.width: " << select2.width << endl;
	cout << "select2.area: " << select2.area() << endl;*/
	return 0;
}
int file2obj() {
	// change the binary data to obj,and check whether it preicse or not;
	return 0;
}

void roi_storage(Rect* rect_array, string name_prefix, string storage_dir) {
	// store the ROI;
}
void getfiles(string path, vector<string>& files) {
	//�ļ����  
	long   hFile = 0;
	//�ļ���Ϣ������һ���洢�ļ���Ϣ�Ľṹ��  
	struct _finddata_t fileinfo;
	string p;//�ַ��������·��
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//�����ҳɹ��������
	{
		do
		{
			//�����Ŀ¼,����֮�����ļ����ڻ����ļ��У�  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				//�ļ���������"."&&�ļ���������".."
				//.��ʾ��ǰĿ¼
				//..��ʾ��ǰĿ¼�ĸ�Ŀ¼
				//�ж�ʱ�����߶�Ҫ���ԣ���Ȼ�����޵ݹ�������ȥ�ˣ�
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getfiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			//�������,�����б�  
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		//_findclose������������
		_findclose(hFile);
		return;
	}
}
void main5() {
	char* path = "E:\\test_dir_can_delete";
	vector<string> files;
	getfiles(path, files);

}