### 简介

MWbalanced 8051 开源两轮自平衡小车官方Git,由MiaowLabs喵呜实验室强力持续维护,改版本未使用任何实时操作系统。

### 声明

使用GPL协议。

您可以 Fork 本代码，代码的开源/免费使用和引用/修改/衍生代码的开源/免费使用，但不允许修改后和衍生的代码做为闭源的商业软件发布和销售，未经许可 **禁止** 使用将本代码应用于任何商业用途。

### 编译环境

    Windows 系统
    Keil C51 IDE

### 相关网站

喵呜实验室：[http://www.miaowlabs.com](http://www.miaowlabs.com)

喵呜论坛：[http://bbs.miaowlabs.com/](http://bbs.miaowlabs.com/)

### 讨论群

平衡小车Mwbalanced 8051/STM32 Q群：**153337261**

### 更新记录

	1. 更新喵呜手机端，并更新通信协议。

	2. 喵呜手机端，已更新重力感应控制和摇杆控制。
 
	---20161010 by 喵大

	1. 加入舵机驱动代码，文件common/src/servo.c
		      			  common/inc/servo.h

	---20160521 by Alex.G

	1. 加入红外巡线功能代码；
	
	2. 加入超声波避障功能代码；
	
	3. 加入ISP功能代码，再不用冷启动下载程序（不用断电）；
	
	4. 优化了部分代码；
	
	5. 手机APP更新至1.4版本，实现按住走松手停功能；
	
	---20150513	by 喵大
	
	加入电量检测功能（若电量不足，亮红灯警告）；
	
	---20160415 by 喵大
	
	添加注释；
	
	---20160402 by 喵大
	
	上传MWbalancedSTC15二代源代码；
	
	下一步:整理代码，添加注释；
	
	---20160329 by 喵大
	
	将工作频率从33M降至20M；
	
	整理代码，删除冗余部分；
	
	添加注释；
	
	---20151205 by 喵大
	
	修改小车启动轮子可能出现疯转BUG；
	
	添加、修正部分注释；
	
	---20160117 by 喵大

