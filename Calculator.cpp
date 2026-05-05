#include <cmath>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

ref class CalculatorForm : public Form
{
private:
    TextBox^ display;
    double firstValue;
    String^ pendingOperator;
    bool shouldClear;

public:
    CalculatorForm()
    {
        firstValue = 0.0;
        pendingOperator = "";
        shouldClear = false;

        this->Text = "Simple Calculator";
        this->FormBorderStyle = FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->ClientSize = Drawing::Size(290, 365);
        this->StartPosition = FormStartPosition::CenterScreen;

        display = gcnew TextBox();
        display->ReadOnly = true;
        display->Text = "0";
        display->TextAlign = HorizontalAlignment::Right;
        display->Font = gcnew Drawing::Font("Segoe UI", 20, FontStyle::Bold);
        display->Location = Point(10, 10);
        display->Size = Drawing::Size(270, 55);
        this->Controls->Add(display);

        array<String^>^ labels = {
            "7", "8", "9", "/",
            "4", "5", "6", "*",
            "1", "2", "3", "-",
            "0", ".", "=", "+"
        };

        for (int i = 0; i < labels->Length; ++i)
        {
            Button^ button = gcnew Button();
            button->Text = labels[i];
            button->Font = gcnew Drawing::Font("Segoe UI", 14, FontStyle::Regular);
            button->Size = Drawing::Size(62, 62);

            int row = i / 4;
            int col = i % 4;
            button->Location = Point(10 + col * 68, 80 + row * 68);

            if (labels[i] == "=")
                button->BackColor = Color::LightSteelBlue;
            else if (labels[i] == "+" || labels[i] == "-" || labels[i] == "*" || labels[i] == "/")
                button->BackColor = Color::MistyRose;

            button->Click += gcnew EventHandler(this, &CalculatorForm::OnButtonClick);
            this->Controls->Add(button);
        }

        Button^ clearButton = gcnew Button();
        clearButton->Text = "C";
        clearButton->Font = gcnew Drawing::Font("Segoe UI", 14, FontStyle::Regular);
        clearButton->Size = Drawing::Size(270, 40);
        clearButton->Location = Point(10, 352 - 40);
        clearButton->BackColor = Color::LightCoral;
        clearButton->Click += gcnew EventHandler(this, &CalculatorForm::OnClearClick);
        this->Controls->Add(clearButton);
    }

private:
    void OnClearClick(Object^ sender, EventArgs^ e)
    {
        firstValue = 0.0;
        pendingOperator = "";
        shouldClear = false;
        display->Text = "0";
    }

    void OnButtonClick(Object^ sender, EventArgs^ e)
    {
        Button^ button = safe_cast<Button^>(sender);
        String^ value = button->Text;

        if (value[0] >= '0' && value[0] <= '9')
        {
            if (display->Text == "0" || shouldClear)
                display->Text = "";

            shouldClear = false;
            display->Text += value;
            return;
        }

        if (value == ".")
        {
            if (shouldClear)
            {
                display->Text = "0";
                shouldClear = false;
            }

            if (!display->Text->Contains("."))
                display->Text += ".";
            return;
        }

        if (value == "=")
        {
            if (pendingOperator == "")
                return;

            double secondValue = Double::Parse(display->Text);
            double result = ApplyOperation(firstValue, secondValue, pendingOperator);
            display->Text = result.ToString();
            firstValue = result;
            pendingOperator = "";
            shouldClear = true;
            return;
        }

        firstValue = Double::Parse(display->Text);
        pendingOperator = value;
        shouldClear = true;
    }

    double ApplyOperation(double left, double right, String^ op)
    {
        if (op == "+") return left + right;
        if (op == "-") return left - right;
        if (op == "*") return left * right;
        if (op == "/")
        {
            if (right == 0)
            {
                MessageBox::Show("Cannot divide by zero.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return left;
            }
            return left / right;
        }

        return right;
    }
};

[STAThreadAttribute]
int main(array<System::String ^> ^)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew CalculatorForm());
    return 0;
}
