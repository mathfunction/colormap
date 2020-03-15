/*========================================================
	這是關於 python ctypes call C/C++ opencv 的範例
	
	會先從 python BGR uint8 numpy (H,W,3) <----> C++ cv::Mat   
	
	註: C++ 物件的成員變數(member variables) , 在 python ctypes 不容易呼叫到 ~~ 所以建議採用全局變數 (global variables) + 成員函式 (member function) 
	ColorMap

======================================*/

#include <iostream>
#include <cstdlib>
#include <vector>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui.hpp>


// 宣告成 全局變數 取代 成員變數
std::vector<cv::Vec3i> ColorMapColors; 
class ColorMap{	
	public:
		// 建構物件
		ColorMap(){													// opencv BGR 通道
			ColorMapColors.push_back(cv::Vec3i(0,0,0));				// 黑色 0
			ColorMapColors.push_back(cv::Vec3i(0,0,255));			// 紅色 1
			ColorMapColors.push_back(cv::Vec3i(0,255,0));			// 綠色 2
			ColorMapColors.push_back(cv::Vec3i(0,255,255));			// 黃色 3
			ColorMapColors.push_back(cv::Vec3i(255,0,0));			// 藍色 4
			ColorMapColors.push_back(cv::Vec3i(255,0,255));			// 紫色 5
			ColorMapColors.push_back(cv::Vec3i(255,255,0));			// 青色 6
			ColorMapColors.push_back(cv::Vec3i(255,255,255));		// 白色 7
			ColorMapColors.push_back(cv::Vec3i(128,128,128));       // 灰色 8
			std::cout << "create  ColorMap with # = " << ColorMapColors.size() << std::endl;
		}//end_ColorMap


		void assign(cv::Mat img){
			cv::Vec3b* ptrColor;
			for(int i=0;i<img.rows;i++){
				// 注意此時 img 是 bytes
				ptrColor = img.ptr<cv::Vec3b>(i);
				for(int j=0;j<img.cols;j++){
					int min_val = 10000000000;
					int argmin_idx = 0;
					for(int c=0;c<ColorMapColors.size();c++){
						int bInt = static_cast<int>(ptrColor[j][0]);
						int gInt = static_cast<int>(ptrColor[j][1]);
						int rInt = static_cast<int>(ptrColor[j][2]);
						// 計算並找出最小的 l1 norm  
						int l1 = abs(bInt-ColorMapColors[c][0]) + abs(gInt-ColorMapColors[c][1]) + abs(rInt-ColorMapColors[c][2]); 
						if(l1 < min_val){
							min_val = l1;
							argmin_idx = c;
						}//endif
					}//endfor
					// 改變 img[i][j] 的 BGR 值
					ptrColor[j] = cv::Vec3b(ColorMapColors[argmin_idx][0],ColorMapColors[argmin_idx][1],ColorMapColors[argmin_idx][2]);
				}//endfor
			}//endfor
		}//end_assign

};


//=====================================================================================================


extern "C"{
	#if defined _WIN32 || defined _WIN64  
		#define DLLEXPORT __declspec(dllexport)
		// windows 需要有以下 define , 需要用 msvc 編譯 !!
		DLLEXPORT ColorMap* ColorMapNew(){
			return new ColorMap();
		}
		DLLEXPORT unsigned char* ColorMapAssign(ColorMap *cm,unsigned char* ptr,int H,int W){
			cv::Mat img(H,W,CV_8UC3,ptr);
			cm->assign(img);
			cv::imshow("cxx_revised",img);
			// 回傳
			unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char)*H*W*3);
			memcpy(buffer,img.data,H*W*3);
			return buffer;
		}
		DLLEXPORT void release(unsigned char* data){
			std::cout << "release " << std::addressof(data) << " !!" << std::endl;
			free(data);
		}//end_release


	#else
		// linux / mac 
		ColorMap* ColorMapNew(){
			return new ColorMap();
		}
		unsigned char* ColorMapAssign(ColorMap *cm,unsigned char* ptr,int H,int W){
			cv::Mat img(H,W,CV_8UC3,ptr);
			cm->assign(img);
			cv::imshow("cxx_revised",img);
			// 回傳
			unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char)*H*W*3);
			memcpy(buffer,img.data,H*W*3);
			return buffer;
		}
		void release(unsigned char* data){
			std::cout << "release " << std::addressof(data) << " !!" << std::endl;
			free(data);
		}//end_release
	#endif
}//end
