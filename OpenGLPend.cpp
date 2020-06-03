// OpenGLPend.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "OpenGLPend.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "PendCalc.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;
HWND hWnd;
HWND HandlePanel;
HWND EditLen ;
HWND EditA0 ;
HWND EditV0 ;
HWND ButStart ;
HWND ButStop ;
bool is_active = false;
double tstart = (double)clock() / (double)CLOCKS_PER_SEC ; 

HDC   hdc;      // handle to device context
HGLRC hglrc;    // handle to OpenGL rendering context
int width, height;      // the desired width and
unsigned int framecount ;

HPEN pene ;
HPEN penk ;

double Pend_Len = 5 ;

const double dview_a = 0.1 ;
double view_a = 0 ;
const double dview_y = 0.1 ;
double view_y = 0 ;

PendCalc calc ;

std::vector<double> energy_k ;
std::vector<double> energy_e ;

// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void draw() ;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_OPENGLPEND, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OPENGLPEND));

	if (!PendCalc::execTests()) {
		MessageBoxA(hWnd,"Не пройдены тесты","Ошибка",0) ;
		return 1 ;
	}

	// Цикл основного сообщения перестроен для работы в OpenGL панели	
	while (true) {
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			
			if( msg.message == WM_QUIT )
            {
                break;
            }
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else 
			draw() ;
	}
	return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OPENGLPEND));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_OPENGLPEND);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{   

   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   EditLen = CreateWindowEx( WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT("5"),
                        ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_CHILD | WS_VISIBLE ,
                        400, 40, 60, 26,hWnd, NULL, hInstance, NULL );
   
   EditA0 = CreateWindowEx( WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT("0.5"),
                        ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_CHILD | WS_VISIBLE ,
                        400, 100, 60, 26,hWnd, NULL, hInstance, NULL );
   
   EditV0 = CreateWindowEx( WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT("0"),
                        ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_CHILD | WS_VISIBLE ,
                        400, 160, 60, 26,hWnd, NULL, hInstance, NULL );
   
   ButStart = CreateWindow(TEXT("BUTTON"), TEXT("Старт"), BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP,
						400, 200, 130, 26, hWnd, (HMENU)ID_BUTTON_START, NULL, NULL);
   ButStop = CreateWindow(TEXT("BUTTON"), TEXT("Стоп"), BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP,
						400, 200, 130, 26, hWnd, (HMENU)ID_BUTTON_STOP, NULL, NULL);

   // Скрытие кнопки Стоп
   ShowWindow(ButStop,SW_HIDE) ;


   DWORD dwExStyle = WS_EX_CLIENTEDGE|WS_EX_DLGMODALFRAME;
	DWORD dwStyle = WS_CHILD|WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE|SS_SUNKEN;
   HandlePanel = CreateWindowEx(dwExStyle, L"STATIC", L"", dwStyle, 0, 0, 300, 300, hWnd, (HMENU)NULL, hInstance, (LPVOID)NULL);

   //

   ShowWindow(hWnd, nCmdShow);
   SetWindowPos(hWnd,NULL,100,100,600,600,0) ;
   UpdateWindow(hWnd);

   penk = CreatePen(PS_SOLID, 3, RGB(128, 0, 0));
   pene = CreatePen(PS_SOLID, 3, RGB(0, 0, 128));

   SetWindowTextA(hWnd,"Математический маятник") ;

   hdc = GetDC(HandlePanel);

   PIXELFORMATDESCRIPTOR pfd = { 0 };  // create the pfd,
   pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );    // just its size
    pfd.nVersion = 1;   // always 1

    pfd.dwFlags = PFD_SUPPORT_OPENGL |  // OpenGL support - not DirectDraw
                  PFD_DOUBLEBUFFER   |  // double buffering support
                  PFD_DRAW_TO_WINDOW;   // draw to the app window, not to a bitmap image

    pfd.iPixelType = PFD_TYPE_RGBA ;    // red, green, blue, alpha for each pixel
    pfd.cColorBits = 24;                // 24 bit == 8 bits for red, 8 for green, 8 for blue.
                                        // This count of color bits EXCLUDES alpha.

    pfd.cDepthBits = 32;                // 32 bits to measure pixel depth.  That's accurate!

	int chosenPixelFormat = ChoosePixelFormat( hdc, &pfd );
	if( chosenPixelFormat == 0 )
    {
        FatalAppExit( NULL, TEXT("ChoosePixelFormat() failed!") );
    }

	int result = SetPixelFormat( hdc, chosenPixelFormat, &pfd );
    if (result == NULL)
    {
        FatalAppExit( NULL, TEXT("SetPixelFormat() failed!") );
    }

	hglrc = wglCreateContext( hdc );
	wglMakeCurrent( hdc, hglrc );

	width = 300 ;
	height = 300 ;
		

   return TRUE;
}

void InvalidateGra(bool clear) {
	RECT rect ;
	rect.top = 310;
	rect.left = 10;
	rect.bottom = 600 ;		
	rect.right = 600;

	InvalidateRect(hWnd,&rect,clear) ;
    UpdateWindow(hWnd) ;
}

void draw() 
{
	if (!is_active) return ;

	framecount++ ;

    glViewport(0, 0, width, height);    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(float)width/(float)height, 1, 1000);
	    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	double viewx = 5*sin(view_a) ;
	double viewz = 5*cos(view_a) ;
	gluLookAt(  viewx, view_y, viewz,
                0, view_y, 0,
                0, 1, 0);

    glClearColor( 0.0, 0, 0, 0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	    
	double x1 = 0 ;
	double y1 = 2 ;
	double dx = 0.25 ;
			
	double dy = -0.5*Pend_Len; 

	double t = (double)clock() / (double)CLOCKS_PER_SEC-tstart ;
	
	double a = calc.getA(t) ;	
	energy_e.push_back(calc.getE(t)) ;
	energy_k.push_back(calc.getK(t)) ;
	
	if (energy_e.size()>500) {
		energy_e.clear() ;
		energy_k.clear() ;
		InvalidateGra(true) ;
	}

    glBegin (GL_TRIANGLES);
        glColor3f(  0, 0.25, 0 );     
        glVertex3f( x1, y1, 0 );
		glColor3f(  0, 0.75, 0 );     
        glVertex3f( x1+dy*sin(a)-dx*cos(a), y1+dy*cos(a)+dx*sin(a), -dx );        
		glVertex3f( x1+dy*sin(a)+dx*cos(a), y1+dy*cos(a)-dx*sin(a), -dx );        
    glEnd();

	glBegin (GL_TRIANGLES);
        glColor3f(  0,0, 0.25 );     
        glVertex3f( x1, y1, 0 );
		glColor3f(  0,0, 0.75 );     
        glVertex3f( x1+dy*sin(a)-dx*cos(a), y1+dy*cos(a)+dx*sin(a), -dx );        
		glVertex3f( x1+dy*sin(a)-dx*cos(a), y1+dy*cos(a)+dx*sin(a), dx );         
    glEnd();

	glBegin (GL_TRIANGLES);
        glColor3f(  0,0, 0.25 );     
        glVertex3f( x1, y1, 0 );
		glColor3f(  0,0, 0.75 );     
        glVertex3f( x1+dy*sin(a)+dx*cos(a), y1+dy*cos(a)-dx*sin(a), -dx );        
		glVertex3f( x1+dy*sin(a)+dx*cos(a), y1+dy*cos(a)-dx*sin(a), dx );         
    glEnd();

	glBegin (GL_TRIANGLES);
        glColor3f(  0, 0.25, 0 );     
        glVertex3f( x1, y1, 0 );
		glColor3f(  0, 0.75, 0 );     
        glVertex3f( x1+dy*sin(a)-dx*cos(a), y1+dy*cos(a)+dx*sin(a), dx );        
		glVertex3f( x1+dy*sin(a)+dx*cos(a), y1+dy*cos(a)-dx*sin(a), dx );        
    glEnd();

    SwapBuffers(hdc);    
	
	InvalidateGra(false) ;
			
}

void drawGraphic(HDC ghdc) {
	const float M = 100 ;
	const int LEFT = 10 ;
	const int DOWN = 500 ;
	
	POINT pt ;
			
	if (energy_e.size()>0) {
		//MoveToEx(ghdc,LEFT,MID,&pt) ;
	//LineTo(ghdc,LEFT+50,MID+50) ;
	
		MoveToEx(ghdc,LEFT,DOWN,&pt) ;
		LineTo(ghdc,LEFT+600,DOWN) ;

		SelectObject(ghdc,pene) ;
		MoveToEx(ghdc,LEFT,DOWN-(int)(M*energy_e[0]),&pt) ;
		for (unsigned int i=1; i<energy_e.size(); i++) 
		{
			LineTo(ghdc,LEFT+i,DOWN-(int)(M*energy_e[i])) ;
			if (i>600) break ;
		}

		SelectObject(ghdc,penk) ;
		MoveToEx(ghdc,LEFT,DOWN-(int)(M*energy_k[0]),&pt) ;
		for (unsigned int i=1; i<energy_e.size(); i++) 
		{
			LineTo(ghdc,LEFT+i,DOWN-(int)(M*energy_k[i])) ;
			if (i>600) break ;
		}
	}	
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC formhdc;
	char buf[32] ;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON_START:
		    // Запуск таймера
			ShowWindow(ButStart,SW_HIDE) ;
			ShowWindow(ButStop,SW_SHOW) ;
			EnableWindow(EditLen,false) ;
			EnableWindow(EditA0,false) ;
			EnableWindow(EditV0,false) ;
			char buf[16] ;
			GetWindowTextA(EditLen,buf,15) ;	
			Pend_Len = atof(buf) ;
			calc.setLen(Pend_Len) ;			
			GetWindowTextA(EditA0,buf,15) ;	
			calc.setA0(atof(buf)) ; 	
			GetWindowTextA(EditV0,buf,15) ;	
			calc.setV0(atof(buf)) ; 	
			tstart = clock() / CLOCKS_PER_SEC ;
			is_active = true ;
			break ;
		case ID_BUTTON_STOP:
		    // Запуск таймера
			ShowWindow(ButStart,SW_SHOW) ;
			ShowWindow(ButStop,SW_HIDE) ;
			EnableWindow(EditLen,true) ;
			EnableWindow(EditA0,true) ;
			EnableWindow(EditV0,true) ;
			is_active = false ;
			break ;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		formhdc = BeginPaint(hWnd, &ps);
		TextOut(formhdc, 400,20,TEXT("Длина"),5);
		TextOut(formhdc, 400,80,TEXT("A0"),2);
		TextOut(formhdc, 400,140,TEXT("V0"),2);
			
		drawGraphic(formhdc) ;
		// TODO: добавьте любой код отрисовки...
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
        switch( wParam )
        {
        case VK_ESCAPE:
            PostQuitMessage( 0 );
            break;
		case VK_SPACE:
			//draw() ;
			sprintf(buf,"Frames: %d",framecount) ;
			SetWindowTextA(hWnd,buf) ;
			break ;
		case VK_LEFT:
			view_a-=dview_a ;
			break ;        
		case VK_RIGHT:
			view_a+=dview_a ;
			break ;        
        case VK_UP:
			view_y+=dview_y ;
			break ;        		
		case VK_DOWN:
			view_y-=dview_y ;
			break ;        				
		default:
            break;
        }
        return 0;
	case WM_DESTROY:
		wglMakeCurrent( NULL, NULL );

		// Delete the rendering context, we no longer need it.
		wglDeleteContext( hglrc );

		// release your window's DC
		ReleaseDC( HandlePanel, hdc );
    
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
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
