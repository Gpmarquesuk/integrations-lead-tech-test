#include "framework.h"
#include "WindowsAPIProject.h"

#define MAX_LOADSTRING 100
#define MIN_WINDOW_WIDTH 200
#define MIN_WINDOW_HEIGHT 150
#define CONFIG_FILE_PATH L"config.ini"
#define LOG_FILE_PATH L"app.log"


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hWndMain;                                  // Handle for the main window

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);   // Function declaration to register a custom window class
BOOL                InitInstance(HINSTANCE, int);           // Function declaration to initialize the application instance 
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);    // Declaration of the callback function for handling messages that the application receives from Windows events.
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);      // Declaration of the callback function for handling messages related to the "About" dialog box

// Structure to store configuration settings
struct Config
{
    int windowX;         // X-coordinate of the window position
    int windowY;         // Y-coordinate of the window position
    int windowWidth;     // Width of the window
    int windowHeight;    // Height of the window
};


Config g_config;        // Global configuration instance to store window-related settings
POINT initialPosition;  // Store the initial position when dragging or resizing starts
SIZE initialSize;       // Store the initial window size

// Log function to record messages to a log file
void Log(const wchar_t* message)
{

    auto now = std::chrono::system_clock::now();                     // Get the current time
    auto timeNow = std::chrono::system_clock::to_time_t(now);        // Convert the current time to time_t
    // Use localtime_s to convert time_t to a tm structure
    struct tm timeinfo;
    localtime_s(&timeinfo, &timeNow);

    // Open the log file in append mode
    std::wofstream logFile(LOG_FILE_PATH, std::ios_base::app);

    if (logFile.is_open())
    {
        // Write timestamp and message to the log file
        logFile << L"[" << std::put_time(&timeinfo, L"%Y-%m-%d %X") << L"] " << message << std::endl;
        logFile.close();
    }
}

void LoadConfig()
{
    // Log that configuration loading is initiated
    Log(L"Loading configuration...");

    // Default values for the configuration
    g_config.windowX = CW_USEDEFAULT;
    g_config.windowY = 0;
    g_config.windowWidth = CW_USEDEFAULT;
    g_config.windowHeight = 0;

    // Load configuration from file
    std::wifstream configFile(CONFIG_FILE_PATH);

    // Check if the configuration file is open
    if (configFile.is_open())
    {
        // Read configuration values from the file
        configFile >> g_config.windowX >> g_config.windowY >> g_config.windowWidth >> g_config.windowHeight;

        // Close the configuration file
        configFile.close();
    }
    // If the configuration file is not open, default values will be used
}

void SaveConfig()
{
    // Log that configuration saving is initiated
    Log(L"Saving configuration...");

    // Save configuration to file
    std::wofstream configFile(CONFIG_FILE_PATH);

    // Check if the configuration file is open
    if (configFile.is_open())
    {
        // Write configuration values to the file
        configFile << g_config.windowX << L" " << g_config.windowY << L" " << g_config.windowWidth << L" " << g_config.windowHeight;

        // Close the configuration file
        configFile.close();
    }
    // If the configuration file is not open, the configuration won't be saved
}

// Entry point for the Windows application
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    // Suppress unused parameter warnings
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // Load the application title and window class strings from the resource file
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSAPIPROJECT, szWindowClass, MAX_LOADSTRING);
    // Register the application's window class
    MyRegisterClass(hInstance);
    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        // If initialization fails, return FALSE to indicate failure
        return FALSE;
    }
    // Load accelerators from the resource file
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSAPIPROJECT));

    // Main message loop to handle Windows messages
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        // Translate and dispatch messages, and handle accelerators
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    // Return the exit code of the application
    return (int)msg.wParam;
}
//  FUNCTION: MyRegisterClass()
//  PURPOSE: Registers the window class.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSAPIPROJECT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSAPIPROJECT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

// Function to initialize the application instance and create the main window
// Parameters:
//   - nCmdShow: Specifies how the window is to be shown 
// Returns:
//   - BOOL: Indicates the success or failure of the initialization
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    // Set the global instance handle to the provided value
    hInst = hInstance;
    // Load configuration settings from a file
    LoadConfig();

    // Create the main application window
    hWndMain = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        g_config.windowX, g_config.windowY, g_config.windowWidth, g_config.windowHeight, nullptr, nullptr, hInstance, nullptr);

    // Check if window creation was successful
    if (!hWndMain)
    {
        // If window creation fails, return FALSE to indicate initialization failure
        return FALSE;
    }
    // Show the main window according to the specified show command
    ShowWindow(hWndMain, nCmdShow);
    // Force an immediate update of the main window
    UpdateWindow(hWndMain);
    // Return TRUE to indicate successful initialization
    return TRUE;
}


// Window procedure to handle messages that the application receives from Windows when events occur
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static RECT initialRect;  // Store the initial window rectangle

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            // Display the "About" dialog box
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            // User chose to exit, destroy the main window
            DestroyWindow(hWnd);
            break;
        default:
            // Pass unhandled commands to the default window procedure
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_SIZE:
    {
        // Update configuration with the new window size
        RECT rect;
        GetWindowRect(hWnd, &rect);
        g_config.windowWidth = rect.right - rect.left;
        g_config.windowHeight = rect.bottom - rect.top;
    }
    break;
    case WM_GETMINMAXINFO:
    {
        // Set minimum tracking size for the window
        MINMAXINFO* pMinMax = reinterpret_cast<MINMAXINFO*>(lParam);
        pMinMax->ptMinTrackSize.x = MIN_WINDOW_WIDTH;
        pMinMax->ptMinTrackSize.y = MIN_WINDOW_HEIGHT;
    }
    break;
    case WM_MOVE:
    {
        RECT rect;
        GetWindowRect(hWnd, &rect);
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        g_config.windowX = rect.left;
        g_config.windowY = rect.top;

        int newLeft = rect.left;
        int newTop = rect.top;
        int newRight = rect.right;
        int newBottom = rect.bottom;

        // Adjust the window position if it goes beyond the screen edges
        if (rect.left < 0) {
            newLeft = 0;
            newRight = rect.right - rect.left;
        }
        if (rect.top < 0) {
            newTop = 0;
            newBottom = rect.bottom - rect.top;
        }

        if (rect.right > screenWidth) {
            newRight = screenWidth;
            newLeft = newRight - (rect.right - rect.left);
        }
        if (rect.bottom > screenHeight) {
            newBottom = screenHeight;
            newTop = newBottom - (rect.bottom - rect.top);
        }
        // Set the adjusted window position
        SetWindowPos(hWnd, HWND_TOP, newLeft, newTop, newRight - newLeft, newBottom - newTop, SWP_NOZORDER | SWP_NOSIZE);
    }
    break;

    case WM_ENTERSIZEMOVE:
        // The user has started dragging or resizing the window
        GetWindowRect(hWnd, &initialRect);
        initialPosition.x = initialRect.left;
        initialPosition.y = initialRect.top;
        initialSize.cx = initialRect.right - initialRect.left;
        initialSize.cy = initialRect.bottom - initialRect.top;
        break;

    case WM_EXITSIZEMOVE:
    {
        // The user has finished dragging or resizing the window
        RECT rect;
        GetWindowRect(hWnd, &rect);
        POINT finalPosition = { rect.left, rect.top };
        SIZE finalSize = { rect.right - rect.left, rect.bottom - rect.top };

        // Check if the position or size has changed and log the event
        if (initialPosition.x != finalPosition.x || initialPosition.y != finalPosition.y)
        {
            wchar_t logMessage[100];
            swprintf_s(logMessage, L"Window moved from (%d,%d) to (%d,%d)", initialPosition.x, initialPosition.y, finalPosition.x, finalPosition.y);
            Log(logMessage);
        }
        else if (initialSize.cx != finalSize.cx || initialSize.cy != finalSize.cy)
        {
            // Log window size change event
            wchar_t logMessage[100];
            swprintf_s(logMessage, L"Size changed from (%d,%d) to (%d,%d)", initialSize.cx, initialSize.cy, finalSize.cx, finalSize.cy);
            Log(logMessage);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        WCHAR greeting[] = _T("Mirada technical challenge");
        // For this introduction, we just print out "Mirada technical challenge"
        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        // Save the current configuration and exit the application
        SaveConfig();
        PostQuitMessage(0);
        break;
    default:
        // Pass unhandled messages to the default window procedure
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Suppress unused parameter warnings
    UNREFERENCED_PARAMETER(lParam);

    // Handle different messages received by the dialog box
    switch (message)
    {
        // Initialization message for the dialog
    case WM_INITDIALOG:
        // Return TRUE to set the focus to the first control in the dialog
        return (INT_PTR)TRUE;

        // Command message (button click, etc.)
    case WM_COMMAND:
        // Check for specific button IDs (IDOK or IDCANCEL)
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            // Close the dialog box when OK or Cancel is clicked
            EndDialog(hDlg, LOWORD(wParam));
            // Return TRUE to indicate that the message has been handled
            return (INT_PTR)TRUE;
        }
        break;
    }
    // Return FALSE to indicate that the message has not been fully handled
    return (INT_PTR)FALSE;
}





