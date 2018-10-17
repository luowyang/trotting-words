#include <windows.h>
#include <math.h>

BOOLEAN InitWindow(HINSTANCE hInstance, int nCmdShow);									//��ʼ�����ں�������
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);		//��Ϣ����������

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;										//������Ϣ����

	if (InitWindow(hInstance, nCmdShow))				//��ʼ������
	{
		MessageBox(NULL, L"��������ʧ��!", L"��������", NULL);
		return 1;
	}
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);						//��Ϣѭ��
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOLEAN InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;														//���崰�ھ��
	TCHAR szWindowClass[] = L"PATAGON";								//���崰��������
	TCHAR szTitle[] = L"�ȱ������";								//���崰�ڱ���
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

	if (!RegisterClass(&WndClass))		//ע�ᴰ��
		return FALSE;
	hWnd = CreateWindow(
		szWindowClass,					//��������
		szTitle,						//������
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,					//�������Ͻ�����
		0,
		CW_USEDEFAULT,					//����ȱʡ�Ŀ�Ⱥ͸߶�
		0,
		NULL,							//�޸�����
		NULL,							//�����˵�
		hInstance,						//��ǰʵ�����
		NULL);							//��Ҫ�˲���
	if (!hWnd)
		return FALSE;
	ShowWindow(hWnd, nCmdShow);			//��ʾ����
	UpdateWindow(hWnd);					//���²������û���
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)			//��Ϣ������
{
	HDC hDC;								//�����豸�������
	HBRUSH hBrush;							//���廭ˢ���
	HPEN hPen;								//���廭�ʾ��
	PAINTSTRUCT PtStr;						//���������ͼ��Ϣ�Ľṹ�����
	
	switch (message)
	{
	case WM_PAINT:											//�����ͼ��Ϣ
		hDC = BeginPaint(hWnd, &PtStr);						//����豸����ָ��
		


		EndPaint(hWnd, &PtStr);								//ɾ���豸�������
		break;
	case WM_DESTROY:										//�رմ���
		PostQuitMessage(0);
		break;
	default:												//Ĭ����Ϣ����
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
