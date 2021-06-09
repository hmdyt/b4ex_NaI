
vector<vector<Double_t>> fit_C_Co(){
    // initialize TCanvas
    TCanvas* c1 = new TCanvas();

    // initialize some names
    TString FILENAME = "C_Co_1600V_14mV_change.root";
    TString hist_name = "C_Co_1600V";
    TString xtitle = "ADC value";
    TString ytitle = "Events";

    // make histgram
    TH1D* hist = make_hists_by_rootfile_forC(
        FILENAME,
        hist_name,
        hist_name + ";" + xtitle + ";" + ytitle,
        1000,
        0,
        2500
    );

    // initialize TF1 (f1 is LEFT side peak, f2 is RIGHT side peak)
    TF1* f1 = new TF1("f1", "[0]*TMath::Gaus(x,[1],[2]) + [3]*TMath::Gaus(x, [4], [5])", 1250, 1700);
    //TF1* f2 = new TF1("f2", "[0]*TMath::Gaus(x,[1],[2])", 1450, 1700);

    // set param names
    f1->SetParName(0, "Const");
    f1->SetParName(1, "Mean");
    f1->SetParName(2, "Sigma");
    f1->SetParName(3, "Const");
    f1->SetParName(4, "Mean");
    f1->SetParName(5, "Sigma");

    // set initial parameters
    //f1->SetParameters(100, 1350, 50);
    //f2->SetParameters(100, 1350, 100);
	f1->SetParameter(0, 30);
	f1->SetParameter(1, 1350);
	f1->SetParameter(2, 50);
	f1->SetParameter(3, 30);
	f1->SetParameter(4, 1600);
	f1->SetParameter(5, 50);
	

    // execute fitting
    hist->Fit(f1, "R");
    //hist->Fit(f2, "R+");

    hist->Draw();
    c1->Draw();

    // save as svg
	TString hist_title = hist->GetTitle();
	c1->SaveAs("img/calibration/" + hist_title + ".svg");

    // return
    // res -> {{kev}, {mean}, {mean error}}
    vector<Double_t> kev = {1137 * 1000, 1333 * 1000};
    vector<Double_t> mean = {f1->GetParameter(1), f1->GetParameter(4)};
    vector<Double_t> mean_error = {f1->GetParError(1), f1->GetParError(4)};
    vector<vector<Double_t>> res = {kev, mean, mean_error};
    return res;
}
