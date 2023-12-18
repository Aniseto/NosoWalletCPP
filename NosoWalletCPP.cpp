﻿// Start of wxWidgets "Hello World" Program  https://docs.wxwidgets.org/3.2.2.1/plat_msw_install.html#msw_build_apps
// Includes: 
//#include <vector>
// UseDoxygen for code documentation
#include <wx/wx.h>
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include "MainFrame.h"
#include "DataStructures.h"
#include <algorithm>
#include <fstream>
#include <wx/hash.h>
#include <iostream>
#include <string>
#include <botan/auto_rng.h>
#include <botan/rsa.h>
#include <botan/base58.h>
#include <botan/botan.h>
#include <botan/ecdsa.h>
#include <botan/pubkey.h>
#include <botan/bigint.h>
#include <botan/base64.h>
#include <botan/sha2_32.h>
#include <botan/hex.h>
#include <cctype>
#include <botan/hash.h>
#include <filesystem>
#include <wx/statusbr.h>
#include <wx/statusbr.h>
#include <ctime>
#include <wx/timer.h>
#include <botan/auto_rng.h>
#include <botan/ecdsa.h>
#include <botan/hex.h>
#include <botan/bigint.h>
#include <botan/base58.h>
#include <botan/base64.h>
#include <botan/hex.h>
#include <botan/rmd160.h>

#include <botan/botan.h>
#include <botan/pubkey.h>
#include <botan/ec_group.h>
#include <botan/ecdsa.h>
#include <botan/auto_rng.h>
#include <botan/base64.h>
#include <botan/alg_id.h>
#include <botan/p11_ecdsa.h>
#include <botan/pem.h>

#include "Communication.h"
#include <botan/pkcs8.h>
#include <botan/pk_keys.h>
#include <iostream>
#include <vector>
#include <bitset>


namespace fs = std::filesystem; /// Added for Unix Like system compatibility: END.

MainFrame::MainFrame(const wxString& title): wxFrame(nullptr,wxID_ANY,title) {   
	wxPanel* panel = new wxPanel(this);

    //Menu Definition

    MainMenu = new wxMenu;
    openItem = new wxMenuItem(MainMenu, wxID_OPEN, "Open\tCtrl+O");
    saveItem = new wxMenuItem(MainMenu, wxID_SAVE, "Save\tCtrl+S");
    exitItem = new wxMenuItem(MainMenu, wxID_EXIT, "Exit\tAlt+F4");

    // Add menu items to the File menu
    MainMenu->Append(openItem);
    MainMenu->Append(saveItem);
    MainMenu->AppendSeparator();
    MainMenu->Append(exitItem);

    // Create the menu bar
    menuBar = new wxMenuBar;
    menuBar->Append(MainMenu, "&File");

    // Set the menu bar for the frame
    SetMenuBar(menuBar);
    
    //Timer

    timer = new wxTimer;


    //Button Definitions

	wxButton* Connect_Button = new wxButton(panel, wxID_ANY, "Reconnect", wxPoint(10, 1), wxSize(150, 25));
    wxButton* GenerateKeysButton = new wxButton(panel, wxID_ANY, "Generate NOSO Address", wxPoint(10, 30), wxSize(150, 25));
    wxStaticText* LogTextBoxLabel = new wxStaticText(panel, wxID_ANY, "Log: ", wxPoint(10, 475));
    LogTextBoxLabel->SetFont(wxFontInfo(8).Bold());
   

    //Grid Creation
    wxStaticText* GridTextBoxLabel = new wxStaticText(panel, wxID_ANY, "Noso Addresses: ", wxPoint(10, 75));
    GridTextBoxLabel->SetFont(wxFontInfo(8).Bold());
    TextBox = new wxTextCtrl(panel, wxID_ANY, "Text Box", wxPoint(10, 500), wxSize(595, 250), wxTE_MULTILINE);
    NosoAddressGrid = new wxGrid(panel, wxID_ANY, wxPoint(10, 75), wxSize(595, 250));
    NosoAddressGrid->HideRowLabels();
    NosoAddressGrid->CreateGrid(4, 4);  
    NosoAddressGrid->SetColSize(0, 250);
    NosoAddressGrid->SetColSize(1, 75);
    NosoAddressGrid->SetColSize(2, 125);
    NosoAddressGrid->SetColSize(3, 125);

    NosoAddressGrid->SetColLabelValue(0,"Address");
    NosoAddressGrid->SetColLabelValue(1,"Label");
    NosoAddressGrid->SetColLabelValue(2,"Pending");
    NosoAddressGrid->SetColLabelValue(3,"Balance");
   
    //NosoAddressGrid->Hide();
   

    //Bind Functions
    
    Connect_Button->Bind(wxEVT_BUTTON, &MainFrame::OnConnectButtonClicked, this);
    GenerateKeysButton->Bind(wxEVT_BUTTON, &MainFrame::GenerateKeys, this);
    
    this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);

    //Status Bar creation


    statusBar = new wxStatusBar(this, wxID_ANY);
    statusBar->SetDoubleBuffered(true);
    statusBar->SetFieldsCount(3);
    SetStatusBar(statusBar);

    //Send funds GUI controls
    wxFont TotalNosoFont(25, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    wxFont TotalNosoTextFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    SourceAddressText= new wxStaticText(panel, wxID_ANY, "Source Address: ", wxPoint(10, 350));
    SourceAddressText->SetFont(wxFontInfo(8).Bold());
    SourceAddressCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(135, 350));
    SourceAddressCtrl->SetSize(wxSize(250, -1));
    DestinationAddressText = new wxStaticText(panel, wxID_ANY, "Destination Address: ", wxPoint(10, 375));
    DestinationAddressText->SetFont(wxFontInfo(8).Bold());
    DestinationAddressCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(135, 375));
    DestinationAddressCtrl->SetSize(wxSize(250, -1));
    AmountToSendText = new wxStaticText(panel, wxID_ANY, "Amount To Send: ", wxPoint(10, 400));
    AmountToSendText->SetFont(wxFontInfo(8).Bold());
    AmountToSendCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(135, 400));
    AmountToSendCtrl->SetSize(wxSize(250, -1));
    ReferenceToSendText = new wxStaticText(panel, wxID_ANY, "Reference: ", wxPoint(10, 425));
    ReferenceToSendText->SetFont(wxFontInfo(8).Bold());
    ReferenceToSendCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(135, 425));
    ReferenceToSendCtrl->SetSize(wxSize(250, -1));
    TotalNosoText = new wxStaticText(panel, wxID_ANY, "Total Noso: ", wxPoint(175, 20));
    TotalNosoText->SetFont(TotalNosoTextFont);
    TotalNoso = new wxStaticText(panel, wxID_ANY, "0.00000000", wxPoint(425, 15));
    TotalNoso->SetFont(TotalNosoFont);


    wxButton* SendFunds_Button = new wxButton(panel, wxID_ANY, "Send", wxPoint(175, 450), wxSize(150, 25));
    SendFunds_Button->Bind(wxEVT_BUTTON, &MainFrame::OnSendNosoButtonClicked, this);
}


void MainFrame::OnConnectButtonClicked(wxCommandEvent& evt)

{
    TextBox->Clear();
    TextBox->AppendText("Connecting to Mainnet....\n");
    std::string NODESTATUS_COMMAND = "NODESTATUS\n";
    std::string DefaultNodeIp = "4.233.61.8";						//PENDDING: Send command to NODE LIST, and connect to nodes starting from the old ones until connection is OK.
    int DefaultNodePort = 8080;										//PENDING: Set PORT from List of Nodes.
    std::string NodeStatus = SendStringToNode(DefaultNodeIp, DefaultNodePort, NODESTATUS_COMMAND);
    std::istringstream NodeStatusIss(NodeStatus);
    NodeStatusData data;

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

    if (CurrentBlockString == "0") {
        statusBar->SetStatusText("Current Block: Error Getting Summary", 0);
    }
    else {

    
    statusBar->SetStatusText("Current Block: " + CurrentBlockString, 0);

    TextBox->AppendText("Connection Successful\n");
    //InitializeWallet();
    }
    //InitializeWallet();

}

std::vector<TSummaryData> MainFrame::DownloadSumary()
{
    TextBox->AppendText("Downloading Summary....\n");
    std::string DefaultNodeIp = "4.233.61.8";						//PENDING: Send command to NODE LIST, and connect to nodes starting from the old ones until connection is OK.
    int DefaultNodePort = 8080;
    std::string GETZIPSUMARY_COMMAND = "GETZIPSUMARY\n";
    std::string GetZipSumaryResponse = SendStringToNode(DefaultNodeIp, DefaultNodePort, GETZIPSUMARY_COMMAND);
    

    wxString zipFileName = "summary.zip";
    wxString outputDir = (fs::current_path() / "").string();

    UnzipFile(zipFileName, outputDir);


    std::string filename = (fs::current_path() / "data" / "sumary.psk").string();
    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile) {
        TextBox->AppendText("Cannot open the file.\n");
    }
    else {
        TextBox->AppendText("File Opened.\n");
    }

    inputFile.seekg(0, std::ios::end); // Move pointer to end file
    std::streampos fileSize = inputFile.tellg(); // Getting file Size
    inputFile.seekg(0, std::ios::beg); // Moving pointer to the beginning 
    size_t numRecords = fileSize / sizeof(TSummaryData); // Calculate number of registers

    TextBox->AppendText("Loading total Noso Addresses: \n");
    statusBar->SetStatusText("NOSO Addresses Loaded : " + std::to_string(numRecords), 1);


    std::vector<TSummaryData> dataVector(numRecords);

    


    inputFile.read(reinterpret_cast<char*>(dataVector.data()), fileSize);

    inputFile.close();
    return dataVector;
    //UpdateTable(dataVector);
    
    



}


void MainFrame::OnClose(wxCloseEvent& evt) {
    wxLogMessage("Wallet Closed");
    evt.Skip();

}

void MainFrame::GetMasterNodeList(wxCommandEvent& evt)
{
    std::string NODESTATUS_COMMAND = "NSLMNS\n";
    std::string DefaultNodeIp = "4.233.61.8";						//PENDING: Send command to NODE LIST, and connect to nodes starting from the old ones until connection is OK.
    int DefaultNodePort = 8080;										//PENDING: Set PORT from List of Nodes.
    std::string MasterNodeListString = SendStringToNode(DefaultNodeIp, DefaultNodePort, NODESTATUS_COMMAND);
    TextBox->SetLabel(MasterNodeListString);
    
    
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

    
    // Generate Keys using Botan Library
    Botan::AutoSeeded_RNG rng;
    Botan::ECDSA_PrivateKey private_key(rng, Botan::EC_Group("secp256k1"));
    std::vector<uint8_t> publicKeyPointBytes = private_key.public_point().encode(Botan::PointGFp::UNCOMPRESSED);
    std::vector<uint8_t> privateKeyBytes = Botan::BigInt::encode(private_key.private_value());
    std::string privateKeyBase64 = Botan::base64_encode(privateKeyBytes.data(), privateKeyBytes.size());
    std::string publicKeyPointBase64 = Botan::base64_encode(publicKeyPointBytes.data(), publicKeyPointBytes.size());

    //Save Generated Keys, public and private to Local Memory Wallet just to use it. MyWallet is a WalletData object

    MyWallet.SetPrivateKey(privateKeyBase64);
    MyWallet.SetPublicKey(publicKeyPointBase64);
     

    //Generate NOSO ADDRESS
    
    Sha256 = getHashSha256ToString(publicKeyPointBase64);
    MD160 = CalculateMD160(Sha256);
    Base58= EncodeBase58(MD160);  
    Checksum = CalculateCheckSum(Base58);
    CheckSumBase58 = BmDecto58(std::to_string(Checksum));

    //Final Noso Address: N + Base58 + Base58(CheckSum)
    NosoAddress = "N" + Base58 + CheckSumBase58;
    MyWallet.SetHash(NosoAddress);
    TextBox->AppendText("\nNOSO Address Generated :\n");
    TextBox->AppendText(NosoAddress);


    //Save New NOSO Address to local wallet file.

    std::string NosoWalletCPPPath = (fs::current_path() / "data" / "walletcpp.pkw").string();
    SaveWalletDataToFile(MyWallet, NosoWalletCPPPath);
    
    //Load again all addresses and update table

    //***** Potential Improvement, just add the new address to Grid, do not load again all addresses from file 

    walletCPPDataLoaded = ReadWalletDataFromNosoCPP(NosoWalletCPPPath);
    TextBox->AppendText("\nTotal NOSOCPP address loaded : ");
    TextBox->AppendText(std::to_string(walletCPPDataLoaded.size()));
    UpdateTable(walletCPPDataLoaded);

}


bool MainFrame::UnzipFile(const wxString& zipFileName, const wxString& outputDir)   //https://docs.wxwidgets.org/3.1/overview_archive.html
{
    wxFileInputStream fis(zipFileName);

    if (!fis.IsOk())
    {
       
        wxString ErrorOpening;
         
        TextBox->AppendText(ErrorOpening + "\n");
        return false;
    }

    wxZipInputStream zis(fis);
    std::unique_ptr<wxZipEntry> upZe;

    while (upZe.reset(zis.GetNextEntry()), upZe) 
    {
       
        wxString strFileName = outputDir + wxFileName::GetPathSeparator() + upZe->GetName();
        int nPermBits = upZe->GetMode();
        wxFileName fn;

        if (upZe->IsDir()) 
            fn.AssignDir(strFileName);
        else 
            fn.Assign(strFileName);

       
        if (!wxDirExists(fn.GetPath()))
            wxFileName::Mkdir(fn.GetPath(), nPermBits, wxPATH_MKDIR_FULL);

        if (upZe->IsDir()) 
            continue; 

        
        if (!zis.CanRead())
        {
            
            wxString ErrorReading;
            
            TextBox->AppendText(ErrorReading + "\n");
            return false;
        }

        wxFileOutputStream fos(strFileName);

        if (!fos.IsOk())
        {
            
            wxString ErrorCreating;
            
            TextBox->AppendText(ErrorCreating + "\n");
            return false;
        }

        zis.Read(fos);

    }



    return true;
}

std::string MainFrame::CalculateMD160(const std::string& SHA256String)
{
    Botan::RIPEMD_160 hash;
    Botan::secure_vector<uint8_t> hashResult = hash.process(reinterpret_cast<const uint8_t*>(SHA256String.data()), SHA256String.size());
    std::string hashHex = Botan::hex_encode(hashResult);

    for (char& c : hashHex) {
        c = std::toupper(c);
    }

    return hashHex;

}


std::string MainFrame::EncodeBase58(const std::string& MD160String)
{

    std::vector<uint8_t> inputData = Botan::hex_decode(MD160String);
   
    std::string base58Result = Botan::base58_encode(inputData.data(), inputData.size());
   
    return base58Result;
    
}

int MainFrame::CalculateCheckSum(const std::string& StringChecksum)

{
        
       int total = 0;

        for (int counter = 0; counter < StringChecksum.length(); ++counter) {
            total += B58Alphabet.find(StringChecksum[counter]);
        }

        return total;
   
}

std::string MainFrame::BmDecto58(const std::string& number)
{
    

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


std::string MainFrame::SignMessage(const std::string& message, const std::string& privateKeyBase64)

{
    try {
        Botan::AutoSeeded_RNG rng;
        std::vector<unsigned char> messages = nosoBase64Decode(message);
        Botan::secure_vector<uint8_t> hash_result_Message = calculateSHA1(messages);
        Botan::secure_vector<uint8_t> decodedPrivatKey = Botan::base64_decode(privateKeyBase64);
        Botan::AlgorithmIdentifier alg_id("ECDSA", Botan::AlgorithmIdentifier::USE_NULL_PARAM);

        Botan::BigInt private_key_value = Botan::BigInt::decode(decodedPrivatKey.data(), decodedPrivatKey.size());
        Botan::EC_Group secp256k1("secp256k1");
        Botan::ECDSA_PrivateKey private_key(rng, secp256k1, private_key_value);

        Botan::PK_Signer signer(private_key, rng, "EMSA1(SHA-1)", Botan::DER_SEQUENCE);
        signer.update(messages);
        std::vector<uint8_t> signature = signer.signature(rng);

        std::string signature_base64 = Botan::base64_encode(signature.data(), signature.size());
        return signature_base64;

    }


    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return ex.what();
    }
}




bool MainFrame::VerifySignature(const std::string& message, const std::string& signatureBase64, const std::string& publicKeyBase64)
{
    Botan::AutoSeeded_RNG rng;
    Botan::secure_vector<uint8_t> decodedPublicKey = Botan::base64_decode(publicKeyBase64);
    Botan::secure_vector<uint8_t> signature = Botan::base64_decode(signatureBase64.data(), signatureBase64.size());
    std::vector<unsigned char> messages = nosoBase64Decode(message);
   


    if (decodedPublicKey.size() != 65) {
        std::cerr << "Invalid UNCOMPRESSED public key format" << std::endl;
        return false;
    }
    try {
        std::vector<uint8_t> xCoord(decodedPublicKey.begin() + 1, decodedPublicKey.begin() + 33);
        std::vector<uint8_t> yCoord(decodedPublicKey.begin() + 33, decodedPublicKey.end());

        Botan::BigInt x_bigint(xCoord.data(), xCoord.size());
        Botan::BigInt y_bigint(yCoord.data(), yCoord.size());

        Botan::EC_Group secp256k1("secp256k1");

        Botan::PointGFp point = secp256k1.point(x_bigint, y_bigint);

        Botan::ECDSA_PublicKey publicKey(secp256k1, point);
        Botan::PK_Verifier verifier(publicKey, "EMSA1(SHA-256)", Botan::DER_SEQUENCE);
        verifier.update(messages);

        return verifier.check_signature(signature);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return false;
    }
}


std::string MainFrame::addPEMHeaders(const std::string& privateKeyBase64, const std::string& header, const std::string& footer)
{
    
    std::string pemPrivateKey = header + "\n";
    pemPrivateKey += privateKeyBase64 + "\n";
    pemPrivateKey += footer + "\n";

    return pemPrivateKey;
}
    
   
bool MainFrame::SaveWalletDataToFile(const WalletData& walletData, const std::string& filePath)
{
   
    std::ofstream outputFile(filePath, std::ios::binary | std::ios::app);

    // Check if the file was opened successfully
    if (!outputFile) {
      
        TextBox->AppendText("\nError: Could not open the file for writing.");
        return false;
    }

    // Write the WalletData structure to the file
    outputFile.write(reinterpret_cast<const char*>(&walletData), sizeof(WalletData));

    // Check for write errors
    if (outputFile.fail()) {
       
        TextBox->AppendText("\nError: Failed to write data to the file.");
        outputFile.close(); // Close the file
        return false;
    }

    // Close the file
    outputFile.close();

   
    TextBox->AppendText("\nNew Address added to walletcpp.pkw");
    return true;
}

void MainFrame::UpdateDateAndTime()
{
    wxDateTime currentDateTime = wxDateTime::Now();
    wxString dateTimeStr = currentDateTime.Format("%Y-%m-%d %H:%M:%S");
    statusBar->SetStatusText(dateTimeStr, 2);
    

}

void MainFrame::OnOpen(wxCommandEvent& event)
{
}
void MainFrame::OnSave(wxCommandEvent& event)
{
}
void MainFrame::OnExit(wxCommandEvent& event)
{

}
void MainFrame::InitializeWallet()
{
 
    //TotalBalance
    //int64_t TotalBalance = 0;
    int64_t Accomulated = 0;
    //SetTimer
    timer->SetOwner(this);
    timer->Start(1000);  // 1000 ms = 1 second
    Connect(timer->GetId(), wxEVT_TIMER, wxTimerEventHandler(MainFrame::OnTimer), NULL, this);

    //Call Connect ButtonFuction
    wxCommandEvent fakeEvent(wxEVT_BUTTON, wxID_ANY); // Create a fake button click event
    OnConnectButtonClicked(fakeEvent); // Call the function
    
    SumarydataVector = MainFrame::DownloadSumary();
    
    //Check if walletcpp.pkw exists on /data directory, if exists load all addresses, if no create a new NOSO address, save to file and load Address

    std::string WalletCPPFullPath = (fs::current_path() / "data" / "walletcpp.pkw").string();

    if (DoesFileExist(WalletCPPFullPath))
    {
        TextBox->AppendText("\nNOSOCPP Wallet File Detected. Loading addresses\n");
   
        walletCPPDataLoaded = ReadWalletDataFromNosoCPP(WalletCPPFullPath);
 
        //int64_t TotalBalance = 0;
        //Show Data on central Table
        NosoAddressGrid->DeleteRows();
        for (size_t i = 0; i < walletCPPDataLoaded.size(); ++i) 
        {
            std::string HashKeyLoaded = walletCPPDataLoaded[i].GetHash();
            std::string Label = walletCPPDataLoaded[i].GetLabel();
            std::int64_t Pending = walletCPPDataLoaded[i].GetPending();
          
            std::int64_t Balance = GetBalanceFromNosoAddress(SumarydataVector, HashKeyLoaded.c_str());
          
            std::string ConvertedToDecimal = Int2Curr(Balance);
            Accomulated = 0;
            std::int64_t Accomulated = TotalBalance + Balance;
            TotalBalance = Accomulated;
            
            
            NosoAddressGrid->AppendRows(1); // Add a new row
            NosoAddressGrid->SetCellValue(i, 0, HashKeyLoaded);
            NosoAddressGrid->SetCellValue(i, 1, Label);
            NosoAddressGrid->SetCellValue(i, 2, std::to_string(Pending));
            NosoAddressGrid->SetCellValue(i, 3, ConvertedToDecimal);
            

        } 
        //TextBox->AppendText(std::to_string(TotalBalance));

        }
        else {
        
        TextBox->AppendText("\nWalletcpp.pkw File does not exist, some problems happened as walletcpp.pkw cannot be created in Data directory.\n ");

        }
    
  
    UpdateTable(walletCPPDataLoaded);


    MainFrame::GetPendings();



}

bool MainFrame::DoesFileExist(const std::string& filePath)
{
        std::ifstream file(filePath);
        return file.good();
    
}

std::vector<WalletData> MainFrame::ReadWalletDataFromNosolite(const std::string& filePath)
{
    std::vector<WalletData> dataVectorWallet;
    std::ifstream inputFileWallet(filePath, std::ios::binary);
    if (!inputFileWallet) {
        
        wxString ErrorOpeningWalletFile;
        ErrorOpeningWalletFile.Printf("Cannot open /data/wallet.pkw file");
        TextBox->AppendText(ErrorOpeningWalletFile + "\n");
        return dataVectorWallet; // Return an empty vector in case of an error
    }

    inputFileWallet.seekg(0, std::ios::end);
    std::streampos fileSizeWallet = inputFileWallet.tellg();
    inputFileWallet.seekg(0, std::ios::beg);
    size_t numRecordsWallet = fileSizeWallet / sizeof(WalletData);

    dataVectorWallet.resize(numRecordsWallet);

    inputFileWallet.read(reinterpret_cast<char*>(dataVectorWallet.data()), fileSizeWallet);

    inputFileWallet.close();

    return dataVectorWallet;
    
}

std::vector<WalletData> MainFrame::ReadWalletDataFromNosoCPP(const std::string& filePath)
{
    std::vector<WalletData> dataVectorWalletCPP;
    std::ifstream inputFile(filePath, std::ios::binary);
    if (!inputFile) {
        TextBox->AppendText("\nError: Could not open NosowalletCPP.PKW the file for reading.");
        return dataVectorWalletCPP; // Return an empty vector on error
    }
    WalletData walletData;
    while (inputFile.read(reinterpret_cast<char*>(&walletData), sizeof(WalletData))) {
        dataVectorWalletCPP.push_back(walletData);
    }
    if (inputFile.fail() && !inputFile.eof()) {
        TextBox->AppendText("\nError: Failed to read data from the file.");
        inputFile.close(); // Close the file
        return dataVectorWalletCPP; // Return what was read so far on error
    }
    inputFile.close();

    TextBox->AppendText("\nData read from "+ filePath +" and appended to the vector.");
    
    return dataVectorWalletCPP;
}

void MainFrame::OnTimer(wxTimerEvent& event) {
    UpdateDateAndTime();
}

std::string MainFrame::GetPendings()
{
   
    TextBox->AppendText("\nGetting Pending orders from Node.\n");
    std::string NODESTATUS_COMMAND = "NSLPEND\n";
    std::string DefaultNodeIp = "20.199.50.27";						//PENDDING: Send command to NODE LIST, and connect to nodes starting from the old ones until connection is OK.
    int DefaultNodePort = 8080;										//PENDING: Set PORT from List of Nodes.
    std::string PendingOrdersText = SendStringToNode(DefaultNodeIp, DefaultNodePort, NODESTATUS_COMMAND);
    
    if (PendingOrdersText == "")
    { 
        TextBox->AppendText("No pending Orders");
        return "NULL";
    }
    else 
    {
        ordersVector.clear();
        TextBox->AppendText("Pending Orders: \n");
        TextBox->AppendText(PendingOrdersText);
        TextBox->AppendText("Total lines ");
        TextBox->AppendText(std::to_string(std::count(PendingOrdersText.begin(), PendingOrdersText.end(), ' ')));
        
        //Format: TRFR,N2Xh9dUcsdarJXgx7Q1jusVn8MpLUFh,NoSLwVuSMm8K2iQni5Fk3Y2uJy8SDn,34000000000,3400000

        std::istringstream iss(PendingOrdersText);
        std::vector<std::string> attributes;
        std::string block;
        //ordersVector.clear();

        while (std::getline(iss, block, ' '))
        {
            std::istringstream block_stream(block);
            if (block.find('\n') != std::string::npos) 
            {
                break;  // Exit the loop if we reached the end of the file or detects \n
            }
            std::string attribute;
            PendingOrders order;
            TextBox->AppendText("\nDEBUG : Block Readed \n");
            TextBox->AppendText(block);
            std::getline(block_stream, attribute, ',');
            order.OrderType = attribute;

            std::getline(block_stream, attribute, ',');
            order.SourceAddress = attribute;

            std::getline(block_stream, attribute, ',');
            order.DestinationAddress = attribute;

            std::getline(block_stream, attribute, ',');
            order.Amount = std::stoll(attribute);

            std::getline(block_stream, attribute);
            order.Fee = std::stoll(attribute);

            ordersVector.push_back(order);

        }
       
        TextBox->AppendText("Number of Pending Orders: " + std::to_string(ordersVector.size()) + "\n");

        //int64_t TotalSource = 0;
        for (const auto& order : ordersVector) 
        {
            TextBox->AppendText("\nDEBUG : Pending Order: \n");
            TextBox->AppendText("OrderType: " + order.OrderType + "\n");
            TextBox->AppendText("SourceAddress: " + order.SourceAddress + "\n");
            TextBox->AppendText("DestinationAddress: " + order.DestinationAddress + "\n");
            TextBox->AppendText("Amount: " + std::to_string(order.Amount) + "\n");
            TextBox->AppendText("Fee: " + std::to_string(order.Fee) + "\n");
            
            for (size_t i = 0; i < walletCPPDataLoaded.size(); ++i)
            {
                if (order.SourceAddress == walletCPPDataLoaded[i].GetHash()) {
                    
                    TextBox->AppendText("Address" + order.SourceAddress + " is in my wallet\n");
                    //TotalSource += order.Amount;
                    int64_t TotalSource = GetPendingPaysFromNosoAddress(order.SourceAddress);
                    TextBox->AppendText("\nTotal Pending Pays: " + std::to_string(TotalSource) + "\n");
                    int TotalRows = NosoAddressGrid->GetNumberRows();

                    for (int row = 0; row < TotalRows; ++row) {

                        wxString addressValue = NosoAddressGrid->GetCellValue(row, 0);

                        std::string addressString = addressValue.ToStdString();


                        if (order.SourceAddress == addressString)
                        {



                            //float ResultToShow;
                            double ResultToShow = static_cast<double>(TotalSource) / 100000000.0;
                            //int64_t ResultToShow = TotalSource / 100000000.0;
                            TextBox->AppendText(std::to_string(ResultToShow));
                            TextBox->AppendText("\nAddress found on Grid");
                            TextBox->AppendText("\nTotal Pending Pays to Show: " + std::to_string(ResultToShow) + "\n");
                            if ( ResultToShow > 0)
                            {

                                NosoAddressGrid->SetCellValue(row, 2,std::to_string(ResultToShow));
                                NosoAddressGrid->SetCellBackgroundColour(row, 2, wxColour(255, 0, 0));  // Red color
                                //                                NosoAddressGrid->SetCellBackgroundColour(row, 2, wxColour(255, 0, 0));  // Red color

                                NosoAddressGrid->SetCellTextColour(row, 2, wxColour(255, 255, 255));  // White color
                            }
                            if (ResultToShow < 0)
                            {
								NosoAddressGrid->SetCellValue(row, 2,std::to_string(ResultToShow));
								NosoAddressGrid->SetCellBackgroundColour(row, 2, wxColour(0, 128, 0));  // Red color
								NosoAddressGrid->SetCellTextColour(row, 2, wxColour(255, 255, 255));  // White color
							}
                        }


                        
                        if (order.DestinationAddress == addressString)
                        {
                            //float ResultToShow;
                            double ResultToShow = static_cast<double>(TotalSource) / 100000000.0;
                            //int64_t ResultToShow = TotalSource / 100000000.0;
                            TextBox->AppendText(std::to_string(ResultToShow));
                            TextBox->AppendText("\nAddress found on Grid");
                            TextBox->AppendText("\nTotal Pending Pays to Show: " + std::to_string(ResultToShow) + "\n");
                            if (ResultToShow > 0)
                            {

                                NosoAddressGrid->SetCellValue(row, 2, std::to_string(ResultToShow));
                                NosoAddressGrid->SetCellBackgroundColour(row, 2, wxColour(0, 128, 0));  // Red color
                                //                                NosoAddressGrid->SetCellBackgroundColour(row, 2, wxColour(255, 0, 0));  // Red color

                                NosoAddressGrid->SetCellTextColour(row, 2, wxColour(255, 255, 255));  // White color
                            }
                            if (ResultToShow < 0)
                            {
                                NosoAddressGrid->SetCellValue(row, 2, std::to_string(ResultToShow));
                                NosoAddressGrid->SetCellBackgroundColour(row, 2, wxColour(255, 0, 0));  // Red color
                                NosoAddressGrid->SetCellTextColour(row, 2, wxColour(255, 255, 255));  // White color
                            }
                        }
                    }

                    
              
                }
                
            }

        }
        
	}
    return PendingOrdersText;
}


int64_t MainFrame::GetAddressPendingPays(std::string NosoAddress)
{
    // Get all Pending Orders
    // Search for all orders with same Noso Address in Source or Destination.
    // Add all values and return total.   
    int64_t Total = 0;




  
    return Total;
        ///
    
   
}

std::vector<unsigned char> MainFrame::nosoBase64Decode(const std::string& input) //Thanks to PasichDEV https://github.com/pasichDev/NosoCpp/blob/d8ee2b5de00ac21eb200eef2a8faf4cdec19aa9a/nCripto.cpp#L225

{

    std::vector<int> indexList;
    for (char c : input) {
        auto it = B64Alphabet.find(c);
        if (it != std::string::npos) {
            indexList.push_back(static_cast<int>(it));
        }
    }

    std::string binaryString;
    for (int i : indexList) {
        std::string binary = std::bitset<6>(i).to_string();
        binaryString += binary;
    }

    std::string strAux = binaryString;
    std::vector<unsigned char> tempByteArray;

    while (strAux.length() >= 8) {
        std::string currentGroup = strAux.substr(0, 8);
        int intVal = std::stoi(currentGroup, nullptr, 2);
        tempByteArray.push_back(static_cast<unsigned char>(intVal));
        strAux = strAux.substr(8);
    }

    return tempByteArray;
}

OrderData MainFrame::SendFundsFromAddress(std::string& SourceAddress, std::string& DestinationAddress, int64_t& AmountToSend, int64_t& Commision, std::string& Reference, std::string& OrderTime, int line)
{
   
    OrderData OrderInfo;
    bool Exists = CheckIfNosoAddressExistsOnMyWallet(SourceAddress, walletCPPDataLoaded);
  

    if (CheckIfNosoAddressExistsOnMyWallet(SourceAddress,walletCPPDataLoaded)&&CheckIfNosoAddressIsValid(DestinationAddress))
    {   
        OrderInfo.SetSenderHashAddress(SourceAddress);
        OrderInfo.SetReceiverHashAddress(DestinationAddress);
        int64_t SourceAddressBalance = GetBalanceFromNosoAddress(SumarydataVector,SourceAddress.c_str());
        
        if (AmountToSend < (SourceAddressBalance + 1000000))
        {
         
            OrderInfo.SetAmmountTrfe(AmountToSend);
            OrderInfo.SetAmmountFee(Commision);
            OrderInfo.SetOrderType("TRFR");
            OrderInfo.SetOrderReference(Reference);  
            OrderInfo.SetTimeStamp(std::stoi(OrderTime));
            OrderInfo.SetOrderLines(line);
            OrderInfo.SetOrderID("");
            OrderInfo.SetOrderType("TRFR");
            OrderInfo.SetSenderPublicKey(GetPublicKeyFromNosoAddress(SourceAddress));

            OrderInfo.SetSignature(SignMessage(std::to_string(OrderInfo.GetTimeStamp()) + SourceAddress + DestinationAddress + std::to_string(AmountToSend) +
                std::to_string(Commision) + std::to_string(line), GetPrivateKeyFromNosoAddress(SourceAddress)));
            OrderInfo.SetTrfID(GetTransferHash(std::to_string(OrderInfo.GetTimeStamp()) + SourceAddress + DestinationAddress + std::to_string(AmountToSend) + CurrentBlockString));
         

        }
        else {
            TextBox->AppendText("\nNot enough Source Address Balance!");
        }
        
        return OrderInfo;

    }

    else

    {
        TextBox->AppendText("\nERROR SOURCE ADDRESS OR DESTINATION ADDRESS ARE NOT VALID");
        TextBox->AppendText(SourceAddress);
    }
   
    
    
    return OrderInfo;
}

int64_t MainFrame::GetBalanceFromNosoAddress(const std::vector<TSummaryData>& DataVector, const char* NosoAddress)
{
    
    for (const TSummaryData& datos : DataVector) {
        
        if (std::strcmp(datos.Hash, NosoAddress) == 0) {
         
            return datos.Balance;

        }
    }
    
    return 0; // Control what happens if there is no address, then the balance is 0 ?
}

void MainFrame::UpdateTable(std::vector<WalletData>& dataVectorAddress)
{
    
   // Read info from DataVectorAddress
    NosoAddressGrid->DeleteRows();
    for (size_t i = 0; i < dataVectorAddress.size(); ++i) {
        std::string HashKeyLoaded = dataVectorAddress[i].GetHash();
        std::string Label = dataVectorAddress[i].GetLabel();
        std::int64_t Pending = dataVectorAddress[i].GetPending();
        std::int64_t Balance = GetBalanceFromNosoAddress(SumarydataVector, HashKeyLoaded.c_str());
        std::string decimalBalance = Int2Curr(Balance);// Show data in decimal value
        std::stringstream stream;
        stream << std::fixed << std::setprecision(8) << decimalBalance;
        std::string formattedDecimalBalance = stream.str();
        NosoAddressGrid->AppendRows(1); // Add a new row
        NosoAddressGrid->SetCellValue(i, 0, HashKeyLoaded);
        NosoAddressGrid->SetCellValue(i, 1, Label);
        NosoAddressGrid->SetCellValue(i, 2, std::to_string(Pending));
        NosoAddressGrid->SetCellValue(i, 3, decimalBalance);
    }
    //Update Total value
    wxString TotalNosoToShow = wxString::Format("%lld", TotalBalance);
    //Set TotalNoso value of decimalBalance
    std::string Nosototal = Int2Curr(TotalBalance);
    //std::string NosoTotal = TotalNosoToShow.ToStdString();

    
    TotalNoso->SetLabel(Nosototal);

    //NosoAddressGrid->SetSortingColumn(3,true); // El segundo parámetro (true) indica orden descendente
}

std::string MainFrame::GetPublicKeyFromNosoAddress(const std::string & NosoAddress)
{
   
    std::string notfound="Not Found";

    for (size_t i = 0; i < walletCPPDataLoaded.size(); ++i)
    {
        if (walletCPPDataLoaded[i].GetHash() == NosoAddress)
        {
            
            return walletCPPDataLoaded[i].GetPublicKey();
          
        }
    }

    return notfound;
}

std::string MainFrame::GetPrivateKeyFromNosoAddress(const std::string& NosoAddress)
{


        std::string notfound = "Not Found";

        for (size_t i = 0; i < walletCPPDataLoaded.size(); ++i)
        {
            if (walletCPPDataLoaded[i].GetHash() == NosoAddress)
                return walletCPPDataLoaded[i].GetPrivateKey();
        }

        return notfound;

}

std::string MainFrame::GetTransferHash(const std::string& Transfer)
{
    std::string ResultString = "";
    std::string ResultStringToSHA256 = "";
    std::string ResultStringToHex58 = "";
    std::string Base58Sumatory="";
    std::string Key;

    
    ResultStringToSHA256 = getHashSha256ToString(Transfer);   
    ResultStringToHex58 = EncodeBase58(ResultStringToSHA256);  
    Base58Sumatory = BMB58Resumen(ResultStringToHex58);
    Key = BmDecto58(Base58Sumatory);
    ResultString = "tR" + ResultStringToHex58 + Key;

    return ResultString;
}

bool MainFrame::CheckIfNosoAddressExistsOnMyWallet(const std::string& NosoAddressToCheck,std::vector<WalletData> WalletToSearch)
{

    for (size_t i = 0; i < WalletToSearch.size(); ++i)
    {
       
        if (WalletToSearch[i].GetHash() == NosoAddressToCheck)
        {
            return true;
        }
    }
        return false;
    
     
}

bool MainFrame::CheckIfNosoAddressIsValid(const std::string& NosoAddressToCheckIfValid)
{
    
    bool result = false;

    //Debug
    //TextBox->AppendText("\nTesting If Address is valid ->: ");
    //TextBox->AppendText(NosoAddressToCheckIfValid);
   // TextBox->AppendText("\nLength ->: ");
   // TextBox->AppendText(std::to_string(NosoAddressToCheckIfValid.length()));
    //TextBox->AppendText("\nTesting If Address starts with 'N' ->: ");
    //TextBox->AppendText(NosoAddressToCheckIfValid);
    //if (NosoAddressToCheckIfValid[0] == 'N') {
        //TextBox->AppendText("\nYes !");
   // }
   // else {
       // TextBox->AppendText("\nNo ! ");
   // }
    


    //Debug

    if (NosoAddressToCheckIfValid.length() > 20 && NosoAddressToCheckIfValid[0] == 'N') {
        std::string OrigHash = NosoAddressToCheckIfValid.substr(1, NosoAddressToCheckIfValid.length() - 1);
    
        if (IsValid58(OrigHash)) {
            std:: string Hash=  OrigHash.substr(0, OrigHash.length() - 2);
            int Checksum = CalculateCheckSum(Hash);
       
            std::string CheckSumBase58 = BmDecto58(std::to_string(Checksum));
            OrigHash = 'N' + Hash + CheckSumBase58;
         

            if (OrigHash == NosoAddressToCheckIfValid) {
                result = true;
                return result;
            }
        }
    }

    return result;
}

bool MainFrame::IsValid58(const std::string& Base58Text)
{

    bool result = true;

    if (Base58Text.length() > 0) {
        for (size_t i = 0; i < Base58Text.length(); i++) {
            bool isCharValid = false;

            for (size_t j = 0; j < B58Alphabet.length(); j++) {
                if (Base58Text[i] == B58Alphabet[j]) {
                    isCharValid = true;
                    break;
                }
            }

            if (!isCharValid) {
                result = false;
                break;
            }
        }
    }
    else {
        result = false;
    }

    return result;
    
}

std::string MainFrame::BMB58Resumen(const std::string& Number58)
{
    
    int total = 0;
        for (size_t counter = 0; counter < Number58.length(); counter++) {
            total += B58Alphabet.find(Number58[counter]) - 1;
        }
        return std::to_string(total);
}

void MainFrame::OnSendNosoButtonClicked(wxCommandEvent& evt)
{
    std::thread([this]() {

        wxString SourceAddress = SourceAddressCtrl->GetValue();
        wxString DestinationAddress = DestinationAddressCtrl->GetValue();
        wxString AmountToSend = AmountToSendCtrl->GetValue();
        wxString Reference = ReferenceToSendCtrl->GetValue();
        std::string ReferenceString = Reference.ToStdString(); // Pending controlling no spaces
        std::string SourceAddressString = SourceAddress.ToStdString();
        std::string DestinationAddressString = DestinationAddress.ToStdString();
        std::string AmountToSendString = AmountToSend.ToStdString();
        std::string OrderHashString;
        std::string OrderString;
        std::string ResultOrderID = "";
        int64_t CoinsAvailable = 0;
        double doubleValue;
        int TrxLine = 1;
        AmountToSend.ToDouble(&doubleValue);
        int64_t Amount = static_cast<int64_t>(doubleValue * 100000000);
        int64_t Comission = GetFee(Amount);
        bool SourceAddressExists = "0";
        int64_t CheckFunds = Amount + Comission;


        if (CheckIfNosoAddressIsValid(SourceAddressString) && CheckIfNosoAddressIsValid(DestinationAddressString))
        {

            if (CheckIfNosoAddressExistsOnMyWallet(SourceAddressString, walletCPPDataLoaded))
            {


                if (CoinsAvailable = GetBalanceFromNosoAddress(SumarydataVector, SourceAddressString.c_str()) > CheckFunds)
                {

                    TextBox->AppendText("\n * Sending " + AmountToSend + " NOSO to " + DestinationAddress);
                    OrderData OrderToSend;
                    std::string OrderTime = std::to_string(GetMainetTime());
                    OrderToSend = SendFundsFromAddress(SourceAddressString, DestinationAddressString, Amount, Comission, ReferenceString, OrderTime, TrxLine);
                    OrderHashString = OrderTime + OrderToSend.GetTrfID();
                    ResultOrderID = GetOrderHash(std::to_string(TrxLine) + OrderHashString);
                    OrderString = GetPTCEcn("ORDER");
                    OrderString += "ORDER " + std::to_string(TrxLine) + " $";
                    OrderString += GetStringFromOrder(OrderToSend) + "\n";
                    std::string SendOrderResult;
                    std::string SEND_COMMAND = OrderString;
                    std::string DefaultNodeIp = "20.199.50.27";						//PENDING: Send command to NODE LIST, and connect to nodes starting from the old ones until connection is OK.
                    int DefaultNodePort = 8080;										//PENDING: Set PORT from List of Nodes.
                    std::string ResultOrder = SendStringToNode(DefaultNodeIp, DefaultNodePort, SEND_COMMAND);
                    TextBox->AppendText("\n * Order SENT !,OrderID :  ");
                    TextBox->AppendText(ResultOrder);
                    //TextBox->Connect(wxID_ANY, wxEVT_TEXT_URL, wxTextUrlEventHandler(MyFrame::OnUrlClick), NULL, this);

                    wxString url = "https://explorer.nosocoin.com/getordersinfo.html?orderid=" + ResultOrder;
                    wxString textWithUrl = "Check Explorer: " + url;

                    TextBox->AppendText(textWithUrl);
                    TextBox->AppendText("\n * Transaction details -> https://explorer.nosocoin.com/getordersinfo.html?orderid=" + ResultOrder);
                        
                    
                    return;



                }
                else
                {
                    TextBox->AppendText("\nNot enough Noso to send !");
                    TextBox->AppendText("\nCoins Available: ");
                    CoinsAvailable = GetBalanceFromNosoAddress(SumarydataVector, SourceAddressString.c_str());
                    TextBox->AppendText(std::to_string(CoinsAvailable));
                }
            }
            else
            {
                TextBox->AppendText("\nSource Address is VALID but does not exist on your WalletCPP !");
                return;
            }
        }
        else
        {
            if (!CheckIfNosoAddressIsValid(SourceAddressString))
            {
                TextBox->AppendText("\nSource Address is NOT VALID !");
            }
            if (!CheckIfNosoAddressIsValid(DestinationAddressString))
            {
                TextBox->AppendText("\nDestination Address are NOT VALID !");
            }

            return;
        }


        }).detach();
}

int64_t MainFrame::GetMainetTime()
{
    std::string ip = "20.199.50.27";
    int port = 8080;
    std::string command = "NSLTIME\n";
    std::string TimeString = SendStringToNode(ip, port, command);
    int64_t TimeInt = stoll(TimeString);
    
    return TimeInt;
    
}

std::string MainFrame::AddChar(char C, const std::string& S, int N)
{
    std::ostringstream oss;
    if (S.length() < N) {
        oss << std::setw(N) << std::setfill(C) << std::right << S;
    }
    else {
        oss << S;
    }
    return oss.str();
    
}

std::string MainFrame::Int2Curr(int64_t Value)
{
    std::string Result = std::to_string(std::abs(Value));
    Result = AddChar('0', Result, 9);
    Result.insert(Result.length() - 8, ".");

    if (Value < 0)
        Result = "-" + Result;

    return Result;
    
}

int64_t MainFrame::Curr2Int(const std::string& CurrStr)
{
    int64_t Result = 0;
	std::string Str = CurrStr;
	Str.erase(std::remove(Str.begin(), Str.end(), ','), Str.end());
	Str.erase(std::remove(Str.begin(), Str.end(), '.'), Str.end());
	Result = std::stoll(Str);
	return Result;
	
    
}

int64_t MainFrame::GetMaximumToSend(int64_t ammount)
{
    //Calculate the maximum amount that can be sent using a specific Amount
    int64_t Available = ammount;
    int64_t maximum = (Available * Comisiontrfr) / (Comisiontrfr + 1);
    int64_t Fee = maximum / Comisiontrfr;

    if (Fee < MinimunFee) {
        Fee = MinimunFee;
    }

    int64_t SenT = maximum + Fee;
    int64_t Diff = Available - SenT;

    return maximum + Diff;
 
}

std::string MainFrame::GetOrderHash(const std::string& textLine)
{
    
    std::string resultToProcess=getHashSha256ToString(textLine);
    std::string resultHex36= BMHexTo58(resultToProcess, 36); 
    std::string resultNosoBase36 = BMHexTo58(getHashSha256ToString("noso"), 36);
    std::string result = "OR" + resultHex36;

    return result;
}

std::string MainFrame::BMHexTo58(const std::string& numerohex, const Botan::BigInt& alphabetnumber)

{
 
    Botan::BigInt decimalValue(BMHexToDec(numerohex));
    std::string Result = "";
    std::string AlphabetUsed;// = B58Alphabet; //"123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    if (alphabetnumber == 58) {
        AlphabetUsed = B58Alphabet;// "0123456789abcdefghijklmnopqrstuvwxyz";
    }
    if (alphabetnumber == 36) {
        AlphabetUsed = B36Alphabet;// "0123456789abcdefghijklmnopqrstuvwxyz";
    }
   

    while (decimalValue.bits() >= 2) {
        DivResult ResultDiv = DivideBigInt(decimalValue, alphabetnumber);
        decimalValue = ResultDiv.coefficient;
        int remainder = ResultDiv.remainder.to_u32bit();
        Result = AlphabetUsed[remainder] + Result;
    }

    if (decimalValue >= alphabetnumber.to_u32bit()) {
        DivResult ResultDiv = DivideBigInt(decimalValue, alphabetnumber);
        decimalValue = ResultDiv.coefficient;
        int remainder = ResultDiv.remainder.to_u32bit();
        Result = AlphabetUsed[remainder] + Result;
    }

    if (decimalValue > 0) {
        int value = decimalValue.to_u32bit();
        Result = AlphabetUsed[value] + Result;
    }

    return Result;
    

  
    
}

std::string MainFrame::BMHexToDec(const std::string& numerohex)
{
    
    int Long = numerohex.length();
    std::string resultado = "0";

    std::vector<int> DecValues(Long);
    std::vector<std::string> ExpValues(Long);
    std::vector<std::string> MultipliValues(Long);

    // Convert each hexadecimal digit to its decimal value
    for (int counter = 0; counter < Long; ++counter) {
        DecValues[counter] = HexAlphabet.find(toupper(numerohex[counter]));
    }

    // Calculate exponentiation values
    for (int counter = 0; counter < Long; ++counter) {
        ExpValues[counter] = BMExponente("16", std::to_string(Long - 1 - counter));
    }

    // Calculate multiplication values
    for (int counter = 0; counter < Long; ++counter) {
        MultipliValues[counter] = BMMultiplicar(ExpValues[counter], std::to_string(DecValues[counter]));
    }

    // Calculate the final result by adding all multiplication values
    for (int counter = 0; counter < Long; ++counter) {
        resultado = BMAdicion(resultado, MultipliValues[counter]);
    }

    return resultado;
    
  
}

std::string MainFrame::BMExponente(const std::string& Numero1,const std::string& Numero2)
{
    int count = 0;
    std::string resultado = "";

    if (Numero2 == "1") {
        return Numero1;
    }
    else if (Numero2 == "0") {
        return "1";
    }
    else {
        resultado = Numero1;

        for (count = 2; count <= std::stoi(Numero2); ++count) {
            resultado = BMMultiplicar(resultado, Numero1);
        }

        return resultado;
    }
}

std::string MainFrame::BMMultiplicar(const std::string& Numero1, const std::string& Numero2)
{
    int carry = 0;
    int cantidaddeceros = 0;
    std::string TotalSuma = "0";
    std::vector<std::string> sumandos(Numero2.length(), "");

    for (int count = Numero2.length() - 1; count >= 0; --count) {
        for (int count2 = Numero1.length() - 1; count2 >= 0; --count2) {
            int thiscol = (std::stoi(std::string(1, Numero2[count])) * std::stoi(std::string(1, Numero1[count2])) + carry);
            carry = thiscol / 10;
            thiscol -= (carry * 10);
            sumandos[cantidaddeceros] = std::to_string(thiscol) + sumandos[cantidaddeceros];
        }

        if (carry > 0) {
            sumandos[cantidaddeceros] = std::to_string(carry) + sumandos[cantidaddeceros];
        }

        carry = 0;
        sumandos[cantidaddeceros] = PonerCeros(sumandos[cantidaddeceros], cantidaddeceros);
        cantidaddeceros++;
    }

    for (auto& sumando : sumandos) {
        TotalSuma = BMAdicion(sumando, TotalSuma);
    }

    return ClearLeadingCeros(TotalSuma);
}

std::string MainFrame::BMAdicion(std::string& numero1,std::string& numero2)
{
    int longitude = 0;
    int count = 0;
    int carry = 0;
    std::string resultado = "";
    int thiscol = 0;
    int ceros = 0;

    longitude = numero1.length();

    if (numero2.length() > longitude) {
        longitude = numero2.length();
        ceros = numero2.length() - numero1.length();

        while (count < ceros) {
            numero1 = '0' + numero1;
            count = count + 1;
        }
    }
    else {
        ceros = numero1.length() - numero2.length();

        while (count < ceros) {
            numero2 = '0' + numero2;
            count = count + 1;
        }
    }

    for (count = longitude - 1; count >= 0; --count) {
        thiscol = (numero1[count] - '0') + (numero2[count] - '0') + carry;
        carry = 0;

        if (thiscol > 9) {
            thiscol = thiscol - 10;
            carry = 1;
        }

        resultado = std::to_string(thiscol) + resultado;
    }

    if (carry > 0) {
        resultado = '1' + resultado;
    }

    return resultado;
}

std::string MainFrame::PonerCeros(const std::string& numero, int cuantos)
{
    std::string newNumber = numero;
    for (int contador = 0; contador < cuantos; ++contador) {
        newNumber += '0';
    }
    return newNumber;
}

std::string MainFrame::ClearLeadingCeros(const std::string& numero)
{
    std::string result = "";
    int count = 0;
    int movepos = 0;

    if (numero[0] == '-') {
        movepos = 1;
    }

    for (count = 1 + movepos; count <= numero.length(); ++count) {
        if (numero[count - 1] != '0') {
            result += numero[count - 1];
        }

        if (numero[count - 1] == '0' && result.length() > 0) {
            result += numero[count - 1];
        }
    }

    if (result == "") {
        result = "0";
    }

    if (movepos == 1 && result != "0") {
        result = "-" + result;
    }

    return result;
}

int64_t MainFrame::GetFee(int64_t amount)
{
    int64_t result = amount / Comisiontrfr;
    if (result < MinimunFee) {
        result = MinimunFee;
    }
    return result;
}

std::string MainFrame::GetPTCEcn(std::string OrderType)
{
    std::string result;
    int64_t UTCTime = GetMainetTime();
    std::string UTCTimeString = std::to_string(UTCTime);

    result = "NSL" + OrderType + " " + Protocol + " " + ProgramVersion + " " + UTCTimeString + " ";
    return result;
    
}

std::string MainFrame::GetStringFromOrder(OrderData& Order)
{
    std::string OrderString =  "1" + std::to_string(Order.GetTimeStamp()) + Order.GetTrfID();
    std::string OrderHashString = GetOrderHash(OrderString);
    std::string OrderToSend = Order.GetOrderType() + " " + OrderHashString + " 1 TRFR " + std::to_string(Order.GetTimeStamp()) + " " + Order.GetOrderReference() + " 1 " + Order.GetSenderPublicKey() + " " + Order.GetSenderHashAddress() + " " + Order.GetDestinationHashAddress() + " " + std::to_string(Order.GetAmountFee()) + " " + std::to_string(Order.GetAmountTrfe()) + " " + Order.GetSignature() + " " + Order.GetTrfID(); //+ "\n";
    
    return OrderToSend;
}


std::string MainFrame::GetHashOrder(const std::string& value)
{
    std::string result = getHashSha256ToString(value);
    result = BMHexTo58(result, 36);
    return "OR" + result;
   
}

std::string MainFrame::getHashSha256ToString(const std::string& publicKey)
{
    Botan::SHA_256 sha256;
    sha256.update(reinterpret_cast<const Botan::byte*>(publicKey.c_str()), publicKey.length());
    Botan::secure_vector<Botan::byte> digest = sha256.final();

    std::string result = hex_encode(digest);

    for (char& c : result)
    {
        if (c == '-')
            c = ' ';
        c = std::toupper(c);
    }

    return result;

  

}
DivResult MainFrame::DivideBigInt(const Botan::BigInt& numerator, const Botan::BigInt& denominator)
{
        DivResult result;
        result.coefficient = numerator / denominator;
        result.remainder = numerator % denominator;
        return result; 
}
Botan::secure_vector<uint8_t> MainFrame::calculateSHA1(const std::vector<unsigned char>& input)
{
    
        std::unique_ptr<Botan::HashFunction> hash_function(Botan::HashFunction::create("SHA-1"));
        hash_function->update(input.data(), input.size());
        Botan::secure_vector<uint8_t> hash_result = hash_function->final();

        return hash_result;
    
    
}

int64_t MainFrame::GetPendingPaysFromNosoAddress(const std::string& NosoAddress)
{
    int64_t Total = 0;
    for (const auto& order : ordersVector)
    {
        if (order.SourceAddress == NosoAddress)
        {
            Total += order.Amount;
        }
        if (order.DestinationAddress == NosoAddress)
        {
			Total -= order.Amount;
		}
    }
    //TextBox->AppendText("\nTotal Pending Pays from Address: ");
    //TextBox->AppendText(std::to_string(Total));
    
    return Total;

}


 

    
    




 













