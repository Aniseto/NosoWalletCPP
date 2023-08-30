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
#include <cryptopp/ripemd.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>
#include <cryptopp/algebra.h>
#include <wx/hash.h>
#include <iostream>
#include <string>
#include <botan/base58.h>
#include <botan/botan.h>
#include <botan/ecdsa.h>
#include <botan/pubkey.h>
#include <botan/bigint.h>
#include <botan/base64.h>
#include <botan/sha2_32.h>
#include <botan/hex.h>
#include <cctype>
//#include <vector>
/// Added for Unix Like system compatibilty
#include <filesystem>
#include <wx/statusbr.h>
#include <wx/statusbr.h>
//#include <curl/curl.h>
#include <ctime>
#include <wx/timer.h>
namespace fs = std::filesystem;
/// Added for Unix Like system compatibilty: END.


//(wxID_OPEN, MyFrame::OnOpen)
//EVT_MENU(wxID_SAVE, MyFrame::OnSave)
//EVT_MENU(wxID_EXIT, MyFrame::OnExit)
//END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title): wxFrame(nullptr,wxID_ANY,title) {   //Constructor Base class
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

	wxButton* Connect_Button = new wxButton(panel, wxID_ANY, "Connect", wxPoint(1, 1), wxSize(150, 25));
    wxButton* GenerateKeysButton = new wxButton(panel, wxID_ANY, "Generate NOSO Address", wxPoint(1, 100), wxSize(150, 25));
    wxStaticText* LogTextBoxLabel = new wxStaticText(panel, wxID_ANY, "Log: ", wxPoint(1, 475));
    LogTextBoxLabel->SetFont(wxFontInfo(8).Bold());
    wxStaticText* dateTimeText = new wxStaticText(panel, wxID_ANY, "Time", wxPoint(1, 250));

    //Dynamic object creation
 
    TextBox = new wxTextCtrl(panel, wxID_ANY, "Text Box", wxPoint(1, 500), wxSize(680, 250), wxTE_MULTILINE);
    NosoAddressGrid = new wxGrid(panel, wxID_ANY, wxPoint(1, 200), wxSize(680, 250));
    NosoAddressGrid->HideRowLabels();
    NosoAddressGrid->AutoSizeColumns();
    NosoAddressGrid->CreateGrid(4, 4);
    NosoAddressGrid->SetColLabelValue(0,"Address");
    NosoAddressGrid->SetColLabelValue(1,"Label");
    NosoAddressGrid->SetColLabelValue(2,"Pending");
    NosoAddressGrid->SetColLabelValue(3,"Balance");

    //Bind Functions
    
    Connect_Button->Bind(wxEVT_BUTTON, &MainFrame::OnConnectButtonClicked, this);
    GenerateKeysButton->Bind(wxEVT_BUTTON, &MainFrame::GenerateKeys, this);
    
    this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);

    //Status Bar creation


    statusBar = new wxStatusBar(this, wxID_ANY);
    statusBar->SetDoubleBuffered(true);
    statusBar->SetFieldsCount(3);
    SetStatusBar(statusBar);
     
    //Connect(wxID_ANY, wxEVT_TIMER, wxTimerEventHandler(MainFrame::OnTimer));
 
      //  SetBackgroundColor(1, wxColor(0, 255, 0)); // Green background for field 2
}


void MainFrame::OnConnectButtonClicked(wxCommandEvent& evt)

{
    TextBox->Clear();
    TextBox->AppendText("Connecting to Mainet....\n");
	std::string NODESTATUS_COMMAND= "NODESTATUS\n";
	std::string DefaultNodeIp = "4.233.61.8";						//PENDDING: Send commmand to NODE LIST, and connect to nodes starting from the old ones until connection is OK.
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
    //CurrentBlock->SetLabel(wxString(CurrentBlockString));
    statusBar->SetStatusText("Current Block: " + CurrentBlockString, 0);
    //StatusBar->SetStatusText("Field 2 Text", 1);
    //SetBackgroundColor(1, wxColor(0, 255, 0));
    // Modify Static text to show Current Block
    //statusBar->SetBackgroundColour(0,wxColor(0, 255, 0));
    //TextBox->Clear();
    TextBox->AppendText("Connection Sucessful\n");
	//wxLogStatus("Connected, NODESTATUS SAVED.");

}

void MainFrame::DownloadSumary()
{
    TextBox->AppendText("Downloading Sumary....\n");
    std::string DefaultNodeIp = "4.233.61.8";						//PENDING: Send commmand to NODE LIST, and connect to nodes starting from the old ones until connection is OK.
    int DefaultNodePort = 8080;
    std::string GETZIPSUMARY_COMMAND = "GETZIPSUMARY\n";
    std::string GetZipSumaryResponse = SendStringToNode(DefaultNodeIp, DefaultNodePort, GETZIPSUMARY_COMMAND);
    //GetSumaryText->SetLabel(wxString(GetZipSumaryResponse));              // Modify Static text to show Current Block

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


}

/*

void MainFrame::OnSyncMainNetTimeButtonClicked(wxCommandEvent& evt) {


    //MainNetTimeText->SetLabel(std::to_string(GetMainetTimeStamp()));

}
*/
/*
void MainFrame::OnTimer(wxTimerEvent& event) {

        time_t ntpTime = GetNTPTime();
        if (ntpTime != 0) {
            // Convert ntpTime to a struct tm
            struct tm* timeInfo = localtime(&ntpTime);

            // Format the date and time string
            wxString dateTimeString = wxString::Format(wxT("%02d/%02d/%02d %02d:%02d:%02d"),
                timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year % 100,
                timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);

            //TextBox->AppendText(dateTimeString);
            dateTimeText->SetLabel(dateTimeString);
           // Connect(wxID_ANY, wxEVT_TIMER, wxTimerEventHandler(MyFrame::OnTimer));
        }
 
   // Pending automatic Time showing Date and Time updated every second and synced to Main Net or NTP Servers.
}
*/
/*
time_t MainFrame::GetNTPTime()
{
    
        CURL* curl = curl_easy_init();
        if (curl) {
            // Set the NTP server URL (e.g., pool.ntp.org)
            curl_easy_setopt(curl, CURLOPT_URL, "http://pool.ntp.org/");

            // Perform the HTTP request
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                // Get the response data and extract the timestamp
                double timestamp;
                curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &timestamp);

                // Adjust the timestamp to Unix time (seconds since epoch)
                time_t ntpTime = static_cast<time_t>(timestamp) + 2208988800;

                // Clean up libcurl
                curl_easy_cleanup(curl);

                return ntpTime;
            }

            // Clean up libcurl in case of an error
            curl_easy_cleanup(curl);
        }

        // Return 0 to indicate an error
        return 0;
    
}
*/
void MainFrame::SyncMainNetTime()
{
    int timer = GetMainetTimeStamp();
    //MainNetTimeText->SetLabel(std::to_string(GetMainetTimeStamp()));
}

void MainFrame::OnClose(wxCloseEvent& evt) {
    wxLogMessage("Wallet Closed");
    evt.Skip();

}

void MainFrame::GetMasterNodeList(wxCommandEvent& evt)
{
    std::string NODESTATUS_COMMAND = "NSLMNS\n";
    std::string DefaultNodeIp = "4.233.61.8";						//PENDING: Send commmand to NODE LIST, and connect to nodes starting from the old ones until connection is OK.
    int DefaultNodePort = 8080;										//PENDING: Set PORT from List of Nodes.
    std::string MasterNodeListString = SendStringToNode(DefaultNodeIp, DefaultNodePort, NODESTATUS_COMMAND);
    TextBox->SetLabel(MasterNodeListString);
    //MasterNodeListText->SetLabel("Master Node List OK");

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

    
    // Generate Keys
    Botan::AutoSeeded_RNG rng;
    Botan::ECDSA_PrivateKey private_key(rng, Botan::EC_Group("secp256k1"));
    std::vector<uint8_t> publicKeyPointBytes = private_key.public_point().encode(Botan::PointGFp::UNCOMPRESSED);
    std::vector<uint8_t> privateKeyBytes = Botan::BigInt::encode(private_key.private_value());
    std::string privateKeyBase64 = Botan::base64_encode(privateKeyBytes.data(), privateKeyBytes.size());
    std::string publicKeyPointBase64 = Botan::base64_encode(publicKeyPointBytes.data(), publicKeyPointBytes.size());

    //Save Generated Keys to Wallet.

    MyWallet.SetPrivateKey(privateKeyBase64);
    MyWallet.SetPublicKey(publicKeyPointBase64);
   

  

    TextBox->AppendText("\nCurrent Wallet PrivateKey: ");
    TextBox->AppendText(MyWallet.GetPrivateKey());
    TextBox->AppendText("\nCurrent Wallet PublicKey: ");
    TextBox->AppendText(MyWallet.GetPublicKey());
    
    ///Show Results

    TextBox->AppendText("\n\nPUBLIC KEY: \n");
    //TextBox->AppendText(publicKeyStr);
    //TextBoz->AppendTest("\n PUBLIC KEY SIZE: \n")
    TextBox->AppendText(publicKeyPointBase64);
    TextBox->AppendText("\n\nPRIVATE KEY: \n");
    //TextBox->AppendText(privateKeyStr);
    TextBox->AppendText(privateKeyBase64);


    //Generate NOSO ADDRESS
    Sha256 = PublicKeyToSHA256(publicKeyPointBase64);
    MD160 = CalculateMD160(Sha256);
    Base58= EncodeBase58(MD160);  
    Checksum = CalculateCheckSum(Base58);
    CheckSumBase58 = BmDecto58(std::to_string(Checksum));

    //Final Noso Address: N + Base58 + Base58(CheckSum)
    NosoAddress = "N" + Base58 + CheckSumBase58;
    MyWallet.SetHash(NosoAddress);
    TextBox->AppendText("\nNOSO Address:\n");
    TextBox->AppendText(NosoAddress);
    TextBox->AppendText("\n\nEND NOSO ADDRESS GENERATION\n");

    //Save New NOSO Address to file.

    std::string NosoWalletCPPPath = (fs::current_path() / "data" / "walletcpp.pkw").string();
    SaveWalletDataToFile(MyWallet, NosoWalletCPPPath);
    std::vector<WalletData> walletCPPDataLoaded = ReadWalletDataFromNosoCPP(NosoWalletCPPPath); 
    TextBox->AppendText("\nTotal NOSOCPP address loaded : ");
    TextBox->AppendText(std::to_string(walletCPPDataLoaded.size()));

    // Create a function to update GRID.

    NosoAddressGrid->DeleteRows();
    for (size_t i = 0; i < walletCPPDataLoaded.size(); ++i) {
        std::string HashKeyLoaded = walletCPPDataLoaded[i].GetHash();
        std::string Label = walletCPPDataLoaded[i].GetLabel();
        std::int64_t Pending = walletCPPDataLoaded[i].GetPending();
        std::int64_t Balance = walletCPPDataLoaded[i].GetBalance();
        NosoAddressGrid->AppendRows(1); // Add a new row
        NosoAddressGrid->SetCellValue(i, 0, HashKeyLoaded);
        NosoAddressGrid->SetCellValue(i, 1, Label);
        NosoAddressGrid->SetCellValue(i, 2, std::to_string(Pending));
        NosoAddressGrid->SetCellValue(i, 3, std::to_string(Balance));
    }
}


bool MainFrame::UnzipFile(const wxString& zipFileName, const wxString& outputDir)   //https://docs.wxwidgets.org/3.1/overview_archive.html
{
    wxFileInputStream fis(zipFileName);

    if (!fis.IsOk())
    {
        //wxLogError(wxS("Couldn't open the file '%s'."), zipFileName);
        wxString ErrorOpening;
        ErrorOpening.Printf("Couldn't open the file '%s'.", zipFileName);
        TextBox->AppendText(ErrorOpening + "\n");
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
            wxString ErrorReading;
            ErrorReading.Printf("Couldn't read the zip entry '%s'.", upZe->GetName());
            TextBox->AppendText(ErrorReading + "\n");
            return false;
        }

        wxFileOutputStream fos(strFileName);

        if (!fos.IsOk())
        {
            wxLogError(wxS("Couldn't create the file '%s'."), strFileName);
            wxString ErrorCreating;
            ErrorCreating.Printf("Couldn't create the file '%s'.", strFileName);
            TextBox->AppendText(ErrorCreating + "\n");
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

std::string MainFrame::SignMessage(const std::string& message, const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey& privateKey)
{
    CryptoPP::AutoSeededRandomPool rng;

    // ECDSA schema
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer signer(privateKey);

    // Sign the message
    std::string signature;
    CryptoPP::StringSource(message, true,
        new CryptoPP::SignerFilter(rng, signer,
            new CryptoPP::StringSink(signature)
        )
    );

    return signature;
    
}

bool MainFrame::VerifyMessage(const std::string& message, const std::string& signature, const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey& publicKey)
{
    //ECDSA Verification
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Verifier verifier(publicKey);

    // Verify message firm
    bool result = false;
    CryptoPP::StringSource(signature + message, true,
        new CryptoPP::SignatureVerificationFilter(
            verifier,
            new CryptoPP::ArraySink(reinterpret_cast<CryptoPP::byte*>(&result), sizeof(result))
        )
    );

    return result;
    
}


void MainFrame::SignAndVerify(wxCommandEvent& evt)
{
    
    /*
    std::string messageoriginal = "This is the message to firm";

    // Firmar el mensaje
    std::string signature = SignMessage(messageoriginal, TestWallet.GetPrivateKey);
    std::cout << "FIRM: " << signature << std::endl;

    // Verificar la firma del mensaje
    bool verified = VerifyMessage(messageoriginal, signature, publicKey);
    if (verified) {
        std::cout << "FIRM VALID." << std::endl;
    }
    else {
        std::cout << "INVALID FIRM." << std::endl;
    }
*/
}

std::string MainFrame::HexToBase64(const std::string& hexString)
{
    std::vector<uint8_t> binaryData;

    for (size_t i = 0; i < hexString.length(); i += 2) {
        std::string byteStr = hexString.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, 0, 16));
        binaryData.push_back(byte);
    }

    // Encode binary data as Base64
    std::string base64String = Botan::base64_encode(binaryData.data(), binaryData.size());

    return base64String;
}
bool MainFrame::SaveWalletDataToFile(const WalletData& walletData, const std::string& filePath)
{
    // Open the file for writing in binary append mode
    //std::ofstream outputFile("walletcpp.pkw", std::ios::binary | std::ios::app);
    std::ofstream outputFile(filePath, std::ios::binary | std::ios::app);

    // Check if the file was opened successfully
    if (!outputFile) {
        std::cerr << "Error: Could not open the file for writing." << std::endl;
        return false;
    }

    // Write the WalletData structure to the file
    outputFile.write(reinterpret_cast<const char*>(&walletData), sizeof(WalletData));

    // Check for write errors
    if (outputFile.fail()) {
        std::cerr << "Error: Failed to write data to the file." << std::endl;
        outputFile.close(); // Close the file
        return false;
    }

    // Close the file
    outputFile.close();

    std::cout << "WalletData appended to walletcpp.pkw" << std::endl;
    return true;
}

void MainFrame::UpdateDateAndTime()
{
    wxDateTime currentDateTime = wxDateTime::Now();
    wxString dateTimeStr = currentDateTime.Format("%Y-%m-%d %H:%M:%S");
    statusBar->SetStatusText(dateTimeStr, 2);
    //dateAndTimeText->SetLabel(dateTimeStr);

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
 
    //SetTimer
    timer->SetOwner(this);
    //timerSetOwner(this);
    timer->Start(1000);  // 1000 ms = 1 second
    Connect(timer->GetId(), wxEVT_TIMER, wxTimerEventHandler(MainFrame::OnTimer), NULL, this);



    //std::string WalletFullPath = (fs::current_path() / "data" / "wallet.pkw").string();
    std::string WalletCPPFullPath= (fs::current_path() / "data" / "walletcpp.pkw").string();
    //Call Connect ButtonFuction
    wxCommandEvent fakeEvent(wxEVT_BUTTON, wxID_ANY); // Create a fake button click event
    OnConnectButtonClicked(fakeEvent); // Call the function
    
    //Download Summary
    MainFrame::DownloadSumary();
    
    //OnDownloadSummaryButtonClicked(fakeEvent);

    //Check if walletcpp.pkw exists on /data directory, if exists load all addresses, if no create a new NOSO address, save to file and load Address


    if (DoesFileExist(WalletCPPFullPath))
    {
        TextBox->AppendText("\nNOSOCPP Wallet File Detected. Loading addresses\n");
        std::vector<WalletData> walletCPPDataLoaded = ReadWalletDataFromNosoCPP(WalletCPPFullPath);
        TextBox->AppendText("\nTotal NOSOCPP address loaded : ");
        TextBox->AppendText(std::to_string(walletCPPDataLoaded.size()));
        NosoAddressGrid->DeleteRows();
        for (size_t i = 0; i < walletCPPDataLoaded.size(); ++i) {
            std::string HashKeyLoaded = walletCPPDataLoaded[i].GetHash();
            std::string Label = walletCPPDataLoaded[i].GetLabel();
            std::int64_t Pending = walletCPPDataLoaded[i].GetPending();
            std::int64_t Balance = walletCPPDataLoaded[i].GetBalance();
            NosoAddressGrid->AppendRows(1); // Add a new row
            NosoAddressGrid->SetCellValue(i, 0, HashKeyLoaded);
            NosoAddressGrid->SetCellValue(i, 1, Label);
            NosoAddressGrid->SetCellValue(i, 2, std::to_string(Pending));
            NosoAddressGrid->SetCellValue(i, 3, std::to_string(Balance));
    }

    }
    else {
        
        TextBox->AppendText("\nWalletcpp.pkw File does not exist, some probems happened as walletcpp.pkw cannot be created in Data directory.\n ");

    }
    //Update TIme
    //MainFrame::UpdateDateAndTime();
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
        //std::cerr << "Cannot open the file." << std::endl;
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

    //std::string test= dataVectorWallet[0].GetHash;



    return dataVectorWallet;
    //return std::vector<WalletData>();
}

std::vector<WalletData> MainFrame::ReadWalletDataFromNosoCPP(const std::string& filePath)
{
    std::vector<WalletData> dataVectorWalletCPP;
    std::ifstream inputFile(filePath, std::ios::binary);
    if (!inputFile) {
        TextBox->AppendText("Error: Could not open NosowalletCPP.PKW the file for reading.");
        return dataVectorWalletCPP; // Return an empty vector on error
    }
    WalletData walletData;
    while (inputFile.read(reinterpret_cast<char*>(&walletData), sizeof(WalletData))) {
        dataVectorWalletCPP.push_back(walletData);
    }
    if (inputFile.fail() && !inputFile.eof()) {
        TextBox->AppendText("Error: Failed to read data from the file.");
        inputFile.close(); // Close the file
        return dataVectorWalletCPP; // Return what was read so far on error
    }
    inputFile.close();

    TextBox->AppendText("Data read from "+ filePath +" and appended to the vector.");
    
    return dataVectorWalletCPP;
}

void MainFrame::OnTimer(wxTimerEvent& event) {
    UpdateDateAndTime();
}










