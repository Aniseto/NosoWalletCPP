#pragma once
#include <wx/wx.h>
#include <wx/textctrl.h>
#include <string>


class MainFrame : public wxFrame
{
public: 
	MainFrame(const wxString& title);
	std::string MasterNodeListString="Empty";

private:

	void OnConnectButtonClicked(wxCommandEvent& evt); //Call Connect to Main net and Get NODESATUS
	void OnDownloadSummaryButtonClicked(wxCommandEvent& evt);
	void OnSyncMainNetTimeButtonClicked(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);
	void GetMasterNodeList(wxCommandEvent& evt);
	void GenerateKeys(wxCommandEvent& evt);
	void GetMasterNodeConfig(wxCommandEvent& evt);

	
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
 


};
