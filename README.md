#MWbalanced-firmware-none

MWbalanced control source code with no RTOS.

这是MWbalanced两轮自平衡小车控制代码，未使用实时操作系统。

**为初学而生，使用简单的8051内核单片机（型号STC15W4K61S2）作为控制芯片。**

使用Keil C51进行编译开发。更详细的开发操作，见喵呜百科的开发指南。

MWbalancedSTC15-2.0两轮自平衡小车对应的控制代码@外部晶振20M, 其他硬件版本可能需要重新调整PID参数。

角度环PD调试指南视频：[http://v.youku.com/v_show/id_XMTQ0MTYwNzY4NA==.html?from=y1.7-1.2](http://v.youku.com/v_show/id_XMTQ0MTYwNzY4NA==.html?from=y1.7-1.2)

**更多教程在喵呜百科**：[http://miaowlabs.com/wiki-MWbalanced.html](http://miaowlabs.com/wiki-MWbalanced.html)

气氛良好的技术交流**Q群**：喵呜实验室自平衡小车 153337261

更新记录

上传MWbalancedSTC15二代源代码；
下一步整理代码，添加注释；

---20160329 by songyimiao


将工作频率从33M降至20M；

整理代码，删除冗余部分；

添加注释；

---20151205 by songyimiao

修改小车启动轮子可能出现疯转BUG；

添加、修正部分注释；

---20160117 by songyimiao