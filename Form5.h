#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::IO;


public value struct Frame
{
public:
    List<Point>^ positions;
};


public ref class Form5 : public Form
{
public:
    Form5(System::Drawing::Size s);

private:
    Timer^ animationTimer;
    Button^ btnPrev;
    int currentFrameIndex;
    List<Frame>^ animationFrames;
    List<Color>^ lineColors;

    void LoadAnimationData(String^ filename);
    void SetupColors();
    void OnPaint(Object^ sender, PaintEventArgs^ e);
    void AnimationTimer_Tick(Object^ sender, EventArgs^ e);
    void btnPrev_Click(Object^ sender, EventArgs^ e);
};