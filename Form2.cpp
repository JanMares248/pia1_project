#include "Form2.h"
#include "Form3.h"
#include "Form4.h"
#include <math.h>



Form2::Form2(int c, System::Drawing::Size s)
{
    this->Text = "Selelct initial position";
    this->ClientSize = s;
    this->DoubleBuffered = true;
    this->BackColor = System::Drawing::Color::Black;

    count = c;

    dragIndex = -1;

    btnPrev = gcnew Button();
    btnPrev->Text = "Previous";
    btnPrev->Size = Drawing::Size(100, 30);
    btnPrev->Location = Drawing::Point(20, this->ClientSize.Height - 50);
    btnPrev->BackColor = Drawing::Color::White;
    btnPrev->ForeColor = Drawing::Color::Black;
    btnPrev->Click += gcnew EventHandler(this, &Form2::btnPrev_Click);
    this->Controls->Add(btnPrev);

    btnNext = gcnew Button();
    btnNext->Text = "Next";
    btnNext->Size = Drawing::Size(100, 30);
    btnNext->Location = Drawing::Point(this->ClientSize.Width - 130,
        this->ClientSize.Height - 50);
    btnNext->BackColor = Drawing::Color::White;
    btnNext->ForeColor = Drawing::Color::Black;
    btnNext->Click += gcnew EventHandler(this, &Form2::btnNext_Click);
    this->Controls->Add(btnNext);

    while (Form4::savedSizes->Count < count)
    {
        Form4::savedSizes->Add(1);
    }
    while (Form4::savedSizes->Count > count)
    {
        Form4::savedSizes->RemoveAt(Form4::savedSizes->Count - 1);
    }

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
}

void Form2::OnPaint(Object^ sender, PaintEventArgs^ e)
{
    for (int i = 0; i < count; i++)
    {

        int currentSizeSetting = Form4::savedSizes[i];
        int currentRadius = GetDynamicRadius(currentSizeSetting);

        //vykresli kruh, sirka a vyska jsou stejne
        e->Graphics->FillEllipse(Brushes::WhiteSmoke,
            savedPoints[i].X - currentRadius, savedPoints[i].Y - currentRadius,
            currentRadius * 2, currentRadius * 2);
    }
}

void Form2::OnMouseDown(Object^ sender, MouseEventArgs^ e)
{

    for (int i = 0; i < count; i++)
    {

        int currentSizeSetting = Form4::savedSizes[i];
        int currentRadius = GetDynamicRadius(currentSizeSetting);

        //vztvoreni virtualniho obdelniku obsahujici kruh pro indikaci kliknuti na kruh
        Rectangle r(savedPoints[i].X - currentRadius, savedPoints[i].Y - currentRadius,
            currentRadius * 2, currentRadius * 2);

        //pokud je kurzor v obdelniku
        if (r.Contains(e->Location))
        {
            //nastaveni dragindexu na dany kruh, jinak = -1
            dragIndex = i;

            //nastaveni Pointu dragOffset
            dragOffset = Point(e->X - savedPoints[i].X, e->Y - savedPoints[i].Y);
            break;
        }
    }
}

void Form2::OnMouseMove(Object^ sender, MouseEventArgs^ e)
{
    if (dragIndex >= 0)
    {  
        //dragoffset je Point, rozdil stareho a noveho bodu po souradnicich

        savedPoints[dragIndex] = Point(e->X - dragOffset.X, e->Y - dragOffset.Y);
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


