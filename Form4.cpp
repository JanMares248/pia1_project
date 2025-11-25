#include "Form4.h"
#include <math.h>

int Form4::getRadius(int sizeSetting)
{
    int size = Math::Max(1, sizeSetting);
  
    double logFactor = Math::Log(size);

    double multiplier = 0.5;
    double scale = 1.0 + multiplier * logFactor;

    return (int)(baseRadius * scale);
}


void Form4::loadSizes()
{
    int pointCount = Form2::savedPoints->Count;

    while (savedSizes->Count < pointCount)
    {
        // default size 1
        savedSizes->Add(1);
    }
    while (savedSizes->Count > pointCount)
    {
        // Remove surplus
        savedSizes->RemoveAt(savedSizes->Count - 1);
    }
}

Form4::Form4(System::Drawing::Size s)
{
    this->Text = "Adjust Ball Sizes";
    this->ClientSize = s;
    this->DoubleBuffered = true;
    this->BackColor = System::Drawing::Color::Black;

    baseRadius = 10;
    selectedIndex = -1;
    sizeEditors = gcnew List<TextBox^>();


    loadSizes();


    for (int i = 0; i < Form2::savedPoints->Count; i++)
    {
        TextBox^ tb = gcnew TextBox();
        tb->Text = savedSizes[i].ToString();
        tb->Size = Drawing::Size(50, 20);
        tb->Tag = i;
        tb->Visible = false;
        tb->TextChanged += gcnew EventHandler(this, &Form4::SizeEditor_TextChanged);
        sizeEditors->Add(tb);
        this->Controls->Add(tb);
    }


    btnPrev = gcnew Button();
    btnPrev->Text = "Previous";
    btnPrev->Size = Drawing::Size(100, 30);
    btnPrev->Location = Drawing::Point(20, this->ClientSize.Height - 50);
    btnPrev->BackColor = Drawing::Color::White;
    btnPrev->ForeColor = Drawing::Color::Black;
    btnPrev->Click += gcnew EventHandler(this, &Form4::btnPrev_Click);
    this->Controls->Add(btnPrev);


    this->Paint += gcnew PaintEventHandler(this, &Form4::OnPaint);
    this->MouseDown += gcnew MouseEventHandler(this, &Form4::OnMouseDown);
}

void Form4::OnPaint(Object^ sender, PaintEventArgs^ e)
{
    Graphics^ g = e->Graphics;

    for (int i = 0; i < Form2::savedPoints->Count; i++)
    {
        Point p = Form2::savedPoints[i];
        int currentRadius = getRadius(savedSizes[i]);

        //zelena, pokud zvoleny
        Brush^ brush = (i == selectedIndex) ? Brushes::Red : Brushes::WhiteSmoke;

        g->FillEllipse(brush,
            p.X - currentRadius, p.Y - currentRadius,
            currentRadius * 2, currentRadius * 2);
    }
}


void Form4::OnMouseDown(Object^ sender, MouseEventArgs^ e)
{
    int oldSelectedIndex = selectedIndex;
    selectedIndex = -1;

    for (int i = 0; i < Form2::savedPoints->Count; i++)
    {
        Point p = Form2::savedPoints[i];
        int currentRadius = getRadius(savedSizes[i]);

        //vytvoreni dynamickeho ctverce kolem kruhu, podle jeho velikost
        //pokud uzivatel klikne do ctverce, zvoli se dany kruh
        double dx = e->X - p.X;
        double dy = e->Y - p.Y;
        double distanceSquared = dx * dx + dy * dy;

        if (distanceSquared <= currentRadius * currentRadius)
        {
            selectedIndex = i;
            break;
        }
    }


    for (int i = 0; i < Form2::savedPoints->Count; i++)
    {
        TextBox^ tb = sizeEditors[i];

        if (i == selectedIndex)
        {

            tb->Text = savedSizes[i].ToString();
            tb->Visible = true;


            Point p = Form2::savedPoints[i];
            int currentRadius = getRadius(savedSizes[i]);
            tb->Location = Drawing::Point(p.X - tb->Size.Width / 2, p.Y + currentRadius + 5);
        }
        else
        {
            tb->Visible = false;
        }
    }

    if (selectedIndex != oldSelectedIndex)
    {
        this->Invalidate();
    }
}


void Form4::SizeEditor_TextChanged(Object^ sender, EventArgs^ e)
{
    TextBox^ tb = safe_cast<TextBox^>(sender);
    int index = safe_cast<int>(tb->Tag);

    int newSize;

    if (Int32::TryParse(tb->Text, newSize))
    {

        if (newSize < 1) {
            newSize = 1;
        }

        savedSizes[index] = newSize;

        this->Invalidate();
    }
}


void Form4::btnPrev_Click(Object^ sender, EventArgs^ e)
{

    Form3^ f3 = gcnew Form3(this->ClientSize);
    f3->Location = this->Location;
    f3->Show();
    this->Close();
}