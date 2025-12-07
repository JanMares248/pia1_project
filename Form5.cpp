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
    String^ fullFilePath = GetPolohyFilePath();
    animationFrames->Clear();

    if (!File::Exists(fullFilePath))
    {
        MessageBox::Show("The file 'polohy.txt' was not found in the application directory. Starting with an empty animation.", "Warning");
        Frame emptyFrame;
        emptyFrame.positions = gcnew List<Point>();
        animationFrames->Add(emptyFrame);
        currentFrameIndex = 0;
        return;
    }

    try
    {
        StreamReader^ din = File::OpenText(fullFilePath);
        String^ line;


        while ((line = din->ReadLine()) != nullptr)
        {
            String^ trimmedLine = line->Trim();
            if (trimmedLine->Length == 0) continue;

            Frame newFrame;
            newFrame.positions = gcnew List<Point>();

            // 1. Rozdìlení øádku na tìlesa pomocí ';'
            //    POUŽITÍ RemoveEmptyEntries ZAJISTÍ, ŽE SE PØESKOÈÍ PRÁZDNÉ ØETÌZCE
            array<String^>^ bodies = trimmedLine->Split(gcnew array<Char>{';'}, StringSplitOptions::RemoveEmptyEntries);

            for (int i = 0; i < bodies->Length; i++)
            {
                String^ bodyString = bodies[i]->Trim();
                if (bodyString->Length == 0) continue;

                // 2. Rozdìlení tìlesa na souøadnice X,Y pomocí ','
                array<String^>^ coords = bodyString->Split(gcnew array<Char>{','}, StringSplitOptions::RemoveEmptyEntries);

                if (coords->Length == 2)
                {
                    String^ xStr = coords[0]->Trim();
                    String^ yStr = coords[1]->Trim();

                    if (xStr->Length > 0 && yStr->Length > 0)
                    {
                        System::Int64 x_long, y_long;

                        try
                        {
                            // 3. Použití Int64::Parse s NumberStyles::Integer pro striktní formát
                            x_long = System::Int64::Parse(xStr, System::Globalization::NumberStyles::Integer);
                            y_long = System::Int64::Parse(yStr, System::Globalization::NumberStyles::Integer);
                        }
                        catch (System::FormatException^)
                        {
                            // Zde je hlášena chyba s konkrétním øetìzcem
                            throw gcnew System::FormatException("Selhalo parsování souøadnic: X='" + xStr + "', Y='" + yStr + "'. Zkontrolujte 'polohy.txt' na skryté znaky (napø. \r).");
                        }

                        // Kontrola, zda se souøadnice vejdou do 32-bit int (pro Point)
                        if (x_long >= System::Int32::MinValue && x_long <= System::Int32::MaxValue &&
                            y_long >= System::Int32::MinValue && y_long <= System::Int32::MaxValue)
                        {
                            int x = (int)x_long;
                            int y = (int)y_long;

                            newFrame.positions->Add(System::Drawing::Point(x, y));
                        }
                        // Jinak jsou souøadnice pøíliš velké a jsou pøeskoèeny.
                    }
                }
            }

            if (newFrame.positions->Count > 0)
            {
                animationFrames->Add(newFrame);
            }
        }

        din->Close();
        currentFrameIndex = 0;
    }
    catch (System::Exception^ ex)
    {
        // Zde se zobrazí zpráva s konkrétním chybným øetìzcem
        MessageBox::Show("Došlo k chybì pøi parsování 'polohy.txt': " + ex->Message, "Závažná chyba: Problém s formátem dat");
    }
}

void Form5::SaveAnimationData()
{
    String^ fullFilePath = GetPolohyFilePath();

    try
    {
        StreamWriter^ sw = gcnew StreamWriter(fullFilePath, false);

        for each (Frame frame in animationFrames)
        {
            String^ line = "";
            for each (Point p in frame.positions)
            {
                line += p.X.ToString() + ";" + p.Y.ToString() + ";";
            }
            if (line->EndsWith(";"))
            {
                line = line->Substring(0, line->Length - 1);
            }

            sw->WriteLine(line);
        }

        sw->Close();
        MessageBox::Show("Data saved successfully to: " + fullFilePath, "Success");
    }
    catch (Exception^ ex)
    {
        MessageBox::Show("An error occurred while saving 'polohy.txt': " + ex->Message, "Error");
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


String^ Form5::GetPolohyFilePath()
{
    String^ fullExePath = System::Windows::Forms::Application::ExecutablePath;

    String^ exeDirectory = System::IO::Path::GetDirectoryName(fullExePath);

    String^ filePath = System::IO::Path::Combine(exeDirectory, "polohy.txt");

    return filePath;
}

void Form5::btnPrev_Click(Object^ sender, EventArgs^ e)
{
    animationTimer->Stop();
    Form4^ f4 = gcnew Form4(this->ClientSize);
    f4->Location = this->Location;
    f4->Show();
    this->Close();
}