#include "Form3.h"
#include "Form4.h"
#include <math.h>


//konstruktor
Arrow::Arrow(Point b, double a, int len)
{
    basePoint = b;
    angle = a;
    length = len;

    //vzpocet pozice hrotu v polarnich souradnicich
    tipPoint = Point((int)(b.X + length * Math::Cos(angle)),
        (int)(b.Y + length * Math::Sin(angle)));
}

Form3::Form3(System::Drawing::Size s)
{
    this->Text = "Initial Velocity Vectors";
    this->ClientSize = s;
    this->DoubleBuffered = true;
    this->BackColor = System::Drawing::Color::Black;

    arrowLen = 80;
    dragIndex = -1;
    arrows = gcnew List<Arrow>();

    btnPrev = gcnew Button();
    btnPrev->Text = "Previous";
    btnPrev->Size = Drawing::Size(100, 30);
    btnPrev->Location = Drawing::Point(20, this->ClientSize.Height - 50);
    btnPrev->BackColor = Drawing::Color::White;
    btnPrev->ForeColor = Drawing::Color::Black;
    btnPrev->Click += gcnew EventHandler(this, &Form3::btnPrev_Click);
    this->Controls->Add(btnPrev);

    btnNext = gcnew Button();
    btnNext->Text = "Next";
    btnNext->Size = Drawing::Size(100, 30);
    btnNext->Location = Drawing::Point(this->ClientSize.Width - 130,
        this->ClientSize.Height - 50);
    btnNext->BackColor = Drawing::Color::White;
    btnNext->ForeColor = Drawing::Color::Black;
    btnNext->Click += gcnew EventHandler(this, &Form3::btnNext_Click);
    this->Controls->Add(btnNext);

    updateSavedArrowStates();

    generateArrows();

    this->Paint += gcnew PaintEventHandler(this, &Form3::OnPaint);
    this->MouseDown += gcnew MouseEventHandler(this, &Form3::OnMouseDown);
    this->MouseMove += gcnew MouseEventHandler(this, &Form3::OnMouseMove);
    this->MouseUp += gcnew MouseEventHandler(this, &Form3::OnMouseUp);
}

void Form3::updateSavedArrowStates()
{

    while (savedArrowStates->Count < Form2::savedPoints->Count)
    {

        Random^ rnd = gcnew Random();
        double initialAngle = rnd->NextDouble() * 2 * Math::PI;
        

        ArrowState newState;
        newState.angle = initialAngle;
        newState.length = arrowLen;
        savedArrowStates->Add(newState);
    }
    while (savedArrowStates->Count > Form2::savedPoints->Count)
    {
        savedArrowStates->RemoveAt(savedArrowStates->Count - 1);
    }
}

void Form3::generateArrows()
{
    arrows->Clear();
    for (int i = 0; i < Form2::savedPoints->Count; i++)
    {
        Point p = Form2::savedPoints[i];
        ArrowState state = savedArrowStates[i];

        arrows->Add(Arrow(p, state.angle, state.length));
    }
}

void Form3::OnPaint(Object^ sender, PaintEventArgs^ e)
{
    Graphics^ g = e->Graphics;

    for (int i = 0; i < Form2::savedPoints->Count; i++)
    {
        Point p = Form2::savedPoints[i];


        int currentSizeSetting = Form4::savedSizes[i];
        int currentRadius = GetDynamicRadius(currentSizeSetting);

        g->FillEllipse(Brushes::WhiteSmoke,
            p.X - currentRadius, p.Y - currentRadius,
            currentRadius * 2, currentRadius * 2);
    }

    for each (Arrow a in arrows)
    {
        //pro nakresleni cervene cary sipky
        Pen^ pen = gcnew Pen(Color::Red, 1.2);
        g->DrawLine(pen, a.basePoint, a.tipPoint);

        drawArrowHead(g, a.tipPoint, a.angle);
    }
}

void Form3::drawArrowHead(Graphics^ g, Point tip, double angle)
{
    int size = 15;
    array<PointF>^ pts = {
        PointF((float)tip.X, (float)tip.Y),
        PointF((float)(tip.X - size * Math::Cos(angle - Math::PI / 6)),
                (float)(tip.Y - size * Math::Sin(angle - Math::PI / 6))),
        PointF((float)(tip.X - size * Math::Cos(angle + Math::PI / 6)),
                (float)(tip.Y - size * Math::Sin(angle + Math::PI / 6)))
    };
    g->FillPolygon(Brushes::Blue, pts);
}

void Form3::OnMouseDown(Object^ sender, MouseEventArgs^ e)
{
    for (int i = 0; i < arrows->Count; i++)
    {
        Arrow a = arrows[i];
        //vygenerovani obdelniku okolo konce sipky
        Rectangle r(a.tipPoint.X - 20, a.tipPoint.Y - 20, 40, 40);
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
        //zvoleni sipky, kterou budeme hybat
        Arrow a = arrows[dragIndex]; // Zde se naète kopie

        double dx = e->X - a.basePoint.X;
        double dy = e->Y - a.basePoint.Y;
        a.angle = Math::Atan2(dy, dx);
        a.length = (int)Math::Sqrt(dx * dx + dy * dy);
        a.tipPoint = Point((int)(a.basePoint.X + a.length * Math::Cos(a.angle)),
            (int)(a.basePoint.Y + a.length * Math::Sin(a.angle)));

        //ulozeni modifikovane kopie zpet do seznamu
        arrows[dragIndex] = a;

        saveCurrentArrowState(dragIndex);

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
    {
        Application::OpenForms[1]->Show();
        this->Close();
    }
}

void Form3::btnNext_Click(Object^ sender, EventArgs^ e)
{
    Form4^ f4 = gcnew Form4(this->ClientSize);
    f4->Location = this->Location;
    f4->Show();
    this->Hide();
}
 
void Form3::saveCurrentArrowState(int index)
{
    if (index >= 0 && index < arrows->Count)
    {
        Arrow a = arrows[index];

        ArrowState state = savedArrowStates[index]; //nacteni kopie stavu
        state.angle = a.angle;
        state.length = a.length;

        savedArrowStates[index] = state; //ulozeni kopie stavu
    }
}



