void make_Na_histogram(TH1D* hist, TString detector_char){
    // set file paras and etc.
    // when you earn new mesurement data file, ADD data file above
    TString FILEPATH = "data/b4_2021_0603/";
    vector<TString> tar_data_path = {
        "Na_run04_change.root",
        "Na_run05_change.root",
        "Na_run06_change.root"
    };

    // register each TTree to TChain
    TChain* chain = new TChain("tree");
    for (int i=0; i<tar_data_path.size(); i++){
        chain->Add(FILEPATH + tar_data_path[i]);
    }

    // set branch address
    Double_t A, B, C;
    chain->SetBranchAddress("area1", &A);
    chain->SetBranchAddress("area2", &B);
    chain->SetBranchAddress("area4", &C);

    // fill
    int n_entries = chain->GetEntries();
    for (int i=0; i<n_entries; i++){
        chain->GetEntry(i);
        if (A > 5000 || B > 5000 || C > 5000) continue;
        if (detector_char == "A") hist->Fill(A);
        if (detector_char == "B") hist->Fill(B);
        if (detector_char == "C") hist->Fill(C);
    }

}