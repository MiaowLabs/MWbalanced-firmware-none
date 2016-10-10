# MWbalanced-firmware-none

MWbalancedSTC15 control source code with no RTOS.

这是MWbalancedSTC15两轮自平衡小车控制代码，未使用实时操作系统。

本代码对于喵呜实验室MWbalancedSTC15第二版硬件。

### 愿景

希望有更多的人参与到代码的开发和维护中来。

Hope to have more people involved in the development and maintenance of the code fo MWbalanced.

### 开源项目

**MWbalanced2.0为初学而生，选用8051内核单片机（型号STC(IAP)15W4K61S2）作为控制芯片,使用Keil C51进行编译开发。**

MWbalanced是喵呜实验室的开源项目之一，更详细的开发操作，见[喵呜百科](http://miaowlabs.com/wiki-MWbalanced.html)。

MWbalanced两轮自平衡小车对应的控制代码@外部晶振20M, 其他硬件版本可能需要重新调整PID参数。

角度环PD调试指南视频：[http://v.qq.com/page/p/3/e/p0192qldm3e.html](http://v.qq.com/page/p/3/e/p0192qldm3e.html)

速度环PI调试指南视频：[http://v.qq.com/page/g/3/m/g0193e9aj3m.html](http://v.qq.com/page/g/3/m/g0193e9aj3m.html)

**更多教程详见喵呜百科**：[http://miaowlabs.com/wiki-MWbalanced.html](http://miaowlabs.com/wiki-MWbalanced.html)

问答论坛：[http://bbs.miaowlabs.com/](http://bbs.miaowlabs.com/)

气氛良好的技术交流**Q群**：喵呜实验室自平衡小车 153337261

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

