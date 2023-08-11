// Start of wxWidgets "Hello World" Program  https://docs.wxwidgets.org/3.2.2.1/plat_msw_install.html#msw_build_apps
// Includes: 
#include <wx/wx.h>
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include "MainFrame.h"
#include "DataStructures.h"
#include "Communication.h"
#include <fstream>
#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/oids.h>
#include <cryptopp/hex.h>
#include <cryptopp/cryptlib.h>
#include <wx/hash.h>
#include <iostream>
#include <string>
#include <cryptopp/ripemd.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>
#include <cryptopp/algebra.h>
#include <botan/base58.h>
#include <botan/botan.h>
#include <botan/ecdsa.h>
#include <botan/pubkey.h>
#include <botan/bigint.h>
#include <botan/base64.h>
#include <botan/sha2_32.h>
#include <botan/hex.h>
#include <cctype>
#include <filesystem>


namespace fs = std::filesystem;



MainFrame::MainFrame(const wxString& title): wxFrame(nullptr,wxID_ANY,title) {   //Constructor Base class
	wxPanel* panel = new wxPanel(this);

    //Button Definitions

	wxButton* Connect_Button = new wxButton(panel, wxID_ANY, "Connect", wxPoint(1, 1), wxSize(150, 25));
    wxButton* Download_Summary = new wxButton(panel, wxID_ANY, "Download Summary", wxPoint(1,26), wxSize(150, 25));
    wxButton* SyncMainNetTime = new wxButton(panel, wxID_ANY, "Sync MainNet Time", wxPoint(1, 51), wxSize(150, 25));
    wxButton* GetMasterNodeList = new wxButton(panel, wxID_ANY, "Get Master Node List", wxPoint(1, 76), wxSize(150, 25));
    wxButton* GenerateKeysButton = new wxButton(panel, wxID_ANY, "Generate NOSO Address", wxPoint(1, 100), wxSize(150, 25));
    wxButton* GetMasterNodeConfigButton = new wxButton(panel, wxID_ANY, "Get Master Node config", wxPoint(1, 124), wxSize(150, 25));

    //Static Text Definitions
    
    wxStaticText* CurrentBlockText = new wxStaticText(panel, wxID_ANY, "-Current Block: ", wxPoint(165, 6.25));
    CurrentBlockText->SetFont(wxFontInfo(8).Bold());
    wxStaticText* SummaryText = new wxStaticText(panel, wxID_ANY, "-Summary Processed: ", wxPoint(165, 32.25));
    SummaryText->SetFont(wxFontInfo(8).Bold());
    wxStaticText* TotalNosoAddressesLoadedText = new wxStaticText(panel, wxID_ANY, "-Total Noso Addesses Loaded : ", wxPoint(400, 35));
    TotalNosoAddressesLoadedText->SetFont(wxFontInfo(8).Bold());
    wxStaticText* MainNetTime = new wxStaticText(panel, wxID_ANY, "-MainNetTime:  ", wxPoint(165, 58.25));
    MainNetTime->SetFont(wxFontInfo(8).Bold());
    wxStaticText* MasterNodeList = new wxStaticText(panel, wxID_ANY, "-MasterNodeList:  ", wxPoint(165, 81.53));
    MasterNodeList->SetFont(wxFontInfo(8).Bold());
    wxStaticText* GenerateKeysText= new wxStaticText(panel, wxID_ANY, "-Generate Keys:  ", wxPoint(165, 104.81));
    GenerateKeysText->SetFont(wxFontInfo(8).Bold());
    wxStaticText* GetMasterNodeconfigText = new wxStaticText(panel, wxID_ANY, "-Get Master Node Config:  ", wxPoint(165, 128.09));
    GetMasterNodeconfigText->SetFont(wxFontInfo(8).Bold());


    //Dynamic object creation
    CurrentBlock = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(325, 6.25));
    GetSumaryText = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(325, 32.25));
    TotalNosoAddressesLoadedValue = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(575, 35));
    MainNetTimeText = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(325, 58.25));
    MasterNodeListText = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(325, 81.53));
    TextBox = new wxTextCtrl(panel, wxID_ANY, "Text Box", wxPoint(1, 200), wxSize(680, 250), wxTE_MULTILINE);
    GenerateKeysText= new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(325, 104.81));
 

    //Bind Operations to Button event

    SyncMainNetTime->Bind(wxEVT_BUTTON, &MainFrame::OnSyncMainNetTimeButtonClicked, this);
    Connect_Button->Bind(wxEVT_BUTTON, &MainFrame::OnConnectButtonClicked, this);
    Download_Summary->Bind(wxEVT_BUTTON, &MainFrame::OnDownloadSummaryButtonClicked, this);
    GetMasterNodeList->Bind(wxEVT_BUTTON, &MainFrame::GetMasterNodeList, this);
    GenerateKeysButton->Bind(wxEVT_BUTTON, &MainFrame::GenerateKeys, this);
    GetMasterNodeConfigButton->Bind(wxEVT_BUTTON, &MainFrame::GetMasterNodeConfig, this);
    this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);

    //Status Bar creation

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
    
    wxString zipFileName = "summary.zip";
    std::string outputDir = fs::current_path() / "";
    UnzipFile(zipFileName, outputDir);

    



    std::string filename = fs::current_path() / "data" / "sumary.psk";
    std::ifstream inputFile(filename, std::ios::binary);
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


void MainFrame::OnSyncMainNetTimeButtonClicked(wxCommandEvent& evt) {


    MainNetTimeText->SetLabel(std::to_string(GetMainetTimeStamp()));

}

void MainFrame::OnTimer(wxTimerEvent& event) {

   

 
   // Pending automatic Time showing Date and Time updated every second and synced to Main Net or NTP Servers.
}

void MainFrame::OnClose(wxCloseEvent& evt) {
    wxLogMessage("Wallet Closed");
    evt.Skip();

}

void MainFrame::GetMasterNodeList(wxCommandEvent& evt)
{
    std::string NODESTATUS_COMMAND = "NSLMNS\n";
    std::string DefaultNodeIp = "20.199.50.27";						//PENDING: Send commmand to NODE LIST, and connect to nodes starting from the old ones until connection is OK.
    int DefaultNodePort = 8080;										//PENDING: Set PORT from List of Nodes.
    std::string MasterNodeListString = SendStringToNode(DefaultNodeIp, DefaultNodePort, NODESTATUS_COMMAND);
    TextBox->SetLabel(MasterNodeListString);
    MasterNodeListText->SetLabel("Master Node List OK");

}

void MainFrame::GenerateKeys(wxCommandEvent& evt)
{
    
    std::string Sha256 = "Empty";
    std::string MD160 = "Empty";
    std::string Base58 = "Empty";
    int Checksum = 0;
    std::string CheckSumBase58 = "Empty";
    std::string NosoAddress = "Empty";
    std::string NosoAddressTest = "Empty";

    
    

    CryptoPP::AutoSeededRandomPool rng;

    // Generate ECDSA secp256k1 keys
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
    privateKey.Initialize(rng, CryptoPP::ASN1::secp256k1());

    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;
    privateKey.MakePublicKey(publicKey);

    // Save private Key to a String
    std::string privateKeyStr;
    CryptoPP::ByteQueue privateKeyQueue;
    privateKey.Save(privateKeyQueue);
    CryptoPP::HexEncoder privateKeyEncoder(new CryptoPP::StringSink(privateKeyStr));
    privateKeyQueue.CopyTo(privateKeyEncoder);
    privateKeyEncoder.MessageEnd();

    // Save public key to a String
    std::string publicKeyStr;
    CryptoPP::ByteQueue publicKeyQueue;
    publicKey.Save(publicKeyQueue);
    CryptoPP::HexEncoder publicKeyEncoder(new CryptoPP::StringSink(publicKeyStr));
    publicKeyQueue.CopyTo(publicKeyEncoder);
    publicKeyEncoder.MessageEnd();

    ///Show Results

    TextBox->AppendText("\n\nPUBLIC KEY: \n");
    TextBox->AppendText(publicKeyStr);
    TextBox->AppendText("\n\nPRIVATE KEY: \n");
    TextBox->AppendText(privateKeyStr);
    Sha256 = PublicKeyToSHA256(publicKeyStr);
    TextBox->AppendText("\n\nSHA256 Public Key encoded\n");
    TextBox->AppendText(Sha256);
    MD160 = CalculateMD160(Sha256);
    TextBox->AppendText("\n\nMD160\n");
    TextBox->AppendText(MD160);
    Base58= EncodeBase58(MD160);
    TextBox->AppendText("\n\nBase58\n");
    TextBox->AppendText(Base58);
    Checksum = CalculateCheckSum(Base58);
    TextBox->AppendText("\n\nCheckSum:\n");
    TextBox->AppendText(std::to_string(Checksum));
    CheckSumBase58 = BmDecto58(std::to_string(Checksum));
    TextBox->AppendText("\nCheckSumBase58:\n");
    TextBox->AppendText(CheckSumBase58);

    //Final Noso Address: N + Base58 + Base58(CheckSum)
    NosoAddressTest = "N" + Base58 + CheckSumBase58;
    TextBox->AppendText("\nNOSO Address:\n");
    TextBox->AppendText(NosoAddressTest);
    TextBox->AppendText("\n\nEND NOSO ADDRESS GENERATION\n");
      
}

void MainFrame::GetMasterNodeConfig(wxCommandEvent& evt)
{
    std::string MasterNodeConfig = GetMasterNodeConf();
    TextBox->Clear();
    TextBox->AppendText(MasterNodeConfig);


}



bool MainFrame::UnzipFile(const wxString& zipFileName, const wxString& outputDir)   //https://docs.wxwidgets.org/3.1/overview_archive.html
{
    wxFileInputStream fis(zipFileName);

    if (!fis.IsOk())
    {
        wxLogError(wxS("Couldn't open the file '%s'."), zipFileName);
        return false;
    }

    wxZipInputStream zis(fis);
    std::unique_ptr<wxZipEntry> upZe;

    while (upZe.reset(zis.GetNextEntry()), upZe) // != nullptr
    {
        // Access meta-data.
        wxString strFileName = outputDir + wxFileName::GetPathSeparator() + upZe->GetName();
        int nPermBits = upZe->GetMode();
        wxFileName fn;

        if (upZe->IsDir()) // This entry is a directory.
            fn.AssignDir(strFileName);
        else // This entry is a regular file.
            fn.Assign(strFileName);

        // Check if the directory exists, and if not, create it recursively.
        if (!wxDirExists(fn.GetPath()))
            wxFileName::Mkdir(fn.GetPath(), nPermBits, wxPATH_MKDIR_FULL);

        if (upZe->IsDir()) // This entry is a directory.
            continue; // Skip the file creation, because this entry is not a regular file, but a directory.

        // Read 'zis' to access the 'upZe's' data.
        if (!zis.CanRead())
        {
            wxLogError(wxS("Couldn't read the zip entry '%s'."), upZe->GetName());
            return false;
        }

        wxFileOutputStream fos(strFileName);

        if (!fos.IsOk())
        {
            wxLogError(wxS("Couldn't create the file '%s'."), strFileName);
            return false;
        }

        zis.Read(fos);

    }



    return true;
}

std::string MainFrame::PublicKeyToSHA256(const std::string& publicKey)
{


        // Create a SHA-256 hash object
        Botan::SHA_256 sha256;

        // Update the hash object with the input data
        sha256.update(reinterpret_cast<const Botan::byte*>(publicKey.data()), publicKey.size());

        // Finalize the hash and get the resulting digest
        Botan::secure_vector<Botan::byte> digest = sha256.final();

        // Convert the digest to a hexadecimal string
        std::string hashString;
        for (Botan::byte b : digest)
        {
            hashString += Botan::hex_encode(&b, 1);
        }

        std::transform(hashString.begin(), hashString.end(), hashString.begin(), [](unsigned char c) 
            
            {
            return std::tolower(c);
            });
        return hashString;
    
}

std::string MainFrame::CalculateMD160(const std::string& SHA256String)

{
    CryptoPP::RIPEMD160 hash;
    CryptoPP::byte digest[CryptoPP::RIPEMD160::DIGESTSIZE];

    hash.CalculateDigest(digest, reinterpret_cast<const CryptoPP::byte*>(SHA256String.c_str()), SHA256String.length());

    CryptoPP::HexEncoder encoder;
    std::string hashHex;
    encoder.Attach(new CryptoPP::StringSink(hashHex));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();

  
    return hashHex;



}

std::string MainFrame::EncodeBase58(const std::string& MD160String)
{

  
      //https://learnmeabitcoin.com/technical/base58 Test Conversion.
    std::vector<uint8_t> inputData = Botan::hex_decode(MD160String);
   
    std::string base58Result = Botan::base58_encode(inputData.data(), inputData.size());

    return base58Result;
    
}

int MainFrame::CalculateCheckSum(const std::string& StringChecksum)

{
        const std::string B58Alphabet = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
       int total = 0;

        for (int counter = 0; counter < StringChecksum.length(); ++counter) {
            total += B58Alphabet.find(StringChecksum[counter]);
        }

        return total;
   
}

std::string MainFrame::BmDecto58(const std::string& number)
{
    const std::string B58Alphabet = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

    std::string decimalValue = number;
    std::string resultado = "";

    while (decimalValue.length() >= 2) {
        int restante;
        int divisionResult = std::stoi(decimalValue) / 58;
        restante = std::stoi(decimalValue) - divisionResult * 58;
        decimalValue = std::to_string(divisionResult);
        resultado = B58Alphabet[restante] + resultado;
    }

    if (std::stoi(decimalValue) >= 58) {
        int restante;
        int divisionResult = std::stoi(decimalValue) / 58;
        restante = std::stoi(decimalValue) - divisionResult * 58;
        decimalValue = std::to_string(divisionResult);
        resultado = B58Alphabet[restante] + resultado;
    }

    if (std::stoi(decimalValue) > 0) {
        resultado = B58Alphabet[std::stoi(decimalValue)] + resultado;
    }

    return resultado;
}




/*
wxButton* button = new wxButton(panel, wxID_ANY, "New NOSO Address", wxPoint(150, 50), wxSize(100, 35));

wxCheckBox* checkBox = new wxCheckBox(panel, wxID_ANY, "CheckBox", wxPoint(200, 100));

wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, "Static Text", wxPoint(120, 150));

wxTextCtrl* textCtrl = new wxTextCtrl(panel, wxID_ANY, "TextCTRL", wxPoint(500, 145), wxSize(200, -1));
/*

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
