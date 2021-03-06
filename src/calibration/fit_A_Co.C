
vector<vector<Double_t>> fit_A_Co(){
    // initiailze file name
    // if Co_runXX are increased, one must add filename above
    vector<TString> file_name = {
        "data/b4_2021_0603/Co_run01_change.root",
        "data/b4_2021_0603/Co_run02_change.root"
        };

    // initialize hist params
    Double_t bin_num = 500;
    Double_t bin_min = 0;
    Double_t bin_max = 2500;

    // initialize hist
    TH1D* hist_area1 = new TH1D("hist_area1", "Co_run1+2 area1;ADC;Event", bin_num, bin_min, bin_max);
    TH1D* hist_area4 = new TH1D("hist_area4", "Co_run1+2 area4;ADC;Event", bin_num, bin_min, bin_max);
    TH1D* hist_area1_l = new TH1D("hist_area4_l", "Co_run1+2 left area4;ADC;Event", bin_num, bin_min, bin_max);
    TH1D* hist_area1_r = new TH1D("hist_area4_r", "Co_run1+2 right area4;ADC;Event", bin_num, bin_min, bin_max);

    // fill data
    for (int i=0; i<file_name.size(); i++){
        fill_cutting(hist_area1, file_name[i], 1);
        fill_cutting(hist_area4, file_name[i], 4);
    }

    // initialize TF1 for PE (gaus + gaus)
    TF1 * f_PE = new TF1("f_PE", "[0]*TMath::Gaus(x,[1],[2]) + [3]*TMath::Gaus(x,[4],[5])", 1300, 1700);
    f_PE->SetParName(0, "Const_left");
    f_PE->SetParName(1, "Mean_left");
    f_PE->SetParName(2, "Sigma_left");
    f_PE->SetParName(3, "Const_right");
    f_PE->SetParName(4, "Mean_right");
    f_PE->SetParName(5, "Sigma_right");
    f_PE->SetParameters(100, 1350, 50, 100, 1600, 100);


    // execute fitting
    hist_area4->Fit(f_PE, "R");

    // fill data into hist_area1_l and _r
    for (int i=0; i<file_name.size(); i++){
        fill_selecting(
            hist_area1_l,
            file_name[i],
            f_PE->GetParameter(1),
            f_PE->GetParameter(2)
        );
        fill_selecting(
            hist_area1_r,
            file_name[i],
            f_PE->GetParameter(4),
            f_PE->GetParameter(5)
        );
    }

    // initialize TF1 for CE
    TF1* f_CE_l = new TF1("erfc_l", "[0] * ROOT::Math::gaussian_cdf_c(x, [2], [1])", 1300, 2000);
    TF1* f_CE_r = new TF1("erfc_r", "[0] * ROOT::Math::gaussian_cdf_c(x, [2], [1])", 1300, 2000);
    f_CE_l->SetParName(0, "Const");
    f_CE_l->SetParName(1, "Mean");
    f_CE_l->SetParName(2, "Sigma");
    f_CE_r->SetParName(0, "Const");
    f_CE_r->SetParName(1, "Mean");
    f_CE_r->SetParName(2, "Sigma");
    f_CE_l->SetParameters(1, 1500, 200);
    f_CE_r->SetParameters(1, 1500, 200);

    // execute fitting
    hist_area1_l->Fit(f_CE_l, "R");
    hist_area1_r->Fit(f_CE_r, "R");

    // draw
    auto c1 = new TCanvas();
    hist_area1->Draw();
    c1->Draw();

    auto c2 = new TCanvas();
    hist_area4->Draw();
    c2->Draw();

    auto c3 = new TCanvas();
    hist_area1_l->Draw();
    c3->Draw();

    auto c4 = new TCanvas();
    hist_area1_r->Draw();
    c4->Draw();

    // save as svg
    TString hist_area1_title = hist_area1->GetTitle();
    TString hist_area4_title = hist_area4->GetTitle();
    TString hist_area1_l_title = hist_area1_l->GetTitle();
    TString hist_area1_r_title = hist_area1_r->GetTitle();
    c1->SaveAs("img/calibration/" + hist_area1_title + ".svg");
    c2->SaveAs("img/calibration/" + hist_area4_title + ".svg");
    c3->SaveAs("img/calibration/" + hist_area1_l_title + ".svg");
    c4->SaveAs("img/calibration/" + hist_area1_r_title + ".svg");

    // return
    // res = {{kevs}, {kev_errors}, {means}, {mean_errors}}
    vector<Double_t> kevs = {1119 * 1000, 963.2 * 1000};
    vector<Double_t> kev_errors = {0, 0};
    vector<Double_t> means = {f_CE_l->GetParameter(1), f_CE_r->GetParameter(1)};
    vector<Double_t> mean_errors = {f_CE_l->GetParError(1), f_CE_r->GetParError(1)};
    vector<vector<Double_t>> res = {kevs, kev_errors, means, mean_errors};
    return res;
    
}
