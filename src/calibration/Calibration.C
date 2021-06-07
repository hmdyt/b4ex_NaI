
vector<Double_t> Calibration(){

	//gROOT->SetStyle("ATLAS");
	//auto c = new TCanvas();
	//c->cd();
	//TH1F *frame = gPad->DrawFrame(0. , 0. , 12. , 120.);

	//TGraphErrors *g = new TGraphErrors("Calibration.txt","%lg %lg %lg");

    TCanvas* c1 = new TCanvas();
	TGraphErrors* g1 = new TGraphErrors();
	ifstream ifs("Calibration.txt");
	Double_t x, y, sy;
	int index = 0;

	while(ifs >> x >> y >> sy){
		g1->SetPoint(index, x, y);
		g1->SetPointError(index, 0, sy);
		index++;

	}
	

	g1->Draw("P");
	
	//Fiti pol1
	TF1 *func = new TF1("func", "[0]*x + [1]", 0, 1500);
	func->SetParameter(1997.71/1173, 0 );
	g1->Fit(func, "R");



	Double_t a = func->GetParameter(0);
	Double_t b = func->GetParameter(1);
	
	TH2D *axis = new TH2D("axis", "keV - ADC value; Energy[keV]; ADC Value", 0, 0, 1500, 0, 0, 3000);
	axis->SetStats(0);
	axis->Draw("AXIS");
	g1->Draw("P SAME");
    c1->Draw();
	c1->SaveAs("img/calibration/B_prop.svg");

	//ADC = a * keV + b
	vector<Double_t> res = {a, b};
	return res;

}
