"""===========================================================================
    此檔案適用在 linux , mac 編譯環境 !! 
	
    編譯指令 : 
        python3 setup.py build
  
    include_dirs : opencv 標頭檔路徑
    library_dirs : opencv library 路徑
    libaries : opencv library 檔名
    

==============================================================================="""

from setuptools import setup, Extension

setup(
    ext_modules=[
    	Extension('colormap',
    		['colormap.cpp'],
    		extra_compile_args=["-std=c++14","-O3"],
    		include_dirs=['.','/opt/intel/openvino_2019.2.275/opencv/include'],
    		library_dirs=['/opt/intel/openvino_2019.2.275/opencv/lib'],
			libraries=['opencv_highgui.4.1.1', 'opencv_video.4.1.1','opencv_videoio.4.1.1','opencv_dnn.4.1.1','opencv_imgproc.4.1.1']
		),
    ]
)