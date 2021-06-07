
TH1D* make_hist(TString filename = "A_Co_1900mV_24V", TString Xaxis_name = "ADC Value", int total_bin = 3000, int bin_min = 0, int bin_max = 3500, const TString branch = "area2" ){

	//gROOT->SetStyle("ATLAS");

	//int total_bin = 3000;
	//int bin_min = 0;
	//int bin_max = 3500;
	//const TString branch = "area2";

	const TString data_dir = "data/b4_2021_0527/";

	//TFile *file = TFile::Open("A_Co_1900V_24mV.root");
	auto file = TFile::Open(data_dir + filename +"_change" + ".root");
	TTree *tree = (TTree*)file->Get("tree");

	TH1D *hist = new TH1D(filename, filename, total_bin, bin_min, bin_max);
	//auto c1 = new TCanvas();

	tree->SetBranchStatus("*", 0);
	tree->SetBranchStatus(branch, 1);

	Double_t area1;
	tree->SetBranchAddress(branch,  &area1);

	for(int i = 0; i<tree->GetEntries(); i++){

		tree->GetEntry(i);
		hist->Fill(area1);
	
		//cout << area1 << endl;


	}

	//hist->SetTitle("ADC ;Counts");
	
	hist->SetXTitle(Xaxis_name);
	hist->SetYTitle("Counts");
	hist->Rebin(6);

	return hist;
}
