#include "CFolder.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

ref class CFileListView: public ListView
{
public:
	CFileListView(CFolder *folder)
	{
		//this->View = System::Windows::Forms::View::List;
		
		UpdateList(folder);
	}

	void UpdateList(CFolder *folder)
	{
		Items->Clear();
		for(int i = 0; i < folder->GetCountSubElement(); i++)
			Items->Add(gcnew System::String(folder->GetNameSubElement(i).data()));
	}	
};