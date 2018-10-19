//Description: ��ʾһ�����ڣ��ڴ��������������֣�����ֱ�Ϊ���塢������Զ������壬�ֺ���8��40����������
//			   ÿһ�е�������̳��ֺ�����ʧ������ÿһ�����ֵ���ɫ��GRB(0,0,0)��RGB(255,255,255)��������
//Author: Luo Wenyang
//Date: 2018/10/19
//Environment: ��ȷ����Visual Studio 2017 IDE�б��룬��������������ο�README-����˵��

#include <windows.h>
#include <tchar.h>

//��ɳ�ʼ�����̣����岢ע�ᴰ���࣬��������ʾ����
//�������ʾ������ʹ�����ʾ��ʽ
//����һ������ֵ��ΪTRUE˵����ʼ���ɹ���ΪFALSE˵����ʼ��ʧ��
BOOLEAN InitWindow(HINSTANCE hInstance, int nCmdShow);

//������Ϣ���������Զ����WM_CREATE��WM_PAINT�Ĵ������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//�����Զ����������
//�����豸����������ֺţ������������������ɫ
//���ش������Զ�������ľ����������崴��ʧ���򷵻�NULL
//�ú��������������������Ҫʱ�����DeleteObject��������ɾ��
HFONT CreateMyFont(HDC hDC, int nTextHeight, LPCWSTR lpszFacename, COLORREF color);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;	//������Ϣ����

	//��ʼ�����ڣ����ʧ�ܾ͵���һ����ʾ�򲢴����˳�
	if (!InitWindow(hInstance, nCmdShow))
	{
		MessageBox(NULL, L"��������ʧ��!", L"��������", NULL);
		return 1;
	}

	//��Ϣѭ��
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);	
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOLEAN InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;														//���崰�ھ��
	TCHAR szWindowClass[] = L"WORDS";								//���崰��������
	TCHAR szTitle[] = L"������ʾ��������";							//���崰�ڱ���
	WNDCLASS WndClass;												//���崰����
	WndClass.cbClsExtra = 0;										//�޴�������չ
	WndClass.cbWndExtra = 0;										//�޴���ʵ����չ
	WndClass.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));	//����Ϊ��ɫ
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//���ڲ��ü�ͷ���
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//����ȱʡͼ��
	WndClass.hInstance = hInstance;									//��ǰӦ�ó�����
	WndClass.lpfnWndProc = WndProc;									//���ڴ�����
	WndClass.lpszClassName = szWindowClass;							//����������
	WndClass.lpszMenuName = NULL;									//�޴��ڲ˵�
	WndClass.style = 0;												//��������Ϊȱʡ����

	if (!RegisterClass(&WndClass))	//ע�ᴰ��
		return FALSE;
	hWnd = CreateWindow(
		szWindowClass,				//��������
		szTitle,					//������
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,				//�������Ͻ�����
		0,
		CW_USEDEFAULT,				//����ȱʡ�Ŀ�Ⱥ͸߶�
		0,
		NULL,						//�޸�����
		NULL,						//�����˵�
		hInstance,					//��ǰʵ�����
		NULL						//����Ҫ���ݲ���
	);
	if (!hWnd)
		return FALSE;
	ShowWindow(hWnd, nCmdShow);		//��ʾ����
	UpdateWindow(hWnd);				//���²������û���
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;						//�����豸�������
	HFONT hF;						//����������
	PAINTSTRUCT PtStr;				//���������ͼ��Ϣ�Ľṹ�����
	TEXTMETRIC tm;					//����������Ϣ�ṹ�����
	static POINT pos;				//�������ڱ�����������Ľṹ�����
	UINT nIDTimer = 1;				//�����ʱ�����Ϊ1��
	UINT nElapse = 1500;			//����ÿ����ʾ����ʱ��Ϊ1.5s
	UINT nDelay = 100;				//�����ӳ�100ms����ʾ����
	static short fontSize[5];		//�����ֺ����飬�ֱ𱣴�������ֵ�����
	static COLORREF fontColor[5];	//����������ɫ���飬�ֱ𱣴����������ɫ��RGBֵ
	static short nCount;			//���徲̬����������ǰ��ʾ���ǵڼ�������
	int i, j;						//������ʱ����������ѭ���ʹ洢��ʱ��ֵ

	//�������ʾ����������
	LPCWSTR lpsz[] = {
		L"��ʾһ�����ڣ��ڴ��������������֣����壩",
		L"����ֱ�Ϊ���塢������Զ������壬�ֺ���8��40�������������壩",
		L"ÿһ�����ֵ���ɫ��GRB(0,0,0)��RGB(255,255,255)������������Բ��",
		L"ÿһ�е�������̳��ֺ�����ʧ��΢���źڣ�",
		L"����������������ж��壨���飩"
	};

	//�����������ֵ�������
	LPCWSTR lpszFacename[] = {
		L"����",
		L"����",
		L"��Բ",
		L"΢���ź�",
		L"����"
	};

	switch (message)
	{
	case WM_CREATE:	//�����ڴ�����Ϣ
		//����һ����ʱ�������ʧ�ܵ�����ʾ���û�����ѡ��������Դ�����ʱ�������˳�
		while (!SetTimer(hWnd, nIDTimer, nElapse, NULL))
			if (IDCANCEL == MessageBox(hWnd, L"��ʱ������ʧ��!", L"Timer1",
				MB_ICONEXCLAMATION | MB_RETRYCANCEL))
				PostQuitMessage(1);

		//�������ɼ���������ֵ��ֺź���ɫ
		for (i = 0; i < 5; i++)
		{
			fontSize[i] = 8 * (i + 1);
			j = (int)(63.75 * i);
			fontColor[i] = RGB(j, j, j);
		}
		break;
	case WM_PAINT:											//�����ͼ��Ϣ
		hDC = BeginPaint(hWnd, &PtStr);						//����豸����ָ��

		//����ʾ����֮ǰ�ӳ�һ��ʱ�䣬�������������л����ĸо�
		Sleep(nDelay);

		//�������壬ѡ���豸������Ȼ���ȡ����߶Ⱥ��м��
		hF = CreateMyFont(hDC, fontSize[nCount], lpszFacename[nCount], fontColor[nCount]);
		if(nCount == 4)
			SetBkColor(hDC, RGB(0, 0, 0));	//Ϊ������ʾ��ɫ�����֣������ֱ�����Ϊ��ɫ
		SelectObject(hDC, hF);
		GetTextMetrics(hDC, &tm);

		//������һ�����ֵ�������㵱ǰ�����ֵ����꣬�����ǰ�ǵ�һ����������������
		//Ȼ�������ǰ������
		pos.y = nCount?(pos.y + tm.tmHeight + tm.tmExternalLeading):0;
		TextOut(hDC, pos.x, pos.y, lpsz[nCount], _tcslen(lpsz[nCount]));
		DeleteObject(hF);		//ɾ���������
		EndPaint(hWnd, &PtStr);	//ɾ���豸�������
		break;
	case WM_TIMER:	//�����ʱ����Ϣ
		//���nIDTimer���һ�μ�ʱ����ѭ���л���ǰ��ʾ���ı��У���������Ϣˢ�������û���
		if (wParam == nIDTimer)
		{
			nCount = (nCount + 1) % 5;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:										//����رմ�����Ϣ
		KillTimer(hWnd, nIDTimer);							//�ͷŶ�ʱ��nIDTimer
		PostQuitMessage(0);									//����WM_QUIT���˳�����
		break;
	default:												//Ĭ����Ϣ������
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HFONT CreateMyFont(HDC hDC, int nTextHeight, LPCWSTR lpszFacename, COLORREF color)
{
	HFONT hF;	//����������

	//�����Զ�������
	hF = CreateFont(
		nTextHeight,			//��������߶ȣ��ֺţ�
		0,						//��ϵͳ���ݸ߿�Ⱦ��������ȵ����ֵ
		0,						//ÿ�����������ҳ�׵ĽǶ�Ϊ0
		0,						//ÿ�����������ҳ�׵ĽǶ�Ϊ0
		400,					//�����ϸ����Ϊ400����׼ֵ��
		0,						//���岻��б
		0,						//���»���
		0,						//���л���
		GB2312_CHARSET,			//�����ַ���
		OUT_DEFAULT_PRECIS,		//Ĭ���������
		CLIP_DEFAULT_PRECIS,	//Ĭ�ϲü�����
		DEFAULT_QUALITY,		//Ĭ���������
		DEFAULT_PITCH,			//Ĭ�������༰ϵ��
		lpszFacename			//�����������������
	);
	SetTextColor(hDC, color);	//����������ɫ
	return hF;
}