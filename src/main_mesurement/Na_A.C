void Na_A(){
    // init hist
    int bin_num = 1000;
    Double_t bin_min = 0;
    Double_t bin_max = 3000;
    TH1D* hist = new TH1D("hist", "Na at detector A", bin_num, bin_min, bin_max);

    // fill hist
    make_Na_histogram(hist, "A");

    // fetch calibration para
    vector<Double_t> calb_para = A();
    Double_t a = calb_para[0];
    Double_t b = calb_para[0];

    // arrange Xaxis scale
    auto calc_kev = [](Double_t x, Double_t a, Double_t b){ return (x - b) / a; };
    hist->GetXaxis()->SetLimits(
        calc_kev(bin_min, a, b),
        calc_kev(bin_max, a, b)
    );

    // TODO: write fitting macro above

    hist->Draw();
}