
// SearchWordDlg.h : header file
//


#pragma once
#include <vector>
#include <random>
#include <thread>
#include <string>
#include <array>
#include <mutex>
#include <fstream>


// CSearchWordDlg dialog
class CSearchWordDlg : public CDialogEx
{
// Construction
public:
	CSearchWordDlg(CWnd* pParent = nullptr);	// standard constructor
	static const int m_NbThreadMax = 100; // Maximum number of thread allowed to by used


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEARCHWORD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// Data Base related objetcs :
	const std::string m_Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // alphabet used
	const int m_SizeWord = 4; // Number of letter in the words of the database	
	const int m_NbWords = static_cast<int>(pow(m_Alphabet.length(), m_SizeWord));
	std::vector<std::string> m_WordDataBase; // word list made of all possible combinations of 4 upper case letters (AAAA to ZZZZ)
	void	InitDataBase(); // Create word data base

	// Thread related objects :
	afx_msg void OnEnChangeEditNbOfThreads();
	CString m_strNbOfThread; // Interface variable: number of thread to adapt according to the hardware configuration
	int m_NbOfThread; // Number of thread to adapt according to the hardware configuration
	
	//Main search function :
	void SearchWord(std::string str_InWord);

	// Results :
	std::string m_resultsfile;
	std::vector<std::string> m_WordFound; // Words found in the word data base
	afx_msg void OnBnClickedButtonSearch();
	void DisplayResults(std::string str = ""); // Function to display found words
	
	
};

