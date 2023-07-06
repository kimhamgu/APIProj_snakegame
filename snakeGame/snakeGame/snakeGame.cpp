// snakeGame.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "snakeGame.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

static POINT center_up = { 10,10 };
static POINT center_down = { 10,411 };
static POINT center_left = { 10,10 };
static POINT center_right = { 450,10 };

static POINT snakeCenter;

void Snake(HDC hdc, int length);
void Draw_Circle(HDC hdc);
void Update();

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SNAKEGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKEGAME));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    //while (true)
    //{
    //    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    //    {
    //        if (msg.message == WM_QUIT)
    //        {
    //            TranslateMessage(&msg);
    //            DispatchMessageW(&msg);
    //        }
    //        else
    //        {
    //            TranslateMessage(&msg);
    //            DispatchMessage(&msg);
    //        }

    //    }
    //    else//동일한 프레임을 하려면 코드 작성.
    //    {
    //        /*Update();
    //        Render();*/
    //    }

    //}

    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKEGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SNAKEGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 476, 480, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{    
    switch (message)
    {
    case WM_CREATE:
        {
            snakeCenter = { 30,30 };
        }
        break;

        

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_KEYDOWN:
        {
            Update();

          /*  if (wParam == VK_RIGHT)
            {
                snakeCenter.x += 5;
            }
            if (wParam == VK_LEFT)
            {
                snakeCenter.x -= 5;
            }
            if (wParam == VK_UP)
            {
                snakeCenter.y -= 5;
            }
            if (wParam == VK_DOWN)
            {
                snakeCenter.y += 5;
            }*/
            
           InvalidateRgn(hWnd, NULL, FALSE);//새 이벤트 불러오기
        }
        break;

    case WM_KEYUP:
        {

        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            for (int i = 0; i < 25; i++)
            {
               Rectangle(hdc, center_up.x - 10, center_up.y - 10, center_up.x + 10, center_up.y + 10);
               center_up.x += 20;
            }
            for (int i = 0; i < 25; i++)
            {
                Rectangle(hdc, center_down.x - 10, center_down.y - 10, center_down.x + 10, center_down.y + 10);
                center_down.x += 20;
            }
            for (int i = 0; i < 25; i++)
            {
                Rectangle(hdc, center_left.x - 10, center_left.y - 10, center_left.x + 10, center_left.y + 10);
                center_left.y += 20;
            }
            for (int i = 0; i < 25; i++)
            {
                Rectangle(hdc, center_right.x - 10, center_right.y - 10, center_right.x + 10, center_right.y + 10);
                center_right.y += 20;
            }            

            Snake(hdc, 2);
            
     
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

//void Draw_Square(HDC hdc)
//{
//    Rectangle(hdc, center.x - 10, center.y - 10, center.x + 10, center.y + 10);
//}

//강사님 팁.

void Update()
{
    //wsad
    // WM_keydown, WM_keyup -> 입력 키를 둔감하게 받을 때.

    // 키 입력 받는게 예민하게 받을 때.
    if (GetKeyState('A') & 0x41)
    {
        snakeCenter.x -= 10;
    }

    if (GetKeyState('D') & 0x44)
    {
        snakeCenter.x += 10;
    }

    if (GetKeyState('S') & 0x53)
    {
        snakeCenter.y += 10;
    }

    if (GetKeyState('W') & 0x57)
    {
        snakeCenter.y -= 10;
    }
}

void Snake(HDC hdc, int length)
{
    for (int i = 0; i <= length; i++) 
    {
        if (i == 0) 
        {
            SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
            //처음 그리는 머리는 빨간색
        }
        else 
        {
            SelectObject(hdc, CreateSolidBrush(RGB(0, 255, 0)));
            //이후 그리는 몸은 녹색
        }
        Draw_Circle(hdc);
    }

    /*SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
    Ellipse(hdc, x1 - 10, y1 - 10, x1 + 10, y1 + 10);
    SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 255)));
    Ellipse(hdc, x2 - 10, y2 - 10, x2 + 10, y2 + 10);*/
}

void Draw_Circle(HDC hdc)
{
    Ellipse(hdc, snakeCenter.x - 10, snakeCenter.y - 10, snakeCenter.x + 10, snakeCenter.y + 10);
}

