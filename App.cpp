#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>

//wxIMPLEMENT_APP(App);
wxIMPLEMENT_APP(App);

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("Noso Wallet");
	mainFrame->SetClientSize(800, 400);
	mainFrame->Center();
	mainFrame->Show();
	//WalletData MyWallet; // Create Empty Wallet Object
	//MyWallet.SetPrivateKey("Hola");
	//std:string privateKeytoset = "Hola";
	//TestWallet.SetPrivateKey = "hole";
	return true;
}
