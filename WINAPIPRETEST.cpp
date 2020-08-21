// WINAPIPRETEST.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "WINAPIPRETEST.h"

#define MAX_LOADSTRING 100
#define TIMER1 1
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];    // имя класса главного окна

HWND mainhwnd;
// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Dialog1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

  
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPIPRETEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPIPRETEST));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}




ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPIPRETEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPIPRETEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

struct Game {
    int rows_ =20, columns_ = 20, cx_, cy_, botmidrectx_,botmidrecty_;
    std::vector <std::vector< std::pair < std::pair<int, int>, std::pair<int, int>>>> rects_;
    std::vector<HBRUSH> br_;
    double startx_, starty_;
    std::vector < std::pair<int, int>>speed_vector_;
    bool firsttime_ = 1;
    std::vector<std::pair<int, int>> matrixcoord_;
    std::vector<std::vector<bool>> gamecenter_;
    void MakeStep();
    std::vector<std::pair<double, double>> centre_pos_;
    std::pair<int, int> GetCurRect(double x, double y);
    void Restart(int rows , int columns );
    void SetRowsAndColumns(int rows, int columns);
    int sec_ = 0;
};
void Game::Restart(int rows, int columns){
    for (int i = 0; i < rects_.size(); i++) {
        rects_[i].clear();
    }
    rects_.clear();
    for (int i = 0; i < gamecenter_.size(); i++) {
        gamecenter_[i].clear();
    }
    gamecenter_.clear();
    firsttime_ = 1;
    matrixcoord_.clear();
    speed_vector_.clear();
    br_.clear();
    centre_pos_.clear();
    rows_ = rows;
    columns_ = columns;
    sec_= 0;
    SetTimer(mainhwnd, TIMER1, 1000, NULL);
    SendMessage(mainhwnd, WM_PAINT, 0, 0);
}
void Game::SetRowsAndColumns(int rows, int columns){
    rows_ = rows;
    columns_ = columns;
}
std::pair<int, int> Game::GetCurRect(double x, double y){
    POINT p;
    p.x = x;
    p.y = y;
    RECT rt;
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < columns_; j++) {
            SetRect(&rt, rects_[i][j].first.first-2,
               rects_[i][j].first.second-2,
               rects_[i][j].second.first+2,
               rects_[i][j].second.second+2);
            if (PtInRect(&rt, p)) {
                return { i,j };
            }
        }
    }
    return { -1,0 };
}
void Game::MakeStep() {
    if (!matrixcoord_.empty()) {
        
        for (int i = 0; i < matrixcoord_.size(); i++) {
            POINT p1,p2;
            RECT rt1, rt2;
            int currentrect_x = matrixcoord_[i].first;
            int currentrect_y = matrixcoord_[i].second;
            double x = centre_pos_[i].first;
            double y = centre_pos_[i].second;
            double normalx = (double)speed_vector_[i].first / 50;
            double normaly = (double)speed_vector_[i].second / 50;
            bool closestep = 0;
            if (normalx == 0&&normaly==0) continue;
            for (int j = 0; j < 25; j++) {
                p2.x=p1.x=x += 2*normalx;
                p2.y=p1.y=y +=2* normaly;
                p1.x -= 10;
                p2.x += 10;
                SetRect(&rt1, 0, 0, rects_[0][0].first.first + 2, cy_);
                SetRect(&rt2, rects_[rows_ - 1][columns_ - 1].second.first - 2, 0, cx_, cy_);
                /*if (PtInRegion(CreateRectRgn(0, 0, rects_[0][0].first.first+2, cy_),x-10, y)
                     ||
                    PtInRegion(CreateRectRgn(rects_[rows_-1][columns_-1].second.first-2, 0, cx_, cy_),x+10, y))*/
                if(PtInRect(&rt1,p1)||PtInRect(&rt2,p2))
                {
                    speed_vector_[i].first *= -1;
                    normalx *= -1;
                    x += normalx * 4;
                    y += normalx * 4;
                    j += 2;
                }
                if (PtInRegion(CreateRectRgn(rects_[0][0].first.first, 0, rects_[rows_ - 1][columns_ - 1].second.first, rects_[0][0].first.second), x, y-10)) {
                    speed_vector_[i].first = 0;
                    speed_vector_[i].second = 0;
                    matrixcoord_[i].first = currentrect_x;
                    matrixcoord_[i].second = currentrect_y;
                    gamecenter_[currentrect_x][currentrect_y] = 1;
                    centre_pos_[i].first = rects_[currentrect_x][currentrect_y].first.first + 10;
                    centre_pos_[i].second = rects_[currentrect_x][currentrect_y].first.second + 10;

                    break;
                }
                std::pair<int, int> tmp = GetCurRect(x, y);
                
                if (gamecenter_[tmp.first][tmp.second])closestep = 1;
                if (tmp.first != rows_ - 1 && tmp.second != columns_ - 1) {
                    if ((gamecenter_[tmp.first + 1][tmp.second] && gamecenter_[tmp.first][tmp.second])
                        || (gamecenter_[tmp.first + 1][tmp.second] && gamecenter_[tmp.first][tmp.second - 1]))
                        closestep = 1;
                }
                
                if(closestep)
                    {
                    if (tmp.first != botmidrectx_ || tmp.first != botmidrecty_) {
                        speed_vector_[i].first = 0;
                        speed_vector_[i].second = 0;
                        matrixcoord_[i].first = currentrect_x;
                        matrixcoord_[i].second = currentrect_y;
                        gamecenter_[currentrect_x][currentrect_y] = 1;
                        centre_pos_[i].first = rects_[currentrect_x][currentrect_y].first.first + 10;
                        centre_pos_[i].second = rects_[currentrect_x][currentrect_y].first.second + 10;
                        break;
                    }
                }
                else {
                    matrixcoord_[i].first=currentrect_x = tmp.first;
                    matrixcoord_[i].second=currentrect_y = tmp.second;
                    centre_pos_[i].first = x;
                    centre_pos_[i].second = y;
                }
                
            }
        }
    }
}

Game play;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HBRUSH brush;
    static RECT rt;
    POINT p;
    mainhwnd = hWnd;
    static HDC hdc;
    static double tmpcordx, tmpcordy, cx, cy;
    static int   botmidrect;
    double normalize_vector;
    std::pair<double, double> norm_vect;
    bool end = 0;

    static bool pause = 0;
    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, TIMER1, 1000, NULL);
        break;
    case WM_SIZE:
        cx = LOWORD(lParam);
        cy = HIWORD(lParam);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
       
        switch (wmId)
        {
        case ID_MENU_RESTART:
            KillTimer(hWnd, TIMER1);
            play.Restart(play.rows_,play.columns_);
            break;
        case ID_MENU_SETTINGS:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, Dialog1);
            break;
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
        switch (wParam)
        {
        case VK_F5:
            SendMessage(hWnd, WM_COMMAND, ID_MENU_RESTART, NULL);
            break;
        }
        break;
    case WM_CHAR:
        switch (wParam) {
        case 'p':
        case 'P':
            if (!pause) {
                pause = 1;
                KillTimer(hWnd, TIMER1);
            }
            else {
                pause = 0;
                SetTimer(hWnd, TIMER1, 1000, NULL);
            }
            break;
        }
    case WM_TIMER:
        play.sec_++;
        hdc = GetDC(hWnd);
        play.MakeStep();
        brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        play.br_.push_back(brush);
        play.matrixcoord_.push_back({ play.botmidrectx_,play.botmidrecty_ });
        SelectObject(hdc, brush);
        GetCursorPos(&p);
        ScreenToClient(hWnd, &p);
        play.centre_pos_.push_back({ play.startx_,play.starty_ });
        if (p.x == play.startx_ && p.y == play.starty_) {
            p.y = play.starty_ - 10;
        }
        if (p.y - play.starty_ >=0) {
            p.y -= 2 * (p.y - play.starty_);
        }
        if (p.y - play.starty_ <= 20) {
            p.y -= 15;
        }
        
        
        normalize_vector = std::sqrt((p.x - play.startx_) * (p.x - play.startx_) + (p.y - play.starty_) * (p.y - play.starty_));
        norm_vect = { (p.x - play.startx_) / normalize_vector,(p.y - play.starty_) / normalize_vector };
        tmpcordx = play.startx_;
        tmpcordy = play.starty_;
        end = 0;
        if (play.gamecenter_[play.botmidrectx_ - 1][play.botmidrecty_ - 1]||
            play.gamecenter_[play.botmidrectx_ - 1][play.botmidrecty_ + 1] ||
            play.gamecenter_[play.botmidrectx_ ][play.botmidrecty_+1] ||
            play.gamecenter_[play.botmidrectx_][play.botmidrecty_ - 1] ||
            play.gamecenter_[play.botmidrectx_ - 1][play.botmidrecty_]){ 

            for (int i = 0; i < 50; i++) {
                p.x = tmpcordx += norm_vect.first;
                p.y = tmpcordy += norm_vect.second;
                std::pair<int, int> cur = play.GetCurRect(p.x, p.y);
                if (cur.first != play.botmidrectx_ || cur.second != play.botmidrecty_) {
                    if (play.gamecenter_[cur.first][cur.second]) {
                        int stop_triangle = 0;
                        //go left?
                        if (!play.gamecenter_[play.botmidrectx_][play.botmidrecty_ - 1]) {
                            p.x = play.rects_[play.botmidrectx_][0].first.first ;
                            p.y = play.rects_[play.botmidrectx_][0].first.second;
                            normalize_vector = std::sqrt((p.x - play.startx_) * (p.x - play.startx_) + (p.y - play.starty_) * (p.y - play.starty_));
                            norm_vect = { (p.x - play.startx_) / normalize_vector,(p.y - play.starty_) / normalize_vector };
                            break;
                        }
                        else {
                            stop_triangle++;
                        }
                        //go right?
                        if (!play.gamecenter_[play.botmidrectx_][play.botmidrecty_ + 1]) {
                            p.x = play.rects_[play.botmidrectx_][play.columns_ - 1].first.first + 20;
                            p.y = play.rects_[play.botmidrectx_][play.columns_ - 1].first.second;
                            normalize_vector = std::sqrt((p.x - play.startx_) * (p.x - play.startx_) + (p.y - play.starty_) * (p.y - play.starty_));
                            norm_vect = { (p.x - play.startx_) / normalize_vector,(p.y - play.starty_) / normalize_vector };
                            break;
                        }
                        else {
                            stop_triangle++;
                        }
                        //go up?
                        if (!play.gamecenter_[play.botmidrectx_ - 1][play.botmidrecty_]) {
                            p.x = play.rects_[play.botmidrectx_ - 1][play.botmidrecty_].first.first + 10;
                            p.y = play.rects_[play.botmidrectx_ - 1][play.botmidrecty_].first.second + 10;
                            normalize_vector = std::sqrt((p.x - play.startx_) * (p.x - play.startx_) + (p.y - play.starty_) * (p.y - play.starty_));
                            norm_vect = { (p.x - play.startx_) / normalize_vector,(p.y - play.starty_) / normalize_vector };
                            break;
                        }
                        else {
                            stop_triangle++;
                        }
                        if (stop_triangle == 3) {
                            end = 1;
                            break;
                        }
                        
                    }
                    else {
                        break;
                    }
                }
            }
        }

        //make vector length equal to 50 for making steps
        norm_vect.first *= 50;
        norm_vect.second *= 50;
        play.speed_vector_.push_back(norm_vect);
      
        if (end) {
            KillTimer(hWnd, TIMER1);
            InvalidateRect(hWnd, NULL, TRUE);
            std::string work = "END\r\n PTS: " + (std::to_string(play.centre_pos_.size()))+"\r\n SEC: "+std::to_string(play.sec_);

            MessageBox(hWnd, work.c_str(), 0, MB_OK);
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
            hdc = BeginPaint(hWnd, &ps);
            play.cx_ = cx;
            play.cy_ = cy;
            tmpcordx = cx / 2 - 20 * play.columns_ / 2;
            tmpcordy = cy / 2 - 20 * play.rows_ / 2;
            if (play.firsttime_) {
                play.rects_.resize(play.rows_);
            }
            for (int i = 0; i < play.rows_; i++) {
                for (int j = 0; j < play.columns_; j++) {
                    Rectangle(hdc, tmpcordx, tmpcordy, tmpcordx + 20, tmpcordy + 20);
                    if (play.firsttime_) {
                        play.rects_[i].push_back({ {tmpcordx,tmpcordy},{tmpcordx + 20 ,tmpcordy + 20} });
                        
                    }
                    tmpcordx += 20;
                }
                tmpcordy += 20;
                tmpcordx = cx / 2 - 20* play.columns_ / 2;
            }
            p.x  = cx / 2;
            p.y  = cy / 2 + 20 * play.rows_ / 2 - 10;
            if (play.firsttime_) {
                play.gamecenter_.resize(play.rows_);
                for (int i = 0; i <play.rows_ ;i++){
                    play.gamecenter_[i].resize(play.columns_);
                }
                std::pair<int, int> mid = play.GetCurRect(p.x, p.y);
                play.botmidrectx_ = mid.first;
                play.botmidrecty_ = mid.second;
                play.startx_ = play.rects_[mid.first][mid.second].first.first + 10;
                play.starty_ = play.rects_[mid.first][mid.second].first.second + 10;
            }

            play.firsttime_ = 0;

        
            for (int i = 0; i < play.centre_pos_.size(); i++) {
              
               SelectObject(hdc, play.br_[i]);
               Ellipse(hdc, play.centre_pos_[i].first - 10,
                   play.centre_pos_[i].second - 10,
                   play.centre_pos_[i].first + 10,
                   play.centre_pos_[i].second + 10);
            }
            Ellipse(hdc, play.startx_ - 10, play.starty_ - 10, play.startx_ + 10, play.starty_ + 10);
       

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        KillTimer(hWnd, TIMER1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

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
INT_PTR CALLBACK Dialog1(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    POINT p;
    static HDC hdc;
    RECT rt;
    int rows=0, columns=0;
    switch (message)
    {
    case WM_COMMAND:
        switch LOWORD(wParam) {
             case IDOK:
                 rows = GetDlgItemInt(hDlg, IDC_ROWS, NULL, NULL);
                 columns = GetDlgItemInt(hDlg, IDC_COLUMNS, NULL, NULL);
                 if (rows < 3 || columns < 3 || rows>20 || columns>20) {
                     MessageBox(hDlg, "WRONG", "WARNING", MB_OK);
                     SetTimer(mainhwnd, TIMER1, 1000, NULL);
                     EndDialog(hDlg, 0);
                 }
                 else {
                     play.Restart(rows, columns);
                     EndDialog(hDlg, 0);
                 }
                  break;
             case IDCANCEL:
                 SetTimer(mainhwnd, TIMER1, 1000, NULL);
                 EndDialog(hDlg, 0);
                 break;
        }
        break;
    case WM_INITDIALOG:
        KillTimer(mainhwnd, TIMER1);
        break;
    default:
        return FALSE;
    }
}

