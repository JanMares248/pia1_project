#pragma once
#include "Form3.h"
#include "Form4.h"


using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;

inline int GetDynamicRadius(int sizeSetting)
{

    const int baseRadius = 10;
    const double multiplier = 0.5;

    int size = Math::Max(1, sizeSetting);

    double logFactor = Math::Log(size);
    double scale = 1.0 + multiplier * logFactor;

    return (int)(baseRadius * scale);
}


public ref class Form2 : public Form
{
public:
    Form2(int count, System::Drawing::Size windowSize);
    
    //globalni pole bodu
    static List<Point>^ savedPoints = gcnew List<Point>();

private: 
    int count;    
    int dragIndex;
    Point dragOffset;
    Button^ btnPrev;
    Button^ btnNext;

    void loadPoints();
    void OnPaint(Object^ sender, PaintEventArgs^ e);
    void OnMouseDown(Object^ sender, MouseEventArgs^ e);
    void OnMouseMove(Object^ sender, MouseEventArgs^ e);
    void OnMouseUp(Object^ sender, MouseEventArgs^ e);
    void btnPrev_Click(Object^ sender, EventArgs^ e);
    void btnNext_Click(Object^ sender, EventArgs^ e);
};

