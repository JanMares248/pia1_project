#pragma once
#include "Form3.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;

public ref class Form4 : public Form
{
public:
    Form4(System::Drawing::Size s);

    static List<int>^ savedSizes = gcnew List<int>();

private:
    int baseRadius;
    int selectedIndex;
    List<TextBox^>^ sizeEditors;


    int getRadius(int sizeSetting);

    void loadSizes();


    void OnPaint(Object^ sender, PaintEventArgs^ e);
    void OnMouseDown(Object^ sender, MouseEventArgs^ e);
    void SizeEditor_TextChanged(Object^ sender, EventArgs^ e);

    Button^ btnPrev;
    Button^ btnNext;
    void btnPrev_Click(Object^ sender, EventArgs^ e);
    void btnNext_Click(Object^ sender, EventArgs^ e);
};
