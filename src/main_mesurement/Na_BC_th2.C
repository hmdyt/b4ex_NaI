void Na_BC_th2(){
	TString FILEPATH ="data/b4_2021_0603/";
	vector<TString> tar_data_path = {
		"Na_run04_change.root",
		"Na_run05_change.root",
		"Na_run06_change.root"
	};

	//resister each TTree to TChain
	TChain* chain = new TChain("tree");
	for(int i = 0; i<tar_data_path.size(); i++){
		chain->Add(FILEPATH + tar_data_path[i]);
	}

	// set Branch address
	Double_t detectorA, detectorB, detectorC;
	//chain->SetBranchAddress("area1", &detectorA);
	chain->SetBranchAddress("area2", &detectorB);
	chain->SetBranchAddress("area4", &detectorC);

	//histgram
	//set parameter
	int x_total_bin = 250;
	int x_bin_min = 0;
	int x_bin_max = 2500;
	int y_total_bin = 250;
	int y_bin_min= 0;
	int y_bin_max = 2500;
	
	//make hist
	TH2D *hist = new TH2D("hist","Na (B and C) ;B_Energy[eV] ;C_Energy[eV]", x_total_bin, x_bin_min, x_bin_max, y_total_bin, y_bin_min,y_bin_max);

	//fill
	int n_entries = chain->GetEntries();
	for(int j = 0; j<n_entries; j++){
		chain->GetEntry(j);
		if (detectorC < 5000) hist->Fill(detectorB, detectorC);
	}

	//calibration
	map<string, vector<Double_t>> cal = {
		{"B", B()},
		{"C", C()}
	};
	auto calc_kev = [](Double_t x, Double_t a, Double_t b){ return (x - b)/ a; };
	hist->GetXaxis()->SetLimits(
		calc_kev(x_bin_min, cal["B"][0], cal["B"][1]),
		calc_kev(x_bin_max, cal["B"][0], cal["B"][1])
	);

	hist->GetYaxis()->SetLimits(
		calc_kev(y_bin_min, cal["C"][0], cal["C"][1]),
		calc_kev(y_bin_max, cal["C"][0], cal["C"][1])
	);
	auto c1 = new TCanvas();
	hist->Draw();
	hist->Draw("colz");
	c1->SaveAs("./img/main_mesurement/Na_BC_th2.svg");
}
