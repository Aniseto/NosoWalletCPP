#pragma once
#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/grid.h>
#include <string>
//#include <cryptopp/eccrypto.h>
//#include <cryptopp/osrng.h>
//#include <cryptopp/oids.h>
//#include <cryptopp/hex.h>
//#include <cryptopp/cryptlib.h>
//#include <cryptopp/ripemd.h>
//#include <cryptopp/cryptlib.h>
//#include <cryptopp/integer.h>
//#include <cryptopp/algebra.h>
#include "DataStructures.h"


class MainFrame : public wxFrame
{
public: 
	MainFrame(const wxString& title);
	std::string MasterNodeListString="Empty";
	std::string GeneratedNosoAddress = "Empty";
	WalletData MyWallet;
	void InitializeWallet();
	bool DoesFileExist(const std::string& filePath);
	std::vector<WalletData> ReadWalletDataFromNosolite(const std::string& filePath);
	std::vector<WalletData> ReadWalletDataFromNosoCPP(const std::string& filePath);



private:


	void OnConnectButtonClicked(wxCommandEvent& evt); //Call Connect to Main net and Get NODESATUS
	void DownloadSumary();
	//void OnDownloadSummaryButtonClicked(wxCommandEvent& evt);
	//time_t GetNTPTime();
	//void SyncMainNetTime();
	void OnClose(wxCloseEvent& evt);
	void GetMasterNodeList(wxCommandEvent& evt);
	void GenerateKeys(wxCommandEvent& evt);
	void GetMasterNodeConfig(wxCommandEvent& evt);
	bool UnzipFile(const wxString& zipFileName, const wxString& outputDir);
	std::string PublicKeyToSHA256(const std::string& publicKey);
	std::string CalculateMD160(const std::string& SHA256String);
	std::string EncodeBase58(const std::string& MD160String);
	int CalculateCheckSum(const std::string& StringChecksum);
	std::string BmDecto58(const std::string& number);
	std::string SignMessage(const std::string& message, const std::string& privateKeyBase64);
	bool VerifySignature(const std::string& message, const std::string& signatureBase64, const std::string& publicKeyBase64);
	std::string base64ToPEMPrivateKey(const std::string& privateKeyBase64);
	std::string addPEMHeaders(const std::string& privateKeyBase64, const std::string& header, const std::string& footer);

	//bool VerifyMessage(const std::string& message, const std::string& signature, const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey& publicKey);
	//void SignAndVerify(wxCommandEvent& evt);
	std::string HexToBase64(const std::string& hexString);
	bool SaveWalletDataToFile(const WalletData& walletData, const std::string& filePath);
	void UpdateDateAndTime();
	void OnTimer(wxTimerEvent& event);
	//std::string ConvertToPEM(const Botan::secure_vector<uint8_t>& data);
	//std::string ConvertToPEM(const Botan::secure_vector<uint8_t>& keyData);
	
	




	void OnOpen(wxCommandEvent& event);

	void OnSave(wxCommandEvent& event);

	void OnExit(wxCommandEvent& event);
	
	//Main Menu definition

	wxMenu* MainMenu;
	wxMenuBar* menuBar;
	wxMenuItem* openItem;
	wxMenuItem* saveItem;
	wxMenuItem* exitItem;

	//Time Controls

	wxTimer* timer;

	//wxStaticText* MainNetTimeText;
    //wxTimer* Timer;
	//void OnTimer(wxTimerEvent& event);
	wxStaticText* dateTimeText;
	//wxStaticText* dateAndTimeText;
	//Log Box

	wxTextCtrl* TextBox;

	//Noso Address Central Grid

	wxGrid* NosoAddressGrid;

	//Status Bar

	wxStatusBar* statusBar;

 
};

 