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
        this->BackColor = System::Drawing::Color::Black;

        comboBox1 = gcnew ComboBox();
        comboBox1->Size = Drawing::Size(100, 20);
        comboBox1->Location = Drawing::Point((this->ClientSize.Width - 100) / 2,
            (this->ClientSize.Height - 20) / 2);
        comboBox1->DropDownStyle = ComboBoxStyle::DropDownList;
        for (int i = 2; i <= 10; i++)
            comboBox1->Items->Add(i.ToString());
        comboBox1->SelectedIndex = 0;
        this->Controls->Add(comboBox1);

        btnNext = gcnew Button();
        btnNext->Text = "Next";
        btnNext->Size = Drawing::Size(100, 30);
        btnNext->Location = Drawing::Point(this->ClientSize.Width - 130,
            this->ClientSize.Height - 50);
        btnNext->BackColor = Drawing::Color::White;
        btnNext->ForeColor = Drawing::Color::Black;
        btnNext->Click += gcnew EventHandler(this, &Form1::btnNext_Click);
        this->Controls->Add(btnNext);
    }

private:
    ComboBox^ comboBox1;
    Button^ btnNext;

    void btnNext_Click(Object^ sender, EventArgs^ e)
    {
        int count = Convert::ToInt32(comboBox1->SelectedItem);
        Form2^ f2 = gcnew Form2(count, this->ClientSize);
        f2->Location = this->Location;
        f2->Show();
        this->Hide();
    }
};

