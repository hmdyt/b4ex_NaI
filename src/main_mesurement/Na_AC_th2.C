void Na_AC_th2(){

	//set bin
	int x_total_bin = 250;
	int x_bin_min = 0;
	int x_bin_max = 2500;
	int y_total_bin = 250;
	int y_bin_min = 0;
	int y_bin_max = 2500;

	//make hist
	TH2D *hist = new TH2D("hist", "Na (A & C) ;A_Energy[eV] ;C_Energy[eV]", x_total_bin, x_bin_min, x_bin_max, y_total_bin, y_bin_min, y_bin_max);

	//data
	const TString filename1 = "Na_run04";
	const TString filename2 = "Na_run05";
	const TString filename3 = "Na_run06";
	const TString data_dir = "data/b4_2021_0603/";

	//tree
	auto file1 = TFile::Open(data_dir + filename1 + "_change" + ".root");
	TTree *tree1 = (TTree*)file1->Get("tree");

	auto file2 = TFile::Open(data_dir + filename2 + "_change" + ".root");
	TTree *tree2 = (TTree*)file2->Get("tree");

	auto file3 = TFile::Open(data_dir + filename3 + "_change" + ".root");
	TTree *tree3 = (TTree*)file3->Get("tree");
	

	double file1_area1;
	double file1_area4;

	double file2_area1;
	double file2_area4;

	double file3_area1;
	double file3_area4;

	//SetBranchAddress
	tree1->SetBranchAddress("area1", &file1_area1);
	tree1->SetBranchAddress("area4", &file1_area4);
	tree2->SetBranchAddress("area1", &file2_area1);
	tree2->SetBranchAddress("area4", &file2_area4);
	tree3->SetBranchAddress("area1", &file3_area1);
	tree3->SetBranchAddress("area4", &file3_area4);

	for(int i = 0; i < tree1->GetEntries(); i++){

		tree1->GetEntry(i);
		if (file1_area4 < 5000) hist->Fill(file1_area1, file1_area4);


	}

	for(int j = 0; j < tree2->GetEntries(); j++){

		tree2->GetEntry(j);
		if (file2_area4 < 5000) hist->Fill(file2_area1, file2_area4);

	}

	for(int j = 0; j < tree3->GetEntries(); j++){

		tree3->GetEntry(j);
		if (file3_area4 < 5000) hist->Fill(file3_area1, file3_area4);

	}
	
	// calibration
	map<string, vector<Double_t>> cal = {
		{"A", A()},
		{"B", B()},
		{"C", C()}
		};
    //cal["A"] = A();
    //cal["B"] = B();
    //cal["C"] = C();
	auto calc_kev = [](Double_t x, Double_t a, Double_t b){ return (x - b) / a; };
	hist->GetXaxis()->SetLimits(
		calc_kev(x_bin_min, cal["A"][0], cal["A"][1]),
		calc_kev(x_bin_max, cal["A"][0], cal["A"][1])
	);
	hist->GetYaxis()->SetLimits(
		calc_kev(y_bin_min, cal["C"][0], cal["C"][1]),
		calc_kev(y_bin_max, cal["C"][0], cal["C"][1])
	);
	// make canvas
	auto c1 = new TCanvas();
	hist->Draw();
	hist->Draw("colz");
	c1->SaveAs("./img/main_mesurement/Na_AC_th2.svg");

}


