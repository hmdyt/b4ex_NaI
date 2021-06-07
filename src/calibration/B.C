vector<Double_t> B(){
    ofstream ofs("Calibration.txt",ios::trunc);
    ofs.close();

    B_Na_1800V_30mV();
    B_Co_1800V_30mV();
    B_Cs_1800V_35mV();

    return Calibration();

}