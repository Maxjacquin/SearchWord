
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
	, m_strNbOfThread(_T("16"))
	, m_NbOfThread(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	

	m_WordDataBase = std::vector<std::string>(static_cast<unsigned int>(pow(m_Alphabet.length(), m_SizeWord)), "");
	

}

void CSearchWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NB_OF_THREADS, m_strNbOfThread);
}

BEGIN_MESSAGE_MAP(CSearchWordDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_EN_CHANGE(IDC_INPUT_WORD, &CSearchWordDlg::OnEnChangeInputWord)
	ON_EN_CHANGE(IDC_EDIT_NB_OF_THREADS, &CSearchWordDlg::OnEnChangeEditNbOfThreads)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CSearchWordDlg::OnBnClickedButtonSearch)
END_MESSAGE_MAP()


// CSearchWordDlg message handlers

BOOL CSearchWordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialisation of the word data base : 
	InitDataBase();

	m_NbOfThread = atoi(CStringA(m_strNbOfThread));

	//m_pMultiSearch = std::make_unique<CMultiSearch>(this, m_WordDataBase, m_ArrayIndFound);
	
	
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
	assert(pos == pow(m_Alphabet.length(), m_SizeWord));

	// Shuffle randomly words in the data base :
	std::random_shuffle(m_WordDataBase.begin(), m_WordDataBase.end());
		
}

void CSearchWordDlg::OnBnClickedButtonSearch()
{
	using namespace std;

	chrono::time_point<chrono::system_clock> start_time, end_time;
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

	double time_search = chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()/1000.0;
	// Display results :
	CString str;
	str.Format(_T("Number of words found :\r\n %d \r\n"), m_WordFound.size());
	
	str.AppendFormat(_T("Time of search :\r\n %.3f s"),time_search);
	
	CDataExchange DXf(this, false);
	DDX_Text(&DXf, IDC_EDIT_RESULTS, str);

}

void CSearchWordDlg::SearchWord(std::string str_InWord)
{
	using namespace std;

	size_t size_max_vec = static_cast<size_t>(floor(m_NbWords / m_NbOfThread)) + m_NbWords % m_NbOfThread;
	size_t NumResultsTot = 0;
	std::mutex NumRes_mutex;  // protects NumResultsTot
	std::vector<std::vector<std::string>> vec_WordFound(m_NbOfThread);
	m_WordFound.clear();
	

	auto _SearchPart = [this, &NumRes_mutex, &NumResultsTot, &vec_WordFound](std::string LetterIn, size_t StartInd, size_t EndInd, size_t i_th/*, std::vector<std::string>& WordFound ""*/)//Core function to find a letter scheme into a part of the word data base
	{
		size_t Size_Ind = EndInd - StartInd + 1;		
		//IndFound[i_th] = std::vector<bool>(false, Size_Ind);
		size_t NumResults=0;
		//std::string Bufstr;
		const unsigned char NbLetter = LetterIn.length();
		assert(NbLetter > 0 && "Number of letters to search are not >0");

		//Ind_found = std::vector<bool>(floor(m_NbWords/ m_NbOfThread), 0);

		//for (auto it = m_WordDataBase.cbegin(); it != m_WordDataBase.end(); it++)
		//{	std::string Bufstr = *it;
		//if (NbLetter < m_SizeWord)
		//	if (LetterIn == Bufstr.std::string::substr(0, NbLetter))
		//	else
			for (size_t i = StartInd; i <= EndInd; i++)
			{
			//	if (NbLetter == m_SizeWord)
			//		(LetterIn == m_WordDataBase[i]) ? IndFound[i_th][i - StartInd] = 0 : IndFound[i_th][i - StartInd] = 1;
			//	else  // NbLetter < m_SizeWord => use of substring
			//		(LetterIn == m_WordDataBase[i].std::string::substr(0, NbLetter)) ? IndFound[i_th][i - StartInd] = 0 : IndFound[i_th][i - StartInd] = 1;
				//if (NbLetter == m_SizeWord)
				//	if (LetterIn == m_WordDataBase[i])
					//{
				//		m_IndFound[i] = true; 
					//	NumResults++;
				//	}
				if // NbLetter < m_SizeWord => use of substring
					(LetterIn == m_WordDataBase[i].std::string::substr(0, NbLetter))
				{
					vec_WordFound[i_th].push_back(m_WordDataBase[i]);
					//m_IndFound.push_back(i);						
				}
			}
	
			const std::lock_guard<std::mutex> lock(NumRes_mutex);
			NumResultsTot += NumResults;

		return;
	};

	
	size_t SizeOfThread;// = static_cast<size_t>(floor(m_NbWords / m_NbOfThread));
	//size_t SizeOffFirstThr = m_NbWords % m_NbOfThread;
	size_t StartInd = 0;
	size_t EndInd = 0;
	std::vector<thread> vec_th(m_NbOfThread);
	//std::vector<thread> IndFound(m_NbOfThread, std::vector<bool>(size_max_vec, 0));
	

	//for (int i_th = 0; i_th < m_NbOfThread; i_th++)
	for (int i_th = 0; i_th < m_NbOfThread; i_th++)
	{
		if (i_th == 0) // the first thread is a bit bigger than the next one if the number of total words are not a multiple of number of threads
			SizeOfThread = static_cast<size_t>(floor(m_NbWords / m_NbOfThread)) + m_NbWords % m_NbOfThread;
		else
			SizeOfThread = static_cast<size_t>(floor(m_NbWords / m_NbOfThread));

		
		//m_ArrayIndFound[i_th] = std::vector<bool>(SizeOfThread,0);
		EndInd = StartInd + SizeOfThread - 1;
		//IndFound[i_th] = vector<bool>(SizeOfThread,false);
	
		vec_th[i_th] = thread(_SearchPart, str_InWord, StartInd, EndInd, i_th/*, vec_WordFound[i_th]*/);
		//vec_th[i_th] = thread(func_SearchPart, str_InWord, StartInd, EndInd, i_th);
		
		StartInd += SizeOfThread;

	}
	
	for (int i_th = 0; i_th < m_NbOfThread; i_th++)
	{
		vec_th[i_th].join(); //Wait for threads to end
		m_WordFound.insert(m_WordFound.end(), vec_WordFound[i_th].begin(), vec_WordFound[i_th].end());; // Concatenate the results
		
		//NumResultsTot += NumResults[i_th];
		//NumResults += IndFound[i_th].size(); // count results		
	}
	
	DisplayResults();

	
}

void CSearchWordDlg::DisplayResults()
{
	CString Cstr = _T("Words found : \n");
	//CString buff;
	/*for (auto it = m_WordFound.cbegin(); it < m_WordFound.end(); it++)
	{
		Cstr.Format(_T("%s%s\n"), Cstr, *it);
	}*/

	//for (size_t i = 0; i <40 /*m_WordFound.size()*/; i++)
	//{
	//	//buff = m_WordFound[i].c_str();
	//	Cstr += m_WordFound[i].c_str();
	//	Cstr += "\r\n";
	//	//Cstr.Format(_T("%s%s\n"), Cstr, m_WordFound[i].c_str());
	//}
	//AfxMessageBox(Cstr);
	std::string outputpath;
	outputpath = "C:\\Users\\maxja\\source\\repos\\SearchWord\\results.txt";
	std::ofstream fout(outputpath.c_str(), std::ios::out);
	//for (auto it = m_WordFound.cbegin(); it < m_WordFound.end(); it++)
	for (size_t i = 0; i < m_WordFound.size(); i++)
	{
		//fout.write((char*)(m_WordFound[0].c_str()), m_WordFound.size() * sizeof(m_WordFound[0].c_str()));
		//fout.write((char*)(&(*it)), sizeof(&(*it)));
		fout.write((char*)(m_WordFound[i].c_str()), m_WordFound[i].size());
		fout << std::endl;
	}

	fout.close();

	
}

void CSearchWordDlg::OnEnChangeEditNbOfThreads()
{
	UpdateData(true);	
	m_NbOfThread = atoi(CStringA(m_strNbOfThread));
	assert(m_NbOfThread <= m_NbThreadMax);
	
}


