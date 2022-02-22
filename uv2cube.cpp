// uv2cube.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include "kb_cv_uv2cube.h"
#include "kb_searchfiles.h"
#include "kb_split_string.h"


int main2(int argc, char* argv[])
{
	std::string path1 = argv[1];
	std::string path2 = argv[2];
	std::string path3 = argv[3];
	int face = atoi(argv[4]);
	std::string path1_out = argv[5];
	std::string path2_out = argv[6];


	cv::Mat mat1 = cv::imread(path1);
	cv::Mat mat2 = cv::imread(path2);
	cv::Mat mat3 = cv::imread(path3);



	cv::Size sz1 = mat1.size();
	cv::Size sz2 = mat2.size();
	std::cout << sz1 << std::endl;
	std::cout << sz2 << std::endl;

	int width = sz2.width;

	kb::CUv2Cube uv2cube(2);
	cv::Vec3b bk(0, 0, 0);

	uv2cube.create(sz1, width);
	uv2cube.run_lut_inv(mat1, mat2, face, bk);

	cv::Mat mat3a;
	cv::dilate(mat3, mat3a, cv::Mat(), cv::Point(-1, -1), 2);

	cv::Mat mat1g;
	cv::Mat mat1gg;
	cv::cvtColor(mat1, mat1g, cv::COLOR_BGR2GRAY);
	cv::cvtColor(mat1g, mat1gg, cv::COLOR_GRAY2BGR);

	cv::Vec3b red(0, 0, 255);
	uv2cube.run_lut_inv(mat1gg, mat3a, face, red);
	

	cv::imwrite(path1_out, mat1);
	cv::imwrite(path2_out, mat1gg);

	return 0;
}

int main1(int argc, char* argv[])
{
	std::string path1 = argv[1];
	int mode = atoi(argv[2]);

	cv::Mat mat1 = cv::imread(path1);

	cv::Size sz1 = mat1.size();
	std::cout << sz1 << std::endl;

	int width = 512;

	kb::CUv2Cube uv2cube(mode);

	uv2cube.create(sz1, width);

	for (int k = 0; k < 6; k++) {
		cv::Mat mat2;
		uv2cube.run_lut(mat1, mat2, k);
		cv::namedWindow("cube", cv::WINDOW_NORMAL);
		cv::imshow("cube", mat2);
		int rtn = cv::waitKeyEx(0);
	}


	return 0;
}

int main0(int argc, char* argv[])
{
	std::string path1 = argv[1];
	int face = atoi(argv[2]);
	cv::VideoCapture cap(path1);

	cv::Mat mat1;
	cap >> mat1;

	cv::Size sz1 = mat1.size();
	int width = 640;

	kb::CUv2Cube uv2cube(face);

	uv2cube.create(sz1, width);

	while (1) {
		cap >> mat1;
		cv::Size sz1a = mat1.size();
		if (sz1a != sz1)
			break;


		cv::Mat mat2;
		uv2cube.run_lut(mat1, mat2, 1);

		cv::namedWindow("cube", cv::WINDOW_NORMAL);
		cv::imshow("cube", mat2);
		int rtn = cv::waitKeyEx(0);
	}

	return 0;
}

int main_uv2cube(int argc, char* argv[])
{
	std::string dname_input1 = argv[1];
	std::string skey1 = argv[2];
	int width = atoi(argv[3]);
	std::string dname_output = argv[4];
	int idx = atoi(argv[5]);
	if (idx >= 6) {
		idx = -1;
	}
	int face = atoi(argv[6]);

	std::vector<std::string> vfname1;
	kb::search_files(dname_input1, skey1.c_str(), vfname1);
	int num_files = vfname1.size();
	std::cout << "number of files = " << num_files << std::endl;
	if (num_files <= 0)
		return -1;

	cv::Size sz1;
	{
		std::string path1 = dname_input1 + vfname1[0];
		cv::Mat mat1 = cv::imread(path1);
		sz1 = mat1.size();
	}
	kb::CUv2Cube uv2cube(face);
	uv2cube.create(sz1, width);

	for (int kk = 0; kk < 6; kk++) {
		char buf[64];
		snprintf(buf, 64, "_%d.txt", kk);
		std::string path2 = dname_output + buf;
		kb::save_mat(path2, uv2cube.cube_matR[kk], 3, 3);
	}

	for (int k = 0; k < num_files; k++) {
		std::string path1 = dname_input1 + vfname1[k];
		cv::Mat mat1 = cv::imread(path1);

		std::string ftitle1, fext1;
		kb::split_filename(vfname1[k], ftitle1, fext1);


		for (int kk = 0; kk < 6; kk++) {
			if (idx >= 0) {
				if (kk != idx)
					continue;
			}
			cv::Mat mat2;
			uv2cube.run_lut(mat1, mat2, kk);

			//cv::namedWindow("cube", cv::WINDOW_NORMAL);
			//cv::imshow("cube", mat2);
			//int rtn = cv::waitKeyEx(0);
			char buf[64];
			snprintf(buf, 64, "_%d.jpg", kk);
			std::string path2 = dname_output + ftitle1 + buf;
			cv::imwrite(path2, mat2);
		}
	}

	return 0;
	//return main2(argc, argv);
}

int main(int argc, char* argv[])
{
	return main_uv2cube(argc, argv);	
}

