#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

void DrawWorld();

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow){

  // Window Class
  WNDCLASS wc = {};
  wc.lpszClassName = "My Window Class";
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.lpfnWndProc = WindowProcedure;
  wc.hInstance = hInst;
  RegisterClass(&wc);

  // Window
  HWND hWnd;
  hWnd = CreateWindow(
    wc.lpszClassName,
    "My OpenGL App",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    0, 0, 800, 600,
    NULL, NULL, NULL, NULL
  );
  
  // Device Context
  HDC hDC;				
  hDC = GetDC(hWnd);
  
  // Pixel Format Descriptor
  PIXELFORMATDESCRIPTOR pfd;
  pfd.nSize        = sizeof(pfd);
  pfd.nVersion     = 1;
  pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | 0;
  pfd.iPixelType   = PFD_TYPE_RGBA;
  pfd.cColorBits   = 32;
  
  // Pixel Format
  int pf;
  pf = ChoosePixelFormat(hDC, &pfd);
  if (pf == 0) {
	 MessageBox(NULL, "ChoosePixelFormat() failed: Cannot find a suitable pixel format.", "Error", MB_OK); 
	 return 0;
  } 
  if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
	 MessageBox(NULL, "SetPixelFormat() failed: Cannot set format specified.", "Error", MB_OK);
	 return 0;
  }
  DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  
  // OpenGL Context
  HGLRC hRC;
  hRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, hRC);

  ShowWindow(hWnd, ncmdshow);
  
  // Window Message
  MSG msg = {};
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  
  wglMakeCurrent(NULL, NULL);
  ReleaseDC(hWnd, hDC);
  wglDeleteContext(hRC);
  DestroyWindow(hWnd);
  
  return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp){
  
  PAINTSTRUCT ps;
	
  switch (msg){
      
    case WM_PAINT:
      DrawWorld();
	    BeginPaint(hWnd, &ps);
	    EndPaint(hWnd, &ps);
      break;
      
    case WM_SIZE:
    	glViewport(0, 0, LOWORD(lp), HIWORD(lp));
    	PostMessage(hWnd, WM_PAINT, 0, 0);
    	break;

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
  }

  return DefWindowProc(hWnd, msg, wp, lp);
}


void DrawWorld(){
  
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_TRIANGLES);
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex2i(0, 1);
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex2i(1, -1);
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex2i(-1, -1);
  glEnd();
  glFlush();
}
