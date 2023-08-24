#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>


//wxIMPLEMENT_APP(App);
wxIMPLEMENT_APP(App);

bool App::OnInit() {

	MainFrame* mainFrame = new MainFrame("Noso Wallet");
	mainFrame->SetClientSize(800, 800);
	mainFrame->Center();
	mainFrame->Show();
	mainFrame->InitializeWallet();
	

	return true;

}
