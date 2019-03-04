# NUIEngine
![](https://github.com/Avens666/NUIEngine/blob/master/bin/img/nuilogo.png?raw=true)

## Introduction

NUIEngine是一个跨平台的图形界面库，基于C++语言开发。其目标是可以在所有平台，简单方便的开发具备NUI界面特性(类似IOS上的具备丰富动画效果的图形界面)的应用程序

请访问我们的网站查看更多介绍和示例视频
http://www.nuiengine.com


NUIEngine is a cross platform graphical interface library, based on C++ language development. The goal is to be able to develop simple, easy to use NUI interface features (similar to the IOS on the rich graphical interface animation) applications
Please visit our website for more presentations and sample videos

http://www.nuiengine.com

A app demo, font view tool

https://www.youtube.com/watch?v=1pyAYLcIoQU

NPlan, a project manage tool power by NUI Engine

https://www.youtube.com/watch?v=FyqRYts4o5E

2018.06.29 Chenzhi 留言
一些用户反馈Demo示例程序很少，确实现在只有一个简单demo，其实NUIEngine的Demo有数十个之多，甚至为Python开发的插件库都有。主要我没精力整理上传。有需求的人可以留言或者发邮件给我
今天上线看到Star有47个了，感谢大家捧场，如果超过了100个Star，我承诺把NPlan这个NUIEngine最正式的App开源，这样大家就不会抱怨例子不够多，不知道如何使用NUIEngine的各种控件及高级功能了

2018.08.30 Chenzhi 留言
NPlan工程已经开源，大家可以通过这个项目了解NUI引擎的各种高级用法。 同时这是个很独特的项目管理软件，我没怎么推广，使用量有限，希望开源后有人能够持续发展，让这个软件更有生命力。

Some user feedback Demo sample programs are very few, and there is only one simple demo now, in fact, we had more than 10 of the Demo of NUIEngine, even for the plug-in library for Python. I didn't have the energy to sort and upload. Any user in need can leave a message or send me an email ask it.
Today, I saw 47 Star on line, thanks to everyone, and if more than 100 Star, I promised to open source for the NPlan, the most formal App of the NUIEngine, so that you won't complain enough, and don't know how to use NUIEngine's controls and advanced functions.

2018.08.07 Chenzhi 留言
今天上传了两个教程文档和几个windows下的demo程序，教程文档可以参考，但不一定和最新的sdk保持一致，可能有出入
demo程序还没有上传代码，主要是一些demo是用老的引擎版本写的，开源的最新版本不一定能够编译通过，没有时间整理，以后有时间再说。

2019.03.04 Chenzhi 留言
今天上传了安卓工程，可以直接编译一个HelloNUI的安卓demo apk，需要更新一些文件，各个目录下都有.  
Upload a android project for HelloNUI demo. need update some files.

上传配套的skia源码一套【third_party/skia_vs2008-source.zip】，这个附带VS2008的工程。 给那些需要之其它环境下编译（比如VS2015）的人使用。 我不确定能否编过，我没VS2015. 

## Getting Started


### First Step | 第一步
1. 下载bin目录下的示例程序（windows版本）
2. 执行bin/Release/HelloNUI.exe，可以看到初步的运行效果

![](https://github.com/Avens666/NUIEngine/blob/master/tutorial/image/hellonui.gif?raw=true)

1 download bin directory under the sample program (Windows version)

2 implementation of bin/Release/HelloNUI.exe, you can see the initial operating results

### Documentation | 文档
NUI Engine的开发文档还在完善中，现阶段可以初步根据sample目录中的示例代码了解NUI引擎的初步使用方法

NUI Engine development documentation is still in the process, at this stage can be initially based on the sample directory of the sample code to understand the basic usage of NUI engine.

### Compile  | 编译方法
NUIengine基于C++开发，代码可以跨平台，支持Windows Linux Android IOS等主流系统

现在只上传了Windows的开发环境，基于VS2010项目工程，用户如果需要可以自己配置其他平台的开发环境

1. 目录说明
``` C++
NUIEngine
├─bin 				//目标程序输出目录
│  ├─Debug			//Debug版exe输出
│  ├─img			//图片资源
│  └─Release		//Release版exe文件输出
├─nuiengine			//nuiengine引擎源码目录
│  ├─BaseClass
│  ├─core
│  │  ├─drawable
│  │  └─views
│  ├─graphic
│  │  └─Src
│  └─include
│      ├─baseclass
│      ├─graphic
│      └─nuiengine
├─sample			//sample程序
│  └─HelloNUI		//HelloNUI sample源码
├─third_party		//第三方库
│  ├─boost				//boost库头文件
│  │  └─boost_1_51_0
│  └─lib				//静态lib库
│  │  ├─skia_debug
│  │  └─skia_release
│  ├─skia				//skia库头文件
│  │  └─include
│  └─tinyxml			//tinyxml库头文件和源文件
│      └─Src
├─tutorial
└─win32						//Windows平台编译工程
  	│  nuiengine_lib.sln	// 使用静态库方式编译nuiengine 的VS解决方案文件
    ├─nuiengine_slib		//nuiengine 静态库VS工程
    └─sample
        └─HelloNUI			//HelloNUI 工程
```

2. 编译步骤
> 现在仅上传了使用Visual Studio 2010 (SP1) 版本的工程。

**2.1** 首先将third_party目录下的  boost.zip skia.zip  tinyxml.zip三个压缩包解压(不带目录解压)到当前目录.注意解压后和上述目录结构一致

**2.2** 将third_party/lib目录下的  lib.7z.001 -  lib.7z.008解压到当前目录

**2.3** 使用VS2010（必须安装SP1补丁）打开 Win32目录下的 nuiengine_lib.sln解决方案文件

**2.4** 编译工程。 （如果一切OK，则可以编译程序，如果有问题，可以邮件联系  cz_666@qq.com ）

如需要其它VS版本，可自行配制，因为skia需要VS2010以上版本（最新的skia需要VS2012以上，nuiengine现在使用的是比较老的版本，兼容VS2010）

NUIengine based on C++ development, the code can cross platform to support Windows Linux Android IOS and other mainstream systems
Now only upload the Windows development environment, based on the VS2010 project, the user needs to configure their own platform development environment.

2 compile step
Now only uploaded using Visual Studio 2010 (SP1) version of the project.

2.1 first of all, decompress of the boost.zip skia.zip tinyxml.zip compression package  under the third_party directory(without directory decompression) to the current directory. Note that the directory structure is same as the above.

2.2 third_party/lib directory  decompress the lib.7z.001 - lib.7z.008 to the current directory

2.3 use VS2010 (must install SP1 patch) open the Win32 directory under the nuiengine_lib.sln solution file

2.4 compiler engineering. (if all OK, you can compile the program, if there is a problem, you can contact cz_666@qq.com e-mail)

### Third Party Lib| 第三方库说明
NUI Engine 使用了skia图形界面库用于底层图形渲染，同时也使用了boost库的部分支持

## Support / Contact
http://www.nuiengine.com
mail: cz_666@qq.com

## License
- 使用Apache 2.0协议开源

- Licensed under either of
 * Apache License, Version 2.0, ([LICENSE-APACHE](LICENSE-APACHE) or http://www.apache.org/licenses/LICENSE-2.0)
