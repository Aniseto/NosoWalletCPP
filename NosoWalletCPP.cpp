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
//#include <cryptopp/eccrypto.h>
#include <bitset>

//const std::string B64Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";



//TODO
//1- Get a list of Master Nodes from a DNS entry.
//2- Get a list of available Nodes ( NSO CONFIG ) and get this list as list of servers to connect and send commands, priorize older
//nodes first, and control if there is an issue conecting then go to next node in the list. Maybe create a Random node select to balance connections
//between current nodes ?
//3- Create Send Noso form, adding Source Address ( from current Wallet addressess list ), Destination Address ( verifying that is a valid NOSO address, 
//Ammount to send, checking if there is enought noso on source address ( checking if ther eis any pending operations also ).
//4- Complete a Send Noso operation manually 

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

    //Send funds 

    SourceAddressText= new wxStaticText(panel, wxID_ANY, "Source Address: ", wxPoint(250, 50));
    SourceAddressText->SetFont(wxFontInfo(8).Bold());
    SourceAddressCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(375, 50));
    SourceAddressCtrl->SetSize(wxSize(200, -1));
    DestinationAddressText = new wxStaticText(panel, wxID_ANY, "Destination Address: ", wxPoint(250, 75));
    DestinationAddressText->SetFont(wxFontInfo(8).Bold());
    DestinationAddressCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(375, 75));
    DestinationAddressCtrl->SetSize(wxSize(200, -1));
    AmountToSendText = new wxStaticText(panel, wxID_ANY, "Amount To Send: ", wxPoint(250, 100));
    AmountToSendText->SetFont(wxFontInfo(8).Bold());
    AmountToSendCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(375, 100));
    AmountToSendCtrl->SetSize(wxSize(200, -1));
    wxButton* SendFunds_Button = new wxButton(panel, wxID_ANY, "Send", wxPoint(425, 150), wxSize(150, 25));
    SendFunds_Button->Bind(wxEVT_BUTTON, &MainFrame::OnSendNosoButtonClicked, this);
}


void MainFrame::OnConnectButtonClicked(wxCommandEvent& evt)

{
    TextBox->Clear();
    TextBox->AppendText("Connecting to Mainet....\n");
    std::string NODESTATUS_COMMAND = "NODESTATUS\n";
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

    if (CurrentBlockString == "0") {
        statusBar->SetStatusText("Current Block: Error Getting Summary", 0);
    }
    else {

    
    statusBar->SetStatusText("Current Block: " + CurrentBlockString, 0);

    TextBox->AppendText("Connection Sucessful\n");
    }

}

std::vector<TSummaryData> MainFrame::DownloadSumary()
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

    //UpdateTableBalance(datavector);


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
   

  

    //TextBox->AppendText("\nCurrent Wallet PrivateKey: ");
    //TextBox->AppendText(MyWallet.GetPrivateKey());
    //TextBox->AppendText("\nCurrent Wallet PublicKey: ");
    //TextBox->AppendText(MyWallet.GetPublicKey());
    
    ///Show Results

    //TextBox->AppendText("\n\nPUBLIC KEY: \n");
    //TextBox->AppendText(publicKeyStr);
    //TextBoz->AppendTest("\n PUBLIC KEY SIZE: \n")
    //TextBox->AppendText(publicKeyPointBase64);
    //TextBox->AppendText("\n\nPRIVATE KEY: \n");
    //TextBox->AppendText(privateKeyStr);
    //TextBox->AppendText(privateKeyBase64);


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
    //std::vector<WalletData> walletCPPDataLoaded = ReadWalletDataFromNosoCPP(NosoWalletCPPPath); 
    walletCPPDataLoaded = ReadWalletDataFromNosoCPP(NosoWalletCPPPath);
    TextBox->AppendText("\nTotal NOSOCPP address loaded : ");
    TextBox->AppendText(std::to_string(walletCPPDataLoaded.size()));

    // Create a function to update GRID.

    UpdateTable(walletCPPDataLoaded);

    /*
    //  //SourceAddress Sender :NZXpFV6SHcJ6xhhX2Bgid4ofQqsbEb
    //DestionAddress       :N2XjEbgabYNXdH1mzoWjJWJgMyPtZFr
    bool IsValid = CheckIfNosoAddressIsValid("NZXpFV6SHcJ6xhhX2Bgid4ofQqsbEb");
    if (IsValid) {
        TextBox->AppendText("\nIs Valid");
    }
    else {
        TextBox->AppendText("\nIs not Valid");
            
    }
    //TEST IsValid58
    TextBox->AppendText("\nTesting is Valid ZXpFV6SHcJ6xhhX2Bgid4ofQqsbEb ");
    bool valid58 = IsValid58("ZXpFV6SHcJ6xhhX2Bgid4ofQqsbEb");
    if (valid58)
    {
        TextBox->AppendText("Is Valid !");
    }
    else {
        TextBox->AppendText("Is NOT Valid !");
    }
    TextBox->AppendText("\nTesting is Valid ZXpFV6SHcJ6xhhX2Bgid4ofQqsbE. ");
    valid58 = IsValid58("ZXpFV6SHcJ6xhhX2Bgid4ofQqsbE.");
    if (valid58)
    {
        TextBox->AppendText("Is Valid !");
    }
    else {
        TextBox->AppendText("Is NOT Valid !");
    }

    TextBox->AppendText("\nTesting is Valid N2XjEbgabYNXdH1mzoWjJWJgMyPtZFr ");
    valid58 = IsValid58("N2XjEbgabYNXdH1mzoWjJWJgMyPtZFr");
    if (valid58)
    {
        TextBox->AppendText("Is Valid !");
    }
    else {
        TextBox->AppendText("Is NOT Valid !");
    }
    */
    //TESTING SendNosoFromAddress
    
    
    OrderData TestOrder;
    std::string SourceAddressNoso = "NZXpFV6SHcJ6xhhX2Bgid4ofQqsbEb";
    std::string DestinationAddressNoso = "N2XjEbgabYNXdH1mzoWjJWJgMyPtZFr";
    std::string ReferenceNoso = "Test Reference";
    std::string OrderTimeTest = std::to_string(GetMainetTime());
    //1 Noso = 100000000
    int64_t AmmountToSend = 3000000; //003000000 = 0,03 Noso
    int64_t Comision = 1000000; // 0000100000 = 0,01 Noso
    TestOrder = SendFundsFromAddress(SourceAddressNoso, DestinationAddressNoso, AmmountToSend, Comision, ReferenceNoso, OrderTimeTest, 1);
    bool IsValid1 = CheckIfNosoAddressExistsOnMyWallet(SourceAddressNoso, walletCPPDataLoaded);
    //bool IsValid2 = CheckIfNosoAddressIsValid(DestinationAddress);

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
   
        std::vector<unsigned char> messages = nosoBase64Decode(message);

        Botan::secure_vector<uint8_t> decodedData = Botan::base64_decode(privateKeyBase64);
       
        Botan::BigInt private_key_value = Botan::BigInt::decode(decodedData.data(), decodedData.size());
        Botan::EC_Group secp256k1("secp256k1");
        Botan::ECDSA_PrivateKey private_key(rng,secp256k1, private_key_value);
    
        Botan::PK_Signer signer(private_key, rng, "EMSA1(SHA-256)", Botan::DER_SEQUENCE);
        signer.update(messages);
        std::vector<uint8_t> signature = signer.signature(rng);
        TextBox->AppendText("\nSignatude completed");
     
       // std::vector<uint8_t> signature = signer.signature(rng);

        std::string signature_base64 = Botan::base64_encode(signature.data(), signature.size());

        return signature_base64;
        //return Botan::hex_encode(signature);

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
    //std::vector<unsigned char> messages = core.nosoBase64Decode("VERIFICATION");


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

    //Call Connect ButtonFuction
    wxCommandEvent fakeEvent(wxEVT_BUTTON, wxID_ANY); // Create a fake button click event
    OnConnectButtonClicked(fakeEvent); // Call the function
    
 
    //OnDownloadSummaryButtonClicked(fakeEvent);
    SumarydataVector = MainFrame::DownloadSumary();
    //Check if walletcpp.pkw exists on /data directory, if exists load all addresses, if no create a new NOSO address, save to file and load Address

    //std::string WalletFullPath = (fs::current_path() / "data" / "wallet.pkw").string();
    std::string WalletCPPFullPath = (fs::current_path() / "data" / "walletcpp.pkw").string();

    if (DoesFileExist(WalletCPPFullPath))
    {
        TextBox->AppendText("\nNOSOCPP Wallet File Detected. Loading addresses\n");
        //std::vector<WalletData> walletCPPDataLoaded = ReadWalletDataFromNosoCPP(WalletCPPFullPath);
        walletCPPDataLoaded = ReadWalletDataFromNosoCPP(WalletCPPFullPath);
        TextBox->AppendText("\nTotal NOSOCPP address loaded : ");
        TextBox->AppendText(std::to_string(walletCPPDataLoaded.size()));

        //Show Data on central Table
        NosoAddressGrid->DeleteRows();
        for (size_t i = 0; i < walletCPPDataLoaded.size(); ++i) {
            std::string HashKeyLoaded = walletCPPDataLoaded[i].GetHash();
            std::string Label = walletCPPDataLoaded[i].GetLabel();
            std::int64_t Pending = walletCPPDataLoaded[i].GetPending();
            //std::int64_t Balance = walletCPPDataLoaded[i].GetBalance();
            //std::int64_t Balance = GetBalanceFromNosoAddresswalletCPPDataLoaded[i].GetBalance();
            std::int64_t Balance = GetBalanceFromNosoAddress(SumarydataVector, HashKeyLoaded.c_str());
            //int64_t integerNumber = 1234567890; // Your int64_t number
            //double decimalBalance = static_cast<double>(Balance); // Convert to double
            double decimalBalance = static_cast<double>(Balance) / 100000000.0;
            std::stringstream stream;
            stream << std::fixed << std::setprecision(8) << decimalBalance;
            std::string formattedDecimalBalance = stream.str();
            
            
            NosoAddressGrid->AppendRows(1); // Add a new row
            NosoAddressGrid->SetCellValue(i, 0, HashKeyLoaded);
            NosoAddressGrid->SetCellValue(i, 1, Label);
            NosoAddressGrid->SetCellValue(i, 2, std::to_string(Pending));
            NosoAddressGrid->SetCellValue(i, 3, formattedDecimalBalance);
    }

    }
    else {
        
        TextBox->AppendText("\nWalletcpp.pkw File does not exist, some probems happened as walletcpp.pkw cannot be created in Data directory.\n ");

    }
    //Download Summary
  
    UpdateTable(walletCPPDataLoaded);


    MainFrame::GetPendings();
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

std::string MainFrame::GetPendings()
{
    //TextBox->Clear();
    TextBox->AppendText("\nGetting Pending orders from Node.\n");
    std::string NODESTATUS_COMMAND = "NSLPEND\n";
    std::string DefaultNodeIp = "20.199.50.27";						//PENDDING: Send commmand to NODE LIST, and connect to nodes starting from the old ones until connection is OK.
    int DefaultNodePort = 8080;										//PENDING: Set PORT from List of Nodes.
    std::string PendingOrders = SendStringToNode(DefaultNodeIp, DefaultNodePort, NODESTATUS_COMMAND);
    if (PendingOrders == "")
    { 
        TextBox->AppendText("No pending Orders");
        return "NULL";
    }
    else {
        TextBox->AppendText(PendingOrders);
        return PendingOrders;
    }
    //Example Answer with pending Order: Getting Pending orders from Node.
    //TRFR, NBFX6wuUKc1hwFWSA9kctv9XhohbEs, N3J2F4YDFFauC1aVBVuStFeFLwcwsDD, 2000000000, 1000000 //Example sending 20 NOSO fomr Addrress NBF* to N3J2*, wich 0.1 commission.
    //TRFR,NbvGykuZ1ZXzdbk9pKkydBFcGbX1Ft,N3J2F4YDFFauC1aVBVuStFeFLwcwsDD,2030000000,1000000 Example sending 20.3 NOSO.
    //TRFR,NBFX6wuUKc1hwFWSA9kctv9XhohbEs,N3J2F4YDFFauC1aVBVuStFeFLwcwsDD,3560000000,1000000 TRFR,NbvGykuZ1ZXzdbk9pKkydBFcGbX1Ft,N3J2F4YDFFauC1aVBVuStFeFLwcwsDD,3270000000,1000000 

}

int64_t MainFrame::GetAddressPendingPays(std::string NosoAddress)
{
   // Get Pending Payments from NosoAddress Address, Send order Pending, and check if the source address has something pending.

    // 1- Get Pending Orders, if NULL then continue, else check if NosoAddress is is the list, and return the total amount pending.
    
    std::string ListOfPendings = GetPendings();

    if (ListOfPendings != "NULL")
    {
        return 0;


    }
    else 
    {
        std::vector<std::string> items;
        size_t pos = 0;
        while ((pos = ListOfPendings.find("TRFR,")) != std::string::npos) {
            items.push_back(ListOfPendings.substr(0, pos));
            ListOfPendings.erase(0, pos + 5); // Move past "TRFR,"
        }

        // Search for the specific address
       // std::string search_address = "NbvGykuZ1ZXzdbk9pKkydBFcGbX1Ft";
        for (const std::string& item : items) {
            if (item.find(NosoAddress) != std::string::npos) {
                // Split the item by commas to extract the desired values
                size_t start = 0;
                size_t end = item.find(",");
                std::vector<std::string> values;
                while (end != std::string::npos) {
                    values.push_back(item.substr(start, end - start));
                    start = end + 1;
                    end = item.find(",", start);
                }
                values.push_back(item.substr(start, end));

                if (values.size() >= 4) {
                   // TRFR, NBFX6wuUKc1hwFWSA9kctv9XhohbEs, N3J2F4YDFFauC1aVBVuStFeFLwcwsDD, 3560000000, 1000000
                    std::string address = values[0];
                    std::string value1 = values[1];
                    std::string value2 = values[2];
                    std::string value3 = values[3];
                    //std::cout << "Address: " << address << std::endl;
                    //std::cout << "Other Values: " << value1 << ", " << value2 << ", " << value3 << std::endl;
                    return std::stoll(value2);
                }
            }
        }

        
        return 0;
    }
    
   
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
    //SourceAddress Sender :NZXpFV6SHcJ6xhhX2Bgid4ofQqsbEb
    //DestionAddress       :N2XjEbgabYNXdH1mzoWjJWJgMyPtZFr
    //AmountToSend         :010000000
    //Comision             :001000000 
    //Reference            :Test
    //OrderTime            :GetMainetTime
    //Line                 :1
    //SendFundsFromAddress(NZXpFV6SHcJ6xhhX2Bgid4ofQqsbEb,N2XjEbgabYNXdH1mzoWjJWJgMyPtZFr,010000000,001000000,Test,GetMainnetTime(),1)
    TextBox->AppendText("\nTesting Send Funds From SendFundsFromAddress function -> Amount To Send:");
    //std::string octalamount=std::oct << AmountToSend;
    TextBox->AppendText(std::to_string(AmountToSend));
    OrderData OrderInfo;
    bool Exists = CheckIfNosoAddressExistsOnMyWallet(SourceAddress, walletCPPDataLoaded);
    TextBox->AppendText("\nChecking if Source Address Exists on WalletCPP -> ");
    TextBox->AppendText(SourceAddress);
    TextBox->AppendText("\nDestination Address : ");
    TextBox->AppendText(DestinationAddress);
    TextBox->AppendText("\nResultado: ");
    TextBox->AppendText(std::to_string(Exists));

    if (CheckIfNosoAddressExistsOnMyWallet(SourceAddress,walletCPPDataLoaded)&&CheckIfNosoAddressIsValid(DestinationAddress))
    {
        TextBox->AppendText("\nSource Address and Destination Addres are VALID !");
        OrderInfo.SetSenderHashAddress(SourceAddress);
        TextBox->AppendText("\nSender Address: Added to Order Info -> ");
        TextBox->AppendText(OrderInfo.GetSenderHashAddress());
        OrderInfo.SetReceiverHashAddress(DestinationAddress);
        TextBox->AppendText("\nDestination Address: Added to Order Info -> ");
        TextBox->AppendText(OrderInfo.GetDestinationHashAddress());
        int64_t SourceAddressBalance = GetBalanceFromNosoAddress(SumarydataVector,SourceAddress.c_str());
        TextBox->AppendText("\nBalance from Source Address: ");
        TextBox->AppendText(std::to_string(SourceAddressBalance));
        TextBox->AppendText("\nAmount To Send : ");
        TextBox->AppendText(std::to_string(AmountToSend));
        ////// BUG Int64 Value.

        if (AmountToSend < (SourceAddressBalance + static_cast<int64_t>(0.1)))
        {
            TextBox->AppendText("\nSource Balance has enought Noso ! = >");
            TextBox->AppendText(std::to_string(AmountToSend));
            OrderInfo.SetAmmountTrfe(AmountToSend);
            TextBox->AppendText("\nTransfering -> ");
            TextBox->AppendText(std::to_string(OrderInfo.GetAmountTrfe()));
            TextBox->AppendText("\nAmount to Send: ");
            OrderInfo.SetAmmountFee(Commision);
            TextBox->AppendText("\nAmount Fee: ");
            TextBox->AppendText(std::to_string(OrderInfo.GetAmountFee()));
            OrderInfo.SetOrderType("TRFR");
            OrderInfo.SetOrderReference(Reference);
            //Continue, Geting Reference.
            OrderInfo.SetTimeStamp(GetMainetTime());
            OrderInfo.SetOrderLines(line);
            OrderInfo.SetOrderID("");
            OrderInfo.SetOrderType("TRFR");
            OrderInfo.SetSenderPublicKey(GetPublicKeyFromNosoAddress(SourceAddress));
            OrderInfo.SetSignature(SignMessage(std::to_string(OrderInfo.GetTimeStamp()) + SourceAddress + DestinationAddress + std::to_string(AmountToSend) +
                std::to_string(Commision) + std::to_string(line), GetPrivateKeyFromNosoAddress(SourceAddress)));
            OrderInfo.SetTrfID(GetTransferHash(std::to_string(OrderInfo.GetTimeStamp()) + SourceAddress + DestinationAddress + std::to_string(AmountToSend) + CurrentBlockString));

            TextBox->AppendText("\nDebug: Order Info Contents ");
            TextBox->AppendText("\nSender Address: ");
            //TextBox->AppendText(OrderInfo.) CREAR TOTS ELS GETTERS de ORDERINFO.

        }
        else {
            TextBox->AppendText("\nNot enought Source Address Balance!");
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
        //TextBox->AppendText("\nTesting this Hash ");
        //TextBox->AppendText(datos.Hash);
        if (std::strcmp(datos.Hash, NosoAddress) == 0) {
            //return datos.Balance;
            //TextBox->AppendText("\nFound Noso Balance from Specific Address -> ");
            //TextBox->AppendText(NosoAddress);
            //TextBox->AppendText("Balance -> ");
            //TextBox->AppendText(std::to_string(datos.Balance));
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
        //std::int64_t Balance = dataVectorAddress[i].GetBalance();
        //TextBox->AppendText("\Query Balance from \n");
        //TextBox->AppendText(HashKeyLoaded);
        std::int64_t Balance = GetBalanceFromNosoAddress(SumarydataVector, HashKeyLoaded.c_str());
        //int64_t integerNumber = 1234567890; // Your int64_t number
        //double decimalBalance = static_cast<double>(Balance); // Convert to double
        double decimalBalance = static_cast<double>(Balance) / 100000000.0;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(8) << decimalBalance;
        std::string formattedDecimalBalance = stream.str();
        // Output the decimal number with precision (e.g., 2 decimal places)
        //std::cout << std::fixed << std::setprecision(2) << decimalBalance << std::endl;

        //NZXpFV6SHcJ6xhhX2Bgid4ofQqsbEb 1 noso.
        NosoAddressGrid->AppendRows(1); // Add a new row
        NosoAddressGrid->SetCellValue(i, 0, HashKeyLoaded);
        NosoAddressGrid->SetCellValue(i, 1, Label);
        NosoAddressGrid->SetCellValue(i, 2, std::to_string(Pending));
        NosoAddressGrid->SetCellValue(i, 3, formattedDecimalBalance);
    }

    

   //Show Info on Central Table
 
}

std::string MainFrame::GetPublicKeyFromNosoAddress(const std::string & NosoAddress)
{
   
    std::string notfound="Not Found";

    for (size_t i = 0; i < walletCPPDataLoaded.size(); ++i)
    {
        if (walletCPPDataLoaded[i].GetHash() == NosoAddress)
            return walletCPPDataLoaded[i].GetPublicKey();
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

    ResultStringToSHA256 = PublicKeyToSHA256(Transfer);
    ResultStringToHex58 = EncodeBase58(ResultStringToSHA256);

    Base58Sumatory = BMB58Sumatory(ResultStringToHex58);
    //sumatoria: = BMB58resumen(Resultado); Pendiente !



    Key = BmDecto58(Base58Sumatory);
    ResultString = "tR" + ResultStringToHex58 + Key;
    /*
        function GetTransferHash(TextLine:string):String;
var
  Resultado : String = '';
  Sumatoria, clave : string;
Begin
Resultado := HashSHA256String(TextLine);
Resultado := BMHexTo58(Resultado,58);
sumatoria := BMB58resumen(Resultado);
clave := BMDecTo58(sumatoria);
Result := 'tR'+Resultado+clave;
End;


        
        Resultado : String = '';
    Sumatoria, clave : string;
    Begin
        Resultado : = HashSHA256String(TextLine);
Resultado: = BMHexTo58(Resultado, 58);
sumatoria: = BMB58resumen(Resultado);
clave: = BMDecTo58(sumatoria);
Result: = 'tR' + Resultado + clave;
    End;
    
    */
    return ResultString;
}

std::string MainFrame::BMB58Sumatory(const std::string& Base58Number)
{
    const std::string B58Alphabet = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    int total = 0;

    for (size_t counter = 0; counter < Base58Number.length(); ++counter) {
        char currentChar = Base58Number[counter];
        size_t pos = B58Alphabet.find(currentChar);

        if (pos != std::string::npos) {
            total += static_cast<int>(pos);
        }
    }

    return std::to_string(total);
   
}

bool MainFrame::CheckIfNosoAddressExistsOnMyWallet(const std::string& NosoAddressToCheck,std::vector<WalletData> WalletToSearch)
{

    for (size_t i = 0; i < WalletToSearch.size(); ++i)
    {
       // TextBox->AppendText(NosoAddressToCheck);
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
    TextBox->AppendText("\nTesting If Address is valid ->: ");
    TextBox->AppendText(NosoAddressToCheckIfValid);
    TextBox->AppendText("\nLongitud de la dirección es de ->: ");
    TextBox->AppendText(std::to_string(NosoAddressToCheckIfValid.length()));
    TextBox->AppendText("\nTesting If Address starts with 'N' ->: ");
    TextBox->AppendText(NosoAddressToCheckIfValid);
    if (NosoAddressToCheckIfValid[0] == 'N') {
        TextBox->AppendText("\nYes !");
    }
    else {
        TextBox->AppendText("\nNo ! ");
    }
    


    //Debug

    if (NosoAddressToCheckIfValid.length() > 20 && NosoAddressToCheckIfValid[0] == 'N') {
        std::string OrigHash = NosoAddressToCheckIfValid.substr(1, NosoAddressToCheckIfValid.length() - 1);
        TextBox->AppendText("\nOriginal Hash :  ");
        TextBox->AppendText(OrigHash);
        if (IsValid58(OrigHash)) {
            std:: string Hash=  OrigHash.substr(0, OrigHash.length() - 2);
            int Checksum = CalculateCheckSum(Hash);
           // Checksum = CalculateCheckSum(Base58);
            std::string CheckSumBase58 = BmDecto58(std::to_string(Checksum));
            TextBox->AppendText("\nCheckSumCaulated");
            TextBox->AppendText(CheckSumBase58);
            //std::string Clave = BmDecto58(BMB58Resumen(OrigHash));
            OrigHash = 'N' + Hash + CheckSumBase58;
            TextBox->AppendText("\nComparing Keys to see if it's valid");
            TextBox->AppendText("");

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
    const std::string B58Alphabet = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

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

DivResult MainFrame::BMDividir(const std::string& FirstNumber, const std::string& SecondNumber)
{
    DivResult result;
    int counter;
    std::string cociente = "";
    int longValue = FirstNumber.length();
    int64_t Divisor = std::stoll(SecondNumber);
    std::string ThisStep = "";

    for (counter = 0; counter < longValue; counter++) {
        ThisStep += FirstNumber[counter];
        int64_t ThisStepInt = std::stoll(ThisStep);

        if (ThisStepInt >= Divisor) {
            cociente += std::to_string(ThisStepInt / Divisor);
            ThisStep = std::to_string(ThisStepInt % Divisor);
        }
        else {
            cociente += '0';
        }
    }

    result.cociente = cociente;
    result.residuo = ThisStep;

    // You may want to clear leading zeros here, as in your Pascal code
    // This can be done by iterating through result.cociente and result.residuo
    // and removing leading '0' characters if necessary.

    return result;
    //return DivResult();
}

std::string MainFrame::BMB58Resumen(const std::string& Number58)
{
    
    const std::string B58Alphabet = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

    int total = 0;
        for (size_t counter = 0; counter < Number58.length(); counter++) {
            total += B58Alphabet.find(Number58[counter]) - 1;
        }
        return std::to_string(total);
}

void MainFrame::OnSendNosoButtonClicked(wxCommandEvent& evt)
{
    /* SourceAddressText= new wxStaticText(panel, wxID_ANY, "Source Address: ", wxPoint(250, 50));
    SourceAddressText->SetFont(wxFontInfo(8).Bold());
    SourceAddressCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(375, 50));
    SourceAddressCtrl->SetSize(wxSize(200, -1));
    DestinationAddressText = new wxStaticText(panel, wxID_ANY, "Destination Address: ", wxPoint(250, 75));
    DestinationAddressText->SetFont(wxFontInfo(8).Bold());
    DestinationAddressCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(375, 75));
    DestinationAddressCtrl->SetSize(wxSize(200, -1));
    AmountToSendText = new wxStaticText(panel, wxID_ANY, "Amount To Send: ", wxPoint(250, 100));
    AmountToSendText->SetFont(wxFontInfo(8).Bold());
    AmountToSendCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(375, 100));
    AmountToSendCtrl->SetSize(wxSize(200, -1));
    wxButton* SendFunds_Button = new wxButton(panel, wxID_ANY, "Send", wxPoint(425, 150), wxSize(150, 25));
    SendFunds_Button->Bind(wxEVT_BUTTON, &MainFrame::OnSendNosoButtonClicked, this);*/
    
    wxString SourceAddress = SourceAddressCtrl->GetValue();
    wxString DestinationAddress = DestinationAddressCtrl->GetValue();
    wxString AmountToSend = AmountToSendCtrl->GetValue();
    std::string SourceAdressString = SourceAddress.ToStdString();
    std::string DestinationAddressString = DestinationAddress.ToStdString();
    int64_t Amount = std::stoll(AmountToSend.ToStdString());
    int64_t Comission = 0.01;
    std::string Reference = "Hello";
    std::string OrderTime = "OrderTme";
    int lines = 1;
    TextBox->Clear();
    TextBox->AppendText("\nSource Address check: ");
    TextBox->AppendText(SourceAdressString);
    TextBox->AppendText("\nDestination Address check: ");
    TextBox->AppendText(DestinationAddressString);
    TextBox->AppendText("\nAmount to Send: ");
    TextBox->AppendText(std::to_string(Amount));
    OrderData test;
    test=SendFundsFromAddress(SourceAdressString, DestinationAddressString, Amount, Comission, Reference, OrderTime, lines);

   


}

int64_t MainFrame::GetMainetTime()
{
    std::string ip = "20.199.50.27";
    int port = 8080;
    std::string command = "NSLTIME\n";
    std::string TimeString = SendStringToNode(ip, port, command);
   int64_t TimeInt = stoll(TimeString);
    //cout << "Time Integer" << TimeInt << endl; // Control String to check answer.
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
    //return std::string();
}

std::string MainFrame::Int2Curr(int64_t Value)
{
    std::string Result = std::to_string(std::abs(Value));
    Result = AddChar('0', Result, 9);
    Result.insert(Result.length() - 7, ".");

    if (Value < 0)
        Result = "-" + Result;

    return Result;
    //return std::string();
}
    
   

    
    





/*
void MainFrame::UpdateTableBalance(std::vector<TSummaryData>& dataVectorUpdate, std::vector<WalletData>& dataVectorAddress)
{


}
*/  



/*
function SendTo(Destination:String;Ammount:int64;Reference:String):string;
var
  CurrTime : String;
  fee : int64;
  ShowAmmount, ShowFee : int64;
  Remaining : int64;
  CoinsAvailable : int64;
  KeepProcess : boolean = true;
  ArrayTrfrs : Array of orderdata;
  counter : integer;
  OrderHashString : string;
  TrxLine : integer = 0;
  ResultOrderID : String = '';
  OrderString : string;
  PreviousRefresh : integer;
Begin
PreviousRefresh := WO_Refreshrate;
result := '';
if reference = '' then reference := 'null';
CurrTime := UTCTime.ToString;
fee := GetFee(Ammount);
ShowAmmount := Ammount;
ShowFee := fee;
Remaining := Ammount+fee;
if WO_Multisend then CoinsAvailable := Int_WalletBalance
else CoinsAvailable := GetAddressBalanceFromSumary(ARRAY_Addresses[0].Hash);
if not IsValidAddressHash(Destination) then
   Destination := ARRAY_Sumary[AddressSumaryIndex(Destination)].Hash;

if Remaining > CoinsAvailable then
      begin
      ToLog(rsError0012);
      KeepProcess := false;
      end;
if KeepProcess then
   begin
   Setlength(ArrayTrfrs,0);
   counter := 0;
   OrderHashString := currtime;
   while Ammount > 0 do
      begin
      if ARRAY_Addresses[counter].Balance-GetAddressPendingPays(ARRAY_Addresses[counter].Hash) > 0 then
         begin
         TrxLine := TrxLine+1;
         Setlength(ArrayTrfrs,length(arraytrfrs)+1);
         ArrayTrfrs[length(arraytrfrs)-1]:= SendFundsFromAddress(ARRAY_Addresses[counter].Hash,
                                            Destination,ammount, fee, reference, CurrTime,TrxLine);
         fee := fee-ArrayTrfrs[length(arraytrfrs)-1].AmmountFee;
         ammount := ammount-ArrayTrfrs[length(arraytrfrs)-1].AmmountTrf;
         OrderHashString := OrderHashString+ArrayTrfrs[length(arraytrfrs)-1].TrfrID;
         end;
      counter := counter +1;
      end;
   for counter := 0 to length(ArrayTrfrs)-1 do
      begin
      ArrayTrfrs[counter].OrderID:=GetOrderHash(IntToStr(trxLine)+OrderHashString);
      ArrayTrfrs[counter].OrderLines:=trxLine;
      end;
   ResultOrderID := GetOrderHash(IntToStr(trxLine)+OrderHashString);
   result := ResultOrderID;
   OrderString := GetPTCEcn('ORDER')+'ORDER '+IntToStr(trxLine)+' $';
   for counter := 0 to length(ArrayTrfrs)-1 do
      begin
      OrderString := orderstring+GetStringfromOrder(ArrayTrfrs[counter])+' $';
      end;
   Setlength(orderstring,length(orderstring)-2);
   //ToLog(OrderString);
   Result := SendOrder(OrderString);
   end;
WO_Refreshrate := PreviousRefresh;
End;

*/









