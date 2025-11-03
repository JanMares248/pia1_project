#include "Form1.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace DragAndDrop;

[STAThreadAttribute]
int main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    Form1^ f1 = gcnew Form1();
    Application::Run(f1);

    return 0;
}
