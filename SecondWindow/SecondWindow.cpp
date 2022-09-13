#include <windows.h>
#include <string>

LRESULT CALLBACK WndProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    TCHAR szAppName[] = L"TestApp";
    WNDCLASS wc;
    HWND hwnd;
    MSG msg;

    // ウィンドウクラスの属性を設定
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szAppName;

    // ウィンドウクラスを登録
    if (!RegisterClass(&wc)) return 0;

    // ウィンドウを作成
    hwnd = CreateWindow(
        szAppName, L"Title",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL,
        hInstance, NULL);


    if (!hwnd) return 0;

    // ウィンドウを表示
    ShowWindow(hwnd, nCmdShow);

    // ウィンドウを再描画
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

std::wstring text;
int moveCounter = 0;
LRESULT CALLBACK WndProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;

    switch (uMsg) {
    case WM_MOUSEMOVE:
        moveCounter++;
		text = std::to_wstring(moveCounter);
		InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    case WM_CHAR: {
        // WPARAM wParamは8バイトであるが、
        // ここでは簡易的に2バイトまでの文字コードしか
        // 入っていないものと仮定して、wchar_t（2バイト）へキャスト。
        // （なお特殊な漢字は4バイトなのでうまく処理できない）
        std::wstring input = { (wchar_t)wParam }; // wchar_t １文字から成る wstring を作成
        // MessageBox(hwnd, input.c_str(), L"", MB_OK);
        text = text + input;
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }
    case WM_PAINT:
		PAINTSTRUCT paint;
        hdc = BeginPaint(hwnd, &paint);
 		TextOut(hdc, 0, 0, text.c_str(), text.length());
		EndPaint(hwnd, &paint);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}