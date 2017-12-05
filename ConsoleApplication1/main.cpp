#include "main.h"
#include "file_name_array.h"
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
			Mat* temp = img_stack.top();
			(*temp).copyTo(img);
			imshow("img", img);
		}
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
		// ֻ��Ҫ���ȡ����� positionʱ���Ŷ�ȡ
		ifstream outer_position(IMAGE_POSITION_FILE_PATH);
		outer_position.read((char*)&last_final_position, sizeof(last_final_position));
		last_final_position -= 1;
	}
	// ���ж�ȡ �ļ��������
	while (!file_name_str_file.eof()) {
		int loop_stop_flag = 0;
		line_count++;
		vector<string> file_vector;
		getline(file_name_str_file, name_str_per_line);
		string::size_type final_comma_position = name_str_per_line.find_last_of(',');
		// ��ȡÿ�е� img�ļ�����vector;size==300
		while (!name_str_per_line.empty()) {
			string::size_type temp_position_4_comma = name_str_per_line.find_first_of(',');
			//string test = name_str_per_line.substr(final_comma_position+1);
			string single_file_name = name_str_per_line.substr(0, temp_position_4_comma);
			file_vector.push_back(single_file_name);
			name_str_per_line = name_str_per_line.substr(temp_position_4_comma + 1);			
		}	
		current_img_count = (line_count-1)*VECTOR_SIZE;
		//continue;
		string file_list[1000]; // ����ǽ��� python��õ� �ļ��� Ŀ¼����һ���������ʽ�ṩ���������	
		int start_point = 0;
		for (vector<string>::iterator iter = file_vector.begin(); iter != file_vector.end(); iter++) {
			// Mat img, showImg;
			// ��¼��ǰ����ͼƬ��λ��
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
					// ��rect_stack���һ��������ӦΪ���stack�ж����һ��Ԫ��
					delete img_stack.top();
					img_stack.pop();
					// ���� �洢�ļ�·�����ļ��������ã�
					if (temp_count != 0) {
						obj2file(rec_arr, temp_count, pure_name);
						store_capture(rec_arr, temp_count, pure_name);
						Rect test_array[15];
						//���ڼ�� �洢��position_data�Ƿ���Ч
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
	// ����  c++ ���ݶ������ļ����д洢�ķ�����
	// ֱ�ӽ� ���ݶ����ָ�봫�ݸ� �������ָ����С֮��ֱ��д���ļ����ɣ�
	// ����Ĺ���ͬ��ֻ��Ҫ ���ݸ� ��ȡ����һ�� ����ָ�룬Ȼ��ָ���� ��Ҫ��ȡ�ı����Ĵ�С��
	// �������̾��൱�ڽ��ڴ��е� ����ԭԭ�������ƶ����� �ļ��У�
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
	//����Ƶ�ļ�����ʵ���ǽ���һ��VideoCapture�ṹ  
	VideoCapture capture(VIDEO_PATH);
	//����Ƿ�������:�ɹ���ʱ��isOpened����ture  
	if (!capture.isOpened())
		cout << "fail to open!" << endl;
	//��ȡ����֡��  
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "������Ƶ��" << totalFrameNumber << "֡" << endl;
	//���ÿ�ʼ֡()  
	long frameToStart = FRAME_TO_START;
	capture.set(CV_CAP_PROP_POS_FRAMES, frameToStart);
	cout << "�ӵ�" << frameToStart << "֡��ʼ��" << endl;
	//���ý���֡  
	int frameToStop = totalFrameNumber;
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

	while (!stop)
	{
		string img_write_url = decompose_path + to_string(currentFrame) + ".jpg";
		//��ȡ��һ֡  
		if (!capture.read(frame))
		{
			cout << "��ȡ��Ƶʧ��" << endl;
			return -1;
		}
		//������˲�����  
		// ���� ����һ�����⣺  capture.read ��ȡ�� ��Ƶ��ÿ֡���洢�� frame��������У�����˵����ָ������������ǲ��� release������ modify�ģ����ԣ������������ʹ���˲����ǣ�������
		imwrite(img_write_url, frame);
		imshow("Extracted frame", frame);
		cout << "���ڶ�ȡ��" << currentFrame << "֡" << endl;
		//waitKey(int delay=0)��delay �� 0ʱ����Զ�ȴ�����delay>0ʱ��ȴ�delay����  
		//��ʱ�����ǰû�а�������ʱ������ֵΪ-1�����򷵻ذ���  
		int c = waitKey(5);
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