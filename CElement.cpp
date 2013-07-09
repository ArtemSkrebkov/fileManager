#include "stdafx.h"
#include "CElement.h"

CElement::CElement(std::string &filePath, std::string &fileName, ETypeElement type)
{
	mFileName = fileName;
	mFilePath = filePath;
	mType = type;
}

void CElement::ReName(std::string fileName)
{
	std::string currentFullName = mFilePath + mFileName;
	std::string newFullName = mFilePath + fileName;

	mFileName = fileName;

	MoveFileEx(currentFullName.data(), newFullName.data(), MOVEFILE_COPY_ALLOWED);
}

void CElement::Move(std::string filePath)
{
	std::string currentFullName = mFilePath + mFileName;
	std::string newFullName = filePath + mFileName;

	mFilePath = filePath;
	//если true - падает с ошибкой, если такой файл существет
	int f = MoveFile(currentFullName.data(), newFullName.data());
	int g = GetLastError();
}

std::string CElement::GetName() const
{
	return mFileName;
}

std::string CElement::GetPath() const
{
	return mFilePath;
}