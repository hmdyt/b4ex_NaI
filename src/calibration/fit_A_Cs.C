vector<Double_t> fit_A_Cs(){
    // initialize some names
    TString FILENAME = "A_Cs_1900V_24mV_dark_change.root";
    TString hist_name = "A_Cs_1900V";
    TString xtitle = "ADC value";
    TString ytitle = "Events";

    // make histgram
    TH1D* hist = make_hists_by_rootfile_forA(
        FILENAME,
        hist_name,
        hist_name + ";" + xtitle + ";" + ytitle,
        1000,
        0,
        2500,
        1
    );

    // initialize TF1 (error function)
    TF1* f = new TF1("erfc", "[0] * ROOT::Math::gaussian_cdf_c(x, [2], [1])", 500, 1500);

    // set param names
    f->SetParName(0, "Const");
    f->SetParName(1, "Mean");
    f->SetParName(2, "Sigma");

    // set initial parameters
    f->SetParameters(50, 1500, 100);

    // execute fitting
    hist->Fit(f, "R");

    // draw
    auto c = new TCanvas();
    hist->Draw();
    c->Draw();
    
    // save as svg
	TString hist_title = hist->GetTitle();
	c->SaveAs("img/calibration/" + hist_title + ".svg");

    // return
    // {kev, kev_error, mean, mean_error}
    vector<Double_t> res = {477*1000, 0, f->GetParameter(1), f->GetParError(1)};
    return res;
}
