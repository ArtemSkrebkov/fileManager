#include "windows.h"

using namespace System;
using namespace System::Windows::Forms;

ref class CDiskComboBox: public ComboBox
{
public:
	CDiskComboBox()
	{
		DWORD maskDisk = GetLogicalDrives();

		for(int i = 0; i < 26; i++)
		{
			char bit = (maskDisk >> i) & 0x00000001;
			if(bit)
			{
				char *charDisk = new char[2];
				charDisk[0] = char(97+i);
				charDisk[1] = '\0';

				String ^strDisk = gcnew String(charDisk);
				Items->Add(strDisk);
			}
		}
		this->SelectedItem = Items[0];
		this->DropDownStyle = ComboBoxStyle::DropDownList;
	}

	String ^GetNameCurrentDisk()
	{
		return this->SelectedItem->ToString();
	}
};