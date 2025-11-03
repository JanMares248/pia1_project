#pragma once
#include "Form2.h"

namespace DragAndDrop {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace System::Collections::Generic;

    public ref class Arrow
    {
    public:
        Point basePoint;
        Point tipPoint;
        double angle;
        int length;

        Arrow(Point b, int len);
    };

    public ref class Form3 : public Form
    {
    public:
        Form3(System::Drawing::Size s);

    private:
        List<Arrow^>^ arrows;
        int dragIndex;
        Button^ btnPrev;
        int arrowLen;

        void generateArrows();
        void OnPaint(Object^ sender, PaintEventArgs^ e);
        void drawArrowHead(Graphics^ g, Point tip, double angle);
        void OnMouseDown(Object^ sender, MouseEventArgs^ e);
        void OnMouseMove(Object^ sender, MouseEventArgs^ e);
        void OnMouseUp(Object^ sender, MouseEventArgs^ e);
        void btnPrev_Click(Object^ sender, EventArgs^ e);
    };
}
