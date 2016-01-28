#include "DataFormats/PatCandidates/interface/Muon.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "rochcor2015.h"
#include "muresolution_run2.h"

class TunePMuonProducer : public edm::EDProducer
{
public:
	TunePMuonProducer(const edm::ParameterSet&);

	virtual void produce(edm::Event&, const edm::EventSetup&);

private:
	const edm::InputTag src;

};

TunePMuonProducer::TunePMuonProducer(const edm::ParameterSet& cfg)
	: src(cfg.getParameter<edm::InputTag>("src"))
{

	produces <pat::MuonCollection>();

}

void TunePMuonProducer::produce(edm::Event& event, const edm::EventSetup&)
{
	edm::Handle<pat::MuonCollection> muons;
	event.getByLabel(src, muons);

	std::auto_ptr<pat::MuonCollection> mus(new pat::MuonCollection);
	reco::Candidate::PolarLorentzVector tmp_muon;
	TLorentzVector corrected_muon;
	
	rochcor2015 *rmcor = new rochcor2015(); // make the pointer of rochcor class



	for(auto mu : *muons) {
		if(mu.tunePMuonBestTrack().isAvailable())
			corrected_muon.SetPtEtaPhiM(mu.tunePMuonBestTrack()->pt(), mu.tunePMuonBestTrack()->eta(), mu.tunePMuonBestTrack()->phi(), 0.1057);

		else
			corrected_muon.SetPtEtaPhiM(0.0, 0.0, 0.0, 0.0);

		int charge = mu.charge();
		float qter = 1.0; // added it by Higgs group’s request to propagate the uncertainty
		//If you run MC, apply the muon momentum correction, “momcor_mc( )” function (only for MC) //ntrk (number of track layer) is one of input and it can slightly improved the extra smearing 
		rmcor->momcor_mc(corrected_muon, charge, 0, qter);
		//If you run data, apply the muon momentum correction, "momcor_data()" function (only for Data) // No run dependence for 2012 data, so default of “runopt=0”
		//rmcor->momcor_data(corrected_muon, charge, 0, qter);

		tmp_muon.SetCoordinates(corrected_muon.Pt(),corrected_muon.Eta(),corrected_muon.Phi(),corrected_muon.M());
		//tmp_muon.SetCoordinates(mu.tunePMuonBestTrack()->pt(), mu.tunePMuonBestTrack()->eta(), mu.tunePMuonBestTrack()->phi(), 0.1057);
		mu.setP4(tmp_muon);
		mus->push_back(mu);
	}
	event.put(mus);
}

DEFINE_FWK_MODULE(TunePMuonProducer);
