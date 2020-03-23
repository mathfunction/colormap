"""
	執行前 : 因為使用 openvino 裡面的 opencv ，所以需在啟動前輸入 啟動 openvino 指令 !! 

"""
import numpy as np
import ctypes
import cv2
import os 


class ColorMap:
	def __init__(self):
		try:
			# 匯入 C/C++ 模組	
			if os.name == 'nt':
				self.cxx = ctypes.cdll.LoadLibrary("./colormap.dll") # 如果作業系統 = windows
			else:
				#================================================================================================
				# 這邊是 python3 setup.py build 編譯後自動產生的資料夾名 , 需要手動改 !!
				env = "lib.macosx-10.13-x86_64-3.7" 
				self.cxx = ctypes.cdll.LoadLibrary("./build/{}/colormap.cpython-37m-darwin.so".format(env))
				#===============================================================================================
			#  ColorMapNew() 宣告物件 !!
			self.colormap_model = self.cxx.ColorMapNew()
			#  ColorMapAssign() 宣告輸出 pointer 型別
			self.cxx.ColorMapAssign.restype = ctypes.POINTER(ctypes.c_uint8)
		
		except Exception as e:
			print(e)
			print("==================================================================")
			print("cmd 啟動 openvino 指令 :")
			print("\t (Windows) \"C:\\Program Files (x86)\\IntelSWTools\\openvino\\bin\\setupvars.bat\"")
			print("\t (Linux/Mac) source /opt/intel/openvino/bin/setupvars.sh ")
			print("==================================================================")

	def run(self,img):
		h = img.shape[0]
		w = img.shape[1]
		rptr = self.cxx.ColorMapAssign(self.colormap_model,img.astype(np.uint8).ctypes.data_as(ctypes.c_char_p),h,w)
		r1dimg = np.ctypeslib.as_array(rptr,(h*w*3,)).copy()
		self.cxx.release(rptr)
		rimg = r1dimg.reshape((h,w,3))
		return rimg

if __name__ == '__main__':
	colormap = ColorMap()
	img = cv2.imread("./cube.jpg")
	rimg = colormap.run(img)
	cv2.imshow("python : input --> output",np.hstack([img,rimg]))
	cv2.waitKey(0)