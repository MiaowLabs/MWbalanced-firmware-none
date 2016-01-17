# MWbalanced-firmware-none

MWbalanced control source code with no RTOS.

这是MWbalanced两轮自平衡小车控制代码，未使用实时操作系统。

使用Keil C51进行编译开发。更详细的开发操作，见[喵呜百科](http://miaowlabs.com/wiki-MWbalanced.html)的开发指南。

MWbalanced Ver1.2硬件版本对应的控制代码@内部RC时钟20M, 其他硬件版本可能需要重新调整PID参数。

[角度环PD调试指南视频](http://v.youku.com/v_show/id_XMTQ0MTYwNzY4NA==.html?from=y1.7-1.2)：[http://v.youku.com/v_show/id_XMTQ0MTYwNzY4NA==.html?from=y1.7-1.2](http://v.youku.com/v_show/id_XMTQ0MTYwNzY4NA==.html?from=y1.7-1.2)

气氛良好的技术交流Q群：喵呜实验室自平衡小车 153337261

更新记录

1. 将工作频率从33M降至20M；
2. 整理代码，删除冗余部分；
3. 添加注释；
                       
---20151205 by songyimiao


1. 修改所有小车启动轮子可能出现疯转BUG；
2. 添加、修正部分注释；
					  
 ---20160117 by songyimiao