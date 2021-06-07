
vector<Double_t> make_C_kev_adc(){
    // initialize TCanvas
    TCanvas* c1 = new TCanvas();

    // initialize TGraph data
    vector<Double_t> kevs;
    vector<Double_t> kevs_error;
    vector<Double_t> means;
    vector<Double_t> means_error;


    // call Na, Co, Cs calibration data {{kev}, {mean}, {mean error}}
    vector<vector<Double_t>> Na = fit_C_Na();
    vector<vector<Double_t>> Co = fit_C_Co();
    vector<vector<Double_t>> Cs = fit_C_Cs();


    // fill calibration data into TGraph data
    kevs.insert(kevs.end(), Na[0].begin(), Na[0].end());
    kevs.insert(kevs.end(), Co[0].begin(), Co[0].end());
    kevs.insert(kevs.end(), Cs[0].begin(), Cs[0].end());

    means.insert(means.end(), Na[1].begin(), Na[1].end());
    means.insert(means.end(), Co[1].begin(), Co[1].end());
    means.insert(means.end(), Cs[1].begin(), Cs[1].end());

    means_error.insert(means_error.end(), Na[2].begin(), Na[2].end());
    means_error.insert(means_error.end(), Co[2].begin(), Co[2].end());
    means_error.insert(means_error.end(), Cs[2].begin(), Cs[2].end());

    for (int i=0; i<kevs.size(); i++) { kevs_error.push_back(.0); } // kevs_error are zero


    // initialize TGraphErrors
    TGraphErrors* g = new TGraphErrors(kevs.size(), &kevs[0], &means[0], &kevs_error[0], &means_error[0]);
    
    // initialize TF1 (pol1)
    TF1* f = new TF1("f", "[0]*x + [1]", 0, 1500 * 1000);
    f->SetParameters(means[0] / kevs[0], 0);

    // execute fitting
    g->Fit(f, "R");
    

    // draw TGraph with axis-histgram
    TH2D* axis = new TH2D("axis", "keV - ADC value; Energy [eV]; ADC value", 0, 0, 1400*1000, 0, 0, 1600);
    axis->SetStats(0);
    axis->Draw("AXIS");
    g->Draw("P SAME");
    c1->Draw();

    // ADC = a * keV + b
    // res -> {a, b}
    Double_t a = f->GetParameter(0);
    Double_t b = f->GetParameter(1);
    vector<Double_t> res = {a, b};
    return res;
}
