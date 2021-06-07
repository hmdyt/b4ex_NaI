

void B_Co_1800V_30mV(){

	auto c1 = new TCanvas();

	ofstream ofs("Calibration.txt",ios::app);

	TH1D* hist1 = make_hist("B_Co_1800V_30mV","ADC Value", 3000, 0, 3500, "area2"); 

	hist1->Draw();
	c1->Draw();

	const Double_t FitMin1 =1900 ;
	const Double_t FitMax1 =2100 ;
	const Double_t FitMin2 =2200 ;
	const Double_t FitMax2 =2400 ;

	TF1* func1 = new TF1("func1", "gaus", FitMin1, FitMax1);
	TF1* func2 = new TF1("func2", "gaus", FitMin2, FitMax2);

	func1->SetParName(0, "Const");
	func1->SetParName(1, "Mean");
	func1->SetParName(2, "Sigma");

	func2->SetParName(0, "Const");
	func2->SetParName(1, "Mean");
	func2->SetParName(2, "Sigma");

	func1->SetParameter(0,50);
	func1->SetParameter(1,2000);
	func1->SetParameter(2,50);

	func2->SetParameter(0, 40);
	func2->SetParameter(1, 2300);
	func2->SetParameter(2, 50);

	hist1->Fit("func1", "R", "same", FitMin1, FitMax1);
	hist1->Fit("func2", "R+", "same", FitMin2, FitMax2);

	func1->SetLineColor(kRed);
	func1->Draw("same");
	func2->SetLineColor(kRed);
	func2->Draw("same");

	//Mean
	Double_t y1 = func1->GetParameter(1);
	Double_t y2 = func2->GetParameter(1);

	//cout << x1 << endl;
	//cout << x2 << endl;

	//MeanError
	Double_t a1 = func1->GetParError(1);
	Double_t a2 = func2->GetParError(1);
	
	Double_t x1 = 1173.;
	Double_t x2 = 1333.;


	ofs << x1 << " " << y1 << " "  << a1 << "\n";
	ofs << x2 << " " << y2 << " "  << a2 << "\n";

	// save as svg
	TString hist1_title = hist1->GetTitle();
	c1->SaveAs("img/calibration/" + hist1_title + ".svg");
}
