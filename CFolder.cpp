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

void CFolder::GoSelect(const std::string elementName)
{
	CElement *findElement = GetSubElement(elementName);

	CFolder *findFolder = dynamic_cast<CFolder *>(findElement);
	//если dynamic_cast отработал не null то значит папка, выполняем переход
	if(findFolder != NULL)
	{
		mFileName = findFolder->mFileName;
		mFilePath = findFolder->mFilePath;
		mSubElement.clear();
		this->LoadSubElement(mFilePath, mFileName);
	}
	else
	{
		CFile *findFile = dynamic_cast<CFile *>(findElement);
		findFile->Execute();
	}
}

std::string CFolder::GetFullName()
{
	return mFilePath + mFileName + "\\";
}

std::string CFolder::GetNameSubElement(int i)
{
	std::string result = "";

	if(i < mSubElement.size())
	{
		result = mSubElement[i]->GetName();
	}

	return result;
}

CElement *CFolder::GetSubElement(int i)const
{

	if(i < mSubElement.size())
		return mSubElement[i];

	return NULL;
}

CElement *CFolder::GetSubElement(const std::string fileName)
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
	std::string fullName;

	if(fileName != "")
		fullName = filePath + fileName + "\\";
	else
		fullName = filePath;

	std::string fullNameSearch = fullName + "*";
	
	HANDLE handleFindFile = FindFirstFile(fullNameSearch.data(), &findData);

	if(handleFindFile != INVALID_HANDLE_VALUE)
	{	
		do
		{
			std::string newFileName = findData.cFileName;
			//если элемент директория
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
	CreateDirectory(newFullName.data(), NULL);//атрибуты секретности по-умолчанию
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