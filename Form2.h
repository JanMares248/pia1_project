#pragma once
#include "Form3.h"


using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;

public ref class Form2 : public Form
{
public:
    Form2(int count, System::Drawing::Size windowSize);

    static List<Point>^ savedPoints = gcnew List<Point>();

private:
    int count;
    int radius;
    array<Point>^ points;
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

