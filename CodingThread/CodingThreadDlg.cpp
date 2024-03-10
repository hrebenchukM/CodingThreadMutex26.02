#include "CodingThreadDlg.h"

CodingThreadDlg* CodingThreadDlg::ptr = NULL;
HANDLE hMutex;
HWND hEditBox; 

CodingThreadDlg::CodingThreadDlg(void)
{
	ptr = this;
}

CodingThreadDlg::~CodingThreadDlg(void)
{
	
}

void CodingThreadDlg::Cls_OnClose(HWND hwnd)
{
	ReleaseMutex(hMutex); // ������������ ��������
	EndDialog(hwnd, 0);

}

DWORD WINAPI Coding_Thread(LPVOID lp)
{
	CodingThreadDlg *ptr = (CodingThreadDlg *)lp;
	char buf[100];
	ifstream in(TEXT("music.txt"),ios::in);
	
	if(!in)
	{
		MessageBox(ptr->hDialog, TEXT("������ �������� �����!"), TEXT("�������"), MB_OK | MB_ICONINFORMATION);
		return 1;
	}

	hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, TEXT("{B8A2C367-10FE-494d-A869-841B2AF972E0}"));//�������� �������� 

	DWORD dwAnswer = WaitForSingleObject(hMutex, INFINITE);//������ ������ �������� (INFINITE-����������� ���-�� ������)

		ofstream out("coding.txt", ios::out | ios::trunc);//coding.txt-����������� ������ ���.
		const int KEY = 100;
		while(!in.eof())
		{
			in.getline(buf,100);
			out << buf << endl;
		}
		out.close();
		in.close();

	

		ReleaseMutex(hMutex); // ������������ ��������

		MessageBox(ptr->hDialog, TEXT("������ ������ � ���� coding.txt ���������!"), TEXT("�������"), MB_OK | MB_ICONINFORMATION);


	return 0;
}
//
//BOOL CodingThreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
//{
//	hDialog = hwnd;
//	hEditBox = GetDlgItem(hwnd, IDC_EDIT1);
//	CreateThread(NULL, 0, Coding_Thread, this, 0, NULL); 
//	return TRUE;
//}
//
BOOL CodingThreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hDialog = hwnd;
	hEditBox = GetDlgItem(hwnd, IDC_EDIT1);
	CreateThread(NULL, 0, Coding_Thread, this, 0, NULL);


	DWORD dwAnswer = WaitForSingleObject(hMutex, INFINITE);

	ifstream codingFile("coding.txt");
	if (codingFile.is_open()) {
		string line;
		string fileContents;
		while (getline(codingFile, line)) {
			fileContents += line + "\r\n";
		}
		codingFile.close();
		SetWindowTextA(hEditBox, fileContents.c_str());
		
	
	}
	else {
		MessageBox(hwnd, TEXT("������ �������� ����� coding.txt!"), TEXT("������"), MB_OK | MB_ICONERROR);
	}


	ReleaseMutex(hMutex);
	return TRUE;
}


BOOL CALLBACK CodingThreadDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
	}
	return FALSE;
	
}