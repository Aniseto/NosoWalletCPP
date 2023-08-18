#pragma once
#include <wx/wx.h>
#include <wx/textctrl.h>
#include <string>
#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/oids.h>
#include <cryptopp/hex.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/ripemd.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/integer.h>
#include <cryptopp/algebra.h>
#include "DataStructures.h"


class MainFrame : public wxFrame
{
public: 
	MainFrame(const wxString& title);
	std::string MasterNodeListString="Empty";
	std::string GeneratedNosoAddress = "Empty";
	//WalletData TestWallet;
	


	
	

private:


	void OnConnectButtonClicked(wxCommandEvent& evt); //Call Connect to Main net and Get NODESATUS
	void OnDownloadSummaryButtonClicked(wxCommandEvent& evt);
	void OnSyncMainNetTimeButtonClicked(wxCommandEvent& evt);
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
	std::string SignMessage(const std::string& message, const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey& privateKey);
	bool VerifyMessage(const std::string& message, const std::string& signature, const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey& publicKey);
	void SignAndVerify(wxCommandEvent& evt);

	
	
	//Time Sync NTP
	wxStaticText* CurrentBlock;
	wxStaticText* GetSumaryText;

	//Noso Addresses Control
	wxStaticText* TotalNosoAddressesLoadedText;
	wxStaticText* TotalNosoAddressesLoadedValue;
	
	//Time Controls

	wxStaticText* MainNetTimeText;
    wxTimer* Timer;
	void OnTimer(wxTimerEvent& event);

	//MasterNode List
	wxStaticText* MasterNodeListText;


	//Text VBox to show data
	wxTextCtrl* TextBox;

	wxStaticText* GenerateKeysText;

	//Noso AddressText

	wxStaticText* GenerateNOSOAddressText;


 


};
