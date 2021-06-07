
vector<Double_t> A(){
    // initialize TGraph data
    vector<Double_t> kevs, kev_errors, means, mean_errors;

    // fetch fitted parameters <- {kev, kev_error, mean, mean_error}
    // Co has to paek ,so paras are {{kevs}, {kev_errors}, {means}, {mean_errors}}
    vector<Double_t> Na = fit_A_Na();
    vector<Double_t> Cs = fit_A_Cs();
    vector<vector<Double_t>> Co = fit_A_Co();

    // fill data
    kevs.push_back(Na[0]);
    kevs.push_back(Cs[0]);
    kevs.push_back(Co[0][0]);
    kevs.push_back(Co[0][1]);

    kev_errors.push_back(Na[1]);
    kev_errors.push_back(Cs[1]);
    kev_errors.push_back(Co[1][0]);
    kev_errors.push_back(Co[1][1]);

    means.push_back(Na[2]);
    means.push_back(Cs[2]);
    means.push_back(Co[2][0]);
    means.push_back(Co[2][1]);

    mean_errors.push_back(Na[3]);
    mean_errors.push_back(Cs[3]);
    mean_errors.push_back(Co[3][0]);
    mean_errors.push_back(Co[3][1]);

    // pedestal
    kevs.push_back(0);
    kev_errors.push_back(0);
    means.push_back(0);
    mean_errors.push_back(0);

    // initialize TGraphErrors
    TGraphErrors* g = new TGraphErrors(kevs.size(), &kevs[0], &means[0], &kev_errors[0], &mean_errors[0]);
    
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
    TCanvas* c = new TCanvas();
    g->Draw("AP");
    c->Draw();

    // ADC = a * keV + b
    // res -> {a, b}
    Double_t a = f->GetParameter(0);
    Double_t b = f->GetParameter(1);
    vector<Double_t> res = {a, b};
    return res;

}
