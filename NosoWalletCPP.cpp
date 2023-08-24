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
namespace fs = std::filesystem;
/// Added for Unix Like system compatibilty: END.

MainFrame::MainFrame(const wxString& title): wxFrame(nullptr,wxID_ANY,title) {   //Constructor Base class
	wxPanel* panel = new wxPanel(this);

    //Button Definitions

	wxButton* Connect_Button = new wxButton(panel, wxID_ANY, "Connect", wxPoint(1, 1), wxSize(150, 25));
    wxButton* Download_Summary = new wxButton(panel, wxID_ANY, "Download Summary", wxPoint(1,26), wxSize(150, 25));
    wxButton* SyncMainNetTime = new wxButton(panel, wxID_ANY, "Sync MainNet Time", wxPoint(1, 51), wxSize(150, 25));
    wxButton* GetMasterNodeList = new wxButton(panel, wxID_ANY, "Get Master Node List", wxPoint(1, 76), wxSize(150, 25));
    wxButton* GenerateKeysButton = new wxButton(panel, wxID_ANY, "Generate NOSO Address", wxPoint(1, 100), wxSize(150, 25));
    wxButton* GetMasterNodeConfigButton = new wxButton(panel, wxID_ANY, "Get Master Node config", wxPoint(1, 124), wxSize(150, 25));
    wxButton* SignAndVerifyButton = new wxButton(panel, wxID_ANY, "Sign and Verify", wxPoint(1, 149), wxSize(150, 25));
    

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
    wxStaticText* LogTextBoxLabel = new wxStaticText(panel, wxID_ANY, "Log: ", wxPoint(1, 475));
    LogTextBoxLabel->SetFont(wxFontInfo(8).Bold());


    //Dynamic object creation
    CurrentBlock = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(325, 6.25));
    GetSumaryText = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(325, 32.25));
    TotalNosoAddressesLoadedValue = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(575, 35));
    MainNetTimeText = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(325, 58.25));
    MasterNodeListText = new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(325, 81.53));
    TextBox = new wxTextCtrl(panel, wxID_ANY, "Text Box", wxPoint(1, 500), wxSize(680, 250), wxTE_MULTILINE);
    NosoAddressGrid = new wxGrid(panel, wxID_ANY, wxPoint(1, 200), wxSize(680, 250));
    NosoAddressGrid->HideRowLabels();
    NosoAddressGrid->AutoSizeColumns();
    NosoAddressGrid->CreateGrid(4, 4);
    NosoAddressGrid->SetColLabelValue(0,"Address");
    NosoAddressGrid->SetColLabelValue(1,"Label");
    NosoAddressGrid->SetColLabelValue(2,"Pending");
    NosoAddressGrid->SetColLabelValue(3,"Balance");

    GenerateKeysText= new wxStaticText(panel, wxID_ANY, "No Data", wxPoint(325, 104.81));
 

    //Bind Operations to Button event

    SyncMainNetTime->Bind(wxEVT_BUTTON, &MainFrame::OnSyncMainNetTimeButtonClicked, this);
    Connect_Button->Bind(wxEVT_BUTTON, &MainFrame::OnConnectButtonClicked, this);
    Download_Summary->Bind(wxEVT_BUTTON, &MainFrame::OnDownloadSummaryButtonClicked, this);
    GetMasterNodeList->Bind(wxEVT_BUTTON, &MainFrame::GetMasterNodeList, this);
    GenerateKeysButton->Bind(wxEVT_BUTTON, &MainFrame::GenerateKeys, this);
    GetMasterNodeConfigButton->Bind(wxEVT_BUTTON, &MainFrame::GetMasterNodeConfig, this);
    SignAndVerifyButton->Bind(wxEVT_BUTTON, &MainFrame::SignAndVerify, this);
    
    this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);

    //Status Bar creation

	wxStatusBar* statusBar =CreateStatusBar(); // Create Status Bar Bottom Window.
    statusBar->SetDoubleBuffered(true);
    
    //TestWallet.

        
}



void MainFrame::OnDownloadSummaryButtonClicked(wxCommandEvent& evt)
{
    TextBox->AppendText("Downloading Sumary....\n");
    std::string DefaultNodeIp = "20.199.50.27";						//PENDING: Send commmand to NODE LIST, and connect to nodes starting from the old ones until connection is OK.
    int DefaultNodePort = 8080;
    std::string GETZIPSUMARY_COMMAND = "GETZIPSUMARY\n";
    std::string GetZipSumaryResponse = SendStringToNode(DefaultNodeIp, DefaultNodePort, GETZIPSUMARY_COMMAND);
    GetSumaryText->SetLabel(wxString(GetZipSumaryResponse));              // Modify Static text to show Current Block
    
    wxString zipFileName = "summary.zip";
    //wxString outputDir = ".\\";
    wxString outputDir = (fs::current_path() / "").string();
    //std::string outputDir = std::current_path() / "";
    //fs::path outputDir = fs::current_path() / "";
    UnzipFile(zipFileName, outputDir);

    
    //std::ifstream inputFile(".\\data\\sumary.psk", std::ios::binary);
    std::string filename = (fs::current_path() / "data" / "sumary.psk").string();
    std::ifstream inputFile(filename, std::ios::binary);
    if (!inputFile) {
        TextBox->AppendText("Cannot open the file.\n");
        //std::cout << "Cannot open the file." << std::endl;
        //return void; 
    }
    else {
        TextBox->AppendText("File Opened.\n");
       // std::cout << "File Opened!";
    }

    inputFile.seekg(0, std::ios::end); // Move pointer to end file
    std::streampos fileSize = inputFile.tellg(); // Getting file Size
    inputFile.seekg(0, std::ios::beg); // Moving pointer to the beginning 
    size_t numRecords = fileSize / sizeof(TSummaryData); // Calculate number of registers

    TextBox->AppendText("Loading total Noso Addresses: \n");
    
    
    TotalNosoAddressesLoadedValue->SetLabel(wxString(std::to_string(numRecords)));
    TextBox->AppendText(std::to_string(numRecords));
    //std::cout << endl << "NOSO Addressess loaded : " << numRecords << std::endl;

    std::vector<TSummaryData> dataVector(numRecords);

    inputFile.read(reinterpret_cast<char*>(dataVector.data()), fileSize);

    inputFile.close();

  
         
}

void MainFrame::OnConnectButtonClicked(wxCommandEvent& evt)

{
    TextBox->Clear();
    TextBox->AppendText("Connecting to Mainet....\n");
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

    //TextBox->Clear();
    TextBox->AppendText("Connection Sucessful\n");
	//wxLogStatus("Connected, NODESTATUS SAVED.");

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

    
       // Initialize the random number generator
    Botan::AutoSeeded_RNG rng;

    // Generate ECDSA secp256k1 keys
    Botan::ECDSA_PrivateKey private_key(rng, Botan::EC_Group("secp256k1"));

    // Extract the public key
   // Botan::ECDSA_PublicKey public_key = private_key;

    // Save private key to a string
    //std::string private_key_str = Botan::hex_encode(private_key.private_key_bits());
    
    //std::string private_key_str_Base64 = Botan::base64_encode(private_key.private_key_bits());

    //std::vector<uint8_t> binary_data(private_key_str.begin(), private_key_str.end());

    // Encode binary data as Base64
    //std::string base64_string = Botan::base64_encode(binary_data.data(), binary_data.size());
    //std::string base64_string2 = HexToBase64(private_key_str);

    // Save public key to a string
   // std::string public_key_str = Botan::hex_encode(public_key.public_key_bits());

    /////TEST NEW Base64

    std::vector<uint8_t> publicKeyPointBytes = private_key.public_point().encode(Botan::PointGFp::UNCOMPRESSED);
    std::vector<uint8_t> privateKeyBytes = Botan::BigInt::encode(private_key.private_value());
    std::string privateKeyBase64 = Botan::base64_encode(privateKeyBytes.data(), privateKeyBytes.size());
    std::string publicKeyPointBase64 = Botan::base64_encode(publicKeyPointBytes.data(), publicKeyPointBytes.size());



  




    //std::vector<uint8_t> privateKeyBytes = Botan::hex_decode(private_key.private_key_bits());

    
    ///Show Results

    TextBox->AppendText("\n\nPUBLIC KEY: \n");
    //TextBox->AppendText(publicKeyStr);
    TextBox->AppendText(publicKeyPointBase64);
    TextBox->AppendText("\n\nPRIVATE KEY: \n");
    //TextBox->AppendText(privateKeyStr);
    TextBox->AppendText(privateKeyBase64);


    //Generate NOSO ADDRESS
    Sha256 = PublicKeyToSHA256(publicKeyPointBase64);
    //TextBox->AppendText("\n\nSHA256 Public Key encoded\n");
    //TextBox->AppendText(Sha256);
    MD160 = CalculateMD160(Sha256);
    //TextBox->AppendText("\n\nMD160\n");
    //TextBox->AppendText(MD160);
    Base58= EncodeBase58(MD160);
    //TextBox->AppendText("\n\nBase58\n");
    //TextBox->AppendText(Base58);
    Checksum = CalculateCheckSum(Base58);
   // TextBox->AppendText("\n\nCheckSum:\n");
    //TextBox->AppendText(std::to_string(Checksum));
    CheckSumBase58 = BmDecto58(std::to_string(Checksum));
    //TextBox->AppendText("\nCheckSumBase58:\n");
    //TextBox->AppendText(CheckSumBase58);

    //Final Noso Address: N + Base58 + Base58(CheckSum)
    NosoAddressTest = "N" + Base58 + CheckSumBase58;
    TextBox->AppendText("\nNOSO Address:\n");
    TextBox->AppendText(NosoAddressTest);
    TextBox->AppendText("\n\nEND NOSO ADDRESS GENERATION\n");
      
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
void MainFrame::InitializeWallet()
{
   // std::string dataDirectory = ".//data//";
    //std::string walletFileName = "wallet.pkw";
   // std::string WalletfullPath = dataDirectory + walletFileName;
    
    std::string WalletFullPath = (fs::current_path() / "data" / "wallet.pkw").string();

    //Call Connect ButtonFuction
    wxCommandEvent fakeEvent(wxEVT_BUTTON, wxID_ANY); // Create a fake button click event
    OnConnectButtonClicked(fakeEvent); // Call the function
    //Download Summary
    OnDownloadSummaryButtonClicked(fakeEvent);

    if (DoesFileExist(WalletFullPath)) {
        TextBox->AppendText("\nWallet.pkw File exists! Loading Addresses.\n");
        std::vector<WalletData> walletDataLoaded = ReadWalletData(WalletFullPath);
        TextBox->AppendText("\nNOSO address Loaded : ");
        TextBox->AppendText(std::to_string(walletDataLoaded.size()));
        std::string PrivateKeyLoaded = walletDataLoaded[0].GetHash();
        std::string Label = walletDataLoaded[0].GetLabel();
        std::int64_t Pending = walletDataLoaded[0].GetPending();
        std::int64_t Balance = walletDataLoaded[0].GetBalance();

        if (!PrivateKeyLoaded.empty()) {
            PrivateKeyLoaded.erase(PrivateKeyLoaded.size() - 1); // Clear last value, as it's a \0 from Char[40].
        }
        TextBox->AppendText("\nNOSO Address: ");
        TextBox->AppendText(PrivateKeyLoaded);//Pending Shows a Noso Adress with a end char or string symbol that needs to be removed.
        NosoAddressGrid->SetCellValue(0, 0, PrivateKeyLoaded);
        NosoAddressGrid->SetCellValue(0, 1 ,Label);
        NosoAddressGrid->SetCellValue(0, 2, std::to_string(Pending));
        NosoAddressGrid->SetCellValue(0, 3, std::to_string(Balance));

    }
    else {
        TextBox->AppendText("\nWallet.pkw File does not exist, creating a new NOSO address and Wallet.pkw file.\n ");

    }

    

    
}

bool MainFrame::DoesFileExist(const std::string& filePath)
{
        std::ifstream file(filePath);
        return file.good();
    
}

std::vector<WalletData> MainFrame::ReadWalletData(const std::string& filePath)
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







