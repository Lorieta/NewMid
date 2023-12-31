#pragma once
using namespace System::Drawing::Drawing2D;
using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;


public ref class Order {
   static String^ currentDirectory = Path::GetDirectoryName(Application::ExecutablePath);

private:
    DataGridView^ dataGridView1;
    RichTextBox^ richTextBox1;
    ComboBox^ comboBox1;
    ComboBox^ comboBox2;
    TextBox^ textBox1;

public:
    Order(DataGridView^ dgv, RichTextBox^ rtb, ComboBox^ cb1, ComboBox^ cb2, TextBox^ tb) {
        dataGridView1 = dgv;
        richTextBox1 = rtb;
        comboBox1 = cb1;
        comboBox2 = cb2;
        textBox1 = tb;
    }
    void Reciept() {

        int columnIndex = 3;

        int total = 0;
        // Calculate total from  the DataGridView
        for each (DataGridViewRow ^ row in dataGridView1->Rows) {
            if (row->IsNewRow) continue;

            Object^ value = row->Cells[columnIndex]->Value;

            if (value != nullptr) {
                total += Convert::ToInt32(value);
            }
        }
        //Get time and date for reciept
        DateTime currentDateTime = DateTime::Now;
        String^ formattedDateTime = currentDateTime.ToString("yyyy-MM-dd HH:mm:ss");

        richTextBox1->Clear();

        richTextBox1->Font = gcnew System::Drawing::Font("Courier New", 10);


        richTextBox1->AppendText("===================================================================" + Environment::NewLine);
        richTextBox1->AppendText("=                            PAHUWAY                              =" + Environment::NewLine);
        richTextBox1->AppendText("=                            RECEIPT                              =" + Environment::NewLine);
        richTextBox1->AppendText("=                                                                 = " + Environment::NewLine);
        richTextBox1->AppendText("=                         DATE: (" + formattedDateTime + ")             =" + Environment::NewLine);
        richTextBox1->AppendText("=                                                                 = " + Environment::NewLine);
        richTextBox1->AppendText("===================================================================" + Environment::NewLine);


        array<int>^ barpadding = gcnew array<int>{5, 5, 8, 5};

        array<int>^ contentPadding = gcnew array<int>{10, 10, 5, 5};

        for (int i = 0; i < dataGridView1->Columns->Count; ++i) {
            richTextBox1->AppendText(String::Format("{0," + (contentPadding[i] + dataGridView1->Columns[i]->HeaderText->Length) + "}   ", dataGridView1->Columns[i]->HeaderText));
        }
        richTextBox1->AppendText(Environment::NewLine);


        // Iterate through each row in the DataGridView and format the spaces of the content
        for each (DataGridViewRow ^ row in dataGridView1->Rows) {
            if (row->IsNewRow) continue;

            for each (DataGridViewCell ^ cell in row->Cells) {
                richTextBox1->AppendText(String::Format("{0," + (barpadding[cell->ColumnIndex] + cell->Value->ToString()->Length) + "}   ", cell->Value->ToString()));
            }

            richTextBox1->AppendText(Environment::NewLine);
        }

        richTextBox1->AppendText("===================================================================" + Environment::NewLine);
        richTextBox1->AppendText(String::Format("                                                TOTAL: Php. {0}", total + ".00") + Environment::NewLine);
    }

    void saveToText() {

        String^ fileName = "Receipt_" + DateTime::Now.ToString("yyyyMMdd_HHmmss") + ".txt";
        String^ filePath = Path::Combine(currentDirectory, fileName);

        try {

            richTextBox1->SaveFile(filePath, RichTextBoxStreamType::PlainText);
            MessageBox::Show("Receipt saved successfully!");
        }
        catch (Exception^ ex) {
            MessageBox::Show("Error saving receipt: " + ex->Message);
        }
    }

    void fillCombobox() {
        comboBox1->Items->Clear();
       
        String^ fileName = "Menu.txt";
        String^ filePath = Path::Combine(currentDirectory, fileName);

        try {
            StreamReader^ file = gcnew StreamReader(filePath);
            String^ line;
            //read the file and put i
            while ((line = file->ReadLine()) != nullptr) {
                comboBox1->Items->Add(line);
            }
            file->Close();
        }
        catch (IOException^ ex) {
            MessageBox::Show("Error: " + ex->Message);
        }

    }

    void buildOrder() {

        if (comboBox1->SelectedItem != nullptr && comboBox2->SelectedItem != nullptr && !String::IsNullOrEmpty(textBox1->Text)) {
            int numericValue;
            if (Int32::TryParse(textBox1->Text, numericValue) && numericValue < 99) {
                array<Object^>^ rowValues = gcnew array<Object^>(dataGridView1->Columns->Count);

                rowValues[0] = comboBox1->SelectedItem;
                rowValues[1] = comboBox2->SelectedItem;
                rowValues[2] = numericValue;

                // total by multiplying Price and Quantity
                double price, quantity;
                if (Double::TryParse(comboBox2->SelectedItem->ToString(), price) &&
                    Double::TryParse(textBox1->Text, quantity)) {
                    double total = price * quantity;
                    rowValues[3] = total;

                    dataGridView1->Rows->Add(rowValues);

                    comboBox1->SelectedIndex = -1;
                    comboBox2->SelectedIndex = -1;
                    textBox1->Text = "";
                }
                else {
                    MessageBox::Show("Invalid numeric values in 'Price' or 'Quantity' columns.");
                }
            }
            else {
                MessageBox::Show("Please enter a valid number (less than 99) in the TextBox.");
            }
        }
        else {
            MessageBox::Show("Please select an item from both ComboBoxes and fill the TextBox.");
        }

    }



    void removeRow() {
        if (dataGridView1->SelectedRows->Count > 0) {
            // Get the index of the selected row
            int selectedIndex = dataGridView1->SelectedRows[0]->Index;

            // Remove the selected row from the DataGridView
            dataGridView1->Rows->RemoveAt(selectedIndex);
        }
        else {
            MessageBox::Show("Please select a row to remove.");
        }

    }
};
#pragma once
