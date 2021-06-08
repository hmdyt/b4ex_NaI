void Na_A(){
    // init hist
    int bin_num = 1000;
    Double_t bin_min = -1000000;
    Double_t bin_max = 3000000;
    TH1D* hist = new TH1D("hist", "Na at detector A ;Energy[eV] ;Counts", bin_num, bin_min, bin_max);

    // fill hist
    TString FILEPATH = "data/b4_2021_0603/";
	vector<TString> tar_data_path ={
		"Na_run04_change.root",
		"Na_run05_change.root",
		"Na_run06_change.root"
	};

	// resister each TTree to TChain
	TChain* chain = new TChain("tree");
	for (int i=0; i<tar_data_path.size(); i++){
		chain->Add(FILEPATH + tar_data_path[i]);
	}
	// Set Branch Address
	Double_t detectorA, detectorB, detectorC;
	chain->SetBranchAddress("area1", &detectorA);
	chain->SetBranchAddress("area2", &detectorB);
	chain->SetBranchAddress("area4", &detectorC);


    // fetch calibration para
    vector<Double_t> calb_para1 = A();
    Double_t A_a = calb_para1[0];
    Double_t A_b = calb_para1[1];

    vector<Double_t> calb_para2 = B();
    Double_t B_a = calb_para2[0];
    Double_t B_b = calb_para2[1];

    vector<Double_t> calb_para3 = C();
    Double_t C_a = calb_para3[0];
    Double_t C_b = calb_para3[1];

    // for calibration
    auto calc_kev = [](Double_t x, Double_t a, Double_t b){ return (x - b) / a; };

    // histgram range
	vector<Double_t> B_temp = Na_B();
	Double_t B_Mean = B_temp[0];
	Double_t B_Sigma = B_temp[1];

	vector<Double_t> C_temp = Na_C();
	Double_t C_Mean = C_temp[0];
	Double_t C_Sigma = C_temp[1];
	
	Double_t calibratedA, calibratedB, calibratedC;
	// Fill:
	int n_entries = chain->GetEntries();
	for(int j = 0; j<n_entries; j++){
		chain->GetEntry(j);
		calibratedA = calc_kev(detectorA, A_a, A_b);
		calibratedB = calc_kev(detectorB, B_a, B_b);
		calibratedC = calc_kev(detectorC, C_a, C_b);
		//cout << B_Mean << " " << B_Sigma << " " << detectorB <<endl;
		//cout << C_Mean << " " << C_Sigma << " " << detectorC << endl;
		if( B_Mean - 3*B_Sigma < calibratedB && calibratedB < B_Mean + 3*B_Sigma && C_Mean - 3*C_Sigma < calibratedC && calibratedC < C_Mean + 3*C_Sigma ){
			hist->Fill(calibratedA);
		}
	}
    // draw and save
    auto c = new TCanvas();
    hist->Draw();
    c->Draw();
    c->SaveAs("img/main_mesurement/Na_A.svg");
	
}
