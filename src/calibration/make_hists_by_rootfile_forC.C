

TH1D* make_hists_by_rootfile_forC(
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
    TString FILEPATH = "data/b4_2021_0527/";

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
