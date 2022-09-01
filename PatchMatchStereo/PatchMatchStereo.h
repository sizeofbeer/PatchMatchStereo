/* -*-c++-*- PatchMatchStereo - Copyright (C) 2020.
* Author	: Yingsong Li(Ethan Li) <ethan.li.whu@gmail.com>
*			  https://github.com/ethan-li-coding
* Describe	: header of patch match stereo class
*/

#pragma once
#include "pms_types.h"


// PatchMatch类
class PatchMatchStereo
{
public:
	PatchMatchStereo();
	~PatchMatchStereo();

public:
	/**
	 * @brief 类的初始化, 完成一些内存的预分配/参数的预设置等
	 * @param width		输入, 核线像对图像宽
	 * @param height	输入, 核线像对图像高
	 * @param option	输入, 算法参数
	 * @return bool
	 */
	bool Initialize(const sint32& width, const sint32& height, const PMSOption& option);

	/**
	 * @brief 匹配
	 * @param img_left	输入, 左图像数据指针, 3通道
	 * @param img_right	输入, 右图像数据指针, 3通道
	 * @param disp_left	输出, 左图像视差图指针, 预先分配和图像等尺寸的内存空间
	 * @return bool
	 */
	bool Match(const uint8* img_left, const uint8* img_right, float32* disp_left);

	/**
	 * @brief 重设
	 * @param width		输入, 核线像对图像宽
	 * @param height	输入, 核线像对图像高
	 * @param option	输入, 算法参数
	 * @return bool
	 */
	bool Reset(const uint32& width, const uint32& height, const PMSOption& option);

	/**
	 * @brief 获取视差图指针
	 * @param view 		0-左视图 1-右视图
	 * @return float*	视差图指针
	 */
	float* GetDisparityMap(const sint32& view) const;


	/**
	 * @brief 获取梯度图指针
	 * @param view 			0-左视图 1-右视图
	 * @return PGradient*	梯度图指针
	 */
	PGradient* GetGradientMap(const sint32& view) const;
private:
	void RandomInitialization() const; 	// 随机初始化
	
	void ComputeGray() const; 			// 计算灰度数据

	void ComputeGradient() const; 		// 计算梯度数据

	void Propagation() const; 			// 迭代传播

	void LRCheck(); 					// 一致性检查

	void FillHolesInDispMap(); 			// 视差图填充

	void PlaneToDisparity() const; 		// 平面转换成视差

	void Release(); 					// 内存释放

private:
	PMSOption option_; // PMS参数

	sint32 width_;
	sint32 height_;

	const uint8* img_left_;
	const uint8* img_right_;

	uint8* gray_left_;
	uint8* gray_right_;

	PGradient* grad_left_;
	PGradient* grad_right_;

	float32* cost_left_; // 左图像聚合代价数据
	float32* cost_right_; // 右图像聚合代价数据

	float32* disp_left_;
	float32* disp_right_;

	DisparityPlane* plane_left_; // 左图像平面集
	DisparityPlane* plane_right_; // 右图像平面集

	bool is_initialized_; // 是否初始化标志

	// 误匹配区像素集
	vector<pair<int, int>> mismatches_left_;
	vector<pair<int, int>> mismatches_right_;
};
