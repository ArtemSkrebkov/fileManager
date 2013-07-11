#include "stdafx.h"
#include "CFolder.h"
#include "limits.h"
CFolder::CFolder(std::string filePath, std::string fileName, bool loadSubElement): CElement(filePath, fileName, ETypeElement::FOLDERS)
{
	mLoadSubElement = loadSubElement;
	std::string tmp = GetParentStrPathFromString("f:\\dgle\\fff");
	tmp = GetParentStrNameFromString("f:\\dgle\\fff");
	if(mLoadSubElement)
	{
		LoadSubElement(filePath, fileName);

		if(fileName != ".")
		{
			std::string parentPath = GetParentStrPathFromString(filePath + fileName);
			std::string parentName = GetParentStrNameFromString(filePath + fileName);
		
			mParent = new CFolder(parentPath, parentName, true);
		}
		else
			mParent = NULL;
	}

	
}

std::string CFolder::GetFullName()
{
	return mFilePath + mFileName + "\\";
}

CElement *CFolder::GetSubElement(int i)const
{

	if(i < mSubElement.size())
		return mSubElement[i];

	return NULL;
}

CElement *CFolder::GetSubElement(const std::string &fileName)
{
	CElement *result = NULL;
	
	for(std::vector<CElement *>::iterator i = mSubElement.begin(); i != mSubElement.end(); i++)
		if((*i)->GetName() == fileName)
			result = *i;

	return result;
}

void CFolder::LoadSubElement(std::string filePath, std::string fileName)
{
	//mLoadSubElement = true;
	for(int i = 0; i < mSubElement.size(); i++)
		delete mSubElement[i];
	mSubElement.clear();

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
	int indexDeleted = -1;
	if(!mLoadSubElement)
		LoadSubElement(mFilePath, mFileName);

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

	std::string fullName = mFilePath + mFileName + "\\";
	RemoveDirectory(fullName.data());
}

void CFolder::Update()
{
	LoadSubElement(mFilePath, mFileName);
}

CFolder::~CFolder()
{
	for(int i = 0; i < mSubElement.size(); i++)
		delete mSubElement[i];
}

std::string CFolder::GetStrPathFromString(std::string fileName)
{
	unsigned int pos = fileName.find_last_of("\\");
	std::string result;
	if(pos != UINT_MAX)
		result = fileName.substr(0, pos + 1);
	else
		result = fileName + "\\";

	return result;
}

std::string CFolder::GetStrNameFromString(std::string fileName)
{
	unsigned int pos = fileName.find_last_of("\\");
	std::string result = "";
	if(pos != UINT_MAX)
		result = fileName.substr(pos + 1, fileName.size());
	else
		result = ".";

	return result;
}

std::string CFolder::GetParentStrPathFromString(std::string fileName)
{
	int pos = fileName.find_last_of("\\");
	std::string result = fileName.substr(0, pos);
	result = GetStrPathFromString(result);
	return result;
}

std::string CFolder::GetParentStrNameFromString(std::string fileName)
{
	int pos = fileName.find_last_of("\\");
	std::string result = fileName.substr(0, pos);
	result = GetStrNameFromString(result);
	return result;
}