
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
