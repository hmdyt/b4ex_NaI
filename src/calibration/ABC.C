TH1D* make_hists_by_rootfile_forA(
    TString file_name = "C_Na_1600V_14mV_change.root",
    TString hist_name = "anonymous",
    TString hist_title = "no title",
    Int_t bin_num = 100,
    Double_t bin_min = 0,
    Double_t bin_max = 8000,
    int which_area = 4
    )
    {
    // set file path
    TString FILEPATH = "~/jupyter/b4ex/nai_2021_05_27/b4_2021_0527/";

    // open root file
    auto file = TFile::Open(FILEPATH + file_name);
    auto tree = dynamic_cast<TTree*>(file->Get("tree"));

    // extract data from tree in root file
    Double_t area1;
    Double_t area2;
    Double_t area3;
    Double_t area4;
    tree->SetBranchAddress("area1", &area1);
    tree->SetBranchAddress("area2", &area2);
    tree->SetBranchAddress("area3", &area3);
    tree->SetBranchAddress("area4", &area4);

    // initialize TH1D
    TH1D* hist = new TH1D(
        hist_name,
        hist_title,
        bin_num,
        bin_min,
        bin_max
    );

    // fill data
    int n_entries = tree->GetEntries();
    for (int i=0; i<n_entries; i++){
        tree->GetEntry(i);
        switch (which_area){
            case 1:
                hist->Fill(area1);
                break;
            case 2:
                hist->Fill(area2);
                break;
            case 3:
                hist->Fill(area3);
                break;
            case 4:
                hist->Fill(area4);
        }
    }

    return hist;
}

vector<Double_t> fit_A_Na(){
	// data file path
	const TString data_dir = "~/jupyter/b4ex/nai_2021_05_27/b4_2021_0603/";

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

    // return
    // {kev, kev_error, mean, mean_error}
    // TODO: fix kev value
    vector<Double_t> res = {340.7 * 1000, 0, func_CE->GetParameter(1), func_CE->GetParError(1)};
    return res;
}

vector<Double_t> fit_A_Cs(){
    // initialize some names
    TString FILENAME = "A_Cs_1900V_24mV_dark_change.root";
    TString hist_name = "A_Cs_1900V";
    TString xtitle = "ADC value";
    TString ytitle = "Events";

    // make histgram
    TH1D* hist = make_hists_by_rootfile_forA(
        FILENAME,
        hist_name,
        hist_name + ";" + xtitle + ";" + ytitle,
        1000,
        0,
        2500,
        1
    );

    // initialize TF1 (error function)
    TF1* f = new TF1("erfc", "[0] * ROOT::Math::gaussian_cdf_c(x, [2], [1])", 500, 1500);

    // set param names
    f->SetParName(0, "Const");
    f->SetParName(1, "Mean");
    f->SetParName(2, "Sigma");

    // set initial parameters
    f->SetParameters(50, 1500, 100);

    // execute fitting
    hist->Fit(f, "R");

    // draw
    hist->Draw();

    // return
    // {kev, kev_error, mean, mean_error}
    vector<Double_t> res = {477*1000, 0, f->GetParameter(1), f->GetParError(1)};
    return res;
}

void fill_cutting(TH1D* hist, TString filename, int branch){
    auto file = TFile::Open(filename);
    auto tree = dynamic_cast<TTree*>(file->Get("tree"));
    Double_t area1, area2, area4;
    tree->SetBranchAddress("area1", &area1);
    tree->SetBranchAddress("area2", &area2);
    tree->SetBranchAddress("area4", &area4);

    for (int i=0; i<tree->GetEntries(); i++){
        tree->GetEntry(i);
        if (area1 < 5000 && area4 < 5000){
            if (branch == 1) hist->Fill(area1);
            if (branch == 2) hist->Fill(area2);
            if (branch == 4) hist->Fill(area4);
        }
    }
}

void fill_selecting(
    TH1D* hist, 
    TString filename,
    Double_t mean,
    Double_t sigma
    )
    {
    auto file = TFile::Open(filename);
    auto tree = dynamic_cast<TTree*>(file->Get("tree"));
    Double_t area1, area4;
    tree->SetBranchAddress("area1", &area1);
    tree->SetBranchAddress("area4", &area4);

    for (int i=0; i<tree->GetEntries(); i++){
        tree->GetEntry(i);
        if (area1 < 5000 && mean-3*sigma < area4 && area4 < mean+sigma*3){
            hist->Fill(area1);
        }else{
            // if (area1 < 5000 && area4 < 5000){ hist->Fill(area1); }
            int pass;
        }
    }
}

vector<vector<Double_t>> fit_A_Co(){
    // initiailze file name
    // if Co_runXX are increased, one must add filename above
    vector<TString> file_name = {
        "~/jupyter/b4ex/nai_2021_05_27/b4_2021_0603/Co_run01_change.root",
        "~/jupyter/b4ex/nai_2021_05_27/b4_2021_0603/Co_run02_change.root"
        };

    // initialize hist params
    Double_t bin_num = 500;
    Double_t bin_min = 0;
    Double_t bin_max = 2500;

    // initialize hist
    TH1D* hist_area1 = new TH1D("hist_area1", "Co_run1+2 area1;ADC;Event", bin_num, bin_min, bin_max);
    TH1D* hist_area4 = new TH1D("hist_area4", "Co_run1+2 area4;ADC;Event", bin_num, bin_min, bin_max);
    TH1D* hist_area1_l = new TH1D("hist_area4_l", "Co_run1+2 left area4;ADC;Event", bin_num, bin_min, bin_max);
    TH1D* hist_area1_r = new TH1D("hist_area4_r", "Co_run1+2 right area4;ADC;Event", bin_num, bin_min, bin_max);

    // fill data
    for (int i=0; i<file_name.size(); i++){
        fill_cutting(hist_area1, file_name[i], 1);
        fill_cutting(hist_area4, file_name[i], 4);
    }

    // initialize TF1 for PE
    TF1* fl = new TF1("fl", "gaus", 1300, 1450);
    TF1* fr = new TF1("fr", "gaus", 1500, 1700);
    fl->SetParName(0, "Const");
    fl->SetParName(1, "Mean");
    fl->SetParName(2, "Sigma");
    fr->SetParName(0, "Const");
    fr->SetParName(1, "Mean");
    fr->SetParName(2, "Sigma");
    fl->SetParameters(100, 1350, 50);
    fr->SetParameters(100, 1600, 100);

    // execute fitting
    hist_area4->Fit(fl, "R");
    hist_area4->Fit(fr, "R+");

    // fill data into hist_area1_l and _r
    for (int i=0; i<file_name.size(); i++){
        fill_selecting(
            hist_area1_l,
            file_name[i],
            fl->GetParameter(1),
            fl->GetParameter(2)
        );
        fill_selecting(
            hist_area1_r,
            file_name[i],
            fr->GetParameter(1),
            fr->GetParameter(2)
        );
    }

    // initialize TF1 for CE
    TF1* f_CE_l = new TF1("erfc_l", "[0] * ROOT::Math::gaussian_cdf_c(x, [2], [1])", 1300, 2000);
    TF1* f_CE_r = new TF1("erfc_r", "[0] * ROOT::Math::gaussian_cdf_c(x, [2], [1])", 1300, 2000);
    f_CE_l->SetParName(0, "Const");
    f_CE_l->SetParName(1, "Mean");
    f_CE_l->SetParName(2, "Sigma");
    f_CE_r->SetParName(0, "Const");
    f_CE_r->SetParName(1, "Mean");
    f_CE_r->SetParName(2, "Sigma");
    f_CE_l->SetParameters(1, 1500, 200);
    f_CE_r->SetParameters(1, 1500, 200);

    // execute fitting
    hist_area1_l->Fit(f_CE_l, "R");
    hist_area1_r->Fit(f_CE_r, "R");

    // draw
    auto c1 = new TCanvas();
    hist_area1->Draw();
    c1->Draw();

    auto c2 = new TCanvas();
    hist_area4->Draw();
    c2->Draw();

    auto c3 = new TCanvas();
    hist_area1_l->Draw();
    c3->Draw();

    auto c4 = new TCanvas();
    hist_area1_r->Draw();
    c4->Draw();

    // return
    // res = {{kevs}, {kev_errors}, {means}, {mean_errors}}
    vector<Double_t> kevs = {1119 * 1000, 963.2 * 1000};
    vector<Double_t> kev_errors = {0, 0};
    vector<Double_t> means = {f_CE_l->GetParameter(1), f_CE_r->GetParameter(1)};
    vector<Double_t> mean_errors = {f_CE_l->GetParError(1), f_CE_r->GetParError(1)};
    vector<vector<Double_t>> res = {kevs, kev_errors, means, mean_errors};
    return res;
    
}

vector<Double_t> A(){
    // initialize TGraph data
    vector<Double_t> kevs, kev_errors, means, mean_errors;

    // fetch fitted parameters <- {kev, kev_error, mean, mean_error}
    // Co has to paek ,so paras are {{kevs}, {kev_errors}, {means}, {mean_errors}}
    vector<Double_t> Na = fit_A_Na();
    vector<Double_t> Cs = fit_A_Cs();
    vector<vector<Double_t>> Co = fit_A_Co();

    // fill data
    kevs.push_back(Na[0]);
    kevs.push_back(Cs[0]);
    kevs.push_back(Co[0][0]);
    kevs.push_back(Co[0][1]);

    kev_errors.push_back(Na[1]);
    kev_errors.push_back(Cs[1]);
    kev_errors.push_back(Co[1][0]);
    kev_errors.push_back(Co[1][1]);

    means.push_back(Na[2]);
    means.push_back(Cs[2]);
    means.push_back(Co[2][0]);
    means.push_back(Co[2][1]);

    mean_errors.push_back(Na[3]);
    mean_errors.push_back(Cs[3]);
    mean_errors.push_back(Co[3][0]);
    mean_errors.push_back(Co[3][1]);

    // pedestal
    kevs.push_back(0);
    kev_errors.push_back(0);
    means.push_back(0);
    mean_errors.push_back(0);

    // initialize TGraphErrors
    TGraphErrors* g = new TGraphErrors(kevs.size(), &kevs[0], &means[0], &kev_errors[0], &mean_errors[0]);
    
    // initialize TF1 (pol1)
    TF1* f = new TF1("f", "[0]*x + [1]", 0, 1500 * 1000);
    f->SetParameters(means[0] / kevs[0], 0);

    // execute fitting
    g->Fit(f, "R");
    
    // draw TGraph with axis-histgram
    TH2D* axis = new TH2D("axis", "keV - ADC value; Energy [eV]; ADC value", 0, 0, 1400*1000, 0, 0, 1600);
    axis->SetStats(0);
    axis->Draw("AXIS");
    g->Draw("P SAME");
    TCanvas* c = new TCanvas();
    g->Draw("AP");
    c->Draw();

    // ADC = a * keV + b
    // res -> {a, b}
    Double_t a = f->GetParameter(0);
    Double_t b = f->GetParameter(1);
    vector<Double_t> res = {a, b};
    return res;

}// This

#include<fstream> 


TH1D* make_hist(TString filename = "A_Co_1900mV_24V", TString Xaxis_name = "ADC Value", int total_bin = 3000, int bin_min = 0, int bin_max = 3500, const TString branch = "area2" ){

	//gROOT->SetStyle("ATLAS");

	//int total_bin = 3000;
	//int bin_min = 0;
	//int bin_max = 3500;
	//const TString branch = "area2";

	const TString data_dir = "~/jupyter/b4ex/nai_2021_05_27/b4_2021_0527/";

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


void B_Co_1800V_30mV(){

	auto c1 = new TCanvas();

	ofstream ofs("Calibration.txt",ios::app);

	TH1D* hist1 = make_hist("B_Co_1800V_30mV","ADC Value", 3000, 0, 3500, "area2"); 

	hist1->Draw();
	c1->Draw();

	const Double_t FitMin1 =1900 ;
	const Double_t FitMax1 =2100 ;
	const Double_t FitMin2 =2200 ;
	const Double_t FitMax2 =2400 ;

	TF1* func1 = new TF1("func1", "gaus", FitMin1, FitMax1);
	TF1* func2 = new TF1("func2", "gaus", FitMin2, FitMax2);

	func1->SetParName(0, "Const");
	func1->SetParName(1, "Mean");
	func1->SetParName(2, "Sigma");

	func2->SetParName(0, "Const");
	func2->SetParName(1, "Mean");
	func2->SetParName(2, "Sigma");

	func1->SetParameter(0,50);
	func1->SetParameter(1,2000);
	func1->SetParameter(2,50);

	func2->SetParameter(0, 40);
	func2->SetParameter(1, 2300);
	func2->SetParameter(2, 50);

	hist1->Fit("func1", "R", "same", FitMin1, FitMax1);
	hist1->Fit("func2", "R+", "same", FitMin2, FitMax2);

	func1->SetLineColor(kRed);
	func1->Draw("same");
	func2->SetLineColor(kRed);
	func2->Draw("same");

	//Mean
	Double_t y1 = func1->GetParameter(1);
	Double_t y2 = func2->GetParameter(1);

	//cout << x1 << endl;
	//cout << x2 << endl;

	//MeanError
	Double_t a1 = func1->GetParError(1);
	Double_t a2 = func2->GetParError(1);
	
	Double_t x1 = 1173.;
	Double_t x2 = 1333.;


	ofs << x1 << " " << y1 << " "  << a1 << "\n";
	ofs << x2 << " " << y2 << " "  << a2 << "\n";


}

void B_Cs_1800V_35mV(){

	ofstream ofs("Calibration.txt",ios::app);

	auto c1 = new TCanvas();

	TH1D* hist1 = make_hist("B_Cs_1800V_35mV"); 

	hist1->Draw();
	c1->Draw();

	const Double_t FitMin = 1050 ;
	const Double_t FitMax = 1250 ;

	TF1* func = new TF1("func", "gaus", FitMin, FitMax);

	func->SetParName(0, "Const");
	func->SetParName(1, "Mean");
	func->SetParName(2, "Sigma");

	func->SetParameter(0, 220);
	func->SetParameter(1, 1150);
	func->SetParameter(2, 50);

	hist1->Fit("func", "R", "same", FitMin, FitMax);
	
	func->SetLineColor(kRed);
	func->Draw("same");


	//Mean
	Double_t y = func->GetParameter(1);

	//MeanError
	Double_t a = func->GetParError(1);

	Double_t x = 661.;

	ofs << x << " " << y << " " << a << "\n";


}

void B_Na_1800V_30mV(){

	ofstream ofs("Calibration.txt",ios::app);

	auto c1 = new TCanvas();

	TH1D* hist1 = make_hist("B_Na_1800V_30mV"); 

	hist1->Draw();
	c1->Draw();

	const Double_t FitMin1 = 800;
	const Double_t FitMax1 = 1000;

	const Double_t FitMin2 = 2100;
	const Double_t FitMax2 = 2300;
	
	TF1* func1 = new TF1("func1", "gaus", FitMin1, FitMax1);
	TF1* func2 = new TF1("func2", "gaus", FitMin2, FitMax2);
	
	func1->SetParName(0, "Const");
	func1->SetParName(1, "Mean");
	func1->SetParName(2, "Sigma");

	func2->SetParName(0, "Const");
	func2->SetParName(1, "Mean");
	func2->SetParName(2, "Sigma");
	
	func1->SetParameter(0, 240);
	func1->SetParameter(1, 900);
	func1->SetParameter(2, 30);

	func2->SetParameter(0, 20);
	func2->SetParameter(1, 2200);
	func2->SetParameter(2, 50);

	hist1->Fit("func1", "R", "same", FitMin1, FitMax1);
	hist1->Fit("func2", "R+", "same", FitMin2, FitMax2);

	
	func1->SetLineColor(kRed);
	func1->Draw("same");

	func2->SetLineColor(kRed);
	func2->Draw("same");

	Double_t y1 = func1->GetParameter(1);
	Double_t y2 = func2->GetParameter(1);

	Double_t a1 = func1->GetParError(1);
	Double_t a2 = func2->GetParError(1);

	Double_t x1 = 511.;
	Double_t x2 = 1275.;

	ofs << x1 << " " << y1 << " " << a1 << "\n";
	ofs << x2 << " " << y2 << " " << a2 << "\n";

}

vector<Double_t> Calibration(){

	//gROOT->SetStyle("ATLAS");
	//auto c = new TCanvas();
	//c->cd();
	//TH1F *frame = gPad->DrawFrame(0. , 0. , 12. , 120.);

	//TGraphErrors *g = new TGraphErrors("Calibration.txt","%lg %lg %lg");

    TCanvas* c1 = new TCanvas();
	TGraphErrors* g1 = new TGraphErrors();
	ifstream ifs("Calibration.txt");
	Double_t x, y, sy;
	int index = 0;

	while(ifs >> x >> y >> sy){
		g1->SetPoint(index, x, y);
		g1->SetPointError(index, 0, sy);
		index++;

	}
	

	g1->Draw("P");
	
	//Fiti pol1
	TF1 *func = new TF1("func", "[0]*x + [1]", 0, 1500);
	func->SetParameter(1997.71/1173, 0 );
	g1->Fit(func, "R");



	Double_t a = func->GetParameter(0);
	Double_t b = func->GetParameter(1);
	
	TH2D *axis = new TH2D("axis", "keV - ADC value; Energy[keV]; ADC Value", 0, 0, 1500, 0, 0, 3000);
	axis->SetStats(0);
	axis->Draw("AXIS");
	g1->Draw("P SAME");
    c1->Draw();

	//ADC = a * keV + b
	vector<Double_t> res = {a, b};
	return res;

}


vector<Double_t> B(){
    ofstream ofs("Calibration.txt",ios::trunc);
    ofs.close();

    B_Na_1800V_30mV();
    B_Co_1800V_30mV();
    B_Cs_1800V_35mV();

    return Calibration();

}TH1D* make_hists_by_rootfile_forC(
    TString file_name = "C_Na_1600V_14mV_change.root",
    TString hist_name = "anonymous",
    TString hist_title = "no title",
    Int_t bin_num = 100,
    Double_t bin_min = 0,
    Double_t bin_max = 8000,
    int which_area = 4
    )
    {
    // set file path
    TString FILEPATH = "~/jupyter/b4ex/nai_2021_05_27/b4_2021_0527/";

    // open root file
    auto file = TFile::Open(FILEPATH + file_name);
    auto tree = dynamic_cast<TTree*>(file->Get("tree"));

    // extract data from tree in root file
    Double_t area1;
    Double_t area2;
    Double_t area3;
    Double_t area4;
    tree->SetBranchAddress("area1", &area1);
    tree->SetBranchAddress("area2", &area2);
    tree->SetBranchAddress("area3", &area3);
    tree->SetBranchAddress("area4", &area4);

    // initialize TH1D
    TH1D* hist = new TH1D(
        hist_name,
        hist_title,
        bin_num,
        bin_min,
        bin_max
    );

    // fill data
    int n_entries = tree->GetEntries();
    for (int i=0; i<n_entries; i++){
        tree->GetEntry(i);
        switch (which_area){
            case 1:
                hist->Fill(area1);
                break;
            case 2:
                hist->Fill(area2);
                break;
            case 3:
                hist->Fill(area3);
                break;
            case 4:
                hist->Fill(area4);
        }
    }

    return hist;
}

vector<vector<Double_t>> fit_C_Na(){
    // initialize TCanvas
    TCanvas* c1 = new TCanvas();

    // initialize some names
    TString FILENAME = "C_Na_1600V_14mV_change.root";
    TString hist_name = "C_Na_1600V";
    TString xtitle = "ADC value";
    TString ytitle = "Events";

    // make histgram
    TH1D* hist = make_hists_by_rootfile_forC(
        FILENAME,
        hist_name,
        hist_name + ";" + xtitle + ";" + ytitle,
        1000,
        0,
        2500
    );

    // initialize TF1 (f1 is LEFT side peak, f2 is RIGHT side peak)
    TF1* f1 = new TF1("f1", "[0]*TMath::Gaus(x,[1],[2])", 550, 700);
    TF1* f2 = new TF1("f2", "[0]*TMath::Gaus(x,[1],[2])", 1450, 1600);

    // set param names
    f1->SetParName(0, "Const");
    f2->SetParName(0, "Const");
    f1->SetParName(1, "Mean");
    f2->SetParName(1, "Mean");
    f1->SetParName(2, "Sigma");
    f2->SetParName(2, "Sigma");

    // set initial parameters
    f1->SetParameters(100, 600, 100);
    f2->SetParameters(100, 1520, 40);

    // execute fitting
    hist->Fit(f1, "R");
    hist->Fit(f2, "R+");

    hist->Draw();
    c1->Draw();

    // return
    // res -> {{kev}, {mean}, {mean error}}
    vector<Double_t> kev = {511 * 1000, 1275 * 1000};
    vector<Double_t> mean = {f1->GetParameter(1), f2->GetParameter(1)};
    vector<Double_t> mean_error = {f1->GetParError(1), f2->GetParError(1)};
    vector<vector<Double_t>> res = {kev, mean, mean_error};
    return res;
}

vector<vector<Double_t>> fit_C_Co(){
    // initialize TCanvas
    TCanvas* c1 = new TCanvas();

    // initialize some names
    TString FILENAME = "C_Co_1600V_14mV_change.root";
    TString hist_name = "C_Co_1600V";
    TString xtitle = "ADC value";
    TString ytitle = "Events";

    // make histgram
    TH1D* hist = make_hists_by_rootfile_forC(
        FILENAME,
        hist_name,
        hist_name + ";" + xtitle + ";" + ytitle,
        1000,
        0,
        2500
    );

    // initialize TF1 (f1 is LEFT side peak, f2 is RIGHT side peak)
    TF1* f1 = new TF1("f1", "[0]*TMath::Gaus(x,[1],[2])", 1250, 1450);
    TF1* f2 = new TF1("f2", "[0]*TMath::Gaus(x,[1],[2])", 1450, 1700);

    // set param names
    f1->SetParName(0, "Const");
    f2->SetParName(0, "Const");
    f1->SetParName(1, "Mean");
    f2->SetParName(1, "Mean");
    f1->SetParName(2, "Sigma");
    f2->SetParName(2, "Sigma");

    // set initial parameters
    f1->SetParameters(100, 1350, 50);
    f2->SetParameters(100, 1350, 100);

    // execute fitting
    hist->Fit(f1, "R");
    hist->Fit(f2, "R+");

    hist->Draw();
    c1->Draw();

    // return
    // res -> {{kev}, {mean}, {mean error}}
    vector<Double_t> kev = {1137 * 1000, 1333 * 1000};
    vector<Double_t> mean = {f1->GetParameter(1), f2->GetParameter(1)};
    vector<Double_t> mean_error = {f1->GetParError(1), f2->GetParError(1)};
    vector<vector<Double_t>> res = {kev, mean, mean_error};
    return res;
}

vector<vector<Double_t>> fit_C_Cs(){
    // initialize TCanvas
    TCanvas* c1 = new TCanvas();

    // initialize some names
    TString FILENAME = "C_Cs_1600V_24mV_change.root";
    TString hist_name = "C_Cs_1600V";
    TString xtitle = "ADC value";
    TString ytitle = "Events";

    // make histgram
    TH1D* hist = make_hists_by_rootfile_forC(
        FILENAME,
        hist_name,
        hist_name + ";" + xtitle + ";" + ytitle,
        1000,
        0,
        1200
    );

    // initialize TF1
    TF1* f1 = new TF1("f1", "[0]*TMath::Gaus(x,[1],[2])", 730, 900);

    // set param names
    f1->SetParName(0, "Const");
    f1->SetParName(1, "Mean");
    f1->SetParName(2, "Sigma");

    // set initial parameters
    f1->SetParameters(100, 800, 40);

    // execute fitting
    hist->Fit(f1, "R");

    hist->Draw();
    c1->Draw();

    // return
    // res -> {{kev}, {mean}, {mean error}}
    vector<Double_t> kev = {661 * 1000};
    vector<Double_t> mean = {f1->GetParameter(1)};
    vector<Double_t> mean_error = {f1->GetParError(1)};
    vector<vector<Double_t>> res = {kev, mean, mean_error};
    return res;
}

vector<Double_t> make_C_kev_adc(){
    // initialize TCanvas
    TCanvas* c1 = new TCanvas();

    // initialize TGraph data
    vector<Double_t> kevs;
    vector<Double_t> kevs_error;
    vector<Double_t> means;
    vector<Double_t> means_error;


    // call Na, Co, Cs calibration data {{kev}, {mean}, {mean error}}
    vector<vector<Double_t>> Na = fit_C_Na();
    vector<vector<Double_t>> Co = fit_C_Co();
    vector<vector<Double_t>> Cs = fit_C_Cs();


    // fill calibration data into TGraph data
    kevs.insert(kevs.end(), Na[0].begin(), Na[0].end());
    kevs.insert(kevs.end(), Co[0].begin(), Co[0].end());
    kevs.insert(kevs.end(), Cs[0].begin(), Cs[0].end());

    means.insert(means.end(), Na[1].begin(), Na[1].end());
    means.insert(means.end(), Co[1].begin(), Co[1].end());
    means.insert(means.end(), Cs[1].begin(), Cs[1].end());

    means_error.insert(means_error.end(), Na[2].begin(), Na[2].end());
    means_error.insert(means_error.end(), Co[2].begin(), Co[2].end());
    means_error.insert(means_error.end(), Cs[2].begin(), Cs[2].end());

    for (int i=0; i<kevs.size(); i++) { kevs_error.push_back(.0); } // kevs_error are zero


    // initialize TGraphErrors
    TGraphErrors* g = new TGraphErrors(kevs.size(), &kevs[0], &means[0], &kevs_error[0], &means_error[0]);
    
    // initialize TF1 (pol1)
    TF1* f = new TF1("f", "[0]*x + [1]", 0, 1500 * 1000);
    f->SetParameters(means[0] / kevs[0], 0);

    // execute fitting
    g->Fit(f, "R");
    

    // draw TGraph with axis-histgram
    TH2D* axis = new TH2D("axis", "keV - ADC value; Energy [eV]; ADC value", 0, 0, 1400*1000, 0, 0, 1600);
    axis->SetStats(0);
    axis->Draw("AXIS");
    g->Draw("P SAME");
    c1->Draw();

    // ADC = a * keV + b
    // res -> {a, b}
    Double_t a = f->GetParameter(0);
    Double_t b = f->GetParameter(1);
    vector<Double_t> res = {a, b};
    return res;
}


vector<Double_t> C(){
    return make_C_kev_adc();
}map<string, vector<Double_t>> ABC(){
    map<string, vector<Double_t>> res;
    res["A"] = A();
    res["B"] = B();
    res["C"] = C();
    return res;
}