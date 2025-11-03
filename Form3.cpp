#include "Form3.h"
#include <math.h>

namespace DragAndDrop {

    Arrow::Arrow(Point b, int len)
    {
        basePoint = b;
        length = len;
        Random^ rnd = gcnew Random();
        angle = rnd->NextDouble() * 2 * Math::PI;
        tipPoint = Point((int)(b.X + length * Math::Cos(angle)),
            (int)(b.Y + length * Math::Sin(angle)));
    }

    Form3::Form3(System::Drawing::Size s)
    {
        this->Text = "Arrow Vectors";
        this->ClientSize = s;
        this->DoubleBuffered = true;

        arrowLen = 80;
        dragIndex = -1;
        arrows = gcnew List<Arrow^>();

        btnPrev = gcnew Button();
        btnPrev->Text = "Previous";
        btnPrev->Size = Drawing::Size(100, 30);
        btnPrev->Location = Drawing::Point(20, this->ClientSize.Height - 50);
        btnPrev->Click += gcnew EventHandler(this, &Form3::btnPrev_Click);
        this->Controls->Add(btnPrev);

        generateArrows();

        this->Paint += gcnew PaintEventHandler(this, &Form3::OnPaint);
        this->MouseDown += gcnew MouseEventHandler(this, &Form3::OnMouseDown);
        this->MouseMove += gcnew MouseEventHandler(this, &Form3::OnMouseMove);
        this->MouseUp += gcnew MouseEventHandler(this, &Form3::OnMouseUp);
    }

    void Form3::generateArrows()
    {
        arrows->Clear();
        for each (Point p in Form2::savedPoints)
        {
            arrows->Add(gcnew Arrow(p, arrowLen));
        }
    }

    void Form3::OnPaint(Object^ sender, PaintEventArgs^ e)
    {
        Graphics^ g = e->Graphics;

        for each (Point p in Form2::savedPoints)
            g->FillEllipse(Brushes::Blue, p.X - 15, p.Y - 15, 30, 30);

        for each (Arrow ^ a in arrows)
        {
            Pen^ pen = gcnew Pen(Color::Red, 2);
            g->DrawLine(pen, a->basePoint, a->tipPoint);
            drawArrowHead(g, a->tipPoint, a->angle);
        }
    }

    void Form3::drawArrowHead(Graphics^ g, Point tip, double angle)
    {
        int size = 10;
        array<PointF>^ pts = {
            PointF((float)tip.X, (float)tip.Y),
            PointF((float)(tip.X - size * Math::Cos(angle - Math::PI / 6)),
                   (float)(tip.Y - size * Math::Sin(angle - Math::PI / 6))),
            PointF((float)(tip.X - size * Math::Cos(angle + Math::PI / 6)),
                   (float)(tip.Y - size * Math::Sin(angle + Math::PI / 6)))
        };
        g->FillPolygon(Brushes::Green, pts);
    }

    void Form3::OnMouseDown(Object^ sender, MouseEventArgs^ e)
    {
        for (int i = 0; i < arrows->Count; i++)
        {
            Arrow^ a = arrows[i];
            Rectangle r(a->tipPoint.X - 6, a->tipPoint.Y - 6, 12, 12);
            if (r.Contains(e->Location))
            {
                dragIndex = i;
                break;
            }
        }
    }

    void Form3::OnMouseMove(Object^ sender, MouseEventArgs^ e)
    {
        if (dragIndex >= 0)
        {
            Arrow^ a = arrows[dragIndex];
            double dx = e->X - a->basePoint.X;
            double dy = e->Y - a->basePoint.Y;
            a->angle = Math::Atan2(dy, dx);
            a->length = (int)Math::Sqrt(dx * dx + dy * dy);
            a->tipPoint = Point((int)(a->basePoint.X + a->length * Math::Cos(a->angle)),
                (int)(a->basePoint.Y + a->length * Math::Sin(a->angle)));
            this->Invalidate();
        }
    }

    void Form3::OnMouseUp(Object^ sender, MouseEventArgs^ e)
    {
        dragIndex = -1;
    }

    void Form3::btnPrev_Click(Object^ sender, EventArgs^ e)
    {
        if (Application::OpenForms->Count > 1)
            Application::OpenForms[1]->Show();
        this->Close();
    }
}
