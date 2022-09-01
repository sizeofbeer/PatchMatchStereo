/* -*-c++-*- PatchMatchStereo - Copyright (C) 2020.
* Author	: Yingsong Li(Ethan Li) <ethan.li.whu@gmail.com>
*			  https://github.com/ethan-li-coding
* Describe	: main
*/

#include "stdafx.h"
#include "PatchMatchStereo.h"
#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std::chrono;


// 显示视差图
void ShowDisparityMap(const float32* disp_map,
					  const sint32& width, const sint32& height, const std::string& name);
// 保存视差图
void SaveDisparityMap(const float32* disp_map,
					  const sint32& width, const sint32& height, const std::string& path);
// 保存视差点云, 需要根据相机参数设置相关参数值
void SavePointCloud(const uint8* img_bytes,
					const float32* disp_map, const sint32& width, const sint32& height, const std::string& path);


/**
 * @brief
 * @param argv 3
 * @param argc argc[1]: 左图像路径; argc[2]: 右图像路径; argc[3]: 最小视差[可选，默认0]; argc[4]: 最大视差[可选，默认64]
 * @param eg. ../Data/Cone/im2.png ../Data/Cone/im6.png 0 64
 * @param eg. ../Data/Reindeer/view1.png ../Data/Reindeer/view5.png 0 128
 * @return
 */
int main(int argv, char** argc)
{
	if (argv < 3) {
		std::cout << "参数过少, 请至少指定左右图像路径!" << std::endl;
		return -1;
	}

	printf("Image Loading...");
	// 读取图像
	std::string path_left = argc[1];
	std::string path_right = argc[2];

	cv::Mat img_left = cv::imread(path_left, cv::IMREAD_COLOR);
	cv::Mat img_right = cv::imread(path_right, cv::IMREAD_COLOR);

	if (img_left.data == nullptr || img_right.data == nullptr) {
		std::cout << "读取图像失败!" << std::endl;
		return -1;
	}
	if (img_left.rows != img_right.rows || img_left.cols != img_right.cols) {
		std::cout << "左右图像尺寸不一致!" << std::endl;
		return -1;
	}

	const sint32 width = static_cast<uint32>(img_left.cols);
	const sint32 height = static_cast<uint32>(img_right.rows);

	// 左右图像的彩色数据
	auto bytes_left = new uint8[width * height * 3];
	auto bytes_right = new uint8[width * height * 3];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			bytes_left[i * 3 * width + 3 * j] = img_left.at<cv::Vec3b>(i, j)[0];
			bytes_left[i * 3 * width + 3 * j + 1] = img_left.at<cv::Vec3b>(i, j)[1];
			bytes_left[i * 3 * width + 3 * j + 2] = img_left.at<cv::Vec3b>(i, j)[2];
			bytes_right[i * 3 * width + 3 * j] = img_right.at<cv::Vec3b>(i, j)[0];
			bytes_right[i * 3 * width + 3 * j + 1] = img_right.at<cv::Vec3b>(i, j)[1];
			bytes_right[i * 3 * width + 3 * j + 2] = img_right.at<cv::Vec3b>(i, j)[2];
		}
	}
	printf("Done!\n");

	PMSOption pms_option; // PMS匹配参数
	pms_option.patch_size = 35; // patch大小

	// 候选视差范围
	pms_option.min_disparity = argv < 4 ? 0 : atoi(argc[3]);
	pms_option.max_disparity = argv < 5 ? 64 : atoi(argc[4]);
	
	pms_option.gamma = 10.0f; // gamma
	pms_option.alpha = 0.9f; // alpha
	pms_option.tau_col = 10.0f; // t_col
	pms_option.tau_grad = 2.0f; // t_grad

	pms_option.num_iters = 3; // 传播迭代次数

	// 一致性检查
	pms_option.is_check_lr = true;
	pms_option.lrcheck_thres = 1.0f;
	// 视差图填充
	pms_option.is_fill_holes = false;
	// 前端平行窗口
	pms_option.is_fource_fpw = false;
	// 整数视差精度
	pms_option.is_integer_disp = false;

	// 定义PMS匹配类实例
	PatchMatchStereo pms;

	printf("PatchMatch Initializing...");
	auto start = std::chrono::steady_clock::now();

	// 初始化
	if (!pms.Initialize(width, height, pms_option)) {
		std::cout << "PMS初始化失败!" << std::endl;
		return -2;
	}
	auto end = std::chrono::steady_clock::now();
	auto tt = duration_cast<std::chrono::milliseconds>(end - start);
	printf("Done! Timing : %lf s\n", tt.count() / 1000.0);

	printf("PatchMatch Matching...");
	start = std::chrono::steady_clock::now();
	// 匹配, disparity数组保存子像素的视差结果
	auto disparity = new float32[uint32(width * height)]();		
	if (!pms.Match(bytes_left, bytes_right, disparity)) {
		std::cout << "PMS匹配失败!" << std::endl;
		return -2;
	}
	end = std::chrono::steady_clock::now();
	tt = duration_cast<std::chrono::milliseconds>(end - start);
	printf("Done! Timing : %lf s\n", tt.count() / 1000.0);

#if 0
	// 显示梯度图
	cv::Mat grad_left_x = cv::Mat(height, width, CV_8UC1);
	auto* grad_map = pms.GetGradientMap(0);
	if (grad_map) {
		for (sint32 i = 0; i < height; i++) {
			for (sint32 j = 0; j < width; j++) {
				const auto grad = grad_map[i * width + j].x;
				grad_left.data[i * width + j] = grad;
			}
		}
	}
	cv::imshow("梯度图-左X", grad_left);
	cv::Mat grad_left_y = cv::Mat(height, width, CV_8UC1);
	auto* grad_map = pms.GetGradientMap(0);
	if (grad_map) {
		for (sint32 i = 0; i < height; i++) {
			for (sint32 j = 0; j < width; j++) {
				const auto grad = grad_map[i * width + j].y;
				grad_left.data[i * width + j] = grad;
			}
		}
	}
	cv::imshow("梯度图-左Y", grad_left);
#endif

	// 显示视差图
	// ShowDisparityMap(pms.GetDisparityMap(0), width, height, "disp-left");
	// ShowDisparityMap(pms.GetDisparityMap(1), width, height, "disp-right");
	// cv::waitKey(0);
	// 保存视差图
	SaveDisparityMap(pms.GetDisparityMap(0), width, height, path_left);
	SaveDisparityMap(pms.GetDisparityMap(1), width, height, path_right);
	// 保存点云
	// SavePointCloud(bytes_left, pms.GetDisparityMap(0), width, height, path_left);
	// SavePointCloud(bytes_right, pms.GetDisparityMap(1), width, height, path_left);

	// 释放内存
	delete[] disparity;
	disparity = nullptr;
	delete[] bytes_left;
	bytes_left = nullptr;
	delete[] bytes_right;
	bytes_right = nullptr;

	// system("pause");
	return 0;
}

void ShowDisparityMap(const float32* disp_map,
					  const sint32& width, const sint32& height, const std::string& name)
{
	const cv::Mat disp_mat = cv::Mat(height, width, CV_8UC1);
	float32 min_disp = float32(width), max_disp = -float32(width);
	for (sint32 i = 0; i < height; i++) {
		for (sint32 j = 0; j < width; j++) {
			const float32 disp = abs(disp_map[i * width + j]);
			if (disp != Invalid_Float) {
				min_disp = std::min(min_disp, disp);
				max_disp = std::max(max_disp, disp);
			}
		}
	}
	for (sint32 i = 0; i < height; i++) {
		for (sint32 j = 0; j < width; j++) {
			const float32 disp = abs(disp_map[i * width + j]);
			if (disp == Invalid_Float) {
				disp_mat.data[i * width + j] = 0;
			}
			else {
				disp_mat.data[i * width + j] = static_cast<uchar>((disp - min_disp) / (max_disp - min_disp) * 255);
			}
		}
	}

	cv::imshow(name, disp_mat);
	cv::Mat disp_color;
	applyColorMap(disp_mat, disp_color, cv::COLORMAP_JET);
	cv::imshow(name + "-color", disp_color);

}

void SaveDisparityMap(const float32* disp_map,
					  const sint32& width, const sint32& height, const std::string& path)
{
	const cv::Mat disp_mat = cv::Mat(height, width, CV_8UC1);
	float32 min_disp = float32(width), max_disp = -float32(width);
	for (sint32 i = 0; i < height; i++) {
		for (sint32 j = 0; j < width; j++) {
			const float32 disp = abs(disp_map[i * width + j]);
			if (disp != Invalid_Float) {
				min_disp = std::min(min_disp, disp);
				max_disp = std::max(max_disp, disp);
			}
		}
	}
	for (sint32 i = 0; i < height; i++) {
		for (sint32 j = 0; j < width; j++) {
			const float32 disp = abs(disp_map[i * width + j]);
			if (disp == Invalid_Float) {
				disp_mat.data[i * width + j] = 0;
			}
			else {
				disp_mat.data[i * width + j] = static_cast<uchar>((disp - min_disp) / (max_disp - min_disp) * 255);
			}
		}
	}

	cv::imwrite(path + "-d.png", disp_mat);
	cv::Mat disp_color;
	applyColorMap(disp_mat, disp_color, cv::COLORMAP_JET);
	cv::imwrite(path + "-c.png", disp_color);
}

void SavePointCloud(const uint8* img_bytes, const float32* disp_map,
					const sint32& width, const sint32& height, const std::string& path)
{
	float32 B = 193.001;		// 基线
	float32 f = 999.421;		// 焦距
	float32 x0l = 294.182;		// 左视图像主点x0
	float32 y0l = 252.932;		// 左视图像主点y0
	float32 x0r = 326.95975;	// 右视图像主点x0

	// 保存点云
	FILE* fp_disp_cloud = nullptr;
	fp_disp_cloud = fopen((path + "-cloud.txt").c_str(), "w");
	if (!fp_disp_cloud) return;

	for (sint32 y = 0; y < height; y++) {
		for (sint32 x = 0; x < width; x++) {
			const float32 disp = abs(disp_map[y * width + x]);
			if (disp == Invalid_Float) continue;

			float32 Z = B * f / (disp + (x0r - x0l));
			float32 X = Z * (x - x0l) / f;
			float32 Y = Z * (y - y0l) / f;
			// X Y Z R G B
			fprintf(fp_disp_cloud, "%f %f %f %d %d %d\n",
					X, Y, Z,
					img_bytes[y * width * 3 + 3 * x + 2],
					img_bytes[y * width * 3 + 3 * x + 1],
					img_bytes[y * width * 3 + 3 * x]);
		}
	}
	fclose(fp_disp_cloud);
}
