#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public: 
	MainFrame(const wxString& title);

private:

	void OnConnectButtonClicked(wxCommandEvent& evt); //Call Connect to Mainet and Get NODESATUS
	wxDECLARE_EVENT_TABLE();

};
