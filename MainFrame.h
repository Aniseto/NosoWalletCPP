#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public: 
	MainFrame(const wxString& title);

private:

	void OnConnectButtonClicked(wxCommandEvent& evt); //Call Connect to Main net and Get NODESATUS
	void OnDownloadSummaryButtonClicked(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);
	wxStaticText* CurrentBlock;
	wxStaticText* GetSummaryText;



};
