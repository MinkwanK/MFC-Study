#define _AFXDLL
#include <afx.h>
#include <Afxtempl.h>
#include <iostream>

int main()
{
	const char* pszBuffer = "Hello";
	const wchar_t* pwszTest = L"World";

	std::cout << pszBuffer << std::endl;
	std::cout << pwszTest << std::endl;

	TCHAR szBuffer[8] = { TEXT("Test") };
	return 0;
}