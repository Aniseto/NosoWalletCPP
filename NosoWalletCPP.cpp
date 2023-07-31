// Start of wxWidgets "Hello World" Program  https://docs.wxwidgets.org/3.2.2.1/plat_msw_install.html#msw_build_apps
// Includes: 
#include <wx/wx.h>
#include "MainFrame.h"
#include "DataStructures.h"
#include "Communication.h"
#include <fstream>




MainFrame::MainFrame(const wxString& title): wxFrame(nullptr,wxID_ANY,title) {   //Constructor Base class
	wxPanel* panel = new wxPanel(this);

	wxButton* Connect_Button = new wxButton(panel, wxID_ANY, "Connect", wxPoint(1, 1), wxSize(150, 25));
    wxButton* Download_Summary = new wxButton(panel, wxID_ANY, "Download Summary", wxPoint(1,26), wxSize(150, 25));

    wxStaticText* CurrentBlockText = new wxStaticText(panel, wxID_ANY, "-Current Block: ", wxPoint(165, 4));
    CurrentBlockText->SetFont(wxFontInfo(8).Bold());
    wxStaticText* SummaryText = new wxStaticText(panel, wxID_ANY, "-Sumary Processed: ", wxPoint(165, 35));
    SummaryText->SetFont(wxFontInfo(8).Bold());
    wxStaticText* TotalNosoAddressesLoadedText = new wxStaticText(panel, wxID_ANY, "-Total Noso Addesses Loaded : ", wxPoint(400, 35));
    TotalNosoAddressesLoadedText->SetFont(wxFontInfo(8).Bold());
    //wxStaticText* TotalNosoAddessesLoadedValue = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(550, 35));
    

    //wxStaticText* CurrentBlock = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(200, 4));
    CurrentBlock = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(285, 4));
    GetSumaryText = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(285, 35));
    TotalNosoAddressesLoadedValue = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(575, 35));

    Connect_Button->Bind(wxEVT_BUTTON, &MainFrame::OnConnectButtonClicked, this);
    Download_Summary->Bind(wxEVT_BUTTON, &MainFrame::OnDownloadSummaryButtonClicked, this);
    this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);


	wxStatusBar* statusBar =CreateStatusBar(); // Create Status Bar Bottom Window.
    statusBar->SetDoubleBuffered(true);

    
} 

void MainFrame::OnDownloadSummaryButtonClicked(wxCommandEvent& evt)
{
    std::string DefaultNodeIp = "20.199.50.27";						//PENDING: Send commmand to NODE LIST, and connect to nodes starting from the old ones until connection is OK.
    int DefaultNodePort = 8080;
    std::string GETZIPSUMARY_COMMAND = "GETZIPSUMARY\n";
    std::string GetZipSumaryResponse = SendStringToNode(DefaultNodeIp, DefaultNodePort, GETZIPSUMARY_COMMAND);
    GetSumaryText->SetLabel(wxString(GetZipSumaryResponse));              // Modify Static text to show Current Block
    
    

    //Unzip File
    system(".\\utils\\minizip\\minizip.exe -x -o .\\summary.zip");


 
    std::ifstream inputFile(".\\data\\sumary.psk", std::ios::binary);
    if (!inputFile) {
        std::cout << "Cannot open the file." << std::endl;
        //return void; 
    }
    else {
        std::cout << "File Opened!";
    }

    inputFile.seekg(0, std::ios::end); // Move pointer to end file
    std::streampos fileSize = inputFile.tellg(); // Getting file Size
    inputFile.seekg(0, std::ios::beg); // Moving pointer to the beginning 
    size_t numRecords = fileSize / sizeof(TSummaryData); // Calculate number of registers

    TotalNosoAddressesLoadedValue->SetLabel(wxString(std::to_string(numRecords)));

    //std::cout << endl << "NOSO Addressess loaded : " << numRecords << std::endl;

    std::vector<TSummaryData> dataVector(numRecords);

    inputFile.read(reinterpret_cast<char*>(dataVector.data()), fileSize);

    inputFile.close();

   
  
         
}

void MainFrame::OnConnectButtonClicked(wxCommandEvent& evt)

{
	wxLogStatus("Connecting to Noso Mainet...");
	std::string NODESTATUS_COMMAND= "NODESTATUS\n";
	std::string DefaultNodeIp = "20.199.50.27";						//PENDDING: Send commmand to NODE LIST, and connect to nodes starting from the old ones until connection is OK.
	int DefaultNodePort = 8080;										//PENDING: Set PORT from List of Nodes.
	std::string NodeStatus = SendStringToNode(DefaultNodeIp, DefaultNodePort, NODESTATUS_COMMAND);
    std::istringstream NodeStatusIss(NodeStatus);
    NodeStatusData data;
    //NODESTATUS 4 117969 0 0 65B69 0.4.1Aa1 1688913930 C4A6A 266 45996FF8BFA3287267CE2CE8746D3E02 FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1 1688913600 NpryectdevepmentfundsGE 0 62 FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1 27D68 9E57A 54526 7DEA3"

    NodeStatusIss >> data.NodeStatus;
    NodeStatusIss >> data.Peers;
    NodeStatusIss >> data.BlockNumber;
    NodeStatusIss >> data.Pending;
    NodeStatusIss >> data.Delta;
    NodeStatusIss >> data.Headers;
    NodeStatusIss >> data.Version;
    NodeStatusIss >> data.UTCTIme;
    NodeStatusIss >> data.MNsHash;
    NodeStatusIss >> data.MNsCount;
    NodeStatusIss >> data.LastBlockHash;
    NodeStatusIss >> data.BestHashDiff;
    NodeStatusIss >> data.LastBlockTimeEnd;
    NodeStatusIss >> data.LastBLockMiner;
    NodeStatusIss >> data.ChecksCount;
    NodeStatusIss >> data.LastBlockPoW;
    NodeStatusIss >> data.LastBlockDiff;
    NodeStatusIss >> data.Summary;
    NodeStatusIss >> data.GVTHash;
    NodeStatusIss >> data.NosoCFG;
    NodeStatusIss >> data.PSOHash;

    std::string CurrentBlockString = std::to_string(data.BlockNumber); //Transform from Integer to String
    CurrentBlock->SetLabel(wxString(CurrentBlockString));              // Modify Static text to show Current Block


	wxLogStatus("Connected, NODESTATUS SAVED.");

}

void MainFrame::OnClose(wxCloseEvent& evt) {
    wxLogMessage("Wallet Closed");
    evt.Skip();

}


/*
wxButton* button = new wxButton(panel, wxID_ANY, "New NOSO Address", wxPoint(150, 50), wxSize(100, 35));

wxCheckBox* checkBox = new wxCheckBox(panel, wxID_ANY, "CheckBox", wxPoint(200, 100));

wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, "Static Text", wxPoint(120, 150));

wxTextCtrl* textCtrl = new wxTextCtrl(panel, wxID_ANY, "TextCTRL", wxPoint(500, 145), wxSize(200, -1));

wxSlider* slider = new wxSlider(panel, wxID_ANY, 25, 0, 100, wxPoint(300,145),wxSize(200,-1));

wxGauge* gauge = new wxGauge(panel, wxID_ANY, 100, wxPoint(500, 255), wxSize(200, -1));
gauge->SetValue(50);

wxArrayString choices;
choices.Add("Item A");
choices.Add("Item B");

wxChoice* choice = new wxChoice(panel, wxID_ANY, wxPoint(150, 375), wxSize(100, -1),choices);
choice->Select(0);

wxSpinCtrl* spinCtrl = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(550, 375), wxSize(100, -1));

wxListBox* listBox = new wxListBox(panel, wxID_ANY, wxPoint(150, 475), wxSize(100, -1),choices);

wxRadioBox* radioBox = new wxRadioBox(panel, wxID_ANY, "radioBox", wxPoint(485, 475), wxDefaultSize, choices);
*/