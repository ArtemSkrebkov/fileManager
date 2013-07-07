#include "stdafx.h"
#include "CFile.h"
//вместо copyfile использовать copyfileex и отображение прогресса

CFile::CFile(std::string filePath, std::string fileName): CElement(filePath, fileName, ETypeElement::FILES)
{

}

void CFile::Execute()
{
	//как-то обработать открыть
	//std::string fullName = mFilePath + elementName;
	HINSTANCE g = ShellExecute(NULL, "open", mFileName.data(), NULL, mFilePath.data(), SW_SHOWNORMAL);
}

void CFile::Copy(std::string filePath)
{
	std::string currentFullName = mFilePath + mFileName;
	std::string newFullName = filePath + mFileName;

	mFilePath = filePath;
	//если true - падает с ошибкой, если такой файл существет
	CopyFile(currentFullName.data(), newFullName.data(), true);
}

void CFile::Delete()
{
	std::string fullName = mFilePath + mFileName;
	int f = DeleteFile(fullName.data());

	mFilePath.clear();
	mFileName.clear();
}