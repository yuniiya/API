#include "GameEngineWindow.h"
#include "GameEngineDebug.h"

// �ʿ��ϱ� ������ ���������� 
// (������ �������� �ڵ�, �޼���, )
LRESULT CALLBACK MessageProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        // �����츦 �����ϰ� ��� 
        GameEngineWindow::GetInst().Off();
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_CLOSE:
    {
        GameEngineWindow::GetInst().Off();
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
GameEngineWindow* GameEngineWindow::Inst_ = new GameEngineWindow();
// �������ڸ��� ����� ���� 

GameEngineWindow::GameEngineWindow()
    : hInst_(nullptr)   // ���μ���
    , hWnd_(nullptr)    // ������
    , WindowOn_(true)
    , HDC_(nullptr)
    , Scale_{}
{
}

GameEngineWindow::~GameEngineWindow()
{
    // �ڵ��� ��������� ��������� �Ѵ�
    if (nullptr != HDC_)
    {
        ReleaseDC(hWnd_, HDC_);
        HDC_ = nullptr;
    }

    if (nullptr != hWnd_)
    {
        DestroyWindow(hWnd_);
        hWnd_ = nullptr;
    }
}

void GameEngineWindow::Off()
{
    WindowOn_ = false;
}

void GameEngineWindow::RegClass(HINSTANCE _hInst)
{
    WNDCLASSEXA wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MessageProcess;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = _hInst;                         // �ܺο��� ���ڷ� �޾ƿͼ� ��� 
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = "GameEngineWindowClass";
    wcex.hIconSm = nullptr;

    RegisterClassExA(&wcex);
}

void GameEngineWindow::CreateGameWindow(HINSTANCE _hInst, const std::string& _Title)
{
    // ��� �ڵ� -> ���μ��� �ڵ��� �̹� ������ ����  
    if (nullptr != hInst_)
    {
        MsgBoxAssert("�����츦 2�� ������ �߽��ϴ�");
        return;
    }

    Title_ = _Title;

    // ���μ��� ����� �� ���� ���� (Ŭ����)
    hInst_ = _hInst;
    RegClass(_hInst);

    // ������ ���� (��ü)
    hWnd_ = CreateWindowExA(0L, "GameEngineWindowClass", Title_.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _hInst, nullptr);

    HDC_ = GetDC(hWnd_);

    if (!hWnd_)
    {
        return;
    }
}

// ������ ����ִ� �κ�
void GameEngineWindow::ShowGameWindow()
{
    // ��� �ڵ� -> ������ �ڵ��� ��������� ���� 
    if (nullptr == hWnd_)
    {
        MsgBoxAssert("���� �����찡 ��������� �ʾҽ��ϴ�. ȭ�鿡 ����� �� �����ϴ�. ");
        return;
    }

    ShowWindow(hWnd_, SW_SHOW);
    UpdateWindow(hWnd_);
}

// �޼����� ��ٸ��� ������ ����
// main���� GameLoop�Լ��� ���޵� �� 
void GameEngineWindow::MessageLoop(void(*_InitFunction)(), void(*_LoopFunction)())
{
    // ������� ���Դٸ� ������� �غ�� ��Ȳ
    // ������ ���� �� �غ��Ұ� �ִٸ� _Init���� �غ��ش޶�
    if (nullptr != _InitFunction)
    {
        _InitFunction();
    }

    MSG msg;


    while (WindowOn_)
    {
        if (0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // ������ �޼����� ó���� �� ���ӷ����� ó���ȴ�
        if (nullptr == _LoopFunction)
        {
            continue;
            // �Ʒ� ����_LoopFunction; ���� ������� �ʰ� �Ѿ 
        }

        _LoopFunction();
    }
}

void GameEngineWindow::SetWindowScaleAndPosition(float4 _Pos, float4 _Scale)
{
    // ������ �۾����� -> EngineWindow���� ũ�⸦ float���� �����ϱ� int����ȯ�ؼ� ���
    RECT Rc = { 0, 0, _Scale.ix(), _Scale.iy() };

    // �޴��ٸ� ������ �������� �۾����� ũ�⸦ ���´�
    AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, FALSE);

    Scale_ = _Scale;

    // ���� �۾������� ũ�⸦ ������� ������ ����� �����Ѵ� 
    SetWindowPos(hWnd_, nullptr
        , _Pos.ix(), _Pos.iy()
        , Rc.right - Rc.left
        , Rc.bottom - Rc.top        // y�� ���� ������ �۾����ϱ� B - T
        , SWP_NOZORDER);
}

