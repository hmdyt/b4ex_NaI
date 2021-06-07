
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
    TF1* f1 = new TF1("f1", "[0]*TMath::Gaus(x,[1],[2])", 1250, 1450);
    TF1* f2 = new TF1("f2", "[0]*TMath::Gaus(x,[1],[2])", 1450, 1700);

    // set param names
    f1->SetParName(0, "Const");
    f2->SetParName(0, "Const");
    f1->SetParName(1, "Mean");
    f2->SetParName(1, "Mean");
    f1->SetParName(2, "Sigma");
    f2->SetParName(2, "Sigma");

    // set initial parameters
    f1->SetParameters(100, 1350, 50);
    f2->SetParameters(100, 1350, 100);

    // execute fitting
    hist->Fit(f1, "R");
    hist->Fit(f2, "R+");

    hist->Draw();
    c1->Draw();

    // return
    // res -> {{kev}, {mean}, {mean error}}
    vector<Double_t> kev = {1137 * 1000, 1333 * 1000};
    vector<Double_t> mean = {f1->GetParameter(1), f2->GetParameter(1)};
    vector<Double_t> mean_error = {f1->GetParError(1), f2->GetParError(1)};
    vector<vector<Double_t>> res = {kev, mean, mean_error};
    return res;
}
