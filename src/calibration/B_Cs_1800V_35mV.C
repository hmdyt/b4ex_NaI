
void B_Cs_1800V_35mV(){

	ofstream ofs("Calibration.txt",ios::app);

	auto c1 = new TCanvas();

	TH1D* hist1 = make_hist("B_Cs_1800V_35mV"); 

	hist1->Draw();
	c1->Draw();

	const Double_t FitMin = 1050 ;
	const Double_t FitMax = 1250 ;

	TF1* func = new TF1("func", "gaus", FitMin, FitMax);

	func->SetParName(0, "Const");
	func->SetParName(1, "Mean");
	func->SetParName(2, "Sigma");

	func->SetParameter(0, 220);
	func->SetParameter(1, 1150);
	func->SetParameter(2, 50);

	hist1->Fit("func", "R", "same", FitMin, FitMax);
	
	func->SetLineColor(kRed);
	func->Draw("same");


	//Mean
	Double_t y = func->GetParameter(1);

	//MeanError
	Double_t a = func->GetParError(1);

	Double_t x = 661 * 1000;

	ofs << x << " " << y << " " << a << "\n";

	// save as svg
	TString hist1_title = hist1->GetTitle();
	c1->SaveAs("img/calibration/" + hist1_title + ".svg");

}
