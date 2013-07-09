#include "CFolder.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

ref class CFileListView: public ListView
{
public:
	CFileListView(CFolder *folder)
	{		
		this->View = System::Windows::Forms::View::Details;
		UpdateList(folder);
	}

	void UpdateList(CFolder *folder)
	{
		Items->Clear();
		this->Columns->Add("Name");
		//this->Columns->Add("Names");
		
		for(int i = 0; i < folder->GetCountSubElement(); i++)
		{
			CElement *addElement = folder->GetSubElement(i);
			
			ListViewItem ^item1 = gcnew ListViewItem;
			item1->Text = gcnew System::String(addElement->GetName().data());
			//item1->SubItems->Add("aa");
			//item1->SubItems->Add("bb");
			this->Items->Add(item1);
		}
	}	
};