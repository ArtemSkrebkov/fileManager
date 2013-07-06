#include "stdafx.h"
#include "CFile.h"
//������ copyfile ������������ copyfileex � ����������� ���������

CFile::CFile(std::string filePath, std::string fileName): CElement(filePath, fileName, ETypeElement::FILES)
{

}

void CFile::Copy(std::string filePath)
{
	std::string currentFullName = mFilePath + mFileName;
	std::string newFullName = filePath + mFileName;

	mFilePath = filePath;
	//���� true - ������ � �������, ���� ����� ���� ���������
	CopyFile(currentFullName.data(), newFullName.data(), true);
}

void CFile::Delete()
{
	std::string fullName = mFilePath + mFileName;
	int f = DeleteFile(fullName.data());

	mFilePath.clear();
	mFileName.clear();
}