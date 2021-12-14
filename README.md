# myled
ロボットシステム学課題


# 説明
 ## 動作
 Raspberry PI 4 Model BでGPIOから出力してledを点灯させるためのデバイスドライバ。ledを点滅させる。また、catで端末に文字を出力する。
 
 ## 配線
 **22番ピン**のGPIO25と**Groundピン**をledでつなぐ。  
 ![IMG_20211214_143236](https://user-images.githubusercontent.com/92069972/145939410-e29f8a9a-2dbd-4a21-ac50-741843d6ff4b.jpg)
 
 
# デモ
https://youtu.be/X_BXumEmRjw

# 使い方  
 ## インストール
     git clone https://github.com/YuwaAoki/myled.git  
     cd myled  
     make  
     sudo insmod myled.ko  
     sudo chmod 666 /dev/myled0  
       
 ## 動作  
 ledの点滅  
 `echo 1 > /dev/myled0`  
 文字を出力  
 `cat /dev/myled0`  
 
 ## ドライバーの削除  
     sudo rmmod myled  
     make clean  
 

# ライセンス
SPDX-License-Identifier: GPL-3.0


# 著作者
Copyright (C) 2021 Ryuichi Ueda. All rights reserved.
