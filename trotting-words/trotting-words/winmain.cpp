//Description: 显示一个窗口，在窗口中有五行文字，字体分别为楷体、黑体和自定义字体，字号由8到40线性增长，
//			   每一行的文字相继出现后又消失，而且每一行文字的颜色由GRB(0,0,0)到RGB(255,255,255)线性增长
//Author: Luo Wenyang
//Date: 2018/10/19
//Environment: 请确保在Visual Studio 2017 IDE中编译，如有其它问题请参看README-运行说明

#include <windows.h>
#include <tchar.h>

//完成初始化过程：定义并注册窗口类，创建并显示窗口
//输入程序示例句柄和窗口显示方式
//返回一个布尔值，为TRUE说明初始化成功，为FALSE说明初始化失败
BOOLEAN InitWindow(HINSTANCE hInstance, int nCmdShow);

//窗口消息处理函数，自定义对WM_CREATE与WM_PAINT的处理过程
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//创建自定义风格的字体
//输入设备环境句柄，字号，字体名句柄和字体颜色
//返回创建的自定义字体的句柄，如果字体创建失败则返回NULL
//该函数创建的字体对象不再需要时需调用DeleteObject函数将其删除
HFONT CreateMyFont(HDC hDC, int nTextHeight, LPCWSTR lpszFacename, COLORREF color);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;	//定义消息变量

	//初始化窗口，如果失败就弹出一个提示框并错误退出
	if (!InitWindow(hInstance, nCmdShow))
	{
		MessageBox(NULL, L"创建窗口失败!", L"创建窗口", NULL);
		return 1;
	}

	//消息循环
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);	
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOLEAN InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;														//定义窗口句柄
	TCHAR szWindowClass[] = L"WORDS";								//定义窗口类名称
	TCHAR szTitle[] = L"依次显示五行文字";							//定义窗口标题
	WNDCLASS WndClass;												//定义窗口类
	WndClass.cbClsExtra = 0;										//无窗口类扩展
	WndClass.cbWndExtra = 0;										//无窗口实例扩展
	WndClass.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));	//背景为白色
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//窗口采用箭头光标
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//采用缺省图标
	WndClass.hInstance = hInstance;									//当前应用程序句柄
	WndClass.lpfnWndProc = WndProc;									//窗口处理函数
	WndClass.lpszClassName = szWindowClass;							//窗口类名称
	WndClass.lpszMenuName = NULL;									//无窗口菜单
	WndClass.style = 0;												//窗口类型为缺省类型

	if (!RegisterClass(&WndClass))	//注册窗口
		return FALSE;
	hWnd = CreateWindow(
		szWindowClass,				//窗口类名
		szTitle,					//标题名
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,				//窗口左上角坐标
		0,
		CW_USEDEFAULT,				//采用缺省的宽度和高度
		0,
		NULL,						//无父窗口
		NULL,						//无主菜单
		hInstance,					//当前实例句柄
		NULL						//不需要传递参数
	);
	if (!hWnd)
		return FALSE;
	ShowWindow(hWnd, nCmdShow);		//显示窗口
	UpdateWindow(hWnd);				//更新并绘制用户区
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;						//定义设备环境句柄
	HFONT hF;						//定义字体句柄
	PAINTSTRUCT PtStr;				//定义包含绘图信息的结构体变量
	TEXTMETRIC tm;					//定义字体信息结构体变量
	static POINT pos;				//定义用于保存文字坐标的结构体变量
	UINT nIDTimer = 1;				//定义计时器编号为1号
	UINT nElapse = 1500;			//定义每轮显示文字时间为1.5s
	UINT nDelay = 100;				//定义延迟100ms后显示文字
	static short fontSize[5];		//定义字号数组，分别保存各行文字的字体
	static COLORREF fontColor[5];	//定义文字颜色数组，分别保存各行文字颜色的RGB值
	static short nCount;			//定义静态变量，代表当前显示的是第几行文字
	int i, j;						//定义临时变量，用于循环和存储临时数值

	//定义待显示的五行文字
	LPCWSTR lpsz[] = {
		L"显示一个窗口，在窗口中有五行文字（楷体）",
		L"字体分别为楷体、黑体和自定义字体，字号由8到40线性增长（黑体）",
		L"每一行文字的颜色由GRB(0,0,0)到RGB(255,255,255)线性增长（幼圆）",
		L"每一行的文字相继出现后又消失（微软雅黑）",
		L"其他所需参数请自行定义（隶书）"
	};

	//定义五行文字的字体名
	LPCWSTR lpszFacename[] = {
		L"楷体",
		L"黑体",
		L"幼圆",
		L"微软雅黑",
		L"隶书"
	};

	switch (message)
	{
	case WM_CREATE:	//处理窗口创建消息
		//创建一个计时器，如果失败弹出提示框，用户可以选择继续尝试创建计时器或者退出
		while (!SetTimer(hWnd, nIDTimer, nElapse, NULL))
			if (IDCANCEL == MessageBox(hWnd, L"定时器创建失败!", L"Timer1",
				MB_ICONEXCLAMATION | MB_RETRYCANCEL))
				PostQuitMessage(1);

		//按线性律计算各行文字的字号和颜色
		for (i = 0; i < 5; i++)
		{
			fontSize[i] = 8 * (i + 1);
			j = (int)(63.75 * i);
			fontColor[i] = RGB(j, j, j);
		}
		break;
	case WM_PAINT:											//处理绘图消息
		hDC = BeginPaint(hWnd, &PtStr);						//获得设备环境指针

		//在显示文字之前延迟一段时间，产生“文字在切换”的感觉
		Sleep(nDelay);

		//定义字体，选入设备环境，然后获取字体高度和行间距
		hF = CreateMyFont(hDC, fontSize[nCount], lpszFacename[nCount], fontColor[nCount]);
		if(nCount == 4)
			SetBkColor(hDC, RGB(0, 0, 0));	//为了能显示白色的文字，把文字背景设为黑色
		SelectObject(hDC, hF);
		GetTextMetrics(hDC, &tm);

		//根据上一行文字的坐标计算当前行文字的坐标，如果当前是第一行文字则将坐标置零
		//然后输出当前行文字
		pos.y = nCount?(pos.y + tm.tmHeight + tm.tmExternalLeading):0;
		TextOut(hDC, pos.x, pos.y, lpsz[nCount], _tcslen(lpsz[nCount]));
		DeleteObject(hF);		//删除字体对象
		EndPaint(hWnd, &PtStr);	//删除设备环境句柄
		break;
	case WM_TIMER:	//处理计时器消息
		//如果nIDTimer完成一次计时，就循环切换当前显示的文本行，并发出消息刷新整个用户区
		if (wParam == nIDTimer)
		{
			nCount = (nCount + 1) % 5;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:										//处理关闭窗口消息
		KillTimer(hWnd, nIDTimer);							//释放定时器nIDTimer
		PostQuitMessage(0);									//发送WM_QUIT，退出程序
		break;
	default:												//默认消息处理函数
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HFONT CreateMyFont(HDC hDC, int nTextHeight, LPCWSTR lpszFacename, COLORREF color)
{
	HFONT hF;	//定义字体句柄

	//创建自定义字体
	hF = CreateFont(
		nTextHeight,			//设置字体高度（字号）
		0,						//让系统根据高宽比决定字体宽度的最佳值
		0,						//每行文字相对于页底的角度为0
		0,						//每个文字相对于页底的角度为0
		400,					//字体粗细度设为400（标准值）
		0,						//字体不倾斜
		0,						//无下划线
		0,						//无中划线
		GB2312_CHARSET,			//中文字符集
		OUT_DEFAULT_PRECIS,		//默认输出精度
		CLIP_DEFAULT_PRECIS,	//默认裁剪精度
		DEFAULT_QUALITY,		//默认输出质量
		DEFAULT_PITCH,			//默认字体间距及系列
		lpszFacename			//设置所用字体的名字
	);
	SetTextColor(hDC, color);	//设置字体颜色
	return hF;
}