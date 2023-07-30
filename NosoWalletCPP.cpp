// Start of wxWidgets "Hello World" Program  https://docs.wxwidgets.org/3.2.2.1/plat_msw_install.html#msw_build_apps
// Includes: 
#include <wx/wx.h>
#include "MainFrame.h"
#include <wx/spinctrl.h>

enum IDs {
	CONNECT_BUTTON_ID=2 //Connect Button ID.
};


//Event Table

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(CONNECT_BUTTON_ID, MainFrame::OnConnectButtonClicked)

wxEND_EVENT_TABLE()



MainFrame::MainFrame(const wxString& title): wxFrame(nullptr,wxID_ANY,title) {   //Constructor Base class
	wxPanel* panel = new wxPanel(this);

	wxButton* Connect_Button = new wxButton(panel, CONNECT_BUTTON_ID, "Connect", wxPoint(1, 1), wxSize(100, 25));
	

	CreateStatusBar(); // Create Status Bar Bottom Window.
} 

void MainFrame::OnConnectButtonClicked(wxCommandEvent& evt)

{
	wxLogStatus("Connecting to Mainet");

}


/*
wxButton* button = new wxButton(panel, wxID_ANY, "New NOSO Address", wxPoint(150, 50), wxSize(100, 35));

wxCheckBox* checkBox = new wxCheckBox(panel, wxID_ANY, "CheckBox", wxPoint(200, 100));

wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, "Static Text", wxPoint(120, 150));

wxTextCtrl* textCtrl = new wxTextCtrl(panel, wxID_ANY, "TextCTRL", wxPoint(500, 145), wxSize(200, -1));

wxSlider* slider = new wxSlider(panel, wxID_ANY, 25, 0, 100, wxPoint(300,145),wxSize(200,-1));

wxGauge* gauge = new wxGauge(panel, wxID_ANY, 100, wxPoint(500, 255), wxSize(200, -1));
gauge->SetValue(50);

wxArrayString choices;
choices.Add("Item A");
choices.Add("Item B");

wxChoice* choice = new wxChoice(panel, wxID_ANY, wxPoint(150, 375), wxSize(100, -1),choices);
choice->Select(0);

wxSpinCtrl* spinCtrl = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(550, 375), wxSize(100, -1));

wxListBox* listBox = new wxListBox(panel, wxID_ANY, wxPoint(150, 475), wxSize(100, -1),choices);

wxRadioBox* radioBox = new wxRadioBox(panel, wxID_ANY, "radioBox", wxPoint(485, 475), wxDefaultSize, choices);
*/