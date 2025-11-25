#pragma once
#include "Form2.h"
#include "Form4.h"


using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;

public value struct Arrow
{
public:
    Point basePoint;
    Point tipPoint;
    double angle;
    int length;

    Arrow(Point b, double a, int len);

};

public value struct ArrowState
{
    double angle;
    int length;
};



public ref class Form3 : public Form
{
public:

    Form3(System::Drawing::Size s);

    static List<ArrowState>^ savedArrowStates = gcnew List<ArrowState>();

private:
    List<Arrow>^ arrows;
    int dragIndex;
    Button^ btnPrev;
    Button^ btnNext;
    int arrowLen;

    void generateArrows();
    void OnPaint(Object^ sender, PaintEventArgs^ e);

    //pro drag and drop sipky
    void drawArrowHead(Graphics^ g, Point tip, double angle);
    void OnMouseDown(Object^ sender, MouseEventArgs^ e);
    void OnMouseMove(Object^ sender, MouseEventArgs^ e);
    void OnMouseUp(Object^ sender, MouseEventArgs^ e);
    void btnPrev_Click(Object^ sender, EventArgs^ e);
    void btnNext_Click(Object^ sender, EventArgs^ e);

    void updateSavedArrowStates();
    void saveCurrentArrowState(int index);
};

