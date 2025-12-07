#include "Form2.h"
#include "Form3.h"
#include "Form4.h"
#include "Form5.h"
#include "Body.h"
#include "Case.h"
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
        //deffaultni naplneni
        savedSizes->Add(1);
    }
    while (savedSizes->Count > pointCount)
    {
        //odstrani nadbytecna telesa
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

    btnNext = gcnew Button();
    btnNext->Text = "Start Simulation";
    btnNext->Size = Drawing::Size(150, 30);
    btnNext->Location = Drawing::Point(this->ClientSize.Width - 180,
        this->ClientSize.Height - 50);
    btnNext->BackColor = Drawing::Color::White;
    btnNext->ForeColor = Drawing::Color::Black;
    btnNext->Click += gcnew EventHandler(this, &Form4::btnNext_Click);
    this->Controls->Add(btnNext);
    

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

        //cervena, pokud zvoleny
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

void NBody::InitializeSimulationData(System::Drawing::Size windowSize)
{

    // savedPoints ... polohy
    // savedPoints ... seznam objektu Point ... .X ; .Y parametry

    // savedArrowStates ... poc rychlost
    // parametry .length; .angle

    // savedSizes ... hmotnosti

     
   Case mainCase();
   int count = Form2::savedPoints->Count;
   if (count == 0) return;

   for (int i = 0; i < count; i++)
   {
       double x = (double)Form2::savedPoints[i].X;
       double y = (double)Form2::savedPoints[i].Y;


       Form3::ArrowState state = Form3::savedArrowStates[i];

       // delka sipky -> pocatecni vektor rychlost
       double speedMagnitude = (double)state.length / 10.0; // deleno 10

       // uhel v radianech
       double angle = state.angle;

       //V_x = |V| * cos(angle), V_y = |V| * sin(angle)
       double vx = speedMagnitude * System::Math::Cos(angle);
       double vy = speedMagnitude * System::Math::Sin(angle);

       // hmostnost m z savedSizes
       double mass = (double)Form4::savedSizes[i];

       //p = m * v
    //    double px = mass * vx;
    //    double py = mass * vy;

       mainCase.Add(Body b(x, y, vx, vy, mass));
   }

   mainCase.SolveEuSymp(1,50000,500);
}

void Form4::btnNext_Click(Object^ sender, EventArgs^ e)
{
    //NBody::InitializeSimulationData(this->ClientSize);

    Form5^ f5 = gcnew Form5(this->ClientSize);
    f5->Location = this->Location;
    f5->Show();
    this->Close();
}

void Form4::btnPrev_Click(Object^ sender, EventArgs^ e)
{

    Form3^ f3 = gcnew Form3(this->ClientSize);
    f3->Location = this->Location;
    f3->Show();
    this->Close();
}