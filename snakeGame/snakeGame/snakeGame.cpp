// snakeGame.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "snakeGame.h"
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <vector>

using namespace std;

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

//vector<int> snake[100]; //0으로 초기화된 100개의 원소를 가진 벡터가 원소인 배열snake를 생성

static POINT snakeCenter;
static POINT snakeArr[100];
static int snakeLength = 5;

//void Snake(HDC hdc, int length);
void Draw_Circle(HDC hdc, int i);
//void Update();

void Init(HWND hWnd, int size);
//void Outline(HWND hWnd, HDC hdc, RECT rect, int size);
void Snake(HDC hdc, int* x, int* y, int length);
void Food_Place(HWND hWnd, RECT rect, int* fx, int* fy);
void Food_Draw(HDC hdc, int* fx, int* fy);

#ifdef UNICODE

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 

#else

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 

#endif

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
    static int num = 0;
    static int direction;
    static UINT checkCollision = 0;
    
    enum { zero, Wall };


    PAINTSTRUCT ps;
    HDC hdc;

    static int width[100] = { 50, 70, 90, 110, 130 };
    static int height[100] = { 50, 50, 50, 50, 50 };
    //뱀의 시작위치

    static int length = 4;
    //뱀의 길이

    static int fx = 30;
    static int fy = 30;
    //먹이의 위치

    static RECT rect;
    //맵의 영역

    static int size;
    //맵테두리 사각형길이의 반

    switch (message)
    {
    case WM_CREATE:
        {
            size = 10;

            GetClientRect(hWnd, &rect);

            Init(hWnd, size);

            break;
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
            SetTimer(hWnd, 1, 80, NULL);            
            
            if (wParam == VK_UP) 
            {
                
                if (num != 2)
                {
                    num = 1;
                }                 
            }
            else if (wParam == VK_DOWN) 
            {
                if (num != 1)
                {
                    num = 2;
                }             
            }
            else if (wParam == VK_LEFT) 
            {
                if (num != 4)
                {
                    num = 3;
                }              
            }
            else if (wParam == VK_RIGHT) 
            {            
                if (num != 3)
                {
                    num = 4;
                }                    
            }          

          

            InvalidateRgn(hWnd, NULL, FALSE);//새 이벤트 불러오기
        }
        break;

    case WM_TIMER:
    {
        switch (wParam)
        {
            case 1: //1번 타이머.
            {
                switch (num)
                {
                case 1:
                {
                    for (int i = 0; i < length; i++)
                    {
                        width[length - i] = width[length - i - 1];
                        height[length - i] = height[length - i - 1];
                    }

                    height[0] -= 20;

                }
                break;

                case 2:
                {
                    for (int i = 0; i < length; i++)
                    {
                        width[length - i] = width[length - i - 1];
                        height[length - i] = height[length - i - 1];
                    }

                    height[0] += 20;

                }
                break;

                case 3:
                {
                    for (int i = 0; i < length; i++)
                    {
                        width[length - i] = width[length - i - 1];
                        height[length - i] = height[length - i - 1];
                    }

                    width[0] -= 20;

                }
                break;

                case 4:
                {
                    for (int i = 0; i < length; i++)
                    {
                        width[length - i] = width[length - i - 1];
                        height[length - i] = height[length - i - 1];
                    }

                    width[0] += 20;

                }
                break;
                }

                // << 창 좌표.
                /*POINT center_up = { 10,10 };
                POINT center_down = { 10,411 };
                POINT center_left = { 10,10 };
                POINT center_right = { 450,10 };*/

                if (width[0] <= 10 || width[0] >= 450 || height[0] <= 10 || height[0] >= 410)
                {
                    checkCollision == Wall;

                    cout << "벽과 충돌 ";
                    KillTimer(hWnd, 1);
                    KillTimer(hWnd, 2);

                    TCHAR msg[40];
                    TCHAR show[40];
                    _stprintf_s(msg, _T("벽과 충돌했습니다."));
                    _stprintf_s(show, _T("GameOver"));
                    MessageBox(hWnd, msg, show, MB_OK);

                    break;
                }

                if (width[0] == fx && height[0] == fy)
                {
                    cout << "먹이 먹음";

                    KillTimer(hWnd, 1);
                    KillTimer(hWnd, 2);
                    //뱀의 머리위치와 먹이의 위치가 같으면?
                    width[length + 1] = width[length];
                    height[length + 1] = height[length];
                    length++;
                    //뱀의 꼬리가 하나 증가

                    Food_Place(hWnd, rect, &fx, &fy);
                    //먹이 위치 재배치

                    KillTimer(hWnd, 2);
                    SetTimer(hWnd, 2, 5000, NULL);
                    //타이머 처음부터 다시발동


                    InvalidateRgn(hWnd, NULL, TRUE);
                    TCHAR msg[40];
                    TCHAR show[40];
                    _stprintf_s(msg, _T("먹이를 먹어 꼬리가 +1 증가하였습니다."));
                    _stprintf_s(show, _T("현재 꼬리의 길이는 %d칸 입니다."), length + 1);
                    MessageBox(hWnd, msg, show, MB_OK);
                    //먹이를 먹었다는 메세지 표시
                }
            }
            break;

            case 2: //2번 타이머.
            {
                hdc = GetDC(hWnd);

                Food_Place(hWnd, rect, &fx, &fy);
                //먹이 좌표 재생성

                ReleaseDC(hWnd, hdc);   

                
            }
            break;

            
        }
       
         
            InvalidateRgn(hWnd, NULL, true);//새 이벤트 불러오기
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
        
            //map 외곽.
            POINT center_up = { 10,10 };
            POINT center_down = { 10,411 };
            POINT center_left = { 10,10 };
            POINT center_right = { 450,10 };

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

            Snake(hdc, width, height, length);
            //뱀 그리기

            Food_Draw(hdc, &fx, &fy);        

            //InvalidateRgn(hWnd, NULL, false);//새 이벤트 불러오기
                                   
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        KillTimer(hWnd, 1);
        KillTimer(hWnd, 2);
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

//void Update()
//{
//    //wsad
//    // WM_keydown, WM_keyup -> 입력 키를 둔감하게 받을 때.
//
//    // 키 입력 받는게 예민하게 받을 때.
//   
//        if (GetKeyState('A') & 0x8000)
//        {       
//            for (int i = 0; i < 50; i++)
//            {
//                snakeCenter.x -= 10;
//
//                cout << snakeCenter.x;
//            }
//           
//        }
//
//        if (GetKeyState('D') & 0x8000)
//        {
//            snakeCenter.x += 10;
//
//            for (int i = 0; i < 50; i++)
//            {
//                snakeCenter.x += 10;
//
//                cout << snakeCenter.x << " ";
//            }
//        }
//
//        if (GetKeyState('S') & 0x8000)
//        {
//            snakeCenter.y += 10;
//        }
//
//        if (GetKeyState('W') & 0x8000)
//        {
//            snakeCenter.y -= 10;
//        }
//}

void Draw_Circle(HDC hdc, int i)
{
    Ellipse(hdc, snakeArr[i].x - 10, snakeArr[i].y - 10, snakeArr[i].x + 10, snakeArr[i].y + 10);
}



//참고.
void Init(HWND hWnd, int size) 
{
    srand(time(NULL));
    SetTimer(hWnd, 2, 5000, NULL);
    //5초마다 1번 타이머가 WM_TIMER 발생
}

//void Outline(HWND hWnd, HDC hdc, RECT rect, int size) 
//{
//    //맵 표시
//    int x = 10;
//    int y = 30;
//    TCHAR str[80];
//
//    _stprintf_s(str, _T("범위좌표 : (%d, %d) / (%d, %d)"), 20, 40, rect.right - rect.right % 20, rect.bottom - rect.bottom % 20);
//    TextOut(hdc, 500, 0, str, _tcslen(str));
//
//    for (x = 10; x < rect.right; x += 20) 
//    {
//        Rectangle(hdc, x - size, y - size, x + size, y + size);
//    }
//    x = 10;
//
//    for (y = 30; y < rect.bottom; y += 20) 
//    {
//        Rectangle(hdc, x - size, y - size, x + size, y + size);
//    }
//    y -= 20;
//
//    for (x = 10; x < rect.right; x += 20) 
//    {
//        Rectangle(hdc, x - size, y - size, x + size, y + size);
//    }
//    x -= 20;
//
//    for (y = 30; y < rect.bottom; y += 20) 
//    {
//        Rectangle(hdc, x - size, y - size, x + size, y + size);
//    }
//}

void Snake(HDC hdc, int* x, int* y, int length) 
{
    for (int i = 0; i <= length; i++) {
        if (i == 0) {
            SelectObject(hdc, CreateSolidBrush(RGB(210, 110, 30)));
            //처음 그리는 머리는 빨간색
        }
        else {
            SelectObject(hdc, CreateSolidBrush(RGB(120, 100, 100)));
            //이후 그리는 몸은 녹색
        }
        Ellipse(hdc, x[i] - 10, y[i] - 10, x[i] + 10, y[i] + 10);
    }

    /*SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
    Ellipse(hdc, x1 - 10, y1 - 10, x1 + 10, y1 + 10);
    SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 255)));
    Ellipse(hdc, x2 - 10, y2 - 10, x2 + 10, y2 + 10);*/
}

void Food_Place(HWND hWnd, RECT rect, int* fx, int* fy) 
{
    int tmp;
    do {
        *fx = (rand() % rect.right);
        tmp = *fx % 10;
        *fx -= tmp;
    } while (*fx % 20 != 10);
    //먹이의 x좌표

    do {
        *fy = (rand() % rect.bottom);
        tmp = *fy % 10;
        *fy -= tmp;
    } while (*fy % 20 != 10);
    //먹이의 y좌표
    //영역 내 먹이를 표시해야 한다. 뱀의 좌표와 일치하기 위해서는 항상 20의 간격을 유지해야 한다.

    InvalidateRgn(hWnd, NULL, TRUE);
}

void Food_Draw(HDC hdc, int* fx, int* fy) 
{
    TCHAR tmp[20];
    _stprintf_s(tmp, _T("먹이좌표 : [%d, %d]"), *fx, *fy);
    TextOut(hdc, 20, 0, tmp, _tcslen(tmp));

    SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 255)));
    //먹이를 먹었을 경우 파란색으로 다시 그린다.
    Rectangle(hdc, *fx - 10, *fy - 10, *fx + 10, *fy + 10);
}


