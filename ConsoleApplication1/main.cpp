#include "main.h"

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
		}
		Mat* temp = img_stack.top();
		(*temp).copyTo(img);
		imshow("img", img);
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
	//get the file_name_list
	// get the file_url in a loop
	string file_list[1000]; // 这个是借助 python获得的 文件名 目录，以一个数组的形式提供给这个函数
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
			//{  // 一个窗口只能设定一个 鼠标活动函数吗？？ 按下a或者b，key变为指定的值，然后变换 窗口的 鼠标活动函数，接下来key就一直保持-1；直到下一次按键；
			//   // 所以，绑定鼠标活动函数之后，只要鼠标有活动就会一直调用该函数，直到推出；
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
				// 关于 存储文件路径，文件名等设置；
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
	waitKey(0);
	return 0;
}

int obj2file(string file_url, Rect* obj_reference, int rect_count) {
	// 关于  c++ 数据对象用文件进行存储的方法：
	// 直接将 数据对象的指针传递给 输出流，指定大小之后，直接写入文件即可；
	// 读入的过程同样只需要 传递给 读取函数一个 变量指针，然后指定好 需要读取的变量的大小；
	// 整个过程就相当于将内存中的 数据原原本本的移动到了 文件中；
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
	cout << "select2 读取完毕" << endl;
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
	//文件句柄  
	long   hFile = 0;
	//文件信息，声明一个存储文件信息的结构体  
	struct _finddata_t fileinfo;
	string p;//字符串，存放路径
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//若查找成功，则进入
	{
		do
		{
			//如果是目录,迭代之（即文件夹内还有文件夹）  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				//文件名不等于"."&&文件名不等于".."
				//.表示当前目录
				//..表示当前目录的父目录
				//判断时，两者都要忽略，不然就无限递归跳不出去了！
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getfiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			//如果不是,加入列表  
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		//_findclose函数结束查找
		_findclose(hFile);
		return;
	}
}
void main5() {
	char* path = "E:\\test_dir_can_delete";
	vector<string> files;
	getfiles(path, files);

}

// tianjia yongyu ceshi de wenzi