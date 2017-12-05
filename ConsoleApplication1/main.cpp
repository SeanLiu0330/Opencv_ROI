#include "main.h"
#include "file_name_array.h"
//寻找 绝对值 最小的 数值
int mini_abs(int a, int b) {
	return abs(a) <= abs(b) ? a : b;
}
void A_on_Mouse(int event, int x, int y, int flags, void*param)//实现画矩形框  
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
		// 获取 正方形的 边框
		x > img.cols ? x = img.cols : x;
		y > img.rows ? y = img.rows : y;
		x < 0 ? x = 0 : x;
		y < 0 ? y = 0 : y;
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
		//需要定义一个栈，原来的img存储到栈中，用于后面删除操作；p1,p2是局部变量，不能传递到buttonup
		rectangle(showImg, last_p1, last_p2, Scalar(255, 255, 0), 2);
		imshow("img", showImg);
		// === 添加 对于 rect_stack的操作；
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
			Mat* temp = img_stack.top();
			(*temp).copyTo(img);
			imshow("img", img);
		}
	}
}
void B_on_Mouse(int event, int x, int y, int flags, void*param)//实现画矩形框并截图  
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
		//显示框出的ROI  
									//改成Rect roi = Rect(p1, p2);为什么不对？  
		Rect roi = Rect(Point(select.x, select.y), Point(x, y));
		if (roi.width && roi.height)//点一下时会没有反应  
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
	if (DECOMPOSE_FLAG) {
		video_decompose();
		return 2;
	}
	//get the file_name_list
	// get the file_url in a loop
	ifstream file_name_str_file(file_name_str_path);
	string name_str_per_line;
	int line_count = 0;
	int current_img_count = 0;
	int last_final_position = MANUAL_SETTING_START_POSITION;
	if (READ_OUTER_POSITION) {
		// 只有要求读取外面的 position时，才读取
		ifstream outer_position(IMAGE_POSITION_FILE_PATH);
		outer_position.read((char*)&last_final_position, sizeof(last_final_position));
		last_final_position -= 1;
	}
	// 逐行读取 文件名，最好
	while (!file_name_str_file.eof()) {
		int loop_stop_flag = 0;
		line_count++;
		vector<string> file_vector;
		getline(file_name_str_file, name_str_per_line);
		string::size_type final_comma_position = name_str_per_line.find_last_of(',');
		// 获取每行的 img文件名的vector;size==300
		while (!name_str_per_line.empty()) {
			string::size_type temp_position_4_comma = name_str_per_line.find_first_of(',');
			//string test = name_str_per_line.substr(final_comma_position+1);
			string single_file_name = name_str_per_line.substr(0, temp_position_4_comma);
			file_vector.push_back(single_file_name);
			name_str_per_line = name_str_per_line.substr(temp_position_4_comma + 1);			
		}	
		current_img_count = (line_count-1)*VECTOR_SIZE;
		//continue;
		string file_list[1000]; // 这个是借助 python获得的 文件名 目录，以一个数组的形式提供给这个函数	
		int start_point = 0;
		for (vector<string>::iterator iter = file_vector.begin(); iter != file_vector.end(); iter++) {
			// Mat img, showImg;
			// 记录当前处理图片的位置
			current_img_count++;			
			if (current_img_count <= last_final_position) {
				continue;
			}
			cout << "current_img: " << current_img_count << endl;
			ofstream img_position_recorder(IMAGE_POSITION_FILE_PATH);
			img_position_recorder.write((char*)&current_img_count,sizeof(current_img_count));
			img_position_recorder.close();
			string current_file_name = *iter;
			string::size_type b = current_file_name.find_first_of('"');
			string::size_type e = current_file_name.find_last_of('"');
			string pure_name = current_file_name.substr(b+1, e-b-1);
			//string test2 = current_file_name.substr(2,6);
			string current_file_path = raw_fig_path + pure_name;			
			img = imread(current_file_path, 1);
			img.copyTo(img4roi);
			Mat* raw_img = new Mat;
			*raw_img = img.clone();
			img_stack.push(raw_img);
			showImg = img.clone();
			select.x = select.y = 0;
			imshow("img", showImg);

			while (1)
			{
				int key = waitKey(10);
				setMouseCallback("img", A_on_Mouse, 0);
				if (key == ' ') {
					Rect rec_arr[MAX_NUM];
					int temp_count = 0;
					while (!rect_stack.empty()) {
						rec_arr[temp_count++] = *rect_stack.top();
						delete rect_stack.top();
						delete img_stack.top();
						rect_stack.pop();
						img_stack.pop();
					}
					// 比rect_stack多出一个操作；应为这个stack中多出来一个元素
					delete img_stack.top();
					img_stack.pop();
					// 关于 存储文件路径，文件名等设置；
					if (temp_count != 0) {
						obj2file(rec_arr, temp_count, pure_name);
						store_capture(rec_arr, temp_count, pure_name);
						Rect test_array[15];
						//用于检查 存储的position_data是否有效
						/*int test_count = 0;
						int* test_count_p = &test_count;
						file2obj(test_array, test_count_p);
						store_capture(test_array, *test_count_p, "test_file_name");*/
					}
					break;
				}
				if (key == 27 || key == 'q') {
					loop_stop_flag = 1;
					break;
				}
				if (key == 'f'&& iter <= file_vector.end() - 5) {
					while (!img_stack.empty()) {
						delete img_stack.top();
						img_stack.pop();
					}
					while (!rect_stack.empty()) {
						delete rect_stack.top();
						rect_stack.pop();
					}
					iter += 4;
					current_img_count += 4;
					break;
				}
				if (key == 'b'&& iter>= file_vector.begin()+2) {
					while (!img_stack.empty()) {
						delete img_stack.top();
						img_stack.pop();
					}
					while (!rect_stack.empty()) {
						delete rect_stack.top();
						rect_stack.pop();
					}					
					iter -= 2;
					current_img_count -= 2;
					break;
				}
			}
			if (loop_stop_flag)
				break;			
		}
		if (loop_stop_flag)
			break;
	}
	file_name_str_file.close();
	waitKey(0);
	return 0;
}

void store_capture(Rect* rec_arr, int capture_num, string file_name) {
	char num2char[MAX_NUM] = { '0','1','2','3','4','5','6','7','8','9','10','11','12','13','14' };
	size_t suffix_position = file_name.find_last_of('.');
	string file_name_without_suffix = file_name.substr(0, suffix_position);
	cout << "file_name_without_suffix: " << file_name_without_suffix << endl;
	for (int i = 0; i < capture_num; i++) {
		string current_file_out_path = fig_output_path + file_name_without_suffix + '_' + num2char[i] + ".jpg";
		cout << "  file_name_for_roi_of_this_pic: " << current_file_out_path << endl;
		Rect roi = rec_arr[i];
		if (roi.width && roi.height) {
			Mat roi_img = img4roi(roi);
			imwrite(current_file_out_path, roi_img);
		}
	}
}
int obj2file(Rect* obj_reference, int rect_count, string file_name) {
	// 关于  c++ 数据对象用文件进行存储的方法：
	// 直接将 数据对象的指针传递给 输出流，指定大小之后，直接写入文件即可；
	// 读入的过程同样只需要 传递给 读取函数一个 变量指针，然后指定好 需要读取的变量的大小；
	// 整个过程就相当于将内存中的 数据原原本本的移动到了 文件中；
	//Rect select = Rect(Point(1, 1), Point(4, 4));	
	size_t suffix_position = file_name.find_last_of('.');
	string file_name_without_suffix = file_name.substr(0, suffix_position) + ".dat";
	string file_url = output_path + file_name_without_suffix;

	ofstream fout;
	fout.open(file_url, ofstream::out | ofstream::trunc);
	fout.write((char *)&rect_count, sizeof(rect_count));
	fout.close();
	fout.open(file_url, ofstream::out | ofstream::app);
	for (int i = 0; i < rect_count; i++) {
		fout.write((char *)(obj_reference + i), sizeof(*obj_reference));
	}
	fout.close();
	return 0;
}
int file2obj(Rect* rec_arr, int* count) {
	// change the binary data to obj,and check whether it preicse or not;
	//string position_dir = output_path
	Rect select2;
	int roi_num;
	ifstream fin("E:\\keti_data\\position_data\\1.dat");
	fin.read((char *)&roi_num, sizeof(roi_num));
	*count = roi_num;
	for (int i = 0; i < roi_num; i++) {
		fin.read((char *)(rec_arr + i), sizeof(select2));
	}
	//fin.read((char *)rec_arr, sizeof(select2)*roi_num);
	fin.close();
	return 0;
}
void roi_storage(Rect* rect_array, string name_prefix, string storage_dir) {
	// store the ROI;
}



int video_decompose() {
	//打开视频文件：其实就是建立一个VideoCapture结构  
	VideoCapture capture(VIDEO_PATH);
	//检测是否正常打开:成功打开时，isOpened返回ture  
	if (!capture.isOpened())
		cout << "fail to open!" << endl;
	//获取整个帧数  
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "整个视频共" << totalFrameNumber << "帧" << endl;
	//设置开始帧()  
	long frameToStart = FRAME_TO_START;
	capture.set(CV_CAP_PROP_POS_FRAMES, frameToStart);
	cout << "从第" << frameToStart << "帧开始读" << endl;
	//设置结束帧  
	int frameToStop = totalFrameNumber;
	if (frameToStop < frameToStart)
	{
		cout << "结束帧小于开始帧，程序错误，即将退出！" << endl;
		return -1;
	}
	else
	{
		cout << "结束帧为：第" << frameToStop << "帧" << endl;
	}
	//获取帧率  
	double rate = capture.get(CV_CAP_PROP_FPS);
	cout << "帧率为:" << rate << endl;
	//定义一个用来控制读取视频循环结束的变量  
	bool stop = false;
	//承载每一帧的图像  
	Mat frame;
	//显示每一帧的窗口  
	namedWindow("Extracted frame");
	//两帧间的间隔时间:  
	//int delay = 1000/rate;  
	int delay = 1000 / rate;
	//利用while循环读取帧  
	//currentFrame是在循环体中控制读取到指定的帧后循环结束的变量  
	long currentFrame = frameToStart;

	while (!stop)
	{
		string img_write_url = decompose_path + to_string(currentFrame) + ".jpg";
		//读取下一帧  
		if (!capture.read(frame))
		{
			cout << "读取视频失败" << endl;
			return -1;
		}
		//这里加滤波程序  
		// 这里 存在一个问题：  capture.read 获取的 视频的每帧都存储在 frame这个变量中；函数说明中指出，这个变量是不能 release或者是 modify的；所以？？？这里对其使用滤波器是？？？？
		imwrite(img_write_url, frame);
		imshow("Extracted frame", frame);
		cout << "正在读取第" << currentFrame << "帧" << endl;
		//waitKey(int delay=0)当delay ≤ 0时会永远等待；当delay>0时会等待delay毫秒  
		//当时间结束前没有按键按下时，返回值为-1；否则返回按键  
		int c = waitKey(5);
		//按下ESC或者到达指定的结束帧后退出读取视频  
		if ((char)c == 27 || currentFrame > frameToStop)
		{
			stop = true;
		}
		//按下按键后会停留在当前帧，等待下一次按键  
		if (c >= 0)
		{
			waitKey(0);
		}
		currentFrame++;

	}
	//关闭视频文件  
	capture.release();
	waitKey(0);
	return 0;
}