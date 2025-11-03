#pragma once
#include "Form2.h"


using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

public ref class Form1 : public Form
{
public:
    Form1()
    {
        this->Text = "Select Count";
        this->ClientSize = Drawing::Size(800, 400);

        comboBox1 = gcnew ComboBox();
        comboBox1->Size = Drawing::Size(100, 20);
        comboBox1->Location = Drawing::Point((this->ClientSize.Width - 100) / 2,
            (this->ClientSize.Height - 20) / 2);
        comboBox1->DropDownStyle = ComboBoxStyle::DropDownList;
        for (int i = 2; i <= 10; i++)
            comboBox1->Items->Add(i.ToString());
        comboBox1->SelectedIndex = 0;
        this->Controls->Add(comboBox1);

        buttonNext = gcnew Button();
        buttonNext->Text = "Next";
        buttonNext->Size = Drawing::Size(100, 30);
        buttonNext->Location = Drawing::Point(this->ClientSize.Width - 130,
            this->ClientSize.Height - 50);
        buttonNext->Click += gcnew EventHandler(this, &Form1::buttonNext_Click);
        this->Controls->Add(buttonNext);
    }

private:
    ComboBox^ comboBox1;
    Button^ buttonNext;

    void buttonNext_Click(Object^ sender, EventArgs^ e)
    {
        int count = Convert::ToInt32(comboBox1->SelectedItem);
        Form2^ f2 = gcnew Form2(count, this->ClientSize);
        f2->Location = this->Location;
        f2->Show();
        this->Hide();
    }
};

