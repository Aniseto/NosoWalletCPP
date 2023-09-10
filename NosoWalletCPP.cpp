// Start of wxWidgets "Hello World" Program  https://docs.wxwidgets.org/3.2.2.1/plat_msw_install.html#msw_build_apps
// Includes: 
//#include <vector>
#include <wx/wx.h>
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include "MainFrame.h"
#include "DataStructures.h"

#include <fstream>

#include <wx/hash.h>
#include <iostream>
#include <string>
//#include <botan/botan.h>
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
#include <botan/auto_rng.h>
#include <botan/ecdsa.h>
#include <botan/hex.h>
//#include <boost/system/detail/error_category.hpp>
#include <botan/bigint.h>
#include <botan/base58.h>
#include <botan/base64.h>
//#include <botan/sha2_56.h>
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
#include <cryptopp/eccrypto.h>




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
void MainFrame::SyncMainNetTime()
{
    int timer = GetMainetTimeStamp();
    //MainNetTimeText->SetLabel(std::to_string(GetMainetTimeStamp()));
}
*/
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


    //TEST SIGN and VERIFY
    std::string TestMessage = "Hello World";
    std::string SignedMessage;
    std::string TestErrorSign = "Pepe";
    

    SignedMessage = SignMessage(TestMessage, privateKeyBase64);
    TextBox->AppendText("\nSigned Message : Hello World Resut ->  ");
    TextBox->AppendText(SignedMessage);
    bool Verify = VerifySignature(TestMessage, SignedMessage, publicKeyPointBase64);
    TextBox->AppendText("\nVerify Result : ");
    TextBox->AppendText(std::to_string(Verify));
    bool Verify2 = VerifySignature(TestErrorSign, SignedMessage, publicKeyPointBase64);
    TextBox->AppendText("\nVerify Result FALSE TEST : ");
    TextBox->AppendText(std::to_string(Verify2));
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


std::string MainFrame::SignMessage(const std::string& message, const std::string& privateKeyBase64)

{
    try {
        Botan::AutoSeeded_RNG rng;
        Botan::AlgorithmIdentifier alg_id("ECDSA", Botan::AlgorithmIdentifier::USE_NULL_PARAM);
   

        Botan::secure_vector<uint8_t> decodedData = Botan::base64_decode(privateKeyBase64);
       
        Botan::BigInt private_key_value = Botan::BigInt::decode(decodedData.data(), decodedData.size());
        Botan::EC_Group secp256k1("secp256k1");
        Botan::ECDSA_PrivateKey private_key(rng,secp256k1, private_key_value);
    
        Botan::PK_Signer signer(private_key, rng, "EMSA1(SHA-256)", Botan::DER_SEQUENCE);
        signer.update(message);
        std::vector<uint8_t> signature = signer.signature(rng);
        TextBox->AppendText("\nSignatude completed");
     
        return Botan::hex_encode(signature);

    }


    catch (const std::exception& ex)
    {
            std::cerr << "Error: " << ex.what() << std::endl;
            return ex.what();
    }
}




bool MainFrame::VerifySignature(const std::string& message, const std::string& signatureHex, const std::string& publicKeyBase64)
{
    try {

      
        
        TextBox->AppendText("\nText Message to verify: ");
        TextBox->AppendText(message);
        TextBox->AppendText("\nSignature: ");
        TextBox->AppendText(signatureHex);
        TextBox->AppendText("\nPublic Key : ");
        TextBox->AppendText(publicKeyBase64);

        Botan::AutoSeeded_RNG rng;
        


        Botan::secure_vector<uint8_t> decodedData = Botan::base64_decode(publicKeyBase64);

        
         std::string pemPublicKey = Botan::PEM_Code::encode(decodedData.data(), decodedData.size(), "PUBLIC KEY");
         pemPublicKey = "-----BEGIN PUBLIC KEY-----\n" + pemPublicKey + "\n-----END PUBLIC KEY-----\n";
         Botan::secure_vector<uint8_t> PemString =Botan::secure_vector<uint8_t>(pemPublicKey.begin(), pemPublicKey.end());
         
        
        //std::string pemKey = Botan::PEM_Code::encode(decodedData.data(), decodedData.size(), "PUBLIC KEY");
        //std::string convertedData= ConvertToPEM(decodedData);
  
        //std::string pemPublicKey = ConvertToPEM(decodedData);
        //TextBox->AppendText(pemPublicKey);Botan::
        //TextBox->AppendText("\nPEM KEY");
        //TextBox->AppendText(pemKey);

        Botan::DataSource_Memory publicKeySource(PemString.data(), PemString.size());
        std::unique_ptr<Botan::Public_Key> publicKey(Botan::X509::load_key(publicKeySource));
        
        Botan::PK_Verifier verifier(*publicKey, "EMSA1(SHA-256)", Botan::DER_SEQUENCE);

        verifier.update(reinterpret_cast<const uint8_t*>(message.data()), message.size());

        //verifier.update(message);
        //return verifier.check_signature(Botan::hex_decode(signatureHex));
        if (verifier.check_signature(Botan::hex_decode(signatureHex)))
        {
            return true;
        }
        else return false;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error verify: " << ex.what() << std::endl;
        TextBox->AppendText(ex.what());
        return false;
    }
}



std::string MainFrame::base64ToPEMPrivateKey(const std::string& privateKeyBase64)
{
    try {
        // Decodificar la clave privada desde Base64
        std::string privateKeyPEM;
        Botan::secure_vector<uint8_t> private_key_data = Botan::base64_decode(privateKeyBase64);

        // Crear un objeto Private_Key usando la clave privada decodificada
        Botan::DataSource_Memory private_key_source(private_key_data);
        std::unique_ptr<Botan::Private_Key> private_key(Botan::PKCS8::load_key(private_key_source));

        if (!private_key) {
            std::cerr << "Error al cargar la clave privada" << std::endl;
            return "Error";
        }

        // Exportar la clave privada en formato PEM
        privateKeyPEM = Botan::PKCS8::PEM_encode(*private_key);

        return privateKeyPEM;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return "Error";
    }
}

std::string MainFrame::addPEMHeaders(const std::string& privateKeyBase64, const std::string& header, const std::string& footer)
{
    std::string pemPrivateKey = header + "\n";
    pemPrivateKey += privateKeyBase64 + "\n";
    pemPrivateKey += footer + "\n";

    return pemPrivateKey;
}
    //return std::string();
   

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













