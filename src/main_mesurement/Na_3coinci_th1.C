void Na_3coinci_th1(){
    // initiailze file name
    // if Co_runXX are increased, one must add filename above
    vector<TString> file_name = {
        "data/b4_2021_0603/Na_run04_change.root",
        "data/b4_2021_0603/Na_run05_change.root",
        "data/b4_2021_0603/Na_run06_change.root",
        };

    // initialize hist params
    Double_t bin_num = 500;
    Double_t bin_min = 0;
    Double_t bin_max = 2500;

    // initialize hist
    TH1D* hist_area1 = new TH1D(
        "Na_area1(A)",
        "Na_area1(A);Energy [eV];Events",
        bin_num,
        bin_min,
        bin_max
        );
    TH1D* hist_area2 = new TH1D(
        "Na_area2(B)",
        "Na_area2(B);Energy [eV];Events",
        bin_num,
        bin_min,
        bin_max
        );
    TH1D* hist_area4 = new TH1D(
        "Na_area4(C)",
        "Na_area4(C);Energy [eV];Events",
        bin_num,
        bin_min,
        bin_max
        );
    
    // fill
    for (int i=0; i<file_name.size(); i++){
        fill_cutting(hist_area1, file_name[i], 1);
        fill_cutting(hist_area2, file_name[i], 2);
        fill_cutting(hist_area4, file_name[i], 4);
    }

    // use calibration
    // def inversed function
    // keV = (ADC - b) / a
    map<string, vector<Double_t>> cal;
    cal["A"] = A();
    cal["B"] = B();
    cal["C"] = C();
    auto calc_kev = [](Double_t x, Double_t a, Double_t b){ return (x - b) / a; };

    // arrange each hist's x axis limit
    hist_area1->GetXaxis()->SetLimits(
        calc_kev(bin_min, cal["A"][0], cal["A"][1]),
        calc_kev(bin_max, cal["A"][0], cal["A"][1])
        );
    hist_area2->GetXaxis()->SetLimits(
        calc_kev(bin_min, cal["B"][0], cal["B"][1]),
        calc_kev(bin_max, cal["B"][0], cal["B"][1])
        );
    hist_area4->GetXaxis()->SetLimits(
        calc_kev(bin_min, cal["C"][0], cal["C"][1]),
        calc_kev(bin_max, cal["C"][0], cal["C"][1])
        );


    //draw
    auto c_Na_area1 = new TCanvas();
    hist_area1->Draw();
    c_Na_area1->Draw();

    auto c_Na_area2 = new TCanvas();
    hist_area2->Draw();
    c_Na_area2->Draw();

    auto c_Na_area4 = new TCanvas();
    hist_area4->Draw();
    c_Na_area4->Draw();
}