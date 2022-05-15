
// SearchWordDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SearchWord.h"
#include "SearchWordDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSearchWordDlg dialog



CSearchWordDlg::CSearchWordDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SEARCHWORD_DIALOG, pParent)
	, m_Alphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZMMMMMMMMM")
	, m_strNbOfThread(_T("4"))
	, m_NbOfThread(4)
	, m_resultsfile("C:\\Users\\maxja\\source\\repos\\SearchWord\\x64\\Release\\results.txt")
	, m_font()

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	

	m_WordDataBase = std::vector<std::string>(static_cast<unsigned int>(pow(m_Alphabet.length(), m_SizeWord)), "");
	

}

void CSearchWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NB_OF_THREADS, m_strNbOfThread);	
	DDX_Text(pDX, IDC_EDIT_ALPHABET, CString(m_Alphabet.c_str()));
	DDX_Text(pDX, IDC_EDIT_RESULT_FILE, CString(m_resultsfile.c_str()));
	
}

BEGIN_MESSAGE_MAP(CSearchWordDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_NB_OF_THREADS, &CSearchWordDlg::OnEnChangeEditNbOfThreads)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CSearchWordDlg::OnBnClickedButtonSearch)	
	//ON_BN_CLICKED(IDC_BUTTON_UPDATE_DATABASE, &CSearchWordDlg::OnBnClickedButtonUpdateDatabase)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_DATABASE, &CSearchWordDlg::OnBnClickedButtonUpdateDatabase)
	ON_EN_CHANGE(IDC_EDIT_RESULT_FILE, &CSearchWordDlg::OnEnChangeEditResultFile)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_RESULTSFILE, &CSearchWordDlg::OnBnClickedButtonChangeResultsfile)
END_MESSAGE_MAP()


// CSearchWordDlg message handlers

BOOL CSearchWordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	m_font.CreateFont(
		26,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,				// nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		PROOF_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial"));                 // lpszFacename

	GetDlgItem(IDC_STATIC)->SetFont(&m_font);
	// Initialisation of the word data base : 
	InitDataBase();

	m_NbOfThread = atoi(CStringA(m_strNbOfThread));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSearchWordDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
				
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSearchWordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSearchWordDlg::InitDataBase()
{
	m_WordDataBase.clear(); 
	m_NbWords = pow(m_Alphabet.length(), m_SizeWord);
	m_WordDataBase = std::vector<std::string>(m_NbWords, "");

	//Creation of DataBase 
	size_t NbLetter = m_Alphabet.length();
	
	int pos = 0;
	for (auto it1 = m_Alphabet.cbegin(); it1 != m_Alphabet.end(); it1++)
	{
		for (auto it2 = m_Alphabet.cbegin(); it2 != m_Alphabet.end(); it2++)
		{
			for (auto it3 = m_Alphabet.cbegin(); it3 != m_Alphabet.end(); it3++)
			{
				for (auto it4 = m_Alphabet.cbegin(); it4 != m_Alphabet.end(); it4++)
				{
					m_WordDataBase[pos].append(1, *it1);
					m_WordDataBase[pos].append(1, *it2);
					m_WordDataBase[pos].append(1, *it3);
					m_WordDataBase[pos].append(1, *it4);
					pos++;
				}
		    }
		}
	}

	/*size_t change_let = 0;
	while (pos != pow(NbLetter, m_SizeWord))
	{
		m_WordDataBase[pos].append(1, m_Alphabet[pos % NbLetter]);

		if (pos / NbLetter > 1)  
			m_WordDataBase[pos].append(1, m_Alphabet[pos % NbLetter]);
			
		pos++;

		for (size_t i_let = 0; i_let < m_SizeWord; i_let++)
		{
			m_WordDataBase[pos].append(1, m_Alphabet[pos%NbLetter]);
			if (pos%)
		};

	}*/

	//Verify the size of the data base.
	//assert(pos == pow(m_Alphabet.length(), m_SizeWord));
	// Shuffle randomly words in the data base :
	std::random_shuffle(m_WordDataBase.begin(), m_WordDataBase.end());
		
}

void CSearchWordDlg::OnBnClickedButtonSearch()
{
	using namespace std;
	m_WordFound.clear(); // clear of any previous search

	// Measure time of serach :
	chrono::time_point<chrono::system_clock> start_time, end_time; // start and end times of search
	start_time = std::chrono::system_clock::now(); 

	CString InputWord;
	CDataExchange DX(this, true);
	DDX_Text(&DX, IDC_INPUT_WORD, InputWord);
	std::string str_InWord = CT2A(InputWord);

	// If the searched word is empty, do nothing
	if (str_InWord.length() == 0)
		return;
	else
		SearchWord(str_InWord); // Main search function
	
	end_time = std::chrono::system_clock::now();

	double time_search = chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
	
	// Display results :
	CString str;
	str.Format(_T("Number of words found :\r\n %d upon a total of %d words \r\n"), m_WordFound.size(),m_NbWords);	
	str.AppendFormat(_T("Time of search :\r\n %.3f ms \r\n"),time_search);
	str.AppendFormat(_T("Results in file : \r\n \"%s\" \r\n"), CString(m_resultsfile.c_str()));
	// Edit GUI
	CDataExchange DXf(this, false);
	DDX_Text(&DXf, IDC_EDIT_RESULTS, str);



	std::string stdstr;
	stdstr = (CStringA(str));
	DisplayResults(stdstr);

}

void CSearchWordDlg::SearchWord(std::string str_InWord)
{
	using namespace std;

	std::vector<std::vector<std::string>> vec_WordFound(m_NbOfThread);
			
	auto _SearchPart = [this, &vec_WordFound](std::string LetterIn, size_t StartInd, size_t EndInd, size_t i_th)//Core lambda function to find a letter scheme into a part of the word data base
	{
		size_t Size_Ind = EndInd - StartInd + 1;		
		const unsigned char NbLetter = LetterIn.length();
		//assert(NbLetter > 0 && "Number of letters to search are not >0");
				
		for (size_t i = StartInd; i <= EndInd; i++)
		{			
			if (LetterIn == m_WordDataBase[i].std::string::substr(0, NbLetter))
			{
				vec_WordFound[i_th].push_back(m_WordDataBase[i]);									
			}
		}
		return;
	};
		
	size_t SizeOfThread;
	size_t StartInd = 0;
	size_t EndInd = 0;
	std::vector<thread> vec_th(m_NbOfThread); // vector of threads
	
	for (int i_th = 0; i_th < m_NbOfThread; i_th++) // loop on threads
	{
		if (i_th == 0) // if the number of total words are not a multiple of number of threads, the first thread is a bit bigger 
			SizeOfThread = static_cast<size_t>(floor(m_NbWords / m_NbOfThread)) + m_NbWords % m_NbOfThread;
		else
			SizeOfThread = static_cast<size_t>(floor(m_NbWords / m_NbOfThread));
		
		// StartInd and EndInd are here to insure that different parts of the word data base are being serched by the threads thus avoiding mutex
		EndInd = StartInd + SizeOfThread - 1;
		
		// Threads :
		vec_th[i_th] = thread(_SearchPart, str_InWord, StartInd, EndInd, i_th);
		
		StartInd += SizeOfThread;

	}
	
	for (int i_th = 0; i_th < m_NbOfThread; i_th++)
	{
		vec_th[i_th].join(); //Wait end of threads
		 // Concatenate results : results have been stored in different vectors to avoid any conflict of access.
		m_WordFound.insert(m_WordFound.end(), vec_WordFound[i_th].begin(), vec_WordFound[i_th].end());
		
	}
	
	DisplayResults();

	
}

void CSearchWordDlg::DisplayResults(std::string str)
{
	std::ofstream fout(m_resultsfile.c_str(), std::ios::out);
	
	// Begining of file 
	fout.write((char*)(str.c_str()), str.size());

	// Loop on words found :
	for (size_t i = 0; i < m_WordFound.size(); i++)
	{		
		fout.write((char*)(m_WordFound[i].c_str()), m_WordFound[i].size());
		fout << std::endl;
	}

	fout.close();

	
}

void CSearchWordDlg::OnEnChangeEditNbOfThreads()
{
	UpdateData(true);	
	m_NbOfThread = atoi(CStringA(m_strNbOfThread));
	//assert(m_NbOfThread <= m_NbThreadMax);
	
}


void CSearchWordDlg::OnEnChangeEditAlphabet()
{

	
}



void CSearchWordDlg::OnBnClickedButtonUpdateDatabase()
{	
	CString Cstr;
	CDataExchange DX(this, true);
	DDX_Text(&DX, IDC_EDIT_ALPHABET, Cstr);
	
	m_Alphabet = CT2A(Cstr);
	InitDataBase();
}


void CSearchWordDlg::OnEnChangeEditResultFile()
{


}


void CSearchWordDlg::OnBnClickedButtonChangeResultsfile()
{
	CString Cstr;
	CDataExchange DX(this, true);
	DDX_Text(&DX, IDC_EDIT_RESULT_FILE, Cstr);

	m_resultsfile = CT2A(Cstr);
}
