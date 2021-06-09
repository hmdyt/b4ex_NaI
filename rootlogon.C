void loadMacro(const TString macro_name){
    // load
    const int ret = gROOT->LoadMacro(macro_name);

    if (ret == 0){
        // load success
        cout << "loaded " + macro_name << endl;
    } else {
        //load failed
        cout << "faild " + macro_name << endl;
        exit(1);
    }
}

void rootlogon(){
    // message
    cout << "executed rootlogon" << endl;

    // calibration
    // for fill
    loadMacro("src/calibration/fill_cutting.C");
    loadMacro("src/calibration/fill_selecting.C");
    // A
    loadMacro("src/calibration/make_hists_by_rootfile_forA.C");
    loadMacro("src/calibration/fit_A_Na.C");
    loadMacro("src/calibration/fit_A_Cs.C");
    loadMacro("src/calibration/fit_A_Co.C");
    loadMacro("src/calibration/A.C");
    // B
    loadMacro("src/calibration/make_hist.C");
    loadMacro("src/calibration/B_Na_1800V_30mV.C");
    loadMacro("src/calibration/B_Cs_1800V_35mV.C");
    loadMacro("src/calibration/B_Co_1800V_30mV.C");
    loadMacro("src/calibration/Calibration.C");
    loadMacro("src/calibration/B.C");
    // C
    loadMacro("src/calibration/make_hists_by_rootfile_forC.C");
    loadMacro("src/calibration/fit_C_Na.C");
    loadMacro("src/calibration/fit_C_Cs.C");
    loadMacro("src/calibration/fit_C_Co.C");
    loadMacro("src/calibration/make_C_kev_adc.C");
    loadMacro("src/calibration/C.C");


    // main_mesurement
    loadMacro("src/main_mesurement/make_Na_histogram.C");
    loadMacro("src/main_mesurement/Na_B.C");
    loadMacro("src/main_mesurement/Na_C.C");
    loadMacro("src/main_mesurement/Na_A.C");
    loadMacro("src/main_mesurement/Na_AB_th2.C");
    loadMacro("src/main_mesurement/Na_BC_th2.C");
    loadMacro("src/main_mesurement/Na_AC_th2.C");
    loadMacro("src/main_mesurement/Na_ABC_th3.C");
}
