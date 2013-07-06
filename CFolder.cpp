#include "stdafx.h"
#include "CFolder.h"

CFolder::CFolder(std::string filePath, std::string fileName, bool loadSubElement): CElement(filePath, fileName, ETypeElement::FOLDERS)
{
	mLoadSubElement = loadSubElement;

	if(mLoadSubElement)
	{
		LoadSubElement(filePath, fileName);
	}
}

CElement *CFolder::GetSubElement(int i)const
{

	if(i < mSubElement.size())
		return mSubElement[i];

	return NULL;
}

CElement *CFolder::GetSubElement(std::string fileName)
{
	CElement *result = NULL;

	for(std::vector<CElement *>::iterator i = mSubElement.begin(); i != mSubElement.end(); i++)
		if((*i)->GetName() == fileName)
			result = *i;

	return result;
}

void CFolder::LoadSubElement(std::string filePath, std::string fileName)
{
	WIN32_FIND_DATA findData;
	
	std::string fullName = filePath + fileName + "\\";
	std::string fullNameSearch = fullName + "*";
	
	HANDLE handleFindFile = FindFirstFile(fullNameSearch.data(), &findData);

	if(handleFindFile != INVALID_HANDLE_VALUE)
	{	
		do
		{
			std::string newFileName = findData.cFileName;
			//���� ������� ����������
			if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if(newFileName == "." /*||newFileName == ""||newFileName == ".."*/) continue;

				mSubElement.push_back(new CFolder(fullName, newFileName, false));
			}
			else
			{
				mSubElement.push_back(new CFile(fullName, newFileName));
			}
		}
		while(FindNextFile(handleFindFile, &findData));
	}

	FindClose(handleFindFile);
}

void CFolder::Copy(std::string filePath)
{
	std::string newFullName = filePath + mFileName + "\\";
	CreateDirectory(newFullName.data(), NULL);//�������� ����������� ��-���������
	for(int i = 0; i < mSubElement.size(); i++)
	{
		if(mSubElement[i]->GetName() != "..")
		{
			if(mSubElement[i]->GetType() == ETypeElement::FOLDERS)
			{
				CFolder *tmp = dynamic_cast<CFolder *>(mSubElement[i]);
				if(!tmp->IsLoadSubElement())
					tmp->LoadSubElement(tmp->GetPath(), tmp->GetName());
				tmp->Copy(newFullName);
			}
			else
				mSubElement[i]->Copy(newFullName);
		}
	}
}

void CFolder::Delete()
{
	for(int i = 0; i < mSubElement.size(); i++)
		if(mSubElement[i]->GetName() != "..")
		{
			if(mSubElement[i]->GetType() == ETypeElement::FOLDERS)
			{
				CFolder *tmp = dynamic_cast<CFolder *>(mSubElement[i]);
				if(!tmp->IsLoadSubElement())
					tmp->LoadSubElement(tmp->GetPath(), tmp->GetName());

				tmp->Delete();
			}
			else
				mSubElement[i]->Delete();
		}

	std::string fullName = mFilePath + mFileName;
	unsigned int g = RemoveDirectory(fullName.data());
}