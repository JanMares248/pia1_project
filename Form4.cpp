#include "Form4.h"
#include <math.h>

// Helper to calculate the actual radius based on the size setting (1=15, 2=30, etc.)
int Form4::getRadius(int sizeSetting)
{
    int size = Math::Max(1, sizeSetting);

    double radiusScale;

    const double C = 10.0;

    if (size == 1)
    {
        radiusScale = 1.0;
    }
    else
    {

        //radiusScale = 1.0 + Math::Log(size) / Math::Log(C) * (C / 2.0);

        radiusScale = 1.0 + Math::Log(size); // For size 1, Log(1)=0, scale=1.

        //radiusScale = 1.0 + 0.5 * Math::Log(size); // Adjust 0.5 for steeper/shallower curve
    }

    
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

    baseRadius = 15;
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
        //green if selelcted
        Brush^ brush = (i == selectedIndex) ? Brushes::Green : Brushes::Blue;

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

        if (newSize < 1) newSize = 1;

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