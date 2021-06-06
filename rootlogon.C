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
    cout << "executed rootlogon" << endl;
    loadMacro("src/calibration/ABC.C");
    loadMacro("src/main_mesurement/Na_3coinci_th1.C");
    loadMacro("src/main_mesurement/Na_AB_th2.C");
    loadMacro("src/main_mesurement/Na_ABC_th3.C");

}