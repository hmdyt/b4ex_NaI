
vector<vector<Double_t>> fit_C_Cs(){
    // initialize TCanvas
    TCanvas* c1 = new TCanvas();

    // initialize some names
    TString FILENAME = "C_Cs_1600V_24mV_change.root";
    TString hist_name = "C_Cs_1600V";
    TString xtitle = "ADC value";
    TString ytitle = "Events";

    // make histgram
    TH1D* hist = make_hists_by_rootfile_forC(
        FILENAME,
        hist_name,
        hist_name + ";" + xtitle + ";" + ytitle,
        1000,
        0,
        1200
    );

    // initialize TF1
    TF1* f1 = new TF1("f1", "[0]*TMath::Gaus(x,[1],[2])", 730, 900);

    // set param names
    f1->SetParName(0, "Const");
    f1->SetParName(1, "Mean");
    f1->SetParName(2, "Sigma");

    // set initial parameters
    f1->SetParameters(100, 800, 40);

    // execute fitting
    hist->Fit(f1, "R");

    hist->Draw();
    c1->Draw();

    // return
    // res -> {{kev}, {mean}, {mean error}}
    vector<Double_t> kev = {661 * 1000};
    vector<Double_t> mean = {f1->GetParameter(1)};
    vector<Double_t> mean_error = {f1->GetParError(1)};
    vector<vector<Double_t>> res = {kev, mean, mean_error};
    return res;
}
