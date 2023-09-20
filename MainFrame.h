#pragma once
#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/grid.h>
#include <string>
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
	std::vector<TSummaryData> DownloadSumary();
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
	std::string HexToBase64(const std::string& hexString);
	bool SaveWalletDataToFile(const WalletData& walletData, const std::string& filePath);
	void UpdateDateAndTime();
	void OnTimer(wxTimerEvent& event);
	std::string GetPendings();
	int64_t GetAddressPendingPays(std::string NosoAddress);
	std::vector<unsigned char> nosoBase64Decode(const std::string& input); // Thanks to PasichDEV https://github.com/pasichDev/NosoCpp/blob/d8ee2b5de00ac21eb200eef2a8faf4cdec19aa9a/nCripto.cpp#L225
	OrderData SendFundsFromAddress(std::string& SourceAddress, std::string& DestinationAddress, int64_t& AmountToSend, int64_t& Commision, std::string& Reference, std::string& OrderTime, int line);
	int64_t GetBalanceFromNosoAddress(const std::vector<TSummaryData>& DataVector, const char* NosoAddress);
	void UpdateTable(std::vector<WalletData>& dataVectorAddressTable);
	std::string GetPublicKeyFromNosoAddress(const std::string& NosoAddress);
	std::string GetPrivateKeyFromNosoAddress(const std::string& NosoAddress);
	std::string GetTransferHash(const std::string& Transfer);
	std::string BMB58Sumatory(const std::string& Base58Number);


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

	std::vector<WalletData> walletCPPDataLoaded;
	std::vector<TSummaryData> SumarydataVector;
	std::string CurrentBlockString;
};



 /*
 Function SendFundsFromAddress(Origen, Destino:String; monto, comision:int64; reference,
  ordertime:String;linea:integer):OrderData;
var
  MontoDisponible, Montotrfr, comisionTrfr : int64;
  OrderInfo : orderdata;
Begin

MontoDisponible := ARRAY_Addresses[WalletAddressIndex(origen)].Balance-GetAddressPendingPays(Origen);
if MontoDisponible>comision then ComisionTrfr := Comision
else comisiontrfr := montodisponible;
if montodisponible>monto+comision then montotrfr := monto
else montotrfr := montodisponible-comision;
if montotrfr <0 then montotrfr := 0;
OrderInfo := Default(OrderData);
OrderInfo.OrderID    := '';
OrderInfo.OrderLines := 1;
OrderInfo.OrderType  := 'TRFR';
OrderInfo.TimeStamp  := StrToInt64(OrderTime);
OrderInfo.reference  := reference;
OrderInfo.TrxLine    := linea;
OrderInfo.Sender     := ARRAY_Addresses[WalletAddressIndex(origen)].PublicKey;
OrderInfo.Address    := ARRAY_Addresses[WalletAddressIndex(origen)].Hash;
OrderInfo.Receiver   := Destino;
OrderInfo.AmmountFee := ComisionTrfr;
OrderInfo.AmmountTrf := montotrfr;
OrderInfo.Signature  := GetStringSigned(ordertime+origen+destino+IntToStr(montotrfr)+
                     IntToStr(comisiontrfr)+IntToStr(linea),
                     ARRAY_Addresses[WalletAddressIndex(origen)].PrivateKey);
OrderInfo.TrfrID     := GetTransferHash(ordertime+origen+destino+IntToStr(monto)+IntToStr(WO_LastBlock));
Result := OrderInfo;
End;
 */