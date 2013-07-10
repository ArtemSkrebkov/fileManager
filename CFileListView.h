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
		this->Columns->Add("Name");
		UpdateList(folder);
	}

	void UpdateList(CFolder *folder)
	{
		Items->Clear();
		//this->Columns->Add("Names");
		
		for(unsigned int i = 0; i < folder->GetCountSubElement(); i++)
		{
			CElement *addElement = folder->GetSubElement(i);
			
			ListViewItem ^item1 = gcnew ListViewItem;
			item1->Text = gcnew System::String(addElement->GetName().data());
			this->Items->Add(item1);
		}
	}	
};