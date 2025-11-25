#include "Form5.h"
#include "Form2.h"
#include "Form4.h"

Form5::Form5(System::Drawing::Size s)
{
    this->Text = "Simulation";
    this->ClientSize = s;
    this->DoubleBuffered = true;
    this->BackColor = System::Drawing::Color::Black;

    currentFrameIndex = 0;
    animationFrames = gcnew List<Frame>();

    SetupColors();

    LoadAnimationData("polohy.txt");

    if (animationFrames->Count == 0)
    {
        MessageBox::Show("Animation data failed to load or 'polohy.txt' is empty/missing/incorrect. Animation will not start.",
            "Debug: Data Load Failure", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    }

    //Timer
    animationTimer = gcnew Timer();
    animationTimer->Interval = 50; // 50ms -> 20 snimku za sekundu
    animationTimer->Tick += gcnew EventHandler(this, &Form5::AnimationTimer_Tick);

    // zacin animaci pokud se nacetli data
    if (animationFrames->Count > 0)
    {
        animationTimer->Start();
    }

    btnPrev = gcnew Button();
    btnPrev->Text = "Adjust Settings";
    btnPrev->Size = Drawing::Size(150, 30);
    btnPrev->Location = Drawing::Point(20, this->ClientSize.Height - 50);
    btnPrev->BackColor = Drawing::Color::White;
    btnPrev->ForeColor = Drawing::Color::Black;
    btnPrev->Click += gcnew EventHandler(this, &Form5::btnPrev_Click);
    this->Controls->Add(btnPrev);

    this->Paint += gcnew PaintEventHandler(this, &Form5::OnPaint);
}

void Form5::SetupColors()
{
    //seznam barev pro animaci trajektorii
    lineColors = gcnew List<Color>();
    lineColors->Add(Color::Red);
    lineColors->Add(Color::Lime);
    lineColors->Add(Color::Yellow);
    lineColors->Add(Color::Cyan);
    lineColors->Add(Color::Magenta);
    lineColors->Add(Color::Orange);
    lineColors->Add(Color::Blue);
    lineColors->Add(Color::White);
    lineColors->Add(Color::Brown);
    lineColors->Add(Color::Violet);
}

void Form5::LoadAnimationData(String^ filename)
{
    // Pocet teles je urcen poctem savedPoints
    int numBalls = Form2::savedPoints->Count;

    if (numBalls == 0)
    {
        MessageBox::Show("Form2::savedPoints is empty. You must set initial ball count and positions in Form1/Form2 before opening Form5.",
            "Debug: Missing Prerequisites", MessageBoxButtons::OK, MessageBoxIcon::Error);
        return;
    }

    String^ exePath = System::Windows::Forms::Application::ExecutablePath;
    String^ exeDirectory = System::IO::Path::GetDirectoryName(exePath);
    String^ fullFilePath = System::IO::Path::Combine(exeDirectory, filename);



    try
    {
        StreamReader^ sr = gcnew StreamReader(fullFilePath);
        String^ line;
        int lineNumber = 0;

        while ((line = sr->ReadLine()) != nullptr)
        {
            lineNumber++;

            if (line->StartsWith("#") || line->Trim() == "") continue;

            // format: X1,Y1;X2,Y2;...;Xn,Yn
            array<String^>^ ballPositions = line->Split(';');

            if (ballPositions->Length != numBalls)
            {
                System::Diagnostics::Debug::WriteLine(String::Format("Skipping line {0}: Expected {1} ball position pairs, but found {2}.",
                    lineNumber, numBalls, ballPositions->Length));
                continue;
            }

            Frame currentFrame;
            currentFrame.positions = gcnew List<Point>();
            bool validFrame = true;

            for (int i = 0; i < numBalls; i++)
            {
                array<String^>^ coords = ballPositions[i]->Split(',');

                if (coords->Length == 2)
                {
                    int x, y;
                    if (Int32::TryParse(coords[0]->Trim(), x) && Int32::TryParse(coords[1]->Trim(), y))
                    {
                        currentFrame.positions->Add(Point(x, y));
                    }
                    else
                    {
                        validFrame = false;
                        System::Diagnostics::Debug::WriteLine(String::Format("Line {0}, Ball {1}: Coordinate could not be parsed.", lineNumber, i + 1));
                        break;
                    }
                }
                else
                {
                    validFrame = false;
                    System::Diagnostics::Debug::WriteLine(String::Format("Line {0}, Ball {1}: Invalid coordinate format.", lineNumber, i + 1));
                    break;
                }
            }

            if (validFrame)
            {
                animationFrames->Add(currentFrame);
            }
        }

        sr->Close();
    }
    catch (FileNotFoundException^)
    {
        // pokud se soubor vubec nenacte
        MessageBox::Show("Error: 'polohy.txt' file not found. Ensure it is in the same directory as the executable:\n" + fullFilePath,
            "File Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
    catch (Exception^ ex)
    {
        // pokud je spatny format
        MessageBox::Show("Unexpected Error loading animation data: " + ex->Message,
            "File Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

void Form5::OnPaint(Object^ sender, PaintEventArgs^ e)
{
    Graphics^ g = e->Graphics;

    if (animationFrames->Count == 0 || currentFrameIndex >= animationFrames->Count)
        return;

    //aktualni snimek
    Frame currentFrame = animationFrames[currentFrameIndex];
    int numBalls = currentFrame.positions->Count;

    //kresleni trajektorii
    for (int i = 0; i < numBalls; i++)
    {
        Color lineColor = lineColors[i % lineColors->Count];
        Pen^ trailPen = gcnew Pen(lineColor, 1.0f);

        for (int j = 0; j < currentFrameIndex; j++)
        {
            //safety check
            if (animationFrames[j].positions->Count > i && animationFrames[j + 1].positions->Count > i)
            {
                Point start = animationFrames[j].positions[i];
                Point end = animationFrames[j + 1].positions[i];
                g->DrawLine(trailPen, start, end);
            }
        }
    }

    //vykresleni teles
    for (int i = 0; i < numBalls; i++)
    {
        Point p = currentFrame.positions[i];

        if (i >= Form4::savedSizes->Count) continue;

        int currentSizeSetting = Form4::savedSizes[i];

        int currentRadius = GetDynamicRadius(currentSizeSetting);

        g->FillEllipse(Brushes::WhiteSmoke,
            p.X - currentRadius, p.Y - currentRadius,
            currentRadius * 2, currentRadius * 2);
    }
}

void Form5::AnimationTimer_Tick(Object^ sender, EventArgs^ e)
{
    currentFrameIndex++;

    if (currentFrameIndex >= animationFrames->Count)
    {
        // zastaveni animace
        animationTimer->Stop();
        //cekat na poslednim snimku
        currentFrameIndex = animationFrames->Count - 1;
    }


    this->Invalidate();
}

void Form5::btnPrev_Click(Object^ sender, EventArgs^ e)
{
    animationTimer->Stop();
    Form4^ f4 = gcnew Form4(this->ClientSize);
    f4->Location = this->Location;
    f4->Show();
    this->Close();
}