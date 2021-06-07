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
