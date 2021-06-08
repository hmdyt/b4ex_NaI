vector<Double_t> Na_B(){
    // init hist
    int bin_num = 1000;
    Double_t bin_min = 0;
    Double_t bin_max = 3000;
    TH1D* hist = new TH1D("hist", "Na at detector B ;Energy[keV] ;Counts", bin_num, bin_min, bin_max);

    // fill hist
    make_Na_histogram(hist, "B");

    // fetch calibration para
    vector<Double_t> calb_para = B();
    Double_t a = calb_para[0];
    Double_t b = calb_para[1];

    // arrange Xaxis scale
    auto calc_kev = [](Double_t x, Double_t a, Double_t b){ return (x - b) / a; };
    hist->GetXaxis()->SetLimits(
        calc_kev(bin_min, a, b) * 1000,
        calc_kev(bin_max, a, b) * 1000
    );

    //fitting
    const double FitMin = 200000;
    const double FitMax = 300000;

    TF1 *func = new TF1("func", "gaus", FitMin, FitMax);

    func->SetParName(0, "Const");
    func->SetParName(1, "Mean");
    func->SetParName(2, "Sigma");

    func->SetParameter(0, 80);
    func->SetParameter(1, 250000);
    func->SetParameter(2, 40000);

	func->SetLineColor(kRed);

	hist->Fit(func, "R");

	double Mean = func->GetParameter(1);
	double Sigma = func->GetParameter(2);

	cout << "Mean; " << Mean << endl;
	cout << "Sigma; " << Sigma << endl;

	vector<Double_t> res = {Mean, Sigma};

    // draw and save
    auto c = new TCanvas();
    hist->Draw();
    c->Draw();
    c->SaveAs("img/main_mesurement/Na_B.svg");

	return res;
}
