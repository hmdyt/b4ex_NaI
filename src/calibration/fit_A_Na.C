vector<Double_t> fit_A_Na(){
	// data file path
	const TString data_dir = "data/b4_2021_0603/";

	// data filename
	const TString filename1 = "Na_run01";
	const TString filename2 = "Na_run02";
	const TString filename3 = "Na_run03";

	/// hist propaties
	int total_bin = 2000;
	int bin_min = 0;
	int bin_max = 2000;

	// open data files
	auto file1 = TFile::Open(data_dir + filename1 + "_change" + ".root");
	TTree *tree1 = (TTree*)file1->Get("tree");

	auto file2 = TFile::Open(data_dir + filename2 + "_change" + ".root");
	TTree *tree2 = (TTree*)file2->Get("tree");

	auto file3 = TFile::Open(data_dir + filename3 + "_change" + ".root");
	TTree *tree3 = (TTree*)file3->Get("tree");

	// initlaize histogram for fitting
	TH1D *hist_PE = new TH1D("Na_PE_Nal(C)", "Na_PE_Nal(C);ADC value;Events", total_bin, bin_min, bin_max);
	TH1D *hist_CE = new TH1D("Na_CE_OSN(A)", "Na_CE_OSN(A);ADC value;Events", total_bin, bin_min, bin_max);

	// initialze area1 and area4 each file has
	// CE means Compton Effect
	// PE means PhotoElectric Effect
	double file1_CE;
	double file2_CE;
	double file3_CE;
	double file1_PE;
	double file2_PE;
	double file3_PE;

	// set branch address to each area
	tree1->SetBranchAddress("area1", &file1_CE);
	tree2->SetBranchAddress("area1", &file2_CE);
	tree3->SetBranchAddress("area1", &file3_CE);
	tree1->SetBranchAddress("area4", &file1_PE);
	tree2->SetBranchAddress("area4", &file2_PE);
	tree3->SetBranchAddress("area4", &file3_PE);

	// fill data -> hist_PE
	for(int i=0; i<tree1->GetEntries(); i++){ tree1->GetEntry(i); hist_PE->Fill(file1_PE); }
	for(int i=0; i<tree2->GetEntries(); i++){ tree2->GetEntry(i); hist_PE->Fill(file2_PE); }
	for(int i=0; i<tree3->GetEntries(); i++){ tree3->GetEntry(i); hist_PE->Fill(file3_PE); }

	// fill data -> hist_CE
	for(int i=0; i<tree1->GetEntries(); i++){ tree1->GetEntry(i); hist_CE->Fill(file1_CE); }
	for(int i=0; i<tree2->GetEntries(); i++){ tree2->GetEntry(i); hist_CE->Fill(file2_CE); }
	for(int i=0; i<tree3->GetEntries(); i++){ tree3->GetEntry(i); hist_CE->Fill(file3_CE); }
	
	// initialize fitting function for hist_PE
	TF1 *func = new TF1("func", "gaus", 550, 650);
	func->SetParName(0, "Const");
	func->SetParName(1, "Mean");
	func->SetParName(2, "Sigma");

	// set initial parameters
	func->SetParameters(100, 100, 100);

	// Execute fitting and Draw hist
	TCanvas *c1 = new TCanvas();
	hist_PE->Fit(func, "R");
	hist_PE->Draw();
	c1->Draw();

	// save as svg
	TString hist_PE_title = hist_PE->GetTitle();
	c1->SaveAs("img/calibration/" + hist_PE_title + ".svg");

	// fetch fitting function's parameters
	Double_t PE_mean = func->GetParameter(1);
	Double_t PE_sigma = func->GetParameter(2);

	// initialize fitting function for hist_CE (efrc)
    TF1* func_CE = new TF1("func_CE", "[0] * ROOT::Math::gaussian_cdf_c(x, [2], [1])", 400, 800);
	func_CE->SetParName(0, "Const");
	func_CE->SetParName(1, "Mean");
	func_CE->SetParName(2, "Sigma");

	// set initial parameters
	func_CE->SetParameters(10, PE_mean, PE_sigma);

	// execute fitting and draw hist
	TCanvas* c2 = new TCanvas();
	hist_CE->Fit(func_CE, "R");
	hist_CE->Draw();
	c2->Draw();

	// save as svg
	TString hist_CE_title = hist_CE->GetTitle();
	c2->SaveAs("img/calibration/" + hist_CE_title + ".svg");

    // return
    // {kev, kev_error, mean, mean_error}
    // TODO: fix kev value
    vector<Double_t> res = {340.7 * 1000, 0, func_CE->GetParameter(1), func_CE->GetParError(1)};
    return res;
}