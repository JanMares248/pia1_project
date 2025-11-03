#include "Form2.h"



Form2::Form2(int c, System::Drawing::Size s)
{
    this->Text = "Drag Circles";
    this->ClientSize = s;
    this->DoubleBuffered = true;

    count = c;
    radius = 15;
    dragIndex = -1;
    points = gcnew array<Point>(count);

    btnPrev = gcnew Button();
    btnPrev->Text = "Previous";
    btnPrev->Size = Drawing::Size(100, 30);
    btnPrev->Location = Drawing::Point(20, this->ClientSize.Height - 50);
    btnPrev->Click += gcnew EventHandler(this, &Form2::btnPrev_Click);
    this->Controls->Add(btnPrev);

    btnNext = gcnew Button();
    btnNext->Text = "Next";
    btnNext->Size = Drawing::Size(100, 30);
    btnNext->Location = Drawing::Point(this->ClientSize.Width - 130,
        this->ClientSize.Height - 50);
    btnNext->Click += gcnew EventHandler(this, &Form2::btnNext_Click);
    this->Controls->Add(btnNext);

    loadPoints();

    this->Paint += gcnew PaintEventHandler(this, &Form2::OnPaint);
    this->MouseDown += gcnew MouseEventHandler(this, &Form2::OnMouseDown);
    this->MouseMove += gcnew MouseEventHandler(this, &Form2::OnMouseMove);
    this->MouseUp += gcnew MouseEventHandler(this, &Form2::OnMouseUp);
}

void Form2::loadPoints()
{
    Random^ rnd = gcnew Random();
    while (savedPoints->Count < count)
    {
        savedPoints->Add(Point(rnd->Next(50, ClientSize.Width - 50),
            rnd->Next(50, ClientSize.Height - 50)));
    }
    while (savedPoints->Count > count)
    {
        savedPoints->RemoveAt(savedPoints->Count - 1);
    }
    for (int i = 0; i < count; i++)
    {
        points[i] = savedPoints[i];
    }
}

void Form2::OnPaint(Object^ sender, PaintEventArgs^ e)
{
    for (int i = 0; i < count; i++)
    {
        e->Graphics->FillEllipse(Brushes::Blue,
            points[i].X - radius, points[i].Y - radius, radius * 2, radius * 2);
    }
}

void Form2::OnMouseDown(Object^ sender, MouseEventArgs^ e)
{
    for (int i = 0; i < count; i++)
    {
        Rectangle r(points[i].X - radius, points[i].Y - radius, radius * 2, radius * 2);
        if (r.Contains(e->Location))
        {
            dragIndex = i;
            dragOffset = Point(e->X - points[i].X, e->Y - points[i].Y);
            break;
        }
    }
}

void Form2::OnMouseMove(Object^ sender, MouseEventArgs^ e)
{
    if (dragIndex >= 0)
    {
        points[dragIndex] = Point(e->X - dragOffset.X, e->Y - dragOffset.Y);
        savedPoints[dragIndex] = points[dragIndex];
        this->Invalidate();
    }
}

void Form2::OnMouseUp(Object^ sender, MouseEventArgs^ e)
{
    dragIndex = -1;
}

void Form2::btnPrev_Click(Object^ sender, EventArgs^ e)
{
    Application::OpenForms[0]->Show();
    this->Close();
}

void Form2::btnNext_Click(Object^ sender, EventArgs^ e)
{
    Form3^ f3 = gcnew Form3(this->ClientSize);
    f3->Location = this->Location;
    f3->Show();
    this->Hide();
}

