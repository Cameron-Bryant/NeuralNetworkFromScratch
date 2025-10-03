#include "framework.h"
#include "CPlusPlusPlot.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#define MAX_LOADSTRING 100

int HEIGHT;
int WIDTH;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
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

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CPLUSPLUSPLOT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CPLUSPLUSPLOT));

    MSG msg;

    // Main message loop:
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

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CPLUSPLUSPLOT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CPLUSPLUSPLOT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, SW_MAXIMIZE); //be maximized when shown

    if (!hWnd)
    {
        return FALSE;
    }
    UpdateWindow(hWnd);

    return TRUE;
}

std::vector<double> parseString(const std::string& stringToParse, std::vector<double>& nameVector) {
    //split
    std::vector<double> result;
    std::string token = "";
    int commaCount = 0;

    for (int i = 0; i < stringToParse.size(); i++) {
        if (stringToParse[i] != ',') { //if not comma put char in token
            token.push_back(stringToParse[i]);
        }
        else { 
            commaCount += 1;
            if (commaCount < 4) { 
                result.push_back(strtod(token.c_str(), NULL));
                token = "";
            }
            else if (commaCount == 4) { 
                result.push_back(strtod(token.c_str(), NULL));
                token = stringToParse.substr(i + 2, stringToParse.size() - (i + 2) - 1);
                if (token == "Setosa") {
                    nameVector.push_back(1.0);
                }
                else if (token == "Versicolor" || token == "Virginica") {
                    nameVector.push_back(-1.0);
                }
                break;
            }
        }
    }
    return result;
}

std::vector<double> getWeightVectorFromCSV(const std::string& line) {
    std::vector<double> weightVector;
    std::istringstream iss(line);
    std::string weight;
    while (std::getline(iss >> std::ws, weight, ',')) {
        if (weight != "") {
            weightVector.push_back(std::stod(weight));
        }
    }
    return weightVector;
}

class Point {
public:
    Point() {}
    Point(const double x, const double y) { this->x = x; this->y = y; }

    std::vector<double> getScreenCoords(double screenHeight, double screenWidth, double scale = 5) {
        std::vector<double> screenPoint = { (screenWidth / 2) + (x * (screenWidth / (scale * 2))),
                                            (screenHeight / 2) - (y * (screenHeight / (scale * 2))) };
        return screenPoint;
    }

    void setX(const double xIn) {
        x = xIn;
    }

    void setY(const double yIn) {
        y = yIn;
    }

    double getX() const { return x; }
    double getY() const { return y; }
    
private:
    double x;
    double y;
};

class Line { 
public:
    Line(const Point& startPoint, const Point& endPoint, double width, double height) {
        slope = (endPoint.getY() - startPoint.getY()) / (endPoint.getX() - startPoint.getX());
        points.push_back(startPoint);
        points.push_back(endPoint);
        intercept = startPoint.getY() - (slope * startPoint.getX()); //y = mx + b -> b = y - mx
        this->width = width;
        this->height = height;
    }

    Point getPointOnLine(double x) const {
        Point p(x, (x * slope) + intercept);
        return p;
    }

    Point getWindowPoint(const Point& p) {
        Point windowCoord;
        windowCoord.setX((width / 2) + (p.getX() * (width / 10)));
        windowCoord.setY((height / 2) + (p.getY() * (height / 10)));
        return windowCoord;
    }

private:
    std::vector<Point> points;
    double slope;
    double intercept;
    double width;
    double height;

};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            RECT clientRect;
            if (GetClientRect(hWnd, &clientRect)) { 
                HEIGHT = clientRect.bottom - clientRect.top;
                WIDTH = clientRect.right - clientRect.left;
            }

            //Draw Axes
            for (int i = 0; i < WIDTH; i++) {
                SetPixelV(hdc, i, HEIGHT / 2, RGB(0, 0, 0));
            }
 
            for (int i = 0; i < HEIGHT; i++) {
                SetPixelV(hdc, WIDTH / 2, i, RGB(0, 0, 0));
            }

            //get Iris data and perceptron weights
            std::vector<std::string> irisCSV;
            std::vector<double> irisClassifications;
            std::vector<std::vector<double>> irisPlotValues;
            std::ifstream file("C:/Users/camer/Desktop/iris.csv");
            std::string line;
            while (std::getline(file, line)) {
                irisCSV.push_back(line);
            }
            file.close();

            for (int i = 0; i < irisCSV.size(); i++) {
                line = irisCSV[i];
                irisPlotValues.push_back(parseString(line, irisClassifications));
            }
            
            std::vector<Point> points;
            for (int i = 0; i < irisPlotValues.size(); i++) {
                Point p(irisPlotValues[i][1], irisPlotValues[i][3]);
                points.push_back(p);
            }

            std::vector<double> pointCoords;
            for (int i = 0; i < points.size(); i++) {
                pointCoords = points[i].getScreenCoords(HEIGHT, WIDTH);
                SetPixelV(hdc, pointCoords[0], pointCoords[1], RGB(255, 0, 0));
            }
                          
            file.open("C:/Users/camer/Desktop/weights.txt"); 
            std::vector<std::vector<double>> weights;
            if (file.is_open()) {
                while (std::getline(file, line)) {
                    weights.push_back(getWeightVectorFromCSV(line));
                }
            }
            file.close();
            
            //draw decision boundary over time in training
            double w1;
            double w2;
            double bias;
            double firstX;
            double lastX;
            double firstY;
            double lastY;
            double sum;

            Point maxPoint;
            Point minPoint;
            maxPoint = points[0];
            minPoint = points[0];

            for (int i = 0; i < points.size(); i++) {
                if (points[i].getX() > maxPoint.getX()) {
                    maxPoint = points[i];
                }
                if (points[i].getX() < minPoint.getX()) {
                    minPoint = points[i];
                }
            }
            
            for (int i = 0; i < 12; i++) { 
                w1 = weights[i][1];
                w2 = weights[i][3];
                bias = weights[i][4];
                firstX = minPoint.getX();
                lastX = maxPoint.getX();
                firstY = ((-(bias / w2) / (bias / w1)) * firstX) + (-bias / w2);
                lastY = ((-(bias / w2) / (bias / w1)) * lastX) + (-bias / w2);

                Point firstPoint(firstX, firstY);
                Point lastPoint(lastX, lastY);

                Line line(firstPoint, lastPoint, WIDTH, HEIGHT);

                for (double j = firstPoint.getX(); j < lastPoint.getX(); j += .001) {
                    Point pixelOnDecisionBoundary;
                    pixelOnDecisionBoundary = line.getPointOnLine(j);
                    pixelOnDecisionBoundary = line.getWindowPoint(pixelOnDecisionBoundary);
                    SetPixelV(hdc, pixelOnDecisionBoundary.getX(), pixelOnDecisionBoundary.getY(), RGB(0, 255, 0));
                }
            }
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

// Message handler for about box.
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
