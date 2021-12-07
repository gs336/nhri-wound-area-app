# YOLOv5 NCNN Implementation

This repo provides C++ implementation of [YOLOv5 model](https://github.com/ultralytics/yolov5) and other models using Tencent's NCNN framework. 

# Notes

Currently NCNN does not support Slice operations with steps, therefore I removed the Slice operation
and replaced the input with a downscaled image and stacked it to match the channel number. This
may slightly reduce the accuracy.

# Credits 

* [YOLOv5 by Ultralytics](https://github.com/ultralytics/yolov5) 
* [NCNN by Tencent](https://github.com/tencent/ncnn)

# Assets files
* 請將網路模型 `yolact-opt.bin` 和 `yolact-opt.param` 丟進 `app/src/main/assets` 之中
* 網路模型下載位置: [Google Drive](https://drive.google.com/drive/folders/14gl780DDihYTO8gttjVE3fBP-t5tZXwJ?usp=sharing)
