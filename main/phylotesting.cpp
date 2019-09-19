/*
 * phylotesting.cpp
 *
 *  Created on: Aug 23, 2013
 *      Author: minh
 */



#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <iqtree_config.h>
#include <numeric>
#include "tree/phylotree.h"
#include "tree/iqtree.h"
#include "tree/phylosupertree.h"
#include "tree/phylotreemixlen.h"
#include "phylotesting.h"

#include "model/modelmarkov.h"
#include "model/modeldna.h"
#include "nclextra/myreader.h"
#include "model/rateheterogeneity.h"
#include "model/rategamma.h"
#include "model/rateinvar.h"
#include "model/rategammainvar.h"
#include "model/ratefree.h"
#include "model/ratefreeinvar.h"
//#include "modeltest_wrapper.h"
#include "model/modelprotein.h"
#include "model/modelbin.h"
#include "model/modelcodon.h"
#include "model/modelmorphology.h"
#include "model/modelmixture.h"
#include "model/modelliemarkov.h"
#include "model/modelpomo.h"
#include "utils/timeutil.h"
#include "model/modelfactorymixlen.h"
#include "tree/phylosupertreeplen.h"

#include "phyloanalysis.h"
#include "gsl/mygsl.h"
#include "utils/MPIHelper.h"
//#include "vectorclass/vectorclass.h"


/******* Binary model set ******/
const char* bin_model_names[] = { "JC2", "GTR2" };


/******* Morphological model set ******/
// 2018-08-20: don't test ORDERED model due to lots of numerical issues
//const char* morph_model_names[] = {"MK", "ORDERED"};
const char* morph_model_names[] = {"MK"};


/******* DNA model set ******/
const char* dna_model_names[] = { "JC", "F81", "K80", "HKY", "TNe",
		"TN", "K81", "K81u", "TPM2", "TPM2u", "TPM3", "TPM3u", "TIMe", "TIM",
		"TIM2e", "TIM2", "TIM3e", "TIM3", "TVMe", "TVM", "SYM", "GTR" };

/* DNA models supported by PhyML/PartitionFinder */
const char* dna_model_names_old[] ={"JC", "F81", "K80", "HKY", "TNe",
 	 	 "TN", "K81", "K81u", "TIMe", "TIM", "TVMe", "TVM", "SYM", "GTR"};

/* DNA model supported by RAxML */
const char* dna_model_names_rax[] ={"GTR"};

/* DNA model supported by MrBayes */
const char *dna_model_names_mrbayes[] = {"JC", "F81", "K80", "HKY", "SYM", "GTR"};

/* DNA model supported by ModelOMatic */
const char *dna_model_names_modelomatic[] = {"JC", "F81", "K80", "HKY", "GTR"};

//const char* dna_freq_names[] = {"+FO"};

// Lie-Markov models without an RY, WS or MK prefix
const char *dna_model_names_lie_markov_fullsym[] = 
    {"1.1", "3.3a", "4.4a", "6.7a", "9.20b", "12.12"};
// Lie-Markov models with RY symmetry/distinguished pairing
const char *dna_model_names_lie_markov_ry[] = {
          "RY2.2b",  "RY3.3b", "RY3.3c",  "RY3.4",   "RY4.4b", 
          "RY4.5a",  "RY4.5b", "RY5.6a",  "RY5.6b",  "RY5.7a",
          "RY5.7b",  "RY5.7c", "RY5.11a", "RY5.11b", "RY5.11c", 
          "RY5.16",  "RY6.6",  "RY6.7b",  "RY6.8a",  "RY6.8b",  
          "RY6.17a", "RY6.17b","RY8.8",   "RY8.10a", "RY8.10b", 
          "RY8.16",  "RY8.17", "RY8.18",  "RY9.20a", "RY10.12",
	  "RY10.34"
};
// Lie-Markov models with WS symmetry/distinguished pairing
const char *dna_model_names_lie_markov_ws[] = {
          "WS2.2b",  "WS3.3b", "WS3.3c",  "WS3.4",   "WS4.4b", 
          "WS4.5a",  "WS4.5b", "WS5.6a",  "WS5.6b",  "WS5.7a",
          "WS5.7b",  "WS5.7c", "WS5.11a", "WS5.11b", "WS5.11c", 
          "WS5.16",  "WS6.6",  "WS6.7b",  "WS6.8a",  "WS6.8b",  
          "WS6.17a", "WS6.17b","WS8.8",   "WS8.10a", "WS8.10b", 
          "WS8.16",  "WS8.17", "WS8.18",  "WS9.20a", "WS10.12",
	  "WS10.34"
};
// Lie-Markov models with MK symmetry/distinguished pairing
const char *dna_model_names_lie_markov_mk[] = {
          "MK2.2b",  "MK3.3b", "MK3.3c",  "MK3.4",   "MK4.4b", 
          "MK4.5a",  "MK4.5b", "MK5.6a",  "MK5.6b",  "MK5.7a",
          "MK5.7b",  "MK5.7c", "MK5.11a", "MK5.11b", "MK5.11c", 
          "MK5.16",  "MK6.6",  "MK6.7b",  "MK6.8a",  "MK6.8b",  
          "MK6.17a", "MK6.17b","MK8.8",   "MK8.10a", "MK8.10b", 
          "MK8.16",  "MK8.17", "MK8.18",  "MK9.20a", "MK10.12",
	  "MK10.34"
};
// Lie-Markov models which are strand symmetric
const char *dna_model_names_lie_markov_strsym[] = {
          "1.1",    "WS2.2b", "3.3a",   "WS3.3b", "WS3.3c", "WS3.4",   
          "WS4.4b", "WS4.5a", "WS4.5b", "WS5.6a", "WS6.6"
};


/****** Protein model set ******/
const char* aa_model_names[] = { "Dayhoff", "mtMAM", "JTT", "WAG",
		"cpREV", "mtREV", "rtREV", "mtART", "mtZOA", "VT", "LG", "DCMut", "PMB",
		"HIVb", "HIVw", "JTTDCMut", "FLU", "Blosum62" , "mtMet" , "mtVer" , "mtInv" };
        
/* Protein models supported by PhyML/PartitionFinder */
const char *aa_model_names_phyml[] = { "Dayhoff", "mtMAM", "JTT", "WAG",
		"cpREV", "mtREV", "rtREV", "mtART", "VT", "LG", "DCMut",
		"HIVb", "HIVw", "Blosum62" };

/* Protein models supported by RAxML */
const char *aa_model_names_rax[] = { "Dayhoff", "mtMAM", "JTT", "WAG",
		"cpREV", "mtREV", "rtREV", "mtART", "mtZOA", "PMB", "HIVb", "HIVw", "JTTDCMut", "FLU", "VT", "LG", "DCMut", "Blosum62" };

const char* aa_model_names_mrbayes[] = {"Poisson", "Dayhoff", "mtMAM", "JTT", "WAG",
		"cpREV", "mtREV", "rtREV", "VT", "Blosum62" };

const char* aa_model_names_modelomatic[] = {"Poisson", "Blosum62", "Dayhoff", "HIVb", "HIVw",
    "JTT", "LG", "VT", "WAG", "cpREV", "mtART", "mtMAM", "mtREV", "rtREV"};

const char *aa_model_names_nuclear[] = {"WAG", "Dayhoff","JTT", "LG", "VT", "DCMut", "PMB", "JTTDCMut", "Blosum62"};

const char *aa_model_names_mitochondrial[] = {"mtREV", "mtMAM", "mtART", "mtZOA", "mtMet" , "mtVer" , "mtInv" };

const char *aa_model_names_chloroplast[] = {"cpREV"};

const char *aa_model_names_viral[] = {"HIVb", "HIVw", "FLU", "rtREV"};

const char* aa_freq_names[] = {"", "+F"};


/****** Codon models ******/
//const char *codon_model_names[] = {"GY", "MG", "MGK", "KOSI07", "SCHN05","KOSI07_GY1KTV","SCHN05_GY1KTV"};
//short int std_genetic_code[]    = {   0,    0,     0,        1,        1,              1,              1};
const char *codon_model_names[] = {"MG", "MGK", "GY", "KOSI07", "SCHN05"};
short int std_genetic_code[]    = {   0,    0,     0,        1,        1};
const char *codon_model_names_modelomatic[] = {"GY"};
short int std_genetic_code_modelomatic[]    = {   0};

const char *codon_freq_names[] = {"", "+F1X4", "+F3X4", "+F"};

//const double TOL_LIKELIHOOD_MODELTEST = 0.1;
const double TOL_GRADIENT_MODELTEST   = 0.0001;

extern double RunKMeans1D(int n, int k, double *points, int *weights, double *centers, int *assignments);

class ModelAdjust {
public:
    ModelAdjust() {
        logl = 0.0;
        df = 0;
        sample_size = 0;
    }
    double logl;
    int df;
    size_t sample_size;
};

class CandidateModel {
    
public:
    
    /** constructor */
    CandidateModel(int flag = 0) {
        this->flag = flag;
    }

    CandidateModel(string name, int flag = 0) {
        this->name = name;
        this->flag = flag;
    }

    CandidateModel(Alignment *aln, int flag = 0) {
        this->flag = flag;
        getUsualModel(aln);
    }

    /**
        get usual model for a given alignment
        @param aln input alignment
        @return length of the alignment
     */
    size_t getUsualModel(Alignment *aln);

    /**
     evaluate this model
     @param model_name model to be tested
     @param params program parameters
     @param in_tree input tree
     @param[in] in_model_info input checkpointing information
     @param[out] out_model_info output checkpointing information
     @param[out] info output model information
     @param models_block models block
     @param num_thread number of threads
     @return tree string
     */
    string evaluate(Params &params, Alignment *in_aln,
                ModelCheckpoint &in_model_info, ModelCheckpoint &out_model_info,
                ModelInfo &info, ModelsBlock *models_block,
                int &num_threads, int brlen_type, ModelAdjust *adjust = NULL);

    /** model name */
    string name;

protected:
    
    
    /** flag */
    int flag;
};

class CandidateModelSet : public vector<CandidateModel> {
public:
    /**
     * get the list of model
     * @param params program parameters
     * @param aln alignment
     * param separate_rate true to separate rates from models
     * @param merge_phase true to consider models for merging phase
     * @return maximum number of rate categories
     */
    int generate(Params &params, Alignment *aln, bool separate_rate, bool merge_phase);
};

string getSeqTypeName(SeqType seq_type) {
    switch (seq_type) {
        case SEQ_BINARY: return "binary";
        case SEQ_DNA: return "DNA";
        case SEQ_PROTEIN: return "protein";
        case SEQ_CODON: return "codon";
        case SEQ_MORPH: return "morphological";
        case SEQ_POMO: return "PoMo";
        case SEQ_UNKNOWN: return "unknown";
        case SEQ_MULTISTATE: return "MultiState";
    }
}

string getUsualModelSubst(SeqType seq_type) {
    switch (seq_type) {
        case SEQ_DNA: return "GTR";
        case SEQ_PROTEIN: return "LG";
        case SEQ_CODON: return "GY";
        case SEQ_BINARY: return "GTR2";
        case SEQ_MORPH: return "MK";
        case SEQ_POMO: return "GTR+P";
        default: ASSERT(0 && "Unprocessed seq_type"); return "";
    }
}

void getRateHet(SeqType seq_type, string model_name, double frac_invariant_sites,
                string rate_set, StrVector &ratehet);

size_t CandidateModel::getUsualModel(Alignment *aln) {
    size_t aln_len = 0;
    if (aln->isSuperAlignment()) {
        SuperAlignment *super_aln = (SuperAlignment*)aln;
        for (auto it = super_aln->partitions.begin(); it != super_aln->partitions.end(); it++) {
            CandidateModel usual_model(*it);
            if (!name.empty())
                name += ',';
            name += usual_model.name;
            aln_len += (*it)->getNSite();
        }
    } else {
        name = getUsualModelSubst(aln->seq_type);
        StrVector ratehet;
        getRateHet(aln->seq_type, Params::getInstance().model_name, aln->frac_invariant_sites, "1", ratehet);
        ASSERT(!ratehet.empty());
        name += ratehet[0];
        aln_len = aln->getNSite();
    }
    return aln_len;
}

void ModelInfo::computeICScores(size_t sample_size) {
    computeInformationScores(logl, df, sample_size, AIC_score, AICc_score, BIC_score);
}

double ModelInfo::computeICScore(size_t sample_size) {
    return computeInformationScore(logl, df, sample_size, Params::getInstance().model_test_criterion);
}

bool ModelCheckpoint::getBestModel(string &best_model) {
    return getString("best_model_" + criterionName(Params::getInstance().model_test_criterion), best_model);
}

bool ModelCheckpoint::getBestModelList(string &best_model_list) {
    return getString("best_model_list_" + criterionName(Params::getInstance().model_test_criterion), best_model_list);
}

void ModelCheckpoint::putBestModelList(string &best_model_list) {
    return put("best_model_list_" + criterionName(Params::getInstance().model_test_criterion), best_model_list);
}

bool  ModelCheckpoint::getBestTree(string &best_tree) {
    return getString("best_tree_" + criterionName(Params::getInstance().model_test_criterion), best_tree);
}

bool ModelCheckpoint::getOrderedModels(PhyloTree *tree, vector<ModelInfo> &ordered_models) {
    double best_score_AIC, best_score_AICc, best_score_BIC;
    if (tree->isSuperTree()) {
        PhyloSuperTree *stree = (PhyloSuperTree*)tree;
        ordered_models.clear();
        for (int part = 0; part != stree->size(); part++) {
            startStruct(stree->at(part)->aln->name);
            ModelInfo info;
            if (!getBestModel(info.name)) return false;
            info.restoreCheckpoint(this);
            info.computeICScores(stree->at(part)->getAlnNSite());
            endStruct();
            ordered_models.push_back(info);
        }
        return true;
    } else {
        CKP_RESTORE2(this, best_score_AIC);
        CKP_RESTORE2(this, best_score_AICc);
        CKP_RESTORE2(this, best_score_BIC);
        double sum_AIC = 0, sum_AICc = 0, sum_BIC = 0;
        string str;
        bool ret = getBestModelList(str);
        if (!ret) return false;
        istringstream istr(str);
        string model;
        ordered_models.clear();
        while (istr >> model) {
            ModelInfo info;
            info.name = model;
            info.restoreCheckpoint(this);
            info.computeICScores(tree->getAlnNSite());
            sum_AIC  += info.AIC_weight = exp(-0.5*(info.AIC_score-best_score_AIC));
            sum_AICc += info.AICc_weight = exp(-0.5*(info.AICc_score-best_score_AICc));
            sum_BIC  += info.BIC_weight = exp(-0.5*(info.BIC_score-best_score_BIC));
            ordered_models.push_back(info);
        }
        sum_AIC = 1.0/sum_AIC;
        sum_AICc = 1.0/sum_AICc;
        sum_BIC = 1.0/sum_BIC;
        for (auto it = ordered_models.begin(); it != ordered_models.end(); it++) {
            it->AIC_weight *= sum_AIC;
            it->AICc_weight *= sum_AICc;
            it->BIC_weight *= sum_BIC;
            it->AIC_conf = it->AIC_weight > 0.05;
            it->AICc_conf = it->AICc_weight > 0.05;
            it->BIC_conf = it->BIC_weight > 0.05;
        }
        return true;
    }
}

/**
 * copy from cvec to strvec
 */
void copyCString(const char **cvec, int n, StrVector &strvec, bool touppercase = false) {
	strvec.resize(n);
	for (int i = 0; i < n; i++) {
		strvec[i] = cvec[i];
        if (touppercase)
            std::transform(strvec[i].begin(), strvec[i].end(), strvec[i].begin(), ::toupper);
    }
}

/**
 * append from cvec to strvec
 */
void appendCString(const char **cvec, int n, StrVector &strvec, bool touppercase = false) {
        strvec.reserve(strvec.size()+n);
	for (int i = 0; i < n; i++) {
	    strvec.push_back(cvec[i]);
            if (touppercase)
	      std::transform(strvec.back().begin(), strvec.back().end(), strvec.back().begin(), ::toupper);
        }
}


int detectSeqType(const char *model_name, SeqType &seq_type) {
    bool empirical_model = false;
    int i;
    string model_str = model_name;
    std::transform(model_str.begin(), model_str.end(), model_str.begin(), ::toupper);
    StrVector model_list;

    seq_type = SEQ_UNKNOWN;
    
    copyCString(bin_model_names, sizeof(bin_model_names)/sizeof(char*), model_list, true);
    for (i = 0; i < model_list.size(); i++)
        if (model_str == model_list[i]) {
            seq_type = SEQ_BINARY;
            break;
        }
    copyCString(morph_model_names, sizeof(morph_model_names)/sizeof(char*), model_list, true);
    for (i = 0; i < model_list.size(); i++)
        if (model_str == model_list[i]) {
            seq_type = SEQ_MORPH;
            break;
        }
    copyCString(dna_model_names, sizeof(dna_model_names)/sizeof(char*), model_list, true);
    for (i = 0; i < model_list.size(); i++)
        if (model_str == model_list[i]) {
            seq_type = SEQ_DNA;
            break;
        }
    copyCString(aa_model_names, sizeof(aa_model_names)/sizeof(char*), model_list, true);
    for (i = 0; i < model_list.size(); i++)
        if (model_str == model_list[i]) {
            seq_type = SEQ_PROTEIN;
            empirical_model = true;
            break;
        }
    copyCString(codon_model_names, sizeof(codon_model_names)/sizeof(char*), model_list, true);
    for (i = 0; i < model_list.size(); i++)
        if (model_str.substr(0,model_list[i].length()) == model_list[i]) {
            seq_type = SEQ_CODON;
            if (std_genetic_code[i]) empirical_model = true;
            break;
        }
        
    return (empirical_model) ? 2 : 1;
}

string detectSeqTypeName(string model_name) {
    SeqType seq_type;
    detectSeqType(model_name.c_str(), seq_type);
    switch (seq_type) {
    case SEQ_BINARY: return "BIN"; break;
    case SEQ_MORPH: return "MORPH"; break;
    case SEQ_DNA: return "DNA"; break;
    case SEQ_PROTEIN: return "AA"; break;
    case SEQ_CODON: return "CODON"; break;
    default: break;
    }
    return "";
}

void computeInformationScores(double tree_lh, int df, int ssize, double &AIC, double &AICc, double &BIC) {
	AIC = -2 * tree_lh + 2 * df;
	AICc = AIC + 2.0 * df * (df + 1) / max(ssize - df - 1, 1);
	BIC = -2 * tree_lh + df * log(ssize);
}

double computeInformationScore(double tree_lh, int df, int ssize, ModelTestCriterion mtc) {
	double AIC, AICc, BIC;
	computeInformationScores(tree_lh, df, ssize, AIC, AICc, BIC);
	if (mtc == MTC_AIC)
		return AIC;
	if (mtc == MTC_AICC)
		return AICc;
	if (mtc == MTC_BIC)
		return BIC;
	return 0.0;
}

string criterionName(ModelTestCriterion mtc) {
	if (mtc == MTC_AIC)
		return "AIC";
	if (mtc == MTC_AICC)
		return "AICc";
	if (mtc == MTC_BIC)
		return "BIC";
	return "";
}

void printSiteLh(const char*filename, PhyloTree *tree, double *ptn_lh,
		bool append, const char *linename) {
	int i;
	double *pattern_lh;
	if (!ptn_lh) {
		pattern_lh = new double[tree->getAlnNPattern()];
		tree->computePatternLikelihood(pattern_lh);
	} else
		pattern_lh = ptn_lh;

	try {
		ofstream out;
		out.exceptions(ios::failbit | ios::badbit);
		if (append) {
			out.open(filename, ios::out | ios::app);
		} else {
			out.open(filename);
			out << 1 << " " << tree->getAlnNSite() << endl;
		}
		IntVector pattern_index;
		tree->aln->getSitePatternIndex(pattern_index);
		if (!linename)
			out << "Site_Lh   ";
		else {
			out.width(10);
			out << left << linename;
		}
		for (i = 0; i < tree->getAlnNSite(); i++)
			out << " " << pattern_lh[pattern_index[i]];
		out << endl;
		out.close();
		if (!append)
			cout << "Site log-likelihoods printed to " << filename << endl;
	} catch (ios::failure) {
		outError(ERR_WRITE_OUTPUT, filename);
	}

	if (!ptn_lh)
		delete[] pattern_lh;
}

void printPartitionLh(const char*filename, PhyloTree *tree, double *ptn_lh,
		bool append, const char *linename) {

    ASSERT(tree->isSuperTree());
    PhyloSuperTree *stree = (PhyloSuperTree*)tree;
	int i;
	double *pattern_lh;
	if (!ptn_lh) {
		pattern_lh = new double[tree->getAlnNPattern()];
		tree->computePatternLikelihood(pattern_lh);
	} else
		pattern_lh = ptn_lh;

    double partition_lh[stree->size()];
    int part;
    double *pattern_lh_ptr = pattern_lh;
    for (part = 0; part < stree->size(); part++) {
        size_t nptn = stree->at(part)->getAlnNPattern();
        partition_lh[part] = 0.0;
        for (i = 0; i < nptn; i++)
            partition_lh[part] += pattern_lh_ptr[i] * stree->at(part)->ptn_freq[i];
        pattern_lh_ptr += nptn;
    }

	try {
		ofstream out;
		out.exceptions(ios::failbit | ios::badbit);
		if (append) {
			out.open(filename, ios::out | ios::app);
		} else {
			out.open(filename);
			out << 1 << " " << stree->size() << endl;
		}
		if (!linename)
			out << "Part_Lh   ";
		else {
			out.width(10);
			out << left << linename;
		}
		for (i = 0; i < stree->size(); i++)
			out << " " << partition_lh[i];
		out << endl;
		out.close();
		if (!append)
			cout << "Partition log-likelihoods printed to " << filename << endl;
	} catch (ios::failure) {
		outError(ERR_WRITE_OUTPUT, filename);
	}

	if (!ptn_lh)
		delete[] pattern_lh;
}

void printSiteLhCategory(const char*filename, PhyloTree *tree, SiteLoglType wsl) {

    if (wsl == WSL_NONE || wsl == WSL_SITE)
        return;
	int ncat = tree->getNumLhCat(wsl);
    if (tree->isSuperTree()) {
        PhyloSuperTree *stree = (PhyloSuperTree*)tree;
        for (auto it = stree->begin(); it != stree->end(); it++) {
            int part_ncat = (*it)->getNumLhCat(wsl);
            if (part_ncat > ncat)
                ncat = part_ncat;
        }
    }
	int i;

    
	try {
		ofstream out;
		out.exceptions(ios::failbit | ios::badbit);
		out.open(filename);
        out << "# Site likelihood per rate/mixture category" << endl
            << "# This file can be read in MS Excel or in R with command:" << endl
            << "#   tab=read.table('" <<  filename << "',header=TRUE,fill=TRUE)" << endl
            << "# Columns are tab-separated with following meaning:" << endl;
        if (tree->isSuperTree()) {
            out << "#   Part:   Partition ID (1=" << ((PhyloSuperTree*)tree)->at(0)->aln->name << ", etc)" << endl
                << "#   Site:   Site ID within partition (starting from 1 for each partition)" << endl;
        } else
            out << "#   Site:   Alignment site ID" << endl;

        out << "#   LnL:    Logarithm of site likelihood" << endl
            << "#           Thus, sum of LnL is equal to tree log-likelihood" << endl
            << "#   LnLW_k: Logarithm of (category-k site likelihood times category-k weight)" << endl
            << "#           Thus, sum of exp(LnLW_k) is equal to exp(LnL)" << endl;

        if (tree->isSuperTree()) {
            out << "Part\tSite\tLnL";
        } else
            out << "Site\tLnL";
        for (i = 0; i < ncat; i++)
            out << "\tLnLW_" << i+1;
		out << endl;
        out.precision(4);
        out.setf(ios::fixed);

        tree->writeSiteLh(out, wsl);

		out.close();
		cout << "Site log-likelihoods per category printed to " << filename << endl;
        /*
        if (!tree->isSuperTree()) {
            cout << "Log-likelihood of constant sites: " << endl;
            double const_prob = 0.0;
            for (i = 0; i < tree->aln->getNPattern(); i++)
                if (tree->aln->at(i).isConst()) {
                    Pattern pat = tree->aln->at(i);
                    for (Pattern::iterator it = pat.begin(); it != pat.end(); it++)
                        cout << tree->aln->convertStateBackStr(*it);
                    cout << ": " << pattern_lh[i] << endl;
                    const_prob += exp(pattern_lh[i]);
                }
            cout << "Probability of const sites: " << const_prob << endl;
        }
        */
	} catch (ios::failure) {
		outError(ERR_WRITE_OUTPUT, filename);
	}

}

void printAncestralSequences(const char *out_prefix, PhyloTree *tree, AncestralSeqType ast) {

//    int *joint_ancestral = NULL;
//    
//    if (tree->params->print_ancestral_sequence == AST_JOINT) {
//        joint_ancestral = new int[nptn*tree->leafNum];    
//        tree->computeJointAncestralSequences(joint_ancestral);
//    }

    string filename = (string)out_prefix + ".state";
//    string filenameseq = (string)out_prefix + ".stateseq";

    try {
		ofstream out;
		out.exceptions(ios::failbit | ios::badbit);
		out.open(filename.c_str());
        out.setf(ios::fixed, ios::floatfield);
        out.precision(5);

//		ofstream outseq;
//		outseq.exceptions(ios::failbit | ios::badbit);
//		outseq.open(filenameseq.c_str());

        NodeVector nodes;
        tree->getInternalNodes(nodes);

        double *marginal_ancestral_prob;
        int *marginal_ancestral_seq;

//        if (tree->params->print_ancestral_sequence == AST_JOINT)
//            outseq << 2*(tree->nodeNum-tree->leafNum) << " " << nsites << endl;
//        else
//            outseq << (tree->nodeNum-tree->leafNum) << " " << nsites << endl;
//
//        int name_width = max(tree->aln->getMaxSeqNameLength(),6)+10;

        out << "# Ancestral state reconstruction for all nodes in " << tree->params->out_prefix << ".treefile" << endl
            << "# This file can be read in MS Excel or in R with command:" << endl
            << "#   tab=read.table('" <<  tree->params->out_prefix << ".state',header=TRUE)" << endl
            << "# Columns are tab-separated with following meaning:" << endl
            << "#   Node:  Node name in the tree" << endl;
        if (tree->isSuperTree()) {
            PhyloSuperTree *stree = (PhyloSuperTree*)tree;
            out << "#   Part:  Partition ID (1=" << stree->at(0)->aln->name << ", etc)" << endl
                << "#   Site:  Site ID within partition (starting from 1 for each partition)" << endl;
        } else
            out << "#   Site:  Alignment site ID" << endl;

        out << "#   State: Most likely state assignment" << endl
            << "#   p_X:   Posterior probability for state X (empirical Bayesian method)" << endl;

        if (tree->isSuperTree()) {
            PhyloSuperTree *stree = (PhyloSuperTree*)tree;
            out << "Node\tPart\tSite\tState";
            for (size_t i = 0; i < stree->front()->aln->num_states; i++)
                out << "\tp_" << stree->front()->aln->convertStateBackStr(i);
        } else {
            out << "Node\tSite\tState";
            for (size_t i = 0; i < tree->aln->num_states; i++)
                out << "\tp_" << tree->aln->convertStateBackStr(i);
        }
        out << endl;


        bool orig_kernel_nonrev;
        tree->initMarginalAncestralState(out, orig_kernel_nonrev, marginal_ancestral_prob, marginal_ancestral_seq);

        for (NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++) {
            PhyloNode *node = (PhyloNode*)(*it);
            PhyloNode *dad = (PhyloNode*)node->neighbors[0]->node;
            
            tree->computeMarginalAncestralState((PhyloNeighbor*)dad->findNeighbor(node), dad,
                marginal_ancestral_prob, marginal_ancestral_seq);
            
//            int *joint_ancestral_node = joint_ancestral + (node->id - tree->leafNum)*nptn;

            // set node name if neccessary
            if (node->name.empty() || !isalpha(node->name[0])) {
                node->name = "Node" + convertIntToString(node->id-tree->leafNum+1);
            }
            
            // print ancestral state probabilities
            tree->writeMarginalAncestralState(out, node, marginal_ancestral_prob, marginal_ancestral_seq);
            
            // print ancestral sequences
//            outseq.width(name_width);
//            outseq << left << node->name << " ";
//            for (i = 0; i < nsites; i++) 
//                outseq << tree->aln->convertStateBackStr(marginal_ancestral_seq[pattern_index[i]]);
//            outseq << endl;
//
//            if (tree->params->print_ancestral_sequence == AST_JOINT) {
//                outseq.width(name_width);
//                outseq << left << (node->name+"_joint") << " ";
//                for (i = 0; i < nsites; i++) 
//                    outseq << tree->aln->convertStateBackStr(joint_ancestral_node[pattern_index[i]]);
//                outseq << endl;
//            }
        }

        tree->endMarginalAncestralState(orig_kernel_nonrev, marginal_ancestral_prob, marginal_ancestral_seq);

		out.close();
//        outseq.close();
		cout << "Ancestral state probabilities printed to " << filename << endl;
//		cout << "Ancestral sequences printed to " << filenameseq << endl;

	} catch (ios::failure) {
		outError(ERR_WRITE_OUTPUT, filename);
	}
    
//    if (joint_ancestral)
//        delete[] joint_ancestral;

}

void printSiteProbCategory(const char*filename, PhyloTree *tree, SiteLoglType wsl) {

    if (wsl == WSL_NONE || wsl == WSL_SITE)
        return;
    // error checking
    if (!tree->getModel()->isMixture()) {
        if (wsl != WSL_RATECAT) {
            outWarning("Switch now to '-wspr' as it is the only option for non-mixture model");
            wsl = WSL_RATECAT;
        }
    } else {
        // mixture model
        if (wsl == WSL_MIXTURE_RATECAT && tree->getModelFactory()->fused_mix_rate) {
            outWarning("-wspmr is not suitable for fused mixture model, switch now to -wspm");
            wsl = WSL_MIXTURE;
        }
    }
	size_t cat, ncat = tree->getNumLhCat(wsl);
    double *ptn_prob_cat = new double[((size_t)tree->getAlnNPattern())*ncat];
	tree->computePatternProbabilityCategory(ptn_prob_cat, wsl);
    
	try {
		ofstream out;
		out.exceptions(ios::failbit | ios::badbit);
		out.open(filename);
        if (tree->isSuperTree())
            out << "Set\t";
        out << "Site";
        for (cat = 0; cat < ncat; cat++)
            out << "\tp" << cat+1;
		out << endl;
		IntVector pattern_index;
        if (tree->isSuperTree()) {
            PhyloSuperTree *super_tree = (PhyloSuperTree*)tree;
            size_t offset = 0;
            for (PhyloSuperTree::iterator it = super_tree->begin(); it != super_tree->end(); it++) {
                size_t part_ncat = (*it)->getNumLhCat(wsl); 
                (*it)->aln->getSitePatternIndex(pattern_index);
                size_t site, nsite = (*it)->aln->getNSite();
                for (site = 0; site < nsite; site++) {
                    out << (it-super_tree->begin())+1 << "\t" << site+1;
                    double *prob_cat = ptn_prob_cat + (offset+pattern_index[site]*part_ncat);
                    for (cat = 0; cat < part_ncat; cat++)
                        out << "\t" << prob_cat[cat];
                    out << endl;
                }
                offset += (*it)->aln->getNPattern()*(*it)->getNumLhCat(wsl);
            }
        } else {
            tree->aln->getSitePatternIndex(pattern_index);
            int nsite = tree->getAlnNSite();
            for (int site = 0; site < nsite; site++) {
                out << site+1;
                double *prob_cat = ptn_prob_cat + pattern_index[site]*ncat;
                for (cat = 0; cat < ncat; cat++) {
                    out << "\t" << prob_cat[cat];
                }
                out << endl;
            }
        }
		out.close();
		cout << "Site probabilities per category printed to " << filename << endl;
	} catch (ios::failure) {
		outError(ERR_WRITE_OUTPUT, filename);
	}

}


void printSiteStateFreq(const char*filename, PhyloTree *tree, double *state_freqs) {

    int i, j, nsites = tree->getAlnNSite(), nstates = tree->aln->num_states;
    double *ptn_state_freq;
    if (state_freqs) {
    	ptn_state_freq = state_freqs;
    } else {
    	ptn_state_freq = new double[((size_t)tree->getAlnNPattern()) * nstates];
        tree->computePatternStateFreq(ptn_state_freq);
    }

	try {
		ofstream out;
		out.exceptions(ios::failbit | ios::badbit);
		out.open(filename);
		IntVector pattern_index;
		tree->aln->getSitePatternIndex(pattern_index);
		for (i = 0; i < nsites; i++) {
			out.width(6);
			out << left << i+1 << " ";
            double *state_freq = &ptn_state_freq[pattern_index[i]*nstates];
			for (j = 0; j < nstates; j++) {
				out.width(15);
				out << state_freq[j] << " ";
			}
			out << endl;
		}
		out.close();
		cout << "Site state frequency vectors printed to " << filename << endl;
	} catch (ios::failure) {
		outError(ERR_WRITE_OUTPUT, filename);
	}
	if (!state_freqs)
        delete [] ptn_state_freq;
}

void printSiteStateFreq(const char* filename, Alignment *aln) {
    if (aln->site_state_freq.empty())
        return;
    int i, j, nsites = aln->getNSite(), nstates = aln->num_states;
	try {
		ofstream out;
		out.exceptions(ios::failbit | ios::badbit);
		out.open(filename);
		IntVector pattern_index;
		aln->getSitePatternIndex(pattern_index);
		for (i = 0; i < nsites; i++) {
			out.width(6);
			out << left << i+1 << " ";
            double *state_freq = aln->site_state_freq[pattern_index[i]];
			for (j = 0; j < nstates; j++) {
				out.width(15);
				out << state_freq[j] << " ";
			}
			out << endl;
		}
		out.close();
		cout << "Site state frequency vectors printed to " << filename << endl;
	} catch (ios::failure) {
		outError(ERR_WRITE_OUTPUT, filename);
	}
}

/*
bool checkModelFile(ifstream &in, bool is_partitioned, ModelCheckpoint &infos) {
	if (!in.is_open()) return false;
	in.exceptions(ios::badbit);
	string str;
	if (is_partitioned) {
		in >> str;
		if (str != "Charset")
			return false;
	}
	in >> str;
	if (str != "Model")
		return false;
	in >> str;
	if (str != "df")
		return false;
	in >> str;
	if (str != "LnL")
		return false;
	in >> str;
	if (str != "TreeLen") {
        outWarning(".model file was produced from a previous version of IQ-TREE");
		return false;
    }
	safeGetline(in, str);
	while (!in.eof()) {
		in >> str;
		if (in.eof())
			break;
		ModelInfo info;
		if (is_partitioned) {
			info.set_name = str;
			in >> str;
		}
		info.name = str;
		in >> info.df >> info.logl >> info.tree_len;
		safeGetline(in, str);
        info.tree = "";
        if (*str.rbegin() == ';') {
            size_t pos = str.rfind('\t');
            if (pos != string::npos)
                info.tree = str.substr(pos+1);
//            else 
//                outWarning(".model file was produced from a previous version of IQ-TREE");
        }
		infos.push_back(info);
		//cout << str << " " << df << " " << logl << endl;
	}
	in.clear();
	return true;
}
bool checkModelFile(string model_file, bool is_partitioned, ModelCheckpoint &infos) {
	if (!fileExists(model_file))
		return false;
	//cout << model_file << " exists, checking this file" << endl;
	ifstream in;
	try {
		in.exceptions(ios::failbit | ios::badbit);
		in.open(model_file.c_str());
		if (!checkModelFile(in, is_partitioned, infos))
			throw false;
		// set the failbit again
		in.exceptions(ios::failbit | ios::badbit);
		in.close();
	} catch (bool ret) {
		in.close();
		return ret;
	} catch (ios::failure) {
		outError("Cannot read file ", model_file);
	}
	return true;
}
*/

/**
 testing the best-fit model
 return in params.freq_type and params.rate_type
 @param set_name for partitioned analysis
 @param in_tree phylogenetic tree
 @param model_info (IN/OUT) information for all models considered
 @param set_name for partition model selection
 @param print_mem_usage true to print RAM memory used (default: false)
 @param merge_phase true to consider models for merging phase
 @return name of best-fit-model
 */
string testModel(Params &params, PhyloTree* in_tree, ModelCheckpoint &model_info,
        ModelsBlock *models_block, int num_threads, int brlen_type,
        string set_name = "", string in_model_name = "", ModelAdjust *adjust = NULL, bool merge_phase = false);

/**
 * select models for all partitions
 * @param[in,out] model_info (IN/OUT) all model information
 * @return total number of parameters
 */
void testPartitionModel(Params &params, PhyloSuperTree* in_tree, ModelCheckpoint &model_info,
                        ModelsBlock *models_block, int num_threads);


/**
 compute log-adapter function according to Whelan et al. 2015
 @param orig_aln original codon alignment
 @param newaln AA alignment
 @param[out] adjusted_df adjusted degree of freedom factor
 @return adjusted log-likelihood factor
 */
double computeAdapter(Alignment *orig_aln, Alignment *newaln, int &adjusted_df) {
    int aa, codon;

    // count codon occurences
    unsigned int codon_counts[orig_aln->num_states];
    orig_aln->computeAbsoluteStateFreq(codon_counts);
    
    // compute AA frequency
//    double aa_freq[newaln->num_states];
//    newaln->computeStateFreq(aa_freq);
    
    // compute codon frequency
    double codon_freq[orig_aln->num_states];
    //orig_aln->computeStateFreq(codon_freq);
    
    double sum = 0.0;
    for (codon = 0; codon < orig_aln->num_states; codon++)
        sum += codon_counts[codon];
    sum = 1.0/sum;
    for (codon = 0; codon < orig_aln->num_states; codon++)
        codon_freq[codon] = sum*codon_counts[codon];
    
    // new rescale codon_freq s.t. codons coding for the same AA
    // have f summing up to the frequency of this AA
    for (aa = 0; aa < newaln->num_states; aa++) {
        double sum = 0;
        for (codon = 0; codon < orig_aln->num_states; codon++)
            if (newaln->convertState(orig_aln->genetic_code[(int)orig_aln->codon_table[codon]]) == aa)
                sum += codon_freq[codon];
        sum = 1.0/sum;
        for (codon = 0; codon < orig_aln->num_states; codon++)
            if (newaln->convertState(orig_aln->genetic_code[(int)orig_aln->codon_table[codon]]) == aa)
                codon_freq[codon] *= sum;
    }
    
    // now compute adapter function
    double adapter = 0.0;
    adjusted_df = 0;
    vector<bool> has_AA;
    has_AA.resize(newaln->num_states, false);
    
    for (codon = 0; codon < orig_aln->num_states; codon++) {
        if (codon_counts[codon] == 0)
            continue;
        has_AA[newaln->convertState(orig_aln->genetic_code[(int)orig_aln->codon_table[codon]])] = true;
        adapter += codon_counts[codon]*log(codon_freq[codon]);
        adjusted_df++;
    }
    for (aa = 0; aa < has_AA.size(); aa++)
        if (has_AA[aa])
            adjusted_df--;
    return adapter;
}

/**
 wrapper function implementing ModelOMatic method of Whelan et al. (2015)
 */
string testModelOMatic(Params &params, PhyloTree* in_tree, ModelCheckpoint &model_info,
    ModelsBlock *models_block, int num_threads, int brlen_type,
    string set_name = "", string in_model_name = "", bool merge_phase = false)
{
    string model_name = testModel(params, in_tree, model_info, models_block, num_threads,
                                  brlen_type, set_name, in_model_name, NULL, merge_phase);
    if (!params.modelomatic || in_tree->aln->seq_type != SEQ_CODON)
        return model_name;

    Checkpoint *checkpoint = &model_info;
    
    string best_model_AIC, best_model_AICc, best_model_BIC;
    double best_score_AIC = -DBL_MAX, best_score_AICc = -DBL_MAX, best_score_BIC = -DBL_MAX;
    string best_tree_AIC, best_tree_AICc, best_tree_BIC;
    string best_model_list;
    
    bool check;
    check = CKP_RESTORE(best_model_AIC) &&
            CKP_RESTORE(best_model_AICc) &&
            CKP_RESTORE(best_model_BIC) &&
            CKP_RESTORE(best_score_AIC) &&
            CKP_RESTORE(best_score_AICc) &&
            CKP_RESTORE(best_score_BIC) &&
            CKP_RESTORE(best_tree_AIC) &&
            CKP_RESTORE(best_tree_AICc) &&
            CKP_RESTORE(best_tree_BIC) &&
            model_info.getBestModelList(best_model_list);
    ASSERT(check);
    
    ModelInfo best_model;
    best_model.name = model_name;
    check = best_model.restoreCheckpoint(&model_info);
    ASSERT(check);
    double score_best = best_model.computeICScore(in_tree->getAlnNSite());
    Alignment *orig_aln = in_tree->aln;

    // now convert Codon to AA
    SeqType test_seq_types[] = {SEQ_PROTEIN, SEQ_DNA};
    for (int id = 0; id < sizeof(test_seq_types) / sizeof(SeqType); id++) {
        Alignment *newaln = NULL;
        // adapter coefficient according to Whelan et al. 2015
        ModelAdjust adjust;
        adjust.logl = 0.0;
        adjust.df = 0;
        adjust.sample_size = orig_aln->getNSite();
        if (test_seq_types[id] == SEQ_PROTEIN) {
            newaln = orig_aln->convertCodonToAA();
            adjust.logl = computeAdapter(orig_aln, newaln, adjust.df);
            if (set_name.empty())
                cout << "Adapter function: " << adjust.logl << "  adjusted_df: " << adjust.df << endl;
        } else if (test_seq_types[id] == SEQ_DNA)
            newaln = orig_aln->convertCodonToDNA();
        else
            ASSERT(0 && "Unhandled SeqType");
        Alignment *prev_aln = in_tree->aln;
        in_tree->aln = newaln;
        ModelInfo info;
        info.name = testModel(params, in_tree, model_info, models_block, num_threads,
            brlen_type, set_name, in_model_name, &adjust, merge_phase);
        check = info.restoreCheckpoint(&model_info);
        ASSERT(check);
        // correct log-likelihood with adapter function
        info.computeICScores(adjust.sample_size);
        double score_new = info.computeICScore(adjust.sample_size);
        if (score_new < score_best) {
            score_best = score_new;
            model_name = info.name;
        } else {
            // revert if score is worse
            in_tree->aln = prev_aln;
            delete newaln;
        }
        if (info.AIC_score < best_score_AIC) {
            best_model_AIC = info.name;
            best_score_AIC = info.AIC_score;
            CKP_RESTORE(best_tree_AIC);
        }
        if (info.AICc_score < best_score_AICc) {
            best_model_AICc = info.name;
            best_score_AICc = info.AICc_score;
            CKP_RESTORE(best_tree_AICc);
        }
        
        if (info.BIC_score < best_score_BIC) {
            best_model_BIC = info.name;
            best_score_BIC = info.BIC_score;
            CKP_RESTORE(best_tree_BIC);
        }
        
        CKP_SAVE(best_tree_AIC);
        CKP_SAVE(best_tree_AICc);
        CKP_SAVE(best_tree_BIC);
        
        CKP_SAVE(best_model_AIC);
        CKP_SAVE(best_model_AICc);
        CKP_SAVE(best_model_BIC);
        
        CKP_SAVE(best_score_AIC);
        CKP_SAVE(best_score_AICc);
        CKP_SAVE(best_score_BIC);
        
        // merge best_model_list
        string model_list_new;
        model_info.getBestModelList(model_list_new);
        best_model_list = best_model_list + " " + model_list_new;
        model_info.putBestModelList(best_model_list);
        model_info.dump();
    }
    if (orig_aln != in_tree->aln) {
        delete orig_aln;
    }
    return model_name;
}

/**
 compute fast ML tree by stepwise addition MP + ML-NNI
 @return the tree string
 */
string computeFastMLTree(Params &params, Alignment *aln,
                       ModelCheckpoint &model_info, ModelsBlock *models_block,
                       int &num_threads, int brlen_type) {
    //string model_name;
    CandidateModel usual_model(aln);
    StrVector model_names;
    convert_string_vec(usual_model.name.c_str(), model_names);
    //set<string> model_set;

    string concat_tree;
    
    IQTree *iqtree = NULL;
    if (aln->isSuperAlignment()) {
        SuperAlignment *saln = (SuperAlignment*)aln;
        if (params.partition_type == BRLEN_OPTIMIZE)
            iqtree = new PhyloSuperTree(saln);
        else
            iqtree = new PhyloSuperTreePlen(saln, brlen_type);
        for (int part = 0; part != model_names.size(); part++)
            saln->partitions[part]->model_name = model_names[part];
    } else if (posRateHeterotachy(model_names[0]) != string::npos)
        iqtree = new PhyloTreeMixlen(aln, 0);
    else
        iqtree = new IQTree(aln);

    if ((params.start_tree == STT_PLL_PARSIMONY || params.start_tree == STT_RANDOM_TREE || params.pll) && !iqtree->isInitializedPLL()) {
        /* Initialized all data structure for PLL*/
        iqtree->initializePLL(params);
    }

    iqtree->setParams(&params);
    iqtree->setLikelihoodKernel(params.SSE);
    iqtree->optimize_by_newton = params.optimize_by_newton;
    iqtree->setNumThreads(num_threads);
    iqtree->setCheckpoint(&model_info);

    iqtree->computeInitialTree(params.SSE);
    iqtree->restoreCheckpoint();
    
    ASSERT(iqtree->root);
    iqtree->initializeModel(params, usual_model.name, models_block);
    if (!iqtree->getModel()->isMixture() || aln->seq_type == SEQ_POMO)
        usual_model.name = iqtree->getModelName();

    iqtree->getModelFactory()->restoreCheckpoint();
    
#ifdef _OPENMP
    if (num_threads <= 0) {
        num_threads = iqtree->testNumThreads();
        omp_set_num_threads(num_threads);
    } else
        iqtree->warnNumThreads();
#endif
    
    iqtree->initializeAllPartialLh();
    double saved_modelEps = params.modelEps;
    params.modelEps = params.modelfinder_eps;
    string initTree;
    
    double start_time = getRealTime();
    
    cout << "Perform fast likelihood tree search search using " << model_names[0] << " model..." << endl;
    
    if (iqtree->getCheckpoint()->getBool("finishedFastMLTree")) {
        // model optimization already done: ignore this step
        iqtree->setCurScore(iqtree->computeLikelihood());
        initTree = iqtree->getTreeString();
        cout << "CHECKPOINT: Tree restored, LogL: " << iqtree->getCurScore() << endl;
    } else {
        bool saved_opt_gammai = params.opt_gammai;
        // disable thorough I+G optimization
        params.opt_gammai = false;
        initTree = iqtree->optimizeModelParameters(false, params.modelEps*50.0);
        if (iqtree->isMixlen())
            initTree = ((ModelFactoryMixlen*)iqtree->getModelFactory())->sortClassesByTreeLength();

        // do quick NNI search
        if (params.start_tree != STT_USER_TREE) {
            cout << "Perform nearest neighbor interchange..." << endl;
            iqtree->doNNISearch(true);
            initTree = iqtree->getTreeString();
        }
        params.opt_gammai = saved_opt_gammai;

        iqtree->saveCheckpoint();
        iqtree->getModelFactory()->saveCheckpoint();
        iqtree->getCheckpoint()->putBool("finishedFastMLTree", true);
        iqtree->getCheckpoint()->dump();
        //        cout << "initTree: " << initTree << endl;
        cout << "Time for fast ML tree search: " << getRealTime() - start_time << " seconds" << endl;
        cout << endl;
    }

    // restore model epsilon
    params.modelEps = saved_modelEps;
    
    // save information to the checkpoint for later retrieval
    if (iqtree->isSuperTree()) {
        PhyloSuperTree *stree = (PhyloSuperTree*)iqtree;
        int part = 0;
        for (auto it = stree->begin(); it != stree->end(); it++, part++) {
            model_info.startStruct((*it)->aln->name);
            (*it)->saveCheckpoint();
            (*it)->getModelFactory()->saveCheckpoint();
            model_info.endStruct();
        }
    } else {
        iqtree->saveCheckpoint();
        iqtree->getModelFactory()->saveCheckpoint();
    }


    delete iqtree;
    return initTree;
}

/**
 Transfer parameters from ModelFinder into the a checkpoint to speed up later stage
 */
void transferModelFinderParameters(IQTree *iqtree, Checkpoint *target) {
    
    Checkpoint *source = iqtree->getCheckpoint();
    
    // transfer the substitution model and site-rate parameters
    if (iqtree->isSuperTree()) {
        DoubleVector tree_lens;
        string struct_name;
        if (iqtree->params->partition_type == BRLEN_SCALE || iqtree->params->partition_type == BRLEN_FIX)
            struct_name = "PartitionModelPlen";
        else
            struct_name = "PartitionModel";
        target->startStruct(struct_name);
        SuperAlignment *super_aln = (SuperAlignment*)iqtree->aln;
        for (auto aln : super_aln->partitions) {
            source->transferSubCheckpoint(target, aln->name + CKP_SEP + "Model");
            source->transferSubCheckpoint(target, aln->name + CKP_SEP + "Rate");

            // transfer partition rates
            if (iqtree->params->partition_type == BRLEN_SCALE) {
                source->startStruct(aln->name);
                ModelInfo info;
                info.name = aln->model_name;
                if (info.restoreCheckpoint(source))
                    tree_lens.push_back(info.tree_len);
                else
                    ASSERT(0 && "Could not restore tree_len");
                source->endStruct();
            }
        }

        if (iqtree->params->partition_type == BRLEN_SCALE) {
            // now normalize the rates
            PhyloSuperTree *tree = (PhyloSuperTree*)iqtree;
            double sum = 0.0;
            size_t nsite = 0;
            int i;
            for (i = 0; i < tree->size(); i++) {
                sum += tree_lens[i] * tree->at(i)->aln->getNSite();
                if (tree->at(i)->aln->seq_type == SEQ_CODON && tree->rescale_codon_brlen)
                    nsite += 3*tree->at(i)->aln->getNSite();
                else
                    nsite += tree->at(i)->aln->getNSite();
            }

            sum /= nsite;
            iqtree->restoreCheckpoint();
            iqtree->scaleLength(sum/iqtree->treeLength());
            iqtree->saveCheckpoint();
            sum = 1.0/sum;
            for (i = 0; i < tree->size(); i++)
                tree_lens[i] *= sum;
            target->putVector("part_rates", tree_lens);            
        }
        target->endStruct();
    } else {
        source->transferSubCheckpoint(target, "Model");
        source->transferSubCheckpoint(target, "Rate");
    }
    
    // transfer tree
    source->transferSubCheckpoint(target, "PhyloTree");
}

void runModelFinder(Params &params, IQTree &iqtree, ModelCheckpoint &model_info)
{
    //    iqtree.setCurScore(-DBL_MAX);
    bool test_only = (params.model_name.find("ONLY") != string::npos) ||
        (params.model_name.substr(0,2) == "MF" && params.model_name.substr(0,3) != "MFP");
    
    bool empty_model_found = params.model_name.empty() && !iqtree.isSuperTree();
    
    if (params.model_name.empty() && iqtree.isSuperTree()) {
        // check whether any partition has empty model_name
        PhyloSuperTree *stree = (PhyloSuperTree*)&iqtree;
        for (auto i = stree->begin(); i != stree->end(); i++)
            if ((*i)->aln->model_name.empty()) {
                empty_model_found = true;
                break;
            }
    }
    
    if (params.model_joint)
        empty_model_found = false;

    // Model already specifed, nothing to do here
    if (!empty_model_found && params.model_name.substr(0, 4) != "TEST" && params.model_name.substr(0, 2) != "MF")
        return;
    if (MPIHelper::getInstance().getNumProcesses() > 1)
        outError("Please use only 1 MPI process! We are currently working on the MPI parallelization of model selection.");
    // TODO: check if necessary
    //        if (iqtree.isSuperTree())
    //            ((PhyloSuperTree*) &iqtree)->mapTrees();
    double cpu_time = getCPUTime();
    double real_time = getRealTime();
    model_info.setFileName((string)params.out_prefix + ".model.gz");
    model_info.setDumpInterval(params.checkpoint_dump_interval);
    
    bool ok_model_file = false;
    if (!params.model_test_again) {
        ok_model_file = model_info.load();
    }
    
    cout << endl;
    
    ok_model_file &= model_info.size() > 0;
    if (ok_model_file)
        cout << "NOTE: Restoring information from model checkpoint file " << model_info.getFileName() << endl;
    
    
    Checkpoint *orig_checkpoint = iqtree.getCheckpoint();
    iqtree.setCheckpoint(&model_info);
    iqtree.restoreCheckpoint();
    
    int partition_type;
    if (CKP_RESTORE2((&model_info), partition_type)) {
        if (partition_type != params.partition_type)
            outError("Mismatch partition type between checkpoint and partition file command option\nRerun with -mredo to ignore .model.gz checkpoint file");
    } else {
        partition_type = params.partition_type;
        CKP_SAVE2((&model_info), partition_type);
    }
    
    ModelsBlock *models_block = readModelsDefinition(params);
    
    // compute initial tree
    if (params.modelfinder_ml_tree) {
        // 2019-09-10: Now perform NNI on the initial tree
        string tree_str = computeFastMLTree(params, iqtree.aln, model_info, models_block, params.num_threads, params.partition_type);
        iqtree.restoreCheckpoint();
    } else {
        iqtree.computeInitialTree(params.SSE);
    
        if (iqtree.isSuperTree()) {
            PhyloSuperTree *stree = (PhyloSuperTree*)&iqtree;
            int part = 0;
            for (auto it = stree->begin(); it != stree->end(); it++, part++) {
                model_info.startStruct((*it)->aln->name);
                (*it)->saveCheckpoint();
                model_info.endStruct();
            }
        } else {
            iqtree.saveCheckpoint();
        }
    }
    
    // also save initial tree to the original .ckp.gz checkpoint
    //        string initTree = iqtree.getTreeString();
    //        CKP_SAVE(initTree);
    //        iqtree.saveCheckpoint();
    //        checkpoint->dump(true);
    
    CandidateModelSet candidate_models;
    int max_cats = candidate_models.generate(params, iqtree.aln, params.model_test_separate_rate, false);
    
    uint64_t mem_size = iqtree.getMemoryRequiredThreaded(max_cats);
    cout << "NOTE: ModelFinder requires " << (mem_size / 1024) / 1024 << " MB RAM!" << endl;
    if (mem_size >= getMemorySize()) {
        outError("Memory required exceeds your computer RAM size!");
    }
#ifdef BINARY32
    if (mem_size >= 2000000000) {
        outError("Memory required exceeds 2GB limit of 32-bit executable");
    }
#endif
    

    if (iqtree.isSuperTree()) {
        // partition model selection
        PhyloSuperTree *stree = (PhyloSuperTree*)&iqtree;
        testPartitionModel(params, stree, model_info, models_block, params.num_threads);
        stree->mapTrees();
        string res_models = "";
        for (auto it = stree->begin(); it != stree->end(); it++) {
            if (it != stree->begin()) res_models += ",";
            res_models += (*it)->aln->model_name;
        }
        iqtree.aln->model_name = res_models;
    } else {
        // single model selection
        iqtree.aln->model_name = testModelOMatic(params, &iqtree, model_info, models_block, params.num_threads, BRLEN_OPTIMIZE);
        
        Checkpoint *checkpoint = &model_info;
        string best_model_AIC, best_model_AICc, best_model_BIC;
        CKP_RESTORE(best_model_AIC);
        CKP_RESTORE(best_model_AICc);
        CKP_RESTORE(best_model_BIC);
        cout << "Akaike Information Criterion:           " << best_model_AIC << endl;
        cout << "Corrected Akaike Information Criterion: " << best_model_AICc << endl;
        cout << "Bayesian Information Criterion:         " << best_model_BIC << endl;
        cout << "Best-fit model: " << iqtree.aln->model_name << " chosen according to "
            << criterionName(params.model_test_criterion) << endl;
    }

    delete models_block;
    
    // force to dump all checkpointing information
    model_info.dump(true);
    
    // transfer models parameters
    transferModelFinderParameters(&iqtree, orig_checkpoint);
    iqtree.setCheckpoint(orig_checkpoint);
    
    params.startCPUTime = cpu_time;
    params.start_real_time = real_time;
    cpu_time = getCPUTime() - cpu_time;
    real_time = getRealTime() - real_time;
    cout << endl;
    cout << "All model information printed to " << model_info.getFileName() << endl;
    cout << "CPU time for ModelFinder: " << cpu_time << " seconds (" << convert_time(cpu_time) << ")" << endl;
    cout << "Wall-clock time for ModelFinder: " << real_time << " seconds (" << convert_time(real_time) << ")" << endl;
    
    //        alignment = iqtree.aln;
    if (test_only) {
        params.min_iterations = 0;
    }
}

/**
 * get the list of substitution models
 */
void getModelSubst(SeqType seq_type, bool standard_code, string model_name,
                   string model_set, char *model_subset, StrVector &model_names) {
    int i, j;
    
    if (model_set == "1") {
        model_names.push_back(getUsualModelSubst(seq_type));
        return;
    }
    
    if (seq_type == SEQ_BINARY) {
        if (model_set.empty()) {
            copyCString(bin_model_names, sizeof(bin_model_names) / sizeof(char*), model_names);
        } else if (model_set[0] == '+') {
            // append model_set into existing models
            convert_string_vec(model_set.c_str()+1, model_names);
            appendCString(bin_model_names, sizeof(bin_model_names) / sizeof(char*), model_names);
        } else {
            convert_string_vec(model_set.c_str(), model_names);
        }
    } else if (seq_type == SEQ_MORPH) {
        if (model_set.empty()) {
            copyCString(morph_model_names, sizeof(morph_model_names) / sizeof(char*), model_names);
        } else if (model_set[0] == '+') {
            // append model_set into existing models
            convert_string_vec(model_set.c_str()+1, model_names);
            appendCString(morph_model_names, sizeof(morph_model_names) / sizeof(char*), model_names);
        } else {
            convert_string_vec(model_set.c_str(), model_names);
        }
    } else if (seq_type == SEQ_DNA || seq_type == SEQ_POMO) {
        if (model_set.empty()) {
            copyCString(dna_model_names, sizeof(dna_model_names) / sizeof(char*), model_names);
            //            copyCString(dna_freq_names, sizeof(dna_freq_names)/sizeof(char*), freq_names);
        } else if (model_set == "partitionfinder" || model_set== "phyml") {
            copyCString(dna_model_names_old, sizeof(dna_model_names_old) / sizeof(char*), model_names);
            //            copyCString(dna_freq_names, sizeof(dna_freq_names)/sizeof(char*), freq_names);
        } else if (model_set == "raxml") {
            copyCString(dna_model_names_rax, sizeof(dna_model_names_rax) / sizeof(char*), model_names);
            //            copyCString(dna_freq_names, sizeof(dna_freq_names)/sizeof(char*), freq_names);
        } else if (model_set == "mrbayes") {
            copyCString(dna_model_names_mrbayes, sizeof(dna_model_names_mrbayes) / sizeof(char*), model_names);
            //            copyCString(dna_freq_names, sizeof(dna_freq_names)/sizeof(char*), freq_names);
        } else if (model_set == "modelomatic") {
            copyCString(dna_model_names_modelomatic, sizeof(dna_model_names_modelomatic) / sizeof(char*), model_names);
        } else if (model_set == "liemarkov") {
            copyCString(dna_model_names_lie_markov_fullsym, sizeof(dna_model_names_lie_markov_fullsym) / sizeof(char*), model_names);
            appendCString(dna_model_names_lie_markov_ry, sizeof(dna_model_names_lie_markov_ry) / sizeof(char*), model_names);
            appendCString(dna_model_names_lie_markov_ws, sizeof(dna_model_names_lie_markov_ws) / sizeof(char*), model_names);
            appendCString(dna_model_names_lie_markov_mk, sizeof(dna_model_names_lie_markov_mk) / sizeof(char*), model_names);
        } else if (model_set == "liemarkovry") {
            copyCString(dna_model_names_lie_markov_fullsym, sizeof(dna_model_names_lie_markov_fullsym) / sizeof(char*), model_names);
            appendCString(dna_model_names_lie_markov_ry, sizeof(dna_model_names_lie_markov_ry) / sizeof(char*), model_names);
        } else if (model_set == "liemarkovws") {
            copyCString(dna_model_names_lie_markov_fullsym, sizeof(dna_model_names_lie_markov_fullsym) / sizeof(char*), model_names);
            appendCString(dna_model_names_lie_markov_ws, sizeof(dna_model_names_lie_markov_ws) / sizeof(char*), model_names);
        } else if (model_set == "liemarkovmk") {
            copyCString(dna_model_names_lie_markov_fullsym, sizeof(dna_model_names_lie_markov_fullsym) / sizeof(char*), model_names);
            appendCString(dna_model_names_lie_markov_mk, sizeof(dna_model_names_lie_markov_mk) / sizeof(char*), model_names);
        } else if (model_set == "strandsymmetric") {
            copyCString(dna_model_names_lie_markov_strsym, sizeof(dna_model_names_lie_markov_strsym) / sizeof(char*), model_names);
            // IMPORTANT NOTE: If you add any more -mset names for sets of Lie Markov models,
            // you also need to change getPrototypeModel function.
        } else if (model_set[0] == '+') {
            // append model_set into existing models
            convert_string_vec(model_set.c_str()+1, model_names);
            appendCString(dna_model_names, sizeof(dna_model_names) / sizeof(char*), model_names);
        } else {
            convert_string_vec(model_set.c_str(), model_names);
        }
        
        if (model_name.find("+LMRY") != string::npos) {
            appendCString(dna_model_names_lie_markov_fullsym, sizeof(dna_model_names_lie_markov_fullsym) / sizeof(char*), model_names);
            appendCString(dna_model_names_lie_markov_ry, sizeof(dna_model_names_lie_markov_ry) / sizeof(char*), model_names);
        } else if (model_name.find("+LMWS") != string::npos) {
            appendCString(dna_model_names_lie_markov_fullsym, sizeof(dna_model_names_lie_markov_fullsym) / sizeof(char*), model_names);
            appendCString(dna_model_names_lie_markov_ws, sizeof(dna_model_names_lie_markov_ws) / sizeof(char*), model_names);
        } else if (model_name.find("+LMMK") != string::npos) {
            appendCString(dna_model_names_lie_markov_fullsym, sizeof(dna_model_names_lie_markov_fullsym) / sizeof(char*), model_names);
            appendCString(dna_model_names_lie_markov_mk, sizeof(dna_model_names_lie_markov_mk) / sizeof(char*), model_names);
        } else if (model_name.find("+LMSS") != string::npos) {
            appendCString(dna_model_names_lie_markov_strsym, sizeof(dna_model_names_lie_markov_strsym) / sizeof(char*), model_names);
        } else if (model_name.find("+LM") != string::npos) {
            appendCString(dna_model_names_lie_markov_fullsym, sizeof(dna_model_names_lie_markov_fullsym) / sizeof(char*), model_names);
            appendCString(dna_model_names_lie_markov_ry, sizeof(dna_model_names_lie_markov_ry) / sizeof(char*), model_names);
            appendCString(dna_model_names_lie_markov_ws, sizeof(dna_model_names_lie_markov_ws) / sizeof(char*), model_names);
            appendCString(dna_model_names_lie_markov_mk, sizeof(dna_model_names_lie_markov_mk) / sizeof(char*), model_names);
        }
    } else if (seq_type == SEQ_PROTEIN) {
        if (model_set.empty()) {
            copyCString(aa_model_names, sizeof(aa_model_names) / sizeof(char*), model_names);
        } else if (model_set == "partitionfinder" || model_set == "phyml") {
            copyCString(aa_model_names_phyml, sizeof(aa_model_names_phyml) / sizeof(char*), model_names);
        } else if (model_set == "raxml") {
            copyCString(aa_model_names_rax, sizeof(aa_model_names_rax) / sizeof(char*), model_names);
        } else if (model_set == "mrbayes") {
            copyCString(aa_model_names_mrbayes, sizeof(aa_model_names_mrbayes) / sizeof(char*), model_names);
        } else if (model_set == "modelomatic") {
            copyCString(aa_model_names_modelomatic, sizeof(aa_model_names_modelomatic) / sizeof(char*), model_names);
        } else if (model_set[0] == '+') {
            // append model_set into existing models
            convert_string_vec(model_set.c_str()+1, model_names);
            appendCString(aa_model_names, sizeof(aa_model_names) / sizeof(char*), model_names);
        } else {
            convert_string_vec(model_set.c_str(), model_names);
        }
        
        if (model_subset) {
            StrVector submodel_names;
            if (strncmp(model_subset, "nuclear", 3) == 0) {
                copyCString(aa_model_names_nuclear, sizeof(aa_model_names_nuclear) / sizeof(char*), submodel_names);
            } else if (strncmp(model_subset, "mitochondrial", 3) == 0) {
                copyCString(aa_model_names_mitochondrial, sizeof(aa_model_names_mitochondrial) / sizeof(char*), submodel_names);
            } else if (strncmp(model_subset, "chloroplast", 3) == 0) {
                copyCString(aa_model_names_chloroplast, sizeof(aa_model_names_chloroplast) / sizeof(char*), submodel_names);
            } else if (strncmp(model_subset, "viral",3) == 0) {
                copyCString(aa_model_names_viral, sizeof(aa_model_names_viral) / sizeof(char*), submodel_names);
            } else {
                outError("Wrong -msub option");
            }
            for (i = 0; i < model_names.size(); i++) {
                bool appear = false;
                for (j = 0; j < submodel_names.size(); j++)
                    if (model_names[i] == submodel_names[j]) {
                        appear = true;
                        break;
                    }
                if (!appear) {
                    model_names.erase(model_names.begin()+i);
                    i--;
                }
            }
        }
        
    } else if (seq_type == SEQ_CODON) {
        if (model_set.empty()) {
            if (standard_code)
                copyCString(codon_model_names, sizeof(codon_model_names) / sizeof(char*), model_names);
            else {
                i = sizeof(codon_model_names) / sizeof(char*);
                for (j = 0; j < i; j++)
                    if (!std_genetic_code[j])
                        model_names.push_back(codon_model_names[j]);
                //                copyCString(codon_model_names, sizeof(codon_model_names) / sizeof(char*) - 1, model_names);
            }
        } else if (model_set == "modelomatic") {
            copyCString(codon_model_names_modelomatic, sizeof(codon_model_names_modelomatic) / sizeof(char*), model_names);
        } else if (model_set[0] == '+') {
            // append model_set into existing models
            convert_string_vec(model_set.c_str()+1, model_names);
            if (standard_code)
                appendCString(codon_model_names, sizeof(codon_model_names) / sizeof(char*), model_names);
            else {
                i = sizeof(codon_model_names) / sizeof(char*);
                for (j = 0; j < i; j++)
                    if (!std_genetic_code[j])
                        model_names.push_back(codon_model_names[j]);
            }
        } else
            convert_string_vec(model_set.c_str(), model_names);
    }
}

void getStateFreqs(SeqType seq_type, char *state_freq_set, StrVector &freq_names) {
    int j;
    
    switch (seq_type) {
        case SEQ_PROTEIN:
            copyCString(aa_freq_names, sizeof(aa_freq_names)/sizeof(char*), freq_names);
            break;
        case SEQ_CODON:
            copyCString(codon_freq_names, sizeof(codon_freq_names) / sizeof(char*), freq_names);
            break;
        default:
            break;
    }
    if (state_freq_set)
        convert_string_vec(state_freq_set, freq_names);
    for (j = 0; j < freq_names.size(); j++) {
        std::transform(freq_names[j].begin(), freq_names[j].end(), freq_names[j].begin(), ::toupper);
        if (freq_names[j] != "" && freq_names[j][0] != '+')
            freq_names[j] = "+" + freq_names[j];
    }
}

/**
 get list of rate heterogeneity
 */
void getRateHet(SeqType seq_type, string model_name, double frac_invariant_sites,
                string rate_set, StrVector &ratehet) {
    const char *rate_options[]    = {  "", "+I", "+ASC", "+G", "+I+G", "+ASC+G", "+R", "+ASC+R"};
    bool test_options_default[]   = {true,   true, false,  true,  true,   false, false,  false};
    bool test_options_fast[]      = {false, false, false, false,  true,   false, false,  false};
    bool test_options_morph[]     = {true,  false,  true,  true, false,    true, false,  false};
    bool test_options_morph_fast[]= {false, false, false, false, false,    true, false,  false};
    bool test_options_noASC_I[]   = {true,  false, false,  true, false,   false, false,  false};
    bool test_options_noASC_I_fast[]={false,false, false,  true, false,   false, false,  false};
    bool test_options_asc[]       ={false,  false,  true, false, false,    true, false,  false};
    bool test_options_new[]       = {true,   true, false,  true,  true,   false,  true,  false};
    bool test_options_morph_new[] = {true,  false,  true,  true, false,    true,  true,   true};
    bool test_options_noASC_I_new[]= {true, false, false,  true, false,   false,  true,  false};
    bool test_options_asc_new[]   = {false, false,  true, false, false,    true, false,   true};
    bool test_options_pomo[]      = {true,  false, false,  true, false,   false, false,  false};
    bool test_options_norate[]    = {true,  false, false, false, false,   false, false,  false};
    bool *test_options = test_options_default;
    //    bool test_options_codon[] =  {true,false,  false,false,  false,    false};
    const int noptions = sizeof(rate_options) / sizeof(char*);
    int i, j;
    
    bool with_new = (model_name.find("NEW") != string::npos || model_name.substr(0,2) == "MF" || model_name.empty());
    bool with_asc = model_name.find("ASC") != string::npos;

    if (seq_type == SEQ_POMO) {
        for (i = 0; i < noptions; i++)
            test_options[i] = test_options_pomo[i];
    }
    // If not PoMo, go on with normal treatment.
    else if (frac_invariant_sites == 0.0) {
        // morphological or SNP data: activate +ASC
        if (with_new && rate_set != "1") {
            if (with_asc)
                test_options = test_options_asc_new;
            else if (seq_type == SEQ_DNA || seq_type == SEQ_BINARY || seq_type == SEQ_MORPH)
                test_options = test_options_morph_new;
            else
                test_options = test_options_noASC_I_new;
        } else if (with_asc)
            test_options = test_options_asc;
        else if (seq_type == SEQ_DNA || seq_type == SEQ_BINARY || seq_type == SEQ_MORPH) {
            if (rate_set == "1")
                test_options = test_options_morph_fast;
            else
                test_options = test_options_morph;
        } else {
            if (rate_set == "1")
                test_options = test_options_noASC_I_fast;
            else
                test_options = test_options_noASC_I;
        }
    } else if (frac_invariant_sites >= 1.0) {
        // 2018-06-12: alignment with only invariant sites, no rate variation added
        test_options = test_options_norate;
    } else {
        // normal data, use +I instead
        if (with_new && rate_set != "1") {
            // change +I+G to +R
            if (with_asc)
                test_options = test_options_asc_new;
            else
                test_options = test_options_new;
        } else if (with_asc) {
            test_options = test_options_asc;
        } else if (rate_set == "1")
            test_options = test_options_fast;
        else
            test_options = test_options_default;
        if (frac_invariant_sites == 0.0) {
            // deactivate +I
            for (j = 0; j < noptions; j++)
                if (strstr(rate_options[j], "+I"))
                    test_options[j] = false;
        }
    }
    if (!rate_set.empty() && rate_set != "1") {
        // take the rate_options from user-specified models
        convert_string_vec(rate_set.c_str(), ratehet);
        if (!ratehet.empty() && ratehet[0] == "default") {
            ratehet.erase(ratehet.begin());
            StrVector ratedef;
            for (j = 0; j < noptions; j++)
                if (test_options[j])
                    ratedef.push_back(rate_options[j]);
            ratehet.insert(ratehet.begin(), ratedef.begin(), ratedef.end());
        }
        for (j = 0; j < ratehet.size(); j++) {
            if (ratehet[j] != "" && ratehet[j][0] != '+' && ratehet[j][0] != '*')
                ratehet[j] = "+" + ratehet[j];
            if (ratehet[j] == "+E") // for equal rate model
                ratehet[j] = "";
        }
    } else {
        for (j = 0; j < noptions; j++)
            if (test_options[j])
                ratehet.push_back(rate_options[j]);
        
    }
}

int CandidateModelSet::generate(Params &params, Alignment *aln, bool separate_rate, bool merge_phase) {
	StrVector model_names;
    StrVector freq_names;
	SeqType seq_type = aln->seq_type;
    
	int i, j;
    string model_set;
    
    if (merge_phase) {
        model_set = params.merge_models;
        if (iEquals(model_set, "all")) {
            model_set = (params.model_set) ? params.model_set : "";
        }
    } else if (params.model_set)
        model_set = params.model_set;

    getModelSubst(seq_type, aln->isStandardGeneticCode(), params.model_name,
                  model_set, params.model_subset, model_names);

	if (model_names.empty()) 
        return 1;
    
    getStateFreqs(seq_type, params.state_freq_set, freq_names);
    
    // combine model_names with freq_names
    if (freq_names.size() > 0) {
        StrVector orig_model_names = model_names;
        model_names.clear();
        for (j = 0; j < orig_model_names.size(); j++) {
            if (aln->seq_type == SEQ_CODON) {
                SeqType seq_type;
                int model_type = detectSeqType(orig_model_names[j].c_str(), seq_type);
                for (i = 0; i < freq_names.size(); i++) {
                    // disallow MG+F
                    if (freq_names[i] == "+F" && orig_model_names[j].find("MG") != string::npos)
                        continue;
                    if (freq_names[i] != "" || (model_type == 2 && orig_model_names[j].find("MG") == string::npos)) 
                        // empirical model also allow ""
                        model_names.push_back(orig_model_names[j] + freq_names[i]);
                }
            } else {
                for (i = 0; i < freq_names.size(); i++)
                    model_names.push_back(orig_model_names[j] + freq_names[i]);
            }
        }
    }


    

    StrVector ratehet;
    int max_cats = params.num_rate_cats;
    string ratehet_set;
    if (merge_phase) {
        ratehet_set = params.merge_rates;
        if (iEquals(ratehet_set, "all"))
            ratehet_set = (params.ratehet_set) ? params.ratehet_set : "";
    } else if (params.ratehet_set)
        ratehet_set = params.ratehet_set;

    getRateHet(seq_type, params.model_name, aln->frac_invariant_sites, ratehet_set, ratehet);

    // add number of rate cateogories for special rate models
    const char *rates[] = {"+R", "*R", "+H", "*H"};

    for (i = 0; i < sizeof(rates)/sizeof(char*); i++)
        if (params.model_name.find(rates[i]) != string::npos)
            ratehet.push_back(rates[i]);

    size_t pos;

    for (i = 0; i < sizeof(rates)/sizeof(char*); i++)
    for (j = 0; j < ratehet.size(); j++)
        if ((pos = ratehet[j].find(rates[i])) != string::npos &&
            (pos >= ratehet[j].length()-2 || !isdigit(ratehet[j][pos+2]) ))
        {
            string str = ratehet[j];
            ratehet[j].insert(pos+2, convertIntToString(params.min_rate_cats));
            max_cats = max(max_cats, params.max_rate_cats);
            for (int k = params.min_rate_cats+1; k <= params.max_rate_cats; k++) {
                ratehet.insert(ratehet.begin()+j+k-params.min_rate_cats, str.substr(0, pos+2) + convertIntToString(k) + str.substr(pos+2));
            }
        }

    string pomo_suffix = (seq_type == SEQ_POMO) ? "+P" : "";
    // TODO DS: should we allow virtual population size?

    // combine substitution models with rate heterogeneity
    if (separate_rate) {
        for (i = 0; i < model_names.size(); i++) 
            push_back(CandidateModel(model_names[i]));
        for (j = 0; j < ratehet.size(); j++)
            if (ratehet[j] != "")
                push_back(CandidateModel(ratehet[j] + pomo_suffix));
    } else {
        for (i = 0; i < model_names.size(); i++)
            for (j = 0; j < ratehet.size(); j++) {
                push_back(CandidateModel(model_names[i] + ratehet[j] + pomo_suffix));
            }
    }
    if (params.model_extra_set) {
        StrVector extra_model_names;
        convert_string_vec(params.model_extra_set, extra_model_names);
        for (auto s : extra_model_names)
            push_back(CandidateModel(s));
    }
    return max_cats;
}

void replaceModelInfo(string &set_name, ModelCheckpoint &model_info, ModelCheckpoint &new_info) {
    for (auto it = new_info.begin(); it != new_info.end(); it++) {
        model_info.put(set_name + CKP_SEP + it->first, it->second);
    }
}

void extractModelInfo(string &orig_set_name, ModelCheckpoint &model_info, ModelCheckpoint &part_model_info) {
    string set_name = orig_set_name + CKP_SEP;
    int len = set_name.length();
    for (auto it = model_info.lower_bound(set_name); it != model_info.end() && it->first.substr(0, len) == set_name; it++) {
        part_model_info.put(it->first.substr(len), it->second);
    }
}

string getSubsetName(PhyloSuperTree *super_tree, set<int> &subset) {
    string set_name;
    for (auto it = subset.begin(); it != subset.end(); it++) {
        if (it != subset.begin())
            set_name += "+";
        set_name += super_tree->at(*it)->aln->name;
    }
    return set_name;
}

int getSubsetAlnLength(PhyloSuperTree *super_tree, set<int> &subset) {
    int len = 0;
    for (auto i : subset) {
        len += super_tree->at(i)->aln->getNSite();
    }
    return len;
}

/**
 * transfer model parameters from two subsets to the target subsets
 */
void transferModelParameters(PhyloSuperTree *super_tree, ModelCheckpoint &model_info, ModelCheckpoint &part_model_info,
                             set<int> &gene_set1, set<int> &gene_set2)
{
    set<int> merged_set;
    merged_set.insert(gene_set1.begin(), gene_set1.end());
    merged_set.insert(gene_set2.begin(), gene_set2.end());
    string set_name = getSubsetName(super_tree, merged_set);
    string set1_name = getSubsetName(super_tree, gene_set1);
    string set2_name = getSubsetName(super_tree, gene_set2);
    double weight1 = getSubsetAlnLength(super_tree, gene_set1);
    double weight2 = getSubsetAlnLength(super_tree, gene_set2);
    double weight_sum = weight1 + weight2;
    weight1 = weight1/weight_sum;
    weight2 = weight2/weight_sum;
    enum MeanComp {GEOM_MEAN, ARIT_MEAN};
    enum ValType {VAL_SINGLE, VAL_VECTOR};
    vector<tuple<ValType, MeanComp,string> > info_strings = {
        make_tuple(VAL_SINGLE, ARIT_MEAN, (string)"RateGamma" + CKP_SEP + "gamma_shape"),
        make_tuple(VAL_SINGLE, ARIT_MEAN, (string)"RateGammaInvar" + CKP_SEP + "gamma_shape"),
        make_tuple(VAL_SINGLE, ARIT_MEAN, (string)"RateGammaInvar" + CKP_SEP + "p_invar"),
        make_tuple(VAL_SINGLE, ARIT_MEAN, (string)"RateInvar" + CKP_SEP + "p_invar")
        //make_tuple(VAL_VECTOR, GEOM_MEAN, (string)"ModelDNA" + CKP_SEP + "rates")
    };
    for (auto info : info_strings) {
        switch (std::get<0>(info)) {
            case VAL_SINGLE: {
                double value1, value2, value;
                bool ok1 = model_info.get(set1_name + CKP_SEP + std::get<2>(info), value1);
                bool ok2 = model_info.get(set2_name + CKP_SEP + std::get<2>(info), value2);
                if (!ok1 || !ok2)
                    continue;
                if (part_model_info.get(std::get<2>(info), value))
                    continue; // value already exist
                switch (std::get<1>(info)) {
                    case ARIT_MEAN:
                        value = weight1*value1 + weight2*value2;
                        break;
                    case GEOM_MEAN:
                        value = sqrt(value1*value2);
                        break;
                }
                part_model_info.put(std::get<2>(info), value);
                break;
            }
            case VAL_VECTOR: {
                DoubleVector value1, value2, value;
                bool ok1 = model_info.getVector(set1_name + CKP_SEP + std::get<2>(info), value1);
                bool ok2 = model_info.getVector(set2_name + CKP_SEP + std::get<2>(info), value2);
                if (!ok1 || !ok2)
                    continue;
                ASSERT(value1.size() == value2.size());
                if (part_model_info.getVector(std::get<2>(info), value))
                    continue; // value already exist
                value.reserve(value1.size());
                for (int i = 0; i < value1.size(); i++)
                switch (std::get<1>(info)) {
                    case ARIT_MEAN:
                        value.push_back(weight1*value1[i] + weight2*value2[i]);
                        break;
                    case GEOM_MEAN:
                        value.push_back(sqrt(value1[i]*value2[i]));
                        break;
                }
                part_model_info.putVector(std::get<2>(info), value);
                break;
            }
        }
    }
}

void mergePartitions(PhyloSuperTree* super_tree, vector<set<int> > &gene_sets, StrVector &model_names) {
	cout << "Merging into " << gene_sets.size() << " partitions..." << endl;
	vector<set<int> >::iterator it;
	SuperAlignment *super_aln = (SuperAlignment*)super_tree->aln;
	vector<PartitionInfo> part_info;
	vector<PhyloTree*> tree_vec;
    SuperAlignment *new_super_aln = new SuperAlignment();
	for (it = gene_sets.begin(); it != gene_sets.end(); it++) {
        Alignment *aln = super_aln->concatenateAlignments(*it);
		PartitionInfo info;
		aln->model_name = model_names[it-gene_sets.begin()];
        info.part_rate = 1.0; // BIG FIX: make -spp works with -m TESTMERGE now!
        info.evalNNIs = 0;
		for (set<int>::iterator i = it->begin(); i != it->end(); i++) {
			if (i != it->begin()) {
				aln->name += "+";
                if (!super_aln->partitions[*i]->position_spec.empty())
                    aln->position_spec += ", ";
			}
			aln->name += super_aln->partitions[*i]->name;
			aln->position_spec += super_aln->partitions[*i]->position_spec;
			if (!super_aln->partitions[*i]->aln_file.empty()) {
                if (aln->aln_file.empty())
                    aln->aln_file = super_aln->partitions[*i]->aln_file;
                else if (aln->aln_file != super_aln->partitions[*i]->aln_file) {
                    aln->aln_file = aln->aln_file + ',' + super_aln->partitions[*i]->aln_file;
                }
			}
			if (!super_aln->partitions[*i]->sequence_type.empty()) {
                if (aln->sequence_type.empty())
                    aln->sequence_type = super_aln->partitions[*i]->sequence_type;
                else if (aln->sequence_type != super_aln->partitions[*i]->sequence_type) {
                    aln->sequence_type = "__NA__";
                }
			}
		}
		info.cur_ptnlh = NULL;
		info.nniMoves[0].ptnlh = NULL;
		info.nniMoves[1].ptnlh = NULL;
		part_info.push_back(info);
		PhyloTree *tree = super_tree->extractSubtree(*it);
        tree->setParams(super_tree->params);
		tree->setAlignment(aln);
		tree_vec.push_back(tree);
        new_super_aln->partitions.push_back(aln);
	}

    // BUG FIX 2016-11-29: when merging partitions with -m TESTMERGE, sequence order is changed
    // get the taxa names from existing tree
    StrVector seq_names;
    if (super_tree->root) {
        super_tree->getTaxaName(seq_names);
    }
    new_super_aln->init(&seq_names);
    
	for (PhyloSuperTree::reverse_iterator tit = super_tree->rbegin(); tit != super_tree->rend(); tit++)
		delete (*tit);
	super_tree->clear();
	super_tree->insert(super_tree->end(), tree_vec.begin(), tree_vec.end());
	super_tree->part_info = part_info;

	delete super_tree->aln;
//    super_tree->aln = new SuperAlignment(super_tree);
    super_tree->setAlignment(new_super_aln);
}

/**
 called when some partition is changed
 */
void fixPartitions(PhyloSuperTree* super_tree) {
    SuperAlignment *super_aln = (SuperAlignment*)super_tree->aln;
    int part;
    bool aln_changed = false;
    for (part = 0; part < super_tree->size(); part++)
        if (super_aln->partitions[part] != super_tree->at(part)->aln) {
            aln_changed = true;
            super_aln->partitions[part] = super_tree->at(part)->aln;
        }
    if (!aln_changed)
        return;
    super_aln->buildPattern();
    super_aln->orderPatternByNumChars(PAT_VARIANT);
    super_tree->deleteAllPartialLh();
}

string CandidateModel::evaluate(Params &params, Alignment *in_aln,
    ModelCheckpoint &in_model_info, ModelCheckpoint &out_model_info,
    ModelInfo &info, ModelsBlock *models_block,
    int &num_threads, int brlen_type, ModelAdjust *adjust)
{
    //string model_name = name;
    IQTree *iqtree = NULL;
    if (in_aln->isSuperAlignment()) {
        SuperAlignment *saln = (SuperAlignment*)in_aln;
        if (params.partition_type == BRLEN_OPTIMIZE)
            iqtree = new PhyloSuperTree(saln);
        else
            iqtree = new PhyloSuperTreePlen(saln, brlen_type);
        StrVector model_names;
        convert_string_vec(name.c_str(), model_names);
        for (int part = 0; part != model_names.size(); part++)
            saln->partitions[part]->model_name = model_names[part];
    } else if (posRateHeterotachy(name) != string::npos)
        iqtree = new PhyloTreeMixlen(in_aln, 0);
    else
        iqtree = new IQTree(in_aln);
    iqtree->setParams(&params);
    iqtree->setLikelihoodKernel(params.SSE);
    iqtree->optimize_by_newton = params.optimize_by_newton;
    iqtree->setNumThreads(num_threads);

    iqtree->setCheckpoint(&in_model_info);
    iqtree->restoreCheckpoint();
    ASSERT(iqtree->root);
    iqtree->initializeModel(params, name, models_block);
    if (!iqtree->getModel()->isMixture() || in_aln->seq_type == SEQ_POMO)
        name = iqtree->getModelName();

    info.name = name;

    if (info.restoreCheckpoint(&in_model_info)) {
        delete iqtree;
        return "";
    }

    iqtree->getModelFactory()->restoreCheckpoint();
    
    // now switch to the output checkpoint
    iqtree->getModelFactory()->setCheckpoint(&out_model_info);
    iqtree->setCheckpoint(&out_model_info);
    
    if (params.model_test_and_tree) {
        //--- PERFORM FULL TREE SEARCH PER MODEL ----//

        // BQM 2017-03-29: disable bootstrap
        int orig_num_bootstrap_samples = params.num_bootstrap_samples;
        int orig_gbo_replicates = params.gbo_replicates;
        params.num_bootstrap_samples = 0;
        params.gbo_replicates = 0;
        STOP_CONDITION orig_stop_condition = params.stop_condition;
        if (params.stop_condition == SC_BOOTSTRAP_CORRELATION)
            params.stop_condition = SC_UNSUCCESS_ITERATION;

        iqtree->aln->model_name = name;
        
        cout << endl << "===> Testing model " << name << endl;

        if (iqtree->root) {
            // start from previous tree
            string initTree = iqtree->getTreeString();
            iqtree->getCheckpoint()->put("initTree", initTree);
            iqtree->saveCheckpoint();
        }

#ifdef _OPENMP
        if (num_threads <= 0) {
            num_threads = iqtree->testNumThreads();
            omp_set_num_threads(num_threads);
        } else
            iqtree->warnNumThreads();
#endif

        runTreeReconstruction(params, iqtree);
        info.logl = iqtree->computeLikelihood();
        info.tree_len = iqtree->treeLength();
        info.tree = iqtree->getTreeString();

        // restore original parameters
        // 2017-03-29: restore bootstrap replicates
        params.num_bootstrap_samples = orig_num_bootstrap_samples;
        params.gbo_replicates = orig_gbo_replicates;
        params.stop_condition = orig_stop_condition;

        int count = iqtree->getCheckpoint()->eraseKeyPrefix("finished");
        cout << count << " finished checkpoint entries erased" << endl;
        iqtree->getCheckpoint()->eraseKeyPrefix("CandidateSet");

    } else {
        //--- FIX TREE TOPOLOGY AND ESTIMATE MODEL PARAMETERS ----//

        if (verbose_mode >= VB_MED)
            cout << "Optimizing model " << info.name << endl;

        #ifdef _OPENMP
        if (num_threads <= 0) {
            num_threads = iqtree->testNumThreads();
            omp_set_num_threads(num_threads);
        } else
            iqtree->warnNumThreads();
        #endif

        iqtree->initializeAllPartialLh();

        for (int step = 0; step < 2; step++) {
            info.logl = iqtree->getModelFactory()->optimizeParameters(brlen_type, false,
                params.modelfinder_eps, TOL_GRADIENT_MODELTEST);
            info.tree_len = iqtree->treeLength();
            iqtree->getModelFactory()->saveCheckpoint();
            iqtree->saveCheckpoint();

            // check if logl(+R[k]) is worse than logl(+R[k-1])
            ModelInfo prev_info;
            if (!prev_info.restoreCheckpointRminus1(&in_model_info, info.name)) break;
            if (prev_info.logl < info.logl + params.modelfinder_eps) break;
            if (step == 0) {
                iqtree->getRate()->initFromCatMinusOne();
            } else if (info.logl < prev_info.logl - params.modelfinder_eps) {
                outWarning("Log-likelihood " + convertDoubleToString(info.logl) + " of " +
                           info.name + " worse than " + prev_info.name + " " + convertDoubleToString(prev_info.logl));
            }
        }

    }

    info.df = iqtree->getModelFactory()->getNParameters(brlen_type);
    if (adjust) {
        info.logl += adjust->logl;
        info.df += adjust->df;
    }
    string tree_string = iqtree->getTreeString();

    delete iqtree;
    return tree_string;
}

/** model information by merging two partitions */
struct ModelPair {
    /** score after merging */
    double score;
    /** ID of partition 1 */
    int part1;
    /** ID of partition 2 */
    int part2;
    /** log-likelihood */
    double logl;
    /** degree of freedom */
    int df;
    /** tree length */
    double tree_len;
    /** IDs of merged partitions */
    set<int> merged_set;
    /** set name */
    string set_name;
    /* best model name */
    string model_name;
};

class ModelPairSet : public multimap<double, ModelPair> {

public:

    /** insert a partition pair */
    void insertPair(ModelPair &pair) {
        insert(value_type(pair.score, pair));
    }

    /** 
        find the maximum compatible partition pairs
        @param num max number of pairs to return
    */
    void getCompatiblePairs(int num, ModelPairSet &res) {
        set<int> part_ids;

        for (auto it = begin(); it != end() && res.size() < num; it++) {

            // check for compatibility
            vector<int> overlap;
            set_intersection(part_ids.begin(), part_ids.end(),
                it->second.merged_set.begin(), it->second.merged_set.end(),
                std::back_inserter(overlap));

            if (!overlap.empty()) continue;

            // take the union
            part_ids.insert(it->second.merged_set.begin(), it->second.merged_set.end());

            // put the compatible pair to the set
            res.insertPair(it->second);
        }
    }

};

string testConcatModel(Params &params, SuperAlignment *super_aln, ModelCheckpoint &model_info,
    ModelsBlock *models_block, int num_threads, ModelInfo &concat_info)
{
    Alignment *conaln = super_aln->concatenateAlignments();
    CandidateModel usual_model;
    size_t ssize = usual_model.getUsualModel(conaln);

    string concat_tree;

    cout << "Testing " << usual_model.name << " on supermatrix..." << endl;
    concat_tree = usual_model.evaluate(params, conaln, model_info, model_info, concat_info,
        models_block, num_threads, BRLEN_OPTIMIZE);

    concat_info.computeICScores(ssize);
    concat_info.saveCheckpoint(&model_info);

    delete conaln;
    return concat_tree;
}

/**
 * k-means clustering of partitions using partition-specific tree length
 * @return score (AIC/BIC/etc.) of the clustering
 * @param[out] gene_sets
 * @param[out[ model_names
 */
double doKmeansClustering(Params &params, PhyloSuperTree *in_tree,
    int ncluster, DoubleVector &lenvec,
    ModelCheckpoint &model_info, ModelsBlock *models_block,
    int num_threads,
    vector<set<int> > &gene_sets, StrVector &model_names)
{
    
    cout << "k-means merging into " << ncluster << " partitions..." << endl;
    
    ASSERT(lenvec.size() == in_tree->size());
    int npart = in_tree->size();
    IntVector weights;
    weights.resize(npart, 1);
    int *clusters = new int[npart];
    double *centers = new double[ncluster];
    RunKMeans1D(npart, ncluster, lenvec.data(), weights.data(), centers, clusters);
    
    SuperAlignment *super_aln = ((SuperAlignment*)in_tree->aln);

    double lhsum = 0.0;
    int dfsum = 0;
    if (params.partition_type == BRLEN_FIX || params.partition_type == BRLEN_SCALE) {
        dfsum = in_tree->getNBranchParameters(BRLEN_OPTIMIZE);
        if (params.partition_type == BRLEN_SCALE)
            dfsum -= 1;
    }

    for (int cluster = 0; cluster < ncluster; cluster++) {
        string set_name;
        set<int> merged_set;
        for (int i = 0; i < in_tree->size(); i++)
            if (clusters[i] == cluster) {
                if (!set_name.empty())
                    set_name += "+";
                set_name += in_tree->at(i)->aln->name;
                merged_set.insert(i);
            }
        gene_sets.push_back(merged_set);
        ModelInfo best_model;
        bool done_before = false;
        {
            // if pairs previously examined, reuse the information
            model_info.startStruct(set_name);
            if (model_info.getBestModel(best_model.name)) {
                best_model.restoreCheckpoint(&model_info);
                done_before = true;
            }
            model_info.endStruct();
        }
        ModelCheckpoint part_model_info;
        if (!done_before) {
            Alignment *aln = super_aln->concatenateAlignments(merged_set);
            PhyloTree *tree = in_tree->extractSubtree(merged_set);
            tree->setAlignment(aln);
            extractModelInfo(set_name, model_info, part_model_info);
            tree->num_precision = in_tree->num_precision;
            tree->setParams(&params);
            tree->sse = params.SSE;
            tree->optimize_by_newton = params.optimize_by_newton;
            tree->num_threads = params.model_test_and_tree ? num_threads : 1;
            /*if (params.model_test_and_tree) {
             tree->setCheckpoint(new Checkpoint());
             tree->saveCheckpoint();
             } else*/
            {
            tree->setCheckpoint(&part_model_info);
            // trick to restore checkpoint
            tree->restoreCheckpoint();
            tree->saveCheckpoint();
            }
            best_model.name = testModelOMatic(params, tree, part_model_info, models_block,
                params.model_test_and_tree ? num_threads : 1, params.partition_type,
                set_name, "", true);
            best_model.restoreCheckpoint(&part_model_info);
            model_names.push_back(best_model.name);
            delete tree;
            delete aln;
        }
        lhsum += best_model.logl;
        dfsum += best_model.df;
        {
            if (!done_before) {
                replaceModelInfo(set_name, model_info, part_model_info);
                model_info.dump();
                cout.width(4);
                cout << right << cluster+1 << " ";
                cout.width(12);
                cout << left << best_model.name << " ";
                cout.width(11);
                cout << best_model.logl << " " << set_name;
                cout << endl;
            }
        }
    }
    
    int ssize = in_tree->getAlnNSite();
    double score = computeInformationScore(lhsum, dfsum, ssize, params.model_test_criterion);
    cout << "k-means score for " << ncluster << " partitions: " << score << " (LnL: " << lhsum << "  " << "df: " << dfsum << ")" << endl;

    delete [] centers;
    delete [] clusters;
    return score;
}

class SubsetPair : public pair<int,int> {
public:
    // distance between two partition pairs
    double distance;
};

bool comparePairs(const SubsetPair &a, const SubsetPair &b) {
    return a.distance < b.distance;
}

bool comparePartition(const pair<int,double> &a, const pair<int, double> &b) {
    return a.second > b.second;
}

/**
 find k-closest partition pairs for rcluster algorithm
 */
void findClosestPairs(SuperAlignment *super_aln, DoubleVector &lenvec, vector<set<int> > &gene_sets,
                      double log_transform, vector<SubsetPair> &closest_pairs) {
    
    for (int part1 = 0; part1 < lenvec.size()-1; part1++)
        for (int part2 = part1+1; part2 < lenvec.size(); part2++)
            if (super_aln->partitions[*gene_sets[part1].begin()]->seq_type == super_aln->partitions[*gene_sets[part2].begin()]->seq_type &&
                super_aln->partitions[*gene_sets[part1].begin()]->genetic_code == super_aln->partitions[*gene_sets[part2].begin()]->genetic_code) {
                // only merge partitions of the same data type
                SubsetPair pair;
                pair.first = part1;
                pair.second = part2;
                if (log_transform)
                    pair.distance = fabs(log(lenvec[part1]) - log(lenvec[part2]));
                else
                    pair.distance = fabs(lenvec[part1] - lenvec[part2]);
                closest_pairs.push_back(pair);
            }
    if (!closest_pairs.empty() && Params::getInstance().partfinder_rcluster < 100) {
        // sort distance
        std::sort(closest_pairs.begin(), closest_pairs.end(), comparePairs);
        size_t num_pairs = round(closest_pairs.size() * (Params::getInstance().partfinder_rcluster/100.0));
        num_pairs = min(num_pairs, Params::getInstance().partfinder_rcluster_max);
        if (num_pairs <= 0) num_pairs = 1;
        closest_pairs.erase(closest_pairs.begin() + num_pairs, closest_pairs.end());
    }
}

/**
 merge vector src into dest, eliminating duplicates
 */
void mergePairs(vector<SubsetPair> &dest, vector<SubsetPair> &src) {
    unordered_set<string> dest_set;
    for (SubsetPair s: dest)
        dest_set.insert(convertIntToString(s.first) + "-" + convertIntToString(s.second));
    for (SubsetPair s: src)
        if (dest_set.find(convertIntToString(s.first) + "-" + convertIntToString(s.second)) == dest_set.end())
            dest.push_back(s);
}

/**
 * select models for all partitions
 * @param[in,out] model_info (IN/OUT) all model information
 * @return total number of parameters
 */
void testPartitionModel(Params &params, PhyloSuperTree* in_tree, ModelCheckpoint &model_info,
    ModelsBlock *models_block, int num_threads)
{
//    params.print_partition_info = true;
//    params.print_conaln = true;
	int i = 0;
//	PhyloSuperTree::iterator it;
	DoubleVector lhvec; // log-likelihood for each partition
	DoubleVector dfvec; // number of parameters for each partition
    DoubleVector lenvec; // tree length for each partition
	double lhsum = 0.0;
	int dfsum = 0;
    if (params.partition_type == BRLEN_FIX || params.partition_type == BRLEN_SCALE) {
        dfsum = in_tree->getNBranchParameters(BRLEN_OPTIMIZE);
        if (params.partition_type == BRLEN_SCALE)
            dfsum -= 1;
    }
	int ssize = in_tree->getAlnNSite();
	int64_t num_model = 0;
    int64_t total_num_model = in_tree->size();

    // 2017-06-07: -rcluster-max for max absolute number of pairs
    if (params.partfinder_rcluster_max == 0)
        params.partfinder_rcluster_max = max((size_t)1000, 10*in_tree->size());

	if (params.partition_merge != MERGE_NONE) {
        double p = params.partfinder_rcluster/100.0;
        size_t num_pairs = round(in_tree->size()*(in_tree->size()-1)*p/2);
        if (p < 1.0)
            num_pairs = min(num_pairs, params.partfinder_rcluster_max);
        total_num_model += num_pairs;
        for (i = in_tree->size()-2; i > 0; i--)
            total_num_model += max(round(i*p), 1.0);
    }


#ifdef _OPENMP
    if (num_threads <= 0) {
        // partition selection scales well with many cores
        num_threads = min((int64_t)countPhysicalCPUCores(), total_num_model);
        num_threads = min(num_threads, params.num_threads_max);
        omp_set_num_threads(num_threads);
        cout << "NUMBER OF THREADS FOR PARTITION FINDING: " << num_threads << endl;
    }
#endif

    double start_time = getRealTime();

	SuperAlignment *super_aln = ((SuperAlignment*)in_tree->aln);

    string concat_tree;
    ModelInfo concat_info;

    // Analysis on supermatrix
    if (false)
    {
        concat_tree = testConcatModel(params, super_aln, model_info, models_block, num_threads, concat_info);

        // read tree with branch lengths for linked partition model
        if (params.partition_type != BRLEN_OPTIMIZE && !concat_tree.empty()) {
            in_tree->readTreeString(concat_tree);
            int part = 0;
            for (auto it = in_tree->begin(); it != in_tree->end(); it++, part++) {
                model_info.startStruct(in_tree->at(part)->aln->name);
                (*it)->saveCheckpoint();
                model_info.endStruct();
            }
        }
        model_info.dump();

        cout << concat_info.name << " / LnL: " << concat_info.logl
             << " / df: " << concat_info.df << " / AIC: " << concat_info.AIC_score
             << " / AICc: " << concat_info.AICc_score << " / BIC: " << concat_info.BIC_score << endl;
    }

	cout << "Selecting individual models for " << in_tree->size() << " charsets using " << criterionName(params.model_test_criterion) << "..." << endl;
	//cout << " No. AIC         AICc        BIC         Charset" << endl;
	cout << " No. Model        Score       TreeLen     Charset" << endl;

	lhvec.resize(in_tree->size());
	dfvec.resize(in_tree->size());
	lenvec.resize(in_tree->size());

    // sort partition by computational cost for OpenMP effciency
    vector<pair<int,double> > partitionID;
    
	for (i = 0; i < in_tree->size(); i++) {
        Alignment *this_aln = in_tree->at(i)->aln;
        // computation cost is proportional to #sequences, #patterns, and #states
        partitionID.push_back({i, ((double)this_aln->getNSeq())*this_aln->getNPattern()*this_aln->num_states});
    }
    
    if (num_threads > 1) {
        std::sort(partitionID.begin(), partitionID.end(), comparePartition);
    }
    
    bool parallel_over_partitions = false;
    int brlen_type = params.partition_type;
    if (brlen_type == TOPO_UNLINKED)
        brlen_type = BRLEN_OPTIMIZE;

    bool test_merge = (params.partition_merge != MERGE_NONE) && params.partition_type != TOPO_UNLINKED && (in_tree->size() > 1);
    
#ifdef _OPENMP
    parallel_over_partitions = !params.model_test_and_tree && (in_tree->size() >= num_threads);
#pragma omp parallel for private(i) schedule(dynamic) reduction(+: lhsum, dfsum) if(parallel_over_partitions)
#endif
	for (int j = 0; j < in_tree->size(); j++) {
        i = partitionID[j].first;
        PhyloTree *this_tree = in_tree->at(i);
		// scan through models for this partition, assuming the information occurs consecutively
		ModelCheckpoint part_model_info;
		extractModelInfo(this_tree->aln->name, model_info, part_model_info);
		// do the computation
        string part_model_name;
        if (params.model_name.empty())
            part_model_name = this_tree->aln->model_name;
        ModelInfo best_model;
		best_model.name = testModelOMatic(params, this_tree, part_model_info, models_block,
            (parallel_over_partitions ? 1 : num_threads), brlen_type, this_tree->aln->name, part_model_name, test_merge);

        bool check = (best_model.restoreCheckpoint(&part_model_info));
        ASSERT(check);

		double score = best_model.computeICScore(this_tree->getAlnNSite());
		this_tree->aln->model_name = best_model.name;
		lhsum += (lhvec[i] = best_model.logl);
		dfsum += (dfvec[i] = best_model.df);
        lenvec[i] = best_model.tree_len;

#ifdef _OPENMP
#pragma omp critical
#endif
        {
            num_model++;
            cout.width(4);
            cout << right << num_model << " ";
            cout.width(12);
            cout << left << best_model.name << " ";
            cout.width(11);
            cout << score << " ";
            cout.width(11);
            cout << best_model.tree_len << " ";
            cout << this_tree->aln->name;
            if (num_model >= 10) {
                double remain_time = (total_num_model-num_model)*(getRealTime()-start_time)/num_model;
                cout << "\t" << convert_time(getRealTime()-start_time) << " (" 
                    << convert_time(remain_time) << " left)";
            }
            cout << endl;
            replaceModelInfo(this_tree->aln->name, model_info, part_model_info);
            model_info.dump();
        }
    }

    // in case ModelOMatic change the alignment
    fixPartitions(in_tree);
    
	double inf_score = computeInformationScore(lhsum, dfsum, ssize, params.model_test_criterion);
	cout << "Full partition model " << criterionName(params.model_test_criterion)
         << " score: " << inf_score << " (LnL: " << lhsum << "  df:" << dfsum << ")" << endl;

	if (!test_merge) {
		super_aln->printBestPartition((string(params.out_prefix) + ".best_scheme.nex").c_str());
		super_aln->printBestPartitionRaxml((string(params.out_prefix) + ".best_scheme").c_str());
        model_info.dump();
        if (params.partition_type != TOPO_UNLINKED && inf_score > concat_info.computeICScore(ssize) + 1.0) {
            cout << endl;
            outWarning("Partition model has worse fit than single model!");
            outWarning("Add MERGE to -m option to increase model fit!");
        }
		return;
	}

    vector<set<int> > gene_sets;
    StrVector model_names;
    StrVector greedy_model_trees;

    gene_sets.resize(in_tree->size());
    model_names.resize(in_tree->size());
    greedy_model_trees.resize(in_tree->size());
    for (i = 0; i < gene_sets.size(); i++) {
        gene_sets[i].insert(i);
        model_names[i] = in_tree->at(i)->aln->model_name;
        greedy_model_trees[i] = in_tree->at(i)->aln->name;
    }

    if (params.partition_merge == MERGE_KMEANS) {
        // kmeans cluster based on parition tree length
        double cur_score = inf_score;
        for (int ncluster = in_tree->size()-1; ncluster >= 1; ncluster--) {
            vector<set<int> > this_gene_sets;
            StrVector this_model_names;
            //double sum = in_tree->size()/std::accumulate(lenvec.begin(), lenvec.end(), 0.0);
            double score = doKmeansClustering(params, in_tree, ncluster, lenvec, model_info,
                models_block, num_threads, this_gene_sets, this_model_names);
            if (score < cur_score) {
                cout << "Better score found: " << score << endl;
                cur_score = score;
                gene_sets = this_gene_sets;
                model_names = this_model_names;
            } else {
                //break;
            }
        }
    } else {
        cout << "Merging models to increase model fit (about " << total_num_model << " total partition schemes)..." << endl;
    }

    /* following implements the greedy algorithm of Lanfear et al. (2012) */
	while (params.partition_merge != MERGE_KMEANS && gene_sets.size() >= 2) {
		// stepwise merging charsets

        // list of all better pairs of partitions than current partitioning scheme
        ModelPairSet better_pairs;

        // 2015-06-24: begin rcluster algorithm
        // compute distance between gene_sets
        ASSERT(gene_sets.size() == lenvec.size());
        // find closest partition pairs
        vector<SubsetPair> closest_pairs;
        findClosestPairs(super_aln, lenvec, gene_sets, false, closest_pairs);
        if (params.partfinder_log_rate) {
            // additional consider pairs by log-rate
            vector<SubsetPair> log_closest_pairs;
            findClosestPairs(super_aln, lenvec, gene_sets, true, log_closest_pairs);
            mergePairs(closest_pairs, log_closest_pairs);
        }
        // sort partition by computational cost for OpenMP effciency
        for (i = 0; i < closest_pairs.size(); i++) {
            // computation cost is proportional to #sequences, #patterns, and #states
            Alignment *this_aln = in_tree->at(closest_pairs[i].first)->aln;
            closest_pairs[i].distance = -((double)this_aln->getNSeq())*this_aln->getNPattern()*this_aln->num_states;
            this_aln = in_tree->at(closest_pairs[i].second)->aln;
            closest_pairs[i].distance -= ((double)this_aln->getNSeq())*this_aln->getNPattern()*this_aln->num_states;
        }
        if (num_threads > 1)
            std::sort(closest_pairs.begin(), closest_pairs.end(), comparePairs);

        size_t num_pairs = closest_pairs.size();
        
#ifdef _OPENMP
#pragma omp parallel for private(i) schedule(dynamic) if(!params.model_test_and_tree)
#endif
        for (size_t pair = 0; pair < num_pairs; pair++) {
            // information of current partitions pair
            ModelPair cur_pair;
            cur_pair.part1 = closest_pairs[pair].first;
            cur_pair.part2 = closest_pairs[pair].second;
            ASSERT(cur_pair.part1 < cur_pair.part2);
            cur_pair.merged_set.insert(gene_sets[cur_pair.part1].begin(), gene_sets[cur_pair.part1].end());
            cur_pair.merged_set.insert(gene_sets[cur_pair.part2].begin(), gene_sets[cur_pair.part2].end());
            cur_pair.set_name = getSubsetName(in_tree, cur_pair.merged_set);
            double weight1 = getSubsetAlnLength(in_tree, gene_sets[cur_pair.part1]);
            double weight2 = getSubsetAlnLength(in_tree, gene_sets[cur_pair.part2]);
            double sum = 1.0 / (weight1 + weight2);
            weight1 *= sum;
            weight2 *= sum;
            ModelInfo best_model;
            bool done_before = false;
#ifdef _OPENMP
#pragma omp critical
#endif
            {
                // if pairs previously examined, reuse the information
                model_info.startStruct(cur_pair.set_name);
                if (model_info.getBestModel(best_model.name)) {
                    best_model.restoreCheckpoint(&model_info);
                    done_before = true;
                }
                model_info.endStruct();
            }
            ModelCheckpoint part_model_info;
            double cur_tree_len = 0.0;
            if (!done_before) {
                Alignment *aln = super_aln->concatenateAlignments(cur_pair.merged_set);
                PhyloTree *tree = in_tree->extractSubtree(cur_pair.merged_set);
                //tree->scaleLength((weight1*lenvec[cur_pair.part1] + weight2*lenvec[cur_pair.part2])/tree->treeLength());
                tree->scaleLength(sqrt(lenvec[cur_pair.part1]*lenvec[cur_pair.part2])/tree->treeLength());
                cur_tree_len = tree->treeLength();
                tree->setAlignment(aln);
                extractModelInfo(cur_pair.set_name, model_info, part_model_info);
                transferModelParameters(in_tree, model_info, part_model_info, gene_sets[cur_pair.part1], gene_sets[cur_pair.part2]);
                tree->num_precision = in_tree->num_precision;
                tree->setParams(&params);
                tree->sse = params.SSE;
                tree->optimize_by_newton = params.optimize_by_newton;
                tree->num_threads = params.model_test_and_tree ? num_threads : 1;
                /*if (params.model_test_and_tree) {
                    tree->setCheckpoint(new Checkpoint());
                    tree->saveCheckpoint();
                } else*/
                {
                    tree->setCheckpoint(&part_model_info);
                    // trick to restore checkpoint
                    tree->restoreCheckpoint();
                    tree->saveCheckpoint();
                }
                best_model.name = testModelOMatic(params, tree, part_model_info, models_block,
                    params.model_test_and_tree ? num_threads : 1, params.partition_type, cur_pair.set_name, "", true);
                best_model.restoreCheckpoint(&part_model_info);
                /*
                if (params.model_test_and_tree) {
                    delete tree->getCheckpoint();
                }
                 */
                delete tree;
                delete aln;
            }
            cur_pair.logl = best_model.logl;
            cur_pair.df = best_model.df;
            cur_pair.model_name = best_model.name;
            cur_pair.tree_len = best_model.tree_len;
            double lhnew = lhsum - lhvec[cur_pair.part1] - lhvec[cur_pair.part2] + best_model.logl;
            int dfnew = dfsum - dfvec[cur_pair.part1] - dfvec[cur_pair.part2] + best_model.df;
            cur_pair.score = computeInformationScore(lhnew, dfnew, ssize, params.model_test_criterion);
#ifdef _OPENMP
#pragma omp critical
#endif
			{
				if (!done_before) {
					replaceModelInfo(cur_pair.set_name, model_info, part_model_info);
                    model_info.dump();
                    num_model++;
					cout.width(4);
					cout << right << num_model << " ";
					cout.width(12);
					cout << left << best_model.name << " ";
					cout.width(11);
                    cout << cur_pair.score << " ";
                    cout.width(11);
                    cout << cur_pair.tree_len << " " << cur_pair.set_name;
                    if (num_model >= 10) {
                        double remain_time = max(total_num_model-num_model, (int64_t)0)*(getRealTime()-start_time)/num_model;
                        cout << "\t" << convert_time(getRealTime()-start_time) << " (" 
                            << convert_time(remain_time) << " left)";
                    }
                    cout << endl;
				}
                if (cur_pair.score < inf_score)
                    better_pairs.insertPair(cur_pair);
			}

        }
		if (better_pairs.empty()) break;
        ModelPairSet compatible_pairs;

        int num_comp_pairs = params.partition_merge == MERGE_RCLUSTERF ? gene_sets.size()/2 : 1;
        better_pairs.getCompatiblePairs(num_comp_pairs, compatible_pairs);
        if (compatible_pairs.size() > 1)
            cout << compatible_pairs.size() << " compatible better partition pairs found" << endl;

        // 2017-12-21: simultaneously merging better pairs
        for (auto it_pair = compatible_pairs.begin(); it_pair != compatible_pairs.end(); it_pair++) {
            ModelPair opt_pair = it_pair->second;

            lhsum = lhsum - lhvec[opt_pair.part1] - lhvec[opt_pair.part2] + opt_pair.logl;
            dfsum = dfsum - dfvec[opt_pair.part1] - dfvec[opt_pair.part2] + opt_pair.df;
            inf_score = computeInformationScore(lhsum, dfsum, ssize, params.model_test_criterion);
            ASSERT(inf_score <= opt_pair.score + 0.1);

            cout << "Merging " << opt_pair.set_name << " with " << criterionName(params.model_test_criterion)
                 << " score: " << inf_score << " (LnL: " << lhsum << "  df: " << dfsum << ")" << endl;
            // change entry opt_part1 to merged one
            gene_sets[opt_pair.part1] = opt_pair.merged_set;
            lhvec[opt_pair.part1] = opt_pair.logl;
            dfvec[opt_pair.part1] = opt_pair.df;
            lenvec[opt_pair.part1] = opt_pair.tree_len;
            model_names[opt_pair.part1] = opt_pair.model_name;
            greedy_model_trees[opt_pair.part1] = "(" + greedy_model_trees[opt_pair.part1] + "," +
                greedy_model_trees[opt_pair.part2] + ")" +
                convertIntToString(in_tree->size()-gene_sets.size()+1) + ":" +
                convertDoubleToString(inf_score);

            // delete entry opt_part2
            lhvec.erase(lhvec.begin() + opt_pair.part2);
            dfvec.erase(dfvec.begin() + opt_pair.part2);
            lenvec.erase(lenvec.begin() + opt_pair.part2);
            gene_sets.erase(gene_sets.begin() + opt_pair.part2);
            model_names.erase(model_names.begin() + opt_pair.part2);
            greedy_model_trees.erase(greedy_model_trees.begin() + opt_pair.part2);

            // decrease part ID for all pairs beyond opt_pair.part2
            auto next_pair = it_pair;
            for (next_pair++; next_pair != compatible_pairs.end(); next_pair++) {
                if (next_pair->second.part1 > opt_pair.part2)
                    next_pair->second.part1--;
                if (next_pair->second.part2 > opt_pair.part2)
                    next_pair->second.part2--;
            }
        }
	}

	string final_model_tree;
	if (greedy_model_trees.size() == 1)
		final_model_tree = greedy_model_trees[0];
	else {
		final_model_tree = "(";
		for (i = 0; i < greedy_model_trees.size(); i++) {
			if (i>0)
				final_model_tree += ",";
			final_model_tree += greedy_model_trees[i];
		}
		final_model_tree += ")";
	}

//    cout << "BEST-FIT PARTITION MODEL: " << endl;
//    cout << "  charpartition " << criterionName(params.model_test_criterion) << " = ";
//    for (i = 0; i < gene_sets.size(); i++) {
//        if (i > 0)
//            cout << ", ";
//        cout << model_names[i] << ":";
//        for (auto j = gene_sets[i].begin(); j != gene_sets[i].end(); j++) {
//            cout << " " << super_aln->partitions[*j]->name;
//        }
//    }
//    cout << ";" << endl;
	cout << "Agglomerative model selection: " << final_model_tree << endl;
    
    if (gene_sets.size() < in_tree->size())
        mergePartitions(in_tree, gene_sets, model_names);

    if (!iEquals(params.merge_models, "all")) {
        // test all candidate models again
        lhsum = 0.0;
        dfsum = 0;
        if (params.partition_type == BRLEN_FIX || params.partition_type == BRLEN_SCALE) {
            dfsum = in_tree->getNBranchParameters(BRLEN_OPTIMIZE);
            if (params.partition_type == BRLEN_SCALE)
                dfsum -= 1;
        }

        // sort partition by computational cost for OpenMP effciency
        partitionID.clear();
        for (i = 0; i < in_tree->size(); i++) {
            Alignment *this_aln = in_tree->at(i)->aln;
            // computation cost is proportional to #sequences, #patterns, and #states
            partitionID.push_back({i, ((double)this_aln->getNSeq())*this_aln->getNPattern()*this_aln->num_states});
        }
        
        if (num_threads > 1) {
            std::sort(partitionID.begin(), partitionID.end(), comparePartition);
        }

    #ifdef _OPENMP
        parallel_over_partitions = !params.model_test_and_tree && (in_tree->size() >= num_threads);
        #pragma omp parallel for private(i) schedule(dynamic) reduction(+: lhsum, dfsum) if(parallel_over_partitions)
    #endif
        for (int j = 0; j < in_tree->size(); j++) {
            i = partitionID[j].first;
            PhyloTree *this_tree = in_tree->at(i);
            // scan through models for this partition, assuming the information occurs consecutively
            ModelCheckpoint part_model_info;
            extractModelInfo(this_tree->aln->name, model_info, part_model_info);
            // do the computation
            string part_model_name;
            if (params.model_name.empty())
                part_model_name = this_tree->aln->model_name;
            ModelInfo best_model;
            best_model.name = testModelOMatic(params, this_tree, part_model_info, models_block,
                (parallel_over_partitions ? 1 : num_threads), brlen_type,
                this_tree->aln->name, part_model_name, false);
            
            bool check = (best_model.restoreCheckpoint(&part_model_info));
            ASSERT(check);
            
            double score = best_model.computeICScore(this_tree->getAlnNSite());
            this_tree->aln->model_name = best_model.name;
            lhsum += (lhvec[i] = best_model.logl);
            dfsum += (dfvec[i] = best_model.df);
            lenvec[i] = best_model.tree_len;
            
    #ifdef _OPENMP
    #pragma omp critical
    #endif
            {
            num_model++;
            cout.width(4);
            cout << right << num_model << " ";
            cout.width(12);
            cout << left << best_model.name << " ";
            cout.width(11);
            cout << score << " " << this_tree->aln->name;
            if (num_model >= 10) {
                double remain_time = (total_num_model-num_model)*(getRealTime()-start_time)/num_model;
                cout << "\t" << convert_time(getRealTime()-start_time) << " ("
                << convert_time(remain_time) << " left)";
            }
            cout << endl;
            replaceModelInfo(this_tree->aln->name, model_info, part_model_info);
            model_info.dump();
            }
        }
    }

    inf_score = computeInformationScore(lhsum, dfsum, ssize, params.model_test_criterion);
    cout << "Best partition model " << criterionName(params.model_test_criterion) << " score: " << inf_score << " (LnL: " << lhsum << "  df:" << dfsum << ")" << endl;

    ((SuperAlignment*)in_tree->aln)->printBestPartition((string(params.out_prefix) + ".best_scheme.nex").c_str());
	((SuperAlignment*)in_tree->aln)->printBestPartitionRaxml((string(params.out_prefix) + ".best_scheme").c_str());
    model_info.dump();
    if (inf_score > concat_info.computeICScore(ssize) + 1.0) {
        cout << endl;
        outWarning("Partition merging found worse model than single model!");
        outWarning("Please do not use partition model!");
    }
}

bool isMixtureModel(ModelsBlock *models_block, string &model_str) {
    size_t mix_pos;
    for (mix_pos = 0; mix_pos < model_str.length(); mix_pos++) {
        size_t next_mix_pos = model_str.find_first_of("+*", mix_pos);
        string sub_model_str = model_str.substr(mix_pos, next_mix_pos-mix_pos);
        if (models_block->findMixModel(sub_model_str))
            return true;
        if (next_mix_pos == string::npos)
            break;
        mix_pos = next_mix_pos;
    }
    return false;
}

string testModel(Params &params, PhyloTree* in_tree, ModelCheckpoint &model_info,
    ModelsBlock *models_block, int num_threads, int brlen_type,
    string set_name, string in_model_name, ModelAdjust *adjust, bool merge_phase)
{
    ModelCheckpoint *checkpoint = &model_info;

	in_tree->params = &params;
	CandidateModelSet candidate_models;
    if (in_model_name.empty())
        candidate_models.generate(params, in_tree->aln, params.model_test_separate_rate, merge_phase);
    else {
        candidate_models.push_back(CandidateModel(in_model_name));
    }

    DoubleVector model_scores;
    int model;
	string best_model;

	int ssize = in_tree->aln->getNSite(); // sample size
    if (adjust)
        ssize = adjust->sample_size;
	if (params.model_test_sample_size)
		ssize = params.model_test_sample_size;
	if (set_name == "") {
		cout << "ModelFinder will test " << candidate_models.size() << " "
			<< getSeqTypeName(in_tree->aln->seq_type)
			<< " models (sample size: " << ssize << ") ..." << endl;
        if (params.model_test_and_tree == 0)
            cout << " No. Model         -LnL         df  AIC          AICc         BIC" << endl;
	}

    //	uint64_t RAM_requirement = 0;
    string best_model_AIC, best_model_AICc, best_model_BIC;
    double best_score_AIC = DBL_MAX, best_score_AICc = DBL_MAX, best_score_BIC = DBL_MAX;
    string best_tree_AIC, best_tree_AICc, best_tree_BIC;

//    CKP_RESTORE(best_score_AIC);
//    CKP_RESTORE(best_score_AICc);
//    CKP_RESTORE(best_score_BIC);
//    CKP_RESTORE(best_model_AIC);
//    CKP_RESTORE(best_model_AICc);
//    CKP_RESTORE(best_model_BIC);

    CKP_RESTORE(best_tree_AIC);
    CKP_RESTORE(best_tree_AICc);
    CKP_RESTORE(best_tree_BIC);

    //------------- MAIN FOR LOOP GOING THROUGH ALL MODELS TO BE TESTED ---------//

	for (model = 0; model < candidate_models.size(); model++) {
		//cout << model_names[model] << endl;
        if (candidate_models[model].name[0] == '+') {
            // now switching to test rate heterogeneity
            if (best_model == "")
                switch (params.model_test_criterion) {
                case MTC_AIC:
                    best_model = best_model_AIC;
                    break;
                case MTC_AICC:
                    best_model = best_model_AICc;
                    break;
                case MTC_BIC:
                    best_model = best_model_BIC;
                    break;
                default: ASSERT(0);
                }
            candidate_models[model].name = best_model + candidate_models[model].name;
        }

		// optimize model parameters
        string orig_model_name = candidate_models[model].name;
        // keep separate output model_info to only update model_info if better model found
        ModelCheckpoint out_model_info;
		ModelInfo info;
		info.set_name = set_name;
        string tree_string;

        /***** main call to estimate model parameters ******/
        tree_string = candidate_models[model].evaluate(params, in_tree->aln,
            model_info, out_model_info, info, models_block, num_threads, brlen_type, adjust);

        info.computeICScores(ssize);
        info.saveCheckpoint(checkpoint);

        ModelInfo prev_info;

        bool skip_model = false;

        if (prev_info.restoreCheckpointRminus1(checkpoint, info.name)) {
            // check stop criterion for +R
            prev_info.computeICScores(ssize);
            switch (params.model_test_criterion) {
            case MTC_ALL:
                if (info.AIC_score > prev_info.AIC_score &&
                    info.AICc_score > prev_info.AICc_score &&
                    info.BIC_score > prev_info.BIC_score) {
                    // skip remaining model
                    skip_model = true;
                }
                break;
            case MTC_AIC:
                if (info.AIC_score > prev_info.AIC_score) {
                    // skip remaining model
                    skip_model = true;
                }
                break;
            case MTC_AICC:
                if (info.AICc_score > prev_info.AICc_score) {
                    // skip remaining model
                    skip_model = true;
                }
                break;
            case MTC_BIC:
                if (info.BIC_score > prev_info.BIC_score) {
                    // skip remaining model
                    skip_model = true;
                }
                break;
            }
        }

		if (info.AIC_score < best_score_AIC) {
            best_model_AIC = info.name;
            best_score_AIC = info.AIC_score;
            if (!tree_string.empty())
                best_tree_AIC = tree_string;
            // only update model_info with better model
            if (params.model_test_criterion == MTC_AIC)
                model_info.putSubCheckpoint(&out_model_info, "");
        }
		if (info.AICc_score < best_score_AICc) {
            best_model_AICc = info.name;
            best_score_AICc = info.AICc_score;
            if (!tree_string.empty())
                best_tree_AICc = tree_string;
            // only update model_info with better model
            if (params.model_test_criterion == MTC_AICC)
                model_info.putSubCheckpoint(&out_model_info, "");
        }

		if (info.BIC_score < best_score_BIC) {
			best_model_BIC = info.name;
            best_score_BIC = info.BIC_score;
            if (!tree_string.empty())
                best_tree_BIC = tree_string;
            // only update model_info with better model
            if (params.model_test_criterion == MTC_BIC)
                model_info.putSubCheckpoint(&out_model_info, "");
        }

        switch (params.model_test_criterion) {
            case MTC_AIC: model_scores.push_back(info.AIC_score); break;
            case MTC_AICC: model_scores.push_back(info.AICc_score); break;
            default: model_scores.push_back(info.BIC_score); break;
        }

        CKP_SAVE(best_tree_AIC);
        CKP_SAVE(best_tree_AICc);
        CKP_SAVE(best_tree_BIC);
        checkpoint->dump();


		if (set_name == "") {
            cout.width(3);
            cout << right << model+1 << "  ";
            cout.width(13);
            cout << left << info.name << " ";
            
            cout.precision(3);
            cout << fixed;
            cout.width(12);
            cout << -info.logl << " ";
            cout.width(3);
            cout << info.df << " ";
            cout.width(12);
            cout << info.AIC_score << " ";
            cout.width(12);
            cout << info.AICc_score << " " << info.BIC_score;
            cout << endl;
        }

        if (skip_model) {
            // skip over all +R model of higher categories
            const char *rates[] = {"+R", "*R", "+H", "*H"};
            size_t posR;
            for (int i = 0; i < sizeof(rates)/sizeof(char*); i++)
                if ((posR = orig_model_name.find(rates[i])) != string::npos)
                    break;
            string first_part = orig_model_name.substr(0, posR+2);
            while (model < candidate_models.size()-1 && candidate_models[model+1].name.substr(0, posR+2) == first_part) {
                model++;
                model_scores.push_back(DBL_MAX);
            }
        }

	}

    ASSERT(model_scores.size() == candidate_models.size());

    if (best_model_BIC.empty())
        outError("No models were examined! Please check messages above");

	int *model_rank = new int[model_scores.size()];

//    string best_tree; // BQM 2015-07-21: With Lars find best model
	/* sort models by their scores */
	switch (params.model_test_criterion) {
	case MTC_AIC:
		best_model = best_model_AIC;
		break;
	case MTC_AICC:
		best_model = best_model_AICc;
		break;
	case MTC_BIC:
		best_model = best_model_BIC;
		break;
    default: ASSERT(0);
	}
	sort_index(model_scores.data(), model_scores.data() + model_scores.size(), model_rank);

    string model_list;
	for (model = 0; model < model_scores.size(); model++) {
        if (model_scores[model_rank[model]] == DBL_MAX)
            break;
        if (model > 0)
            model_list += " ";
		model_list += candidate_models[model_rank[model]].name;
    }

    model_info.putBestModelList(model_list);
    CKP_SAVE(best_model_AIC);
    CKP_SAVE(best_model_AICc);
    CKP_SAVE(best_model_BIC);

    CKP_SAVE(best_score_AIC);
    CKP_SAVE(best_score_AICc);
    CKP_SAVE(best_score_BIC);

    checkpoint->dump();

	delete [] model_rank;

//	in_tree->deleteAllPartialLh();

    string best_tree;
    model_info.getBestTree(best_tree);

    // BQM 2015-07-21 with Lars: load the best_tree
//	if (params.model_test_and_tree)
		in_tree->readTreeString(best_tree);

    
	return best_model;
}

int countDistinctTrees(const char *filename, bool rooted, IQTree *tree, IntVector &distinct_ids, bool exclude_duplicate) {
	StringIntMap treels;
	try {
		ifstream in;
		in.exceptions(ios::failbit | ios::badbit);
		in.open(filename);
		// remove the failbit
		in.exceptions(ios::badbit);
		int tree_id;
		for (tree_id = 0; !in.eof(); tree_id++) {
			if (exclude_duplicate) {
				tree->freeNode();
				tree->readTree(in, rooted);
				tree->setAlignment(tree->aln);
				tree->setRootNode(tree->params->root);
				StringIntMap::iterator it = treels.end();
				ostringstream ostr;
				tree->printTree(ostr, WT_TAXON_ID | WT_SORT_TAXA);
				it = treels.find(ostr.str());
				if (it != treels.end()) { // already in treels
					distinct_ids.push_back(it->second);
				} else {
					distinct_ids.push_back(-1);
					treels[ostr.str()] = tree_id;
				}
			} else {
				// ignore tree
				char ch;
				do {
					in >> ch;
				} while (!in.eof() && ch != ';');
				distinct_ids.push_back(-1);
			}
			char ch;
			in.exceptions(ios::goodbit);
			(in) >> ch;
			if (in.eof()) break;
			in.unget();
			in.exceptions(ios::failbit | ios::badbit);

		}
		in.close();
	} catch (ios::failure) {
		outError("Cannot read file ", filename);
	}
	if (exclude_duplicate)
		return treels.size();
	else
		return distinct_ids.size();
}

//const double TOL_RELL_SCORE = 0.01;

/*
    Problem: solve the following linear system equation:
    a_1*x + b_1*y = c_1
    a_2*x + b_2*y = c_2
    ....
    a_n*x + b_n*y = c_n
    
becomes minimizing weighted least square:

    sum_k { w_k*[ c_k - (a_k*x + b_k*y) ]^2 }


the solution is:

    x = [(sum_k w_k*b_k*c_k)*(sum_k w_k*a_k*b_k) - (sum_k w_k*a_k*c_k)(sum_k w_k*b_k^2)] / 
        [ (sum_k w_k*a_k*b_k)^2 - (sum_k w_k*a_k^2)*(sum_k w_k*b_k^2) ]
    
    y = [(sum_k w_k*a_k*c_k)*(sum_k w_k*a_k*b_k) - (sum_k w_k*b_k*c_k)(sum_k w_k*a_k^2)] / 
        [ (sum_k w_k*a_k*b_k)^2 - (sum_k w_k*a_k^2)*(sum_k w*k*b_k^2) ]
    
    @param n number of data points
    @param w weight vector of length n
    @param a a value vector of length n
    @param b b value vector of length n
    @param c c value vector of length n
    @param[out] x x-value
    @param[out] y y-value
    @return least square value
*/
void doWeightedLeastSquare(int n, double *w, double *a, double *b, double *c, double &x, double &y, double &se) {
    int k;
    double BC = 0.0, AB = 0.0, AC = 0.0, A2 = 0.0, B2 = 0.0;
    double denom;
    for (k = 0; k < n; k++) {
        double wa = w[k]*a[k];
        double wb = w[k]*b[k];
        AB += wa*b[k];
        BC += wb*c[k];
        AC += wa*c[k];
        A2 += wa*a[k];
        B2 += wb*b[k];
    }
    denom = 1.0/(AB*AB - A2*B2);
    x = (BC*AB - AC*B2) * denom;
    y = (AC*AB - BC*A2) * denom;
    
    se = -denom*(B2+A2+2*AB);
    ASSERT(se >= 0.0);
}

/**
    MLE estimates for AU test
*/
class OptimizationAUTest : public Optimization {

public:

    OptimizationAUTest(double d, double c, int nscales, double *bp, double *rr, double *rr_inv) {
        this->d = d;
        this->c = c;
        this->bp = bp;
        this->rr = rr;
        this->rr_inv = rr_inv;
        this->nscales = nscales;
        
    }

	/**
		return the number of dimensions
	*/
	virtual int getNDim() { return 2; }


	/**
		the target function which needs to be optimized
		@param x the input vector x
		@return the function value at x
	*/
	virtual double targetFunk(double x[]) {
        d = x[1];
        c = x[2];
        double res = 0.0;
        for (int k = 0; k < nscales; k++) {
            double cdf = gsl_cdf_ugaussian_P(d*rr[k] + c*rr_inv[k]);
            res += bp[k] * log(1.0 - cdf) + (1.0-bp[k])*log(cdf);
        }
        return res;
    }

    void optimizeDC() {
        double x[3], lower[3], upper[3];
        bool bound_check[3];
        x[1] = d;
        x[2] = c;
        lower[1] = lower[2] = 1e-4;
        upper[1] = upper[2] = 100.0;
        bound_check[1] = bound_check[2] = false;
        minimizeMultiDimen(x, 2, lower, upper, bound_check, 1e-4);
        d = x[1];
        c = x[2];
    }

    double d, c;
    int nscales;
    double *bp;
    double *rr;
    double *rr_inv;
};

/* BEGIN CODE WAS TAKEN FROM CONSEL PROGRAM */

/* binary search for a sorted vector 
   find k s.t. vec[k-1] <= t < vec[k]
 */
int cntdist2(double *vec, int bb, double t)
{
  int i,i0,i1;

  i0=0; i1=bb-1;
  if(t < vec[0]) return 0;
  else if(vec[bb-1] <= t) return bb;

  while(i1-i0>1) {
    i=(i0+i1)/2;
    if(vec[i] <= t) i0=i;
    else i1=i;
  }

  return i1;
}

/*
  smoothing the counting for a sorted vector
  the piecewise linear function connecting
  F(v[i]) =  1/(2n) + i/n, for i=0,...,n-1
  F(1.5v[0]-0.5v[1]) = 0
  F(1.5v[n-1]-0.5v[n-2]) = 1.

  1. F(x)=0 for x<=1.5v[0]-0.5v[1] 

  2. F(x)=1/(2n) + (1/n)*(x-v[0])/(v[1]-v[0])
  for 1.5v[0]-0.5v[1] < x <= v[0]

  3. F(x)=1/(2n) + i/n + (1/n)*(x-v[i])/(v[i]-v[i+1])
  for v[i] < x <= v[i+1], i=0,...,

  4. F(x)=1-(1/2n) + (1/n)*(x-v[n-1])/(v[n-1]-v[n-2])
  for v[n-1] < x <= 1.5v[n-1]-0.5v[n-2]

  5. F(x)=1 for x > 1.5v[n-1]-0.5v[n-2]
 */
double cntdist3(double *vec, int bb, double t)
{
  double p,n;
  int i;
  i=cntdist2(vec,bb,t)-1; /* to find vec[i] <= t < vec[i+1] */
  n=(double)bb;
  if(i<0) {
    if(vec[1]>vec[0]) p=0.5+(t-vec[0])/(vec[1]-vec[0]);
    else p=0.0;
  } else if(i<bb-1) {
    if(vec[i+1]>vec[i]) p=0.5+(double)i+(t-vec[i])/(vec[i+1]-vec[i]);
    else p=0.5+(double)i; /* <- should never happen */
  } else {
    if(vec[bb-1]-vec[bb-2]>0) p=n-0.5+(t-vec[bb-1])/(vec[bb-1]-vec[bb-2]);
    else p=n;
  }
  if(p>n) p=n; else if(p<0.0) p=0.0;
  return p;
}

double log3(double x)
{
  double y,z1,z2,z3,z4,z5;
  if(fabs(x)>1.0e-3) {
    y=-log(1.0-x);
  } else {
    z1=x; z2=z1*x; z3=z2*x; z4=z3*x; z5=z4*x;
    y=((((z5/5.0)+z4/4.0)+z3/3.0)+z2/2.0)+z1;
  }
  return y;
}

int mleloopmax=30;
double mleeps=1e-10;
int mlecoef(double *cnts, double *rr, double bb, int kk,
	    double *coef0, /* set initinal value (size=2) */
	    double *lrt, int *df, /* LRT statistic */
        double *se
	    )
{
  int i,m,loop;
  double coef[2], update[2];
  double d1f, d2f, d11f, d12f, d22f; /* derivatives */
  double v11, v12, v22; /* inverse of -d??f */
  double a,e;
  double s[kk], r[kk],c[kk], b[kk],z[kk],p[kk],d[kk],g[kk],h[kk];

  m=0;
  for(i=0;i<kk;i++)
    {
      r[m]=rr[i]; s[m]=sqrt(rr[i]); c[m]=cnts[i]*bb; b[m]=bb;
      m++;
    }
  if(m<2) return 1;

  coef[0]=coef0[0]; /* signed distance */
  coef[1]=coef0[1]; /* curvature */

  for(loop=0;loop<mleloopmax;loop++) {
    d1f=d2f=d11f=d12f=d22f=0.0;
    for(i=0;i<m;i++) {
      z[i]=coef[0]*s[i]+coef[1]/s[i];
      p[i]=gsl_cdf_ugaussian_P(-z[i]);
      d[i]=gsl_ran_ugaussian_pdf(z[i]);
      if(p[i]>0.0 && p[i]<1.0) {
	g[i]=d[i]*( d[i]*(-c[i]+2.0*c[i]*p[i]-b[i]*p[i]*p[i])/
		    (p[i]*p[i]*(1.0-p[i])*(1.0-p[i]))
		    + z[i]*(c[i]-b[i]*p[i])/(p[i]*(1.0-p[i])) );
	h[i]=d[i]*(c[i]-b[i]*p[i])/(p[i]*(1.0-p[i]));
      } else { g[i]=h[i]=0.0; }
      d1f+= -h[i]*s[i]; d2f+= -h[i]/s[i];
      d11f+= g[i]*r[i]; d12f+= g[i]; d22f+= g[i]/r[i];
    }

    a=d11f*d22f-d12f*d12f;
    if(a==0.0) {
      return 2;
    }
    v11=-d22f/a; v12=d12f/a; v22=-d11f/a;

    /* Newton-Raphson update */
    update[0]=v11*d1f+v12*d2f; update[1]=v12*d1f+v22*d2f;
    coef[0]+=update[0]; coef[1]+=update[1];

    /* check convergence */
    e=-d11f*update[0]*update[0]-2.0*d12f*update[0]*update[1]
      -d22f*update[1]*update[1];

    if(e<mleeps) break;
  }

  /* calc log-likelihood */
  *lrt=0.0; *df=0;
  for(i=0;i<m;i++) {
    if(p[i]>0.0 && p[i]<1.0) {
      *df+=1;
      if(c[i]>0.0) a=c[i]*log(c[i]/b[i]/p[i]); else a=0.0;
      if(c[i]<b[i]) a+=(b[i]-c[i])*(log3(p[i])-log3(c[i]/b[i]));
      *lrt += a;
    }
  }
  *lrt *= 2.0; *df -= 2;

  /* write back the results */
  coef0[0]=coef[0]; coef0[1]=coef[1];
  *se = v11 + v22 - 2*v12;
//  vmat[0][0]=v11;vmat[0][1]=vmat[1][0]=v12;vmat[1][1]=v22; 
  if(loop==mleloopmax || *df< 0) i=1; else i=0;
  return i;
}

/* END CODE WAS TAKEN FROM CONSEL PROGRAM */

/**
    @param tree_lhs RELL score matrix of size #trees x #replicates
*/
void performAUTest(Params &params, PhyloTree *tree, double *pattern_lhs, vector<TreeInfo> &info) {
    
    if (params.topotest_replicates < 10000)
        outWarning("Too few replicates for AU test. At least -zb 10000 for reliable results!");
    
    /* STEP 1: specify scale factors */
    size_t nscales = 10;
    double r[] = {0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4};
    double rr[] = {sqrt(0.5), sqrt(0.6), sqrt(0.7), sqrt(0.8), sqrt(0.9), 1.0, 
        sqrt(1.1), sqrt(1.2), sqrt(1.3), sqrt(1.4)};
    double rr_inv[] = {sqrt(1/0.5), sqrt(1/0.6), sqrt(1/0.7), sqrt(1/0.8), sqrt(1/0.9), 1.0, 
        sqrt(1/1.1), sqrt(1/1.2), sqrt(1/1.3), sqrt(1/1.4)};
        
    /* STEP 2: compute bootstrap proportion */
    size_t ntrees = info.size();
    size_t nboot = params.topotest_replicates;
//    double nboot_inv = 1.0 / nboot;
    
    size_t nptn = tree->getAlnNPattern();
    size_t maxnptn = get_safe_upper_limit(nptn);
    
//    double *bp = new double[ntrees*nscales];
//    memset(bp, 0, sizeof(double)*ntrees*nscales);
    
    double *treelhs;
    cout << (ntrees*nscales*nboot*sizeof(double) >> 20) << " MB required for AU test" << endl;
    treelhs = new double[ntrees*nscales*nboot];
    if (!treelhs)
        outError("Not enough memory to perform AU test!");
    
    size_t k, tid, ptn;

    double start_time = getRealTime();

    cout << "Generating " << nscales << " x " << nboot << " multiscale bootstrap replicates... ";

#ifdef _OPENMP
    #pragma omp parallel private(k, tid, ptn)
    {
    int *rstream;
    init_random(params.ran_seed + omp_get_thread_num(), false, &rstream);
#else
    int *rstream = randstream;
#endif
    size_t boot;
    int *boot_sample = aligned_alloc<int>(maxnptn);
    memset(boot_sample, 0, maxnptn*sizeof(int));
    
    double *boot_sample_dbl = aligned_alloc<double>(maxnptn);
    
#ifdef _OPENMP
    #pragma omp for schedule(dynamic)
#endif
    for (k = 0; k < nscales; k++) {
        string str = "SCALE=" + convertDoubleToString(r[k]);    
		for (boot = 0; boot < nboot; boot++) {
            if (r[k] == 1.0 && boot == 0)
                // 2018-10-23: get one of the bootstrap sample as the original alignment
                tree->aln->getPatternFreq(boot_sample);
            else
                tree->aln->createBootstrapAlignment(boot_sample, str.c_str(), rstream);
            for (ptn = 0; ptn < maxnptn; ptn++)
                boot_sample_dbl[ptn] = boot_sample[ptn];
            double max_lh = -DBL_MAX, second_max_lh = -DBL_MAX;
            int max_tid = -1;
            for (tid = 0; tid < ntrees; tid++) {
                double *pattern_lh = pattern_lhs + (tid*maxnptn);
                double tree_lh;
                if (params.SSE == LK_386) {
                    tree_lh = 0.0;
                    for (ptn = 0; ptn < nptn; ptn++)
                        tree_lh += pattern_lh[ptn] * boot_sample_dbl[ptn];
                } else {
                    tree_lh = tree->dotProductDoubleCall(pattern_lh, boot_sample_dbl, nptn);
                }
                // rescale lh
                tree_lh /= r[k];
                
                // find the max and second max
                if (tree_lh > max_lh) {
                    second_max_lh = max_lh;
                    max_lh = tree_lh;
                    max_tid = tid;
                } else if (tree_lh > second_max_lh)
                    second_max_lh = tree_lh;
                    
                treelhs[(tid*nscales+k)*nboot + boot] = tree_lh; 
            }
            
            // compute difference from max_lh
            for (tid = 0; tid < ntrees; tid++) 
                if (tid != max_tid)
                    treelhs[(tid*nscales+k)*nboot + boot] = max_lh - treelhs[(tid*nscales+k)*nboot + boot];
                else
                    treelhs[(tid*nscales+k)*nboot + boot] = second_max_lh - max_lh;
//            bp[k*ntrees+max_tid] += nboot_inv;
        } // for boot
        
        // sort the replicates
        for (tid = 0; tid < ntrees; tid++) {
            quicksort<double,int>(treelhs + (tid*nscales+k)*nboot, 0, nboot-1);
        }
        
    } // for scale

    aligned_free(boot_sample_dbl);
    aligned_free(boot_sample);

#ifdef _OPENMP
    finish_random(rstream);
    }
#endif

//    if (verbose_mode >= VB_MED) {
//        cout << "scale";
//        for (k = 0; k < nscales; k++)
//            cout << "\t" << r[k];
//        cout << endl;
//        for (tid = 0; tid < ntrees; tid++) {
//            cout << tid;
//            for (k = 0; k < nscales; k++) {
//                cout << "\t" << bp[tid+k*ntrees];
//            }
//            cout << endl;
//        }
//    }
    
    cout << getRealTime() - start_time << " seconds" << endl;
    
    /* STEP 3: weighted least square fit */
    
    double *cc = new double[nscales];
    double *w = new double[nscales];
    double *this_bp = new double[nscales];
    cout << "TreeID\tAU\tRSS\td\tc" << endl;
    for (tid = 0; tid < ntrees; tid++) {
        double *this_stat = treelhs + tid*nscales*nboot;
        double xn = this_stat[(nscales/2)*nboot + nboot/2], x;
        double c, d; // c, d in original paper
        int idf0 = -2;
        double z = 0.0, z0 = 0.0, thp = 0.0, th = 0.0, ze = 0.0, ze0 = 0.0;
        double pval, se;
        int df;
        double rss = 0.0;
        int step;
        const int max_step = 30;
        bool failed = false;
        for (step = 0; step < max_step; step++) {
            x = xn;
            int num_k = 0;
            for (k = 0; k < nscales; k++) {
                this_bp[k] = cntdist3(this_stat + k*nboot, nboot, x) / nboot;
                if (this_bp[k] <= 0 || this_bp[k] >= 1) {
                    cc[k] = w[k] = 0.0;
                } else {
                    double bp_val = this_bp[k];
                    cc[k] = -gsl_cdf_ugaussian_Pinv(bp_val);
                    double bp_pdf = gsl_ran_ugaussian_pdf(cc[k]);
                    w[k] = bp_pdf*bp_pdf*nboot / (bp_val*(1.0-bp_val));
                    num_k++;
                }
            }
            df = num_k-2;
            if (num_k >= 2) {
                // first obtain d and c by weighted least square
                doWeightedLeastSquare(nscales, w, rr, rr_inv, cc, d, c, se);

                // maximum likelhood fit
                double coef0[2] = {d, c};
                int mlefail = mlecoef(this_bp, r, nboot, nscales, coef0, &rss, &df, &se);
                
                if (!mlefail) {
                    d = coef0[0];
                    c = coef0[1];
                }

                se = gsl_ran_ugaussian_pdf(d-c)*sqrt(se);
                
                // second, perform MLE estimate of d and c
    //            OptimizationAUTest mle(d, c, nscales, this_bp, rr, rr_inv);
    //            mle.optimizeDC();
    //            d = mle.d;
    //            c = mle.c;

                /* STEP 4: compute p-value according to Eq. 11 */
                pval = gsl_cdf_ugaussian_Q(d-c);
                z = -pval;
                ze = se;
                // compute sum of squared difference
                rss = 0.0;
                for (k = 0; k < nscales; k++) {
                    double diff = cc[k] - (rr[k]*d + rr_inv[k]*c);
                    rss += w[k] * diff * diff;
                }
                
            } else {
                // not enough data for WLS
                int num0 = 0;
                for (k = 0; k < nscales; k++)
                    if (this_bp[k] <= 0.0) num0++;
                if (num0 > nscales/2)
                    pval = 0.0;
                else
                    pval = 1.0;
                se = 0.0;
                d = c = 0.0;
                rss = 0.0;
                if (verbose_mode >= VB_MED)
                    cout << "   error in wls" << endl;
                //info[tid].au_pvalue = pval;
                //break;
            }

            
            if (verbose_mode >= VB_MED) {
                cout.unsetf(ios::fixed);
                cout << "\t" << step << "\t" << th << "\t" << x << "\t" << pval << "\t" << se << "\t" << nscales-2 << "\t" << d << "\t" << c << "\t" << z << "\t" << ze << "\t" << rss << endl;
            }
            
            if(df < 0 && idf0 < 0) { failed = true; break;} /* degenerated */
            
            if ((df < 0) || (idf0 >= 0 && (z-z0)*(x-thp) > 0.0 && fabs(z-z0)>0.1*ze0)) {
                if (verbose_mode >= VB_MED)
                    cout << "   non-monotone" << endl;
                th=x;
                xn=0.5*x+0.5*thp;
                continue;
            }
            if(idf0 >= 0 && (fabs(z-z0)<0.01*ze0)) {
                if(fabs(th)<1e-10) 
                    xn=th; 
                else th=x;
            } else 
                xn=0.5*th+0.5*x;
            info[tid].au_pvalue = pval;
            thp=x; 
            z0=z;
            ze0=ze;
            idf0 = df;
            if(fabs(x-th)<1e-10) break;
        } // for step
        
        if (failed && verbose_mode >= VB_MED)
            cout << "   degenerated" << endl;
        
        if (step == max_step) {
            if (verbose_mode >= VB_MED)
                cout << "   non-convergence" << endl; 
            failed = true;
        }
        
        double pchi2 = (failed) ? 0.0 : computePValueChiSquare(rss, df);
        cout << tid+1 << "\t" << info[tid].au_pvalue << "\t" << rss << "\t" << d << "\t" << c;
        
        // warning if p-value of chi-square < 0.01 (rss too high)
        if (pchi2 < 0.01) 
            cout << " !!!";
        cout << endl;
    }
    
    delete [] this_bp;
    delete [] w;
    delete [] cc;
//    delete [] bp;
}


void evaluateTrees(string treeset_file, Params &params, IQTree *tree, vector<TreeInfo> &info, IntVector &distinct_ids)
{
	if (treeset_file.empty())
		return;
	cout << endl;
	//MTreeSet trees(treeset_file, params.is_rooted, params.tree_burnin, params.tree_max_count);
	cout << "Reading trees in " << treeset_file << " ..." << endl;
	size_t ntrees = countDistinctTrees(treeset_file.c_str(), params.is_rooted, tree, distinct_ids, params.distinct_trees);
	if (ntrees < distinct_ids.size()) {
		cout << "WARNING: " << distinct_ids.size() << " trees detected but only " << ntrees << " distinct trees will be evaluated" << endl;
	} else {
		cout << ntrees << (params.distinct_trees ? " distinct" : "") << " trees detected" << endl;
	}
	if (ntrees == 0) return;
	ifstream in(treeset_file);

	//if (trees.size() == 1) return;
	//string tree_file = treeset_file;
	string tree_file = params.out_prefix;
	tree_file += ".trees";
	ofstream treeout;
	//if (!params.fixed_branch_length) {
		treeout.open(tree_file.c_str());
	//}
	string score_file = params.out_prefix;
	score_file += ".treelh";
	ofstream scoreout;
	if (params.print_tree_lh)
		scoreout.open(score_file.c_str());
	string site_lh_file = params.out_prefix;
	site_lh_file += ".sitelh";
	if (params.print_site_lh) {
		ofstream site_lh_out(site_lh_file.c_str());
		site_lh_out << ntrees << " " << tree->getAlnNSite() << endl;
		site_lh_out.close();
	}

    if (params.print_partition_lh && !tree->isSuperTree()) {
        outWarning("-wpl does not work with non-partition model");
        params.print_partition_lh = false;
    }
	string part_lh_file = params.out_prefix;
	part_lh_file += ".partlh";
	if (params.print_partition_lh) {
		ofstream part_lh_out(part_lh_file.c_str());
		part_lh_out << ntrees << " " << ((PhyloSuperTree*)tree)->size() << endl;
		part_lh_out.close();
	}

	double time_start = getRealTime();

	int *boot_samples = NULL;
	size_t boot;
	//double *saved_tree_lhs = NULL;
	double *tree_lhs = NULL; // RELL score matrix of size #trees x #replicates
	double *pattern_lh = NULL;
	double *pattern_lhs = NULL;
	double *orig_tree_lh = NULL; // Original tree log-likelihoods
	double *max_lh = NULL;
	double *lhdiff_weights = NULL;
	size_t nptn = tree->getAlnNPattern();
    size_t maxnptn = get_safe_upper_limit(nptn);
    
	if (params.topotest_replicates && ntrees > 1) {
		size_t mem_size = (size_t)params.topotest_replicates*nptn*sizeof(int) +
				ntrees*params.topotest_replicates*sizeof(double) +
				(nptn + ntrees*3 + params.topotest_replicates*2)*sizeof(double) +
				ntrees*sizeof(TreeInfo) +
				params.do_weighted_test*(ntrees * nptn * sizeof(double) + ntrees*ntrees*sizeof(double));
		cout << "Note: " << ((double)mem_size/1024)/1024 << " MB of RAM required!" << endl;
		if (mem_size > getMemorySize()-100000)
			outWarning("The required memory does not fit in RAM!");
		cout << "Creating " << params.topotest_replicates << " bootstrap replicates..." << endl;
		if (!(boot_samples = new int [params.topotest_replicates*nptn]))
			outError(ERR_NO_MEMORY);
#ifdef _OPENMP
        #pragma omp parallel private(boot) if(nptn > 10000)
        {
        int *rstream;
        init_random(params.ran_seed + omp_get_thread_num(), false, &rstream);
        #pragma omp for schedule(static)
#else
        int *rstream = randstream;
#endif
		for (boot = 0; boot < params.topotest_replicates; boot++)
            if (boot == 0)
                tree->aln->getPatternFreq(boot_samples + (boot*nptn));
            else
                tree->aln->createBootstrapAlignment(boot_samples + (boot*nptn), params.bootstrap_spec, rstream);
#ifdef _OPENMP
        finish_random(rstream);
        }
#endif
        cout << "done" << endl;
		//if (!(saved_tree_lhs = new double [ntrees * params.topotest_replicates]))
		//	outError(ERR_NO_MEMORY);
		if (!(tree_lhs = new double [ntrees * params.topotest_replicates]))
			outError(ERR_NO_MEMORY);
		if (params.do_weighted_test || params.do_au_test) {
			if (!(lhdiff_weights = new double [ntrees * ntrees]))
				outError(ERR_NO_MEMORY);
            pattern_lhs = aligned_alloc<double>(ntrees*maxnptn);
//			if (!(pattern_lhs = new double[ntrees* nptn]))
//				outError(ERR_NO_MEMORY);
		}
        pattern_lh = aligned_alloc<double>(maxnptn);
//		if (!(pattern_lh = new double[nptn]))
//			outError(ERR_NO_MEMORY);
		if (!(orig_tree_lh = new double[ntrees]))
			outError(ERR_NO_MEMORY);
		if (!(max_lh = new double[params.topotest_replicates]))
			outError(ERR_NO_MEMORY);
	}
	int tree_index, tid, tid2;
	info.resize(ntrees);
	//for (MTreeSet::iterator it = trees.begin(); it != trees.end(); it++, tree_index++) {
	for (tree_index = 0, tid = 0; tree_index < distinct_ids.size(); tree_index++) {

		cout << "Tree " << tree_index + 1;
		if (distinct_ids[tree_index] >= 0) {
			cout << " / identical to tree " << distinct_ids[tree_index]+1 << endl;
			// ignore tree
			char ch;
			do {
				in >> ch;
			} while (!in.eof() && ch != ';');
			continue;
		}
		tree->freeNode();
		tree->readTree(in, tree->rooted);
        if (!tree->findNodeName(tree->aln->getSeqName(0))) {
            outError("Taxon " + tree->aln->getSeqName(0) + " not found in tree");
        }

        if (tree->rooted && tree->getModel()->isReversible()) {
            if (tree->leafNum != tree->aln->getNSeq()+1)
                outError("Tree does not have same number of taxa as alignment");
            tree->convertToUnrooted();
        } else if (!tree->rooted && !tree->getModel()->isReversible()) {
            if (tree->leafNum != tree->aln->getNSeq())
                outError("Tree does not have same number of taxa as alignment");
            tree->convertToRooted();
        }
		tree->setAlignment(tree->aln);
        tree->setRootNode(params.root);
		if (tree->isSuperTree())
			((PhyloSuperTree*) tree)->mapTrees();

		tree->initializeAllPartialLh();
		tree->fixNegativeBranch(false);
		if (params.fixed_branch_length) {
            tree->setCurScore(tree->computeLikelihood());
        } else if (params.topotest_optimize_model) {
            tree->getModelFactory()->optimizeParameters(BRLEN_OPTIMIZE, false, params.modelEps);
            tree->setCurScore(tree->computeLikelihood());
        } else {
			tree->setCurScore(tree->optimizeAllBranches(100, 0.001));
		}
		treeout << "[ tree " << tree_index+1 << " lh=" << tree->getCurScore() << " ]";
		tree->printTree(treeout);
		treeout << endl;
		if (params.print_tree_lh)
			scoreout << tree->getCurScore() << endl;

		cout << " / LogL: " << tree->getCurScore() << endl;

		if (pattern_lh) {
			double curScore = tree->getCurScore();
            memset(pattern_lh, 0, maxnptn*sizeof(double));
			tree->computePatternLikelihood(pattern_lh, &curScore);
			if (params.do_weighted_test || params.do_au_test)
				memcpy(pattern_lhs + tid*maxnptn, pattern_lh, maxnptn*sizeof(double));
		}
		if (params.print_site_lh) {
			string tree_name = "Tree" + convertIntToString(tree_index+1);
			printSiteLh(site_lh_file.c_str(), tree, pattern_lh, true, tree_name.c_str());
		}
		if (params.print_partition_lh) {
			string tree_name = "Tree" + convertIntToString(tree_index+1);
			printPartitionLh(part_lh_file.c_str(), tree, pattern_lh, true, tree_name.c_str());
		}
		info[tid].logl = tree->getCurScore();

		if (!params.topotest_replicates || ntrees <= 1) {
			tid++;
			continue;
		}
		// now compute RELL scores
		orig_tree_lh[tid] = tree->getCurScore();
		double *tree_lhs_offset = tree_lhs + (tid*params.topotest_replicates);
		for (boot = 0; boot < params.topotest_replicates; boot++) {
			double lh = 0.0;
			int *this_boot_sample = boot_samples + (boot*nptn);
			for (size_t ptn = 0; ptn < nptn; ptn++)
				lh += pattern_lh[ptn] * this_boot_sample[ptn];
			tree_lhs_offset[boot] = lh;
		}
		tid++;
	}

	ASSERT(tid == ntrees);

	if (params.topotest_replicates && ntrees > 1) {
		double *tree_probs = new double[ntrees];
		memset(tree_probs, 0, ntrees*sizeof(double));
		int *tree_ranks = new int[ntrees];

		/* perform RELL BP method */
		cout << "Performing RELL-BP test..." << endl;
		int *maxtid = new int[params.topotest_replicates];
		double *maxL = new double[params.topotest_replicates];
		int *maxcount = new int[params.topotest_replicates];
		memset(maxtid, 0, params.topotest_replicates*sizeof(int));
		memcpy(maxL, tree_lhs, params.topotest_replicates*sizeof(double));
		for (boot = 0; boot < params.topotest_replicates; boot++)
			maxcount[boot] = 1;
		for (tid = 1; tid < ntrees; tid++) {
			double *tree_lhs_offset = tree_lhs + (tid * params.topotest_replicates);
			for (boot = 0; boot < params.topotest_replicates; boot++)
				if (tree_lhs_offset[boot] > maxL[boot] + params.ufboot_epsilon) {
					maxL[boot] = tree_lhs_offset[boot];
					maxtid[boot] = tid;
					maxcount[boot] = 1;
				} else if (tree_lhs_offset[boot] > maxL[boot] - params.ufboot_epsilon &&
						random_double() <= 1.0/(maxcount[boot]+1)) {
					maxL[boot] = max(maxL[boot],tree_lhs_offset[boot]);
					maxtid[boot] = tid;
					maxcount[boot]++;
				}
		}
		for (boot = 0; boot < params.topotest_replicates; boot++)
			tree_probs[maxtid[boot]] += 1.0;
		for (tid = 0; tid < ntrees; tid++) {
			tree_probs[tid] /= params.topotest_replicates;
			info[tid].rell_confident = false;
			info[tid].rell_bp = tree_probs[tid];
		}
		sort_index(tree_probs, tree_probs + ntrees, tree_ranks);
		double prob_sum = 0.0;
		// obtain the confidence set
		for (tid = ntrees-1; tid >= 0; tid--) {
			info[tree_ranks[tid]].rell_confident = true;
			prob_sum += tree_probs[tree_ranks[tid]];
			if (prob_sum > 0.95) break;
		}

		// sanity check
		for (tid = 0, prob_sum = 0.0; tid < ntrees; tid++)
			prob_sum += tree_probs[tid];
		if (fabs(prob_sum-1.0) > 0.01)
			outError("Internal error: Wrong ", __func__);

		delete [] maxcount;
		delete [] maxL;
		delete [] maxtid;

		/* now do the SH test */
		cout << "Performing KH and SH test..." << endl;
		// SH centering step
		for (boot = 0; boot < params.topotest_replicates; boot++)
			max_lh[boot] = -DBL_MAX;
		double *avg_lh = new double[ntrees];
		for (tid = 0; tid < ntrees; tid++) {
			avg_lh[tid] = 0.0;
			double *tree_lhs_offset = tree_lhs + (tid * params.topotest_replicates);
			for (boot = 0; boot < params.topotest_replicates; boot++)
				avg_lh[tid] += tree_lhs_offset[boot];
			avg_lh[tid] /= params.topotest_replicates;
			for (boot = 0; boot < params.topotest_replicates; boot++) {
				max_lh[boot] = max(max_lh[boot], tree_lhs_offset[boot] - avg_lh[tid]);
			}
		}

		double orig_max_lh = orig_tree_lh[0];
		size_t orig_max_id = 0;
		double orig_2ndmax_lh = -DBL_MAX;
		size_t orig_2ndmax_id = -1;
		// find the max tree ID
		for (tid = 1; tid < ntrees; tid++)
			if (orig_max_lh < orig_tree_lh[tid]) {
				orig_max_lh = orig_tree_lh[tid];
				orig_max_id = tid;
			}
		// find the 2nd max tree ID
		for (tid = 0; tid < ntrees; tid++)
			if (tid != orig_max_id && orig_2ndmax_lh < orig_tree_lh[tid]) {
				orig_2ndmax_lh = orig_tree_lh[tid];
				orig_2ndmax_id = tid;
			}


		// SH compute p-value
		for (tid = 0; tid < ntrees; tid++) {
			double *tree_lhs_offset = tree_lhs + (tid * params.topotest_replicates);
			// SH compute original deviation from max_lh
			info[tid].kh_pvalue = 0.0;
			info[tid].sh_pvalue = 0.0;
			size_t max_id = (tid != orig_max_id) ? orig_max_id : orig_2ndmax_id;
			double orig_diff = orig_tree_lh[max_id] - orig_tree_lh[tid] - avg_lh[tid];
			double *max_kh = tree_lhs + (max_id * params.topotest_replicates);
			for (boot = 0; boot < params.topotest_replicates; boot++) {
				if (max_lh[boot] - tree_lhs_offset[boot] > orig_diff)
					info[tid].sh_pvalue += 1.0;
				//double max_kh_here = max(max_kh[boot]-avg_lh[max_id], tree_lhs_offset[boot]-avg_lh[tid]);
				double max_kh_here = (max_kh[boot]-avg_lh[max_id]);
				if (max_kh_here - tree_lhs_offset[boot] > orig_diff)
					info[tid].kh_pvalue += 1.0;
			}
			info[tid].sh_pvalue /= params.topotest_replicates;
			info[tid].kh_pvalue /= params.topotest_replicates;
		}

		if (params.do_weighted_test) {

			cout << "Computing pairwise logl difference variance ..." << endl;
			/* computing lhdiff_weights as 1/sqrt(lhdiff_variance) */
			for (tid = 0; tid < ntrees; tid++) {
				double *pattern_lh1 = pattern_lhs + (tid * maxnptn);
				lhdiff_weights[tid*ntrees+tid] = 0.0;
				for (tid2 = tid+1; tid2 < ntrees; tid2++) {
					double lhdiff_variance = tree->computeLogLDiffVariance(pattern_lh1, pattern_lhs + (tid2*maxnptn));
					lhdiff_weights[tid*ntrees+tid2] = 1.0/sqrt(lhdiff_variance);
					lhdiff_weights[tid2*ntrees+tid] = lhdiff_weights[tid*ntrees+tid2];
				}
			}

			// Weighted KH and SH test
			cout << "Performing WKH and WSH test..." << endl;
			for (tid = 0; tid < ntrees; tid++) {
				double *tree_lhs_offset = tree_lhs + (tid * params.topotest_replicates);
				info[tid].wkh_pvalue = 0.0;
				info[tid].wsh_pvalue = 0.0;
				double worig_diff = -DBL_MAX;
				size_t max_id = -1;
				for (tid2 = 0; tid2 < ntrees; tid2++)
					if (tid2 != tid) {
						double wdiff = (orig_tree_lh[tid2] - orig_tree_lh[tid])*lhdiff_weights[tid*ntrees+tid2];
						if (wdiff > worig_diff) {
							worig_diff = wdiff;
							max_id = tid2;
						}
					}
				for (boot = 0; boot < params.topotest_replicates; boot++) {
					double wmax_diff = -DBL_MAX;
					for (tid2 = 0; tid2 < ntrees; tid2++)
						if (tid2 != tid)
							wmax_diff = max(wmax_diff,
									(tree_lhs[tid2*params.topotest_replicates+boot] - avg_lh[tid2] -
									tree_lhs_offset[boot] + avg_lh[tid]) * lhdiff_weights[tid*ntrees+tid2]);
					if (wmax_diff > worig_diff)
						info[tid].wsh_pvalue += 1.0;
					wmax_diff = (tree_lhs[max_id*params.topotest_replicates+boot] - avg_lh[max_id] -
							tree_lhs_offset[boot] + avg_lh[tid]);
					if (wmax_diff >  orig_tree_lh[max_id] - orig_tree_lh[tid])
						info[tid].wkh_pvalue += 1.0;
				}
				info[tid].wsh_pvalue /= params.topotest_replicates;
				info[tid].wkh_pvalue /= params.topotest_replicates;
			}
		}
        
        delete [] avg_lh;
        
		/* now to ELW - Expected Likelihood Weight method */
		cout << "Performing ELW test..." << endl;

		for (boot = 0; boot < params.topotest_replicates; boot++)
			max_lh[boot] = -DBL_MAX;
		for (tid = 0; tid < ntrees; tid++) {
			double *tree_lhs_offset = tree_lhs + (tid * params.topotest_replicates);
			for (boot = 0; boot < params.topotest_replicates; boot++)
				max_lh[boot] = max(max_lh[boot], tree_lhs_offset[boot]);
		}
		double *sumL = new double[params.topotest_replicates];
		memset(sumL, 0, sizeof(double) * params.topotest_replicates);
		for (tid = 0; tid < ntrees; tid++) {
			double *tree_lhs_offset = tree_lhs + (tid * params.topotest_replicates);
			for (boot = 0; boot < params.topotest_replicates; boot++) {
				tree_lhs_offset[boot] = exp(tree_lhs_offset[boot] - max_lh[boot]);
				sumL[boot] += tree_lhs_offset[boot];
			}
		}
		for (tid = 0; tid < ntrees; tid++) {
			double *tree_lhs_offset = tree_lhs + (tid * params.topotest_replicates);
			tree_probs[tid] = 0.0;
			for (boot = 0; boot < params.topotest_replicates; boot++) {
				tree_probs[tid] += (tree_lhs_offset[boot] / sumL[boot]);
			}
			tree_probs[tid] /= params.topotest_replicates;
			info[tid].elw_confident = false;
			info[tid].elw_value = tree_probs[tid];
		}

		sort_index(tree_probs, tree_probs + ntrees, tree_ranks);
		prob_sum = 0.0;
		// obtain the confidence set
		for (tid = ntrees-1; tid >= 0; tid--) {
			info[tree_ranks[tid]].elw_confident = true;
			prob_sum += tree_probs[tree_ranks[tid]];
			if (prob_sum > 0.95) break;
		}

		// sanity check
		for (tid = 0, prob_sum = 0.0; tid < ntrees; tid++)
			prob_sum += tree_probs[tid];
		if (fabs(prob_sum-1.0) > 0.01)
			outError("Internal error: Wrong ", __func__);
		delete [] sumL;

        if (params.do_au_test) {
            cout << "Performing approximately unbiased (AU) test..." << endl;
            performAUTest(params, tree, pattern_lhs, info);
        }

		delete [] tree_ranks;
		delete [] tree_probs;

	}
	if (max_lh)
		delete [] max_lh;
	if (orig_tree_lh)
		delete [] orig_tree_lh;
	if (pattern_lh)
        aligned_free(pattern_lh);
	if (pattern_lhs)
        aligned_free(pattern_lhs);
	if (lhdiff_weights)
		delete [] lhdiff_weights;
	if (tree_lhs)
		delete [] tree_lhs;
	//if (saved_tree_lhs)
	//	delete [] saved_tree_lhs;
	if (boot_samples)
		delete [] boot_samples;

	if (params.print_tree_lh) {
		scoreout.close();
	}

	treeout.close();
	in.close();

	cout << "Time for evaluating all trees: " << getRealTime() - time_start << " sec." << endl;

}


void evaluateTrees(string treeset_file, Params &params, IQTree *tree) {
	vector<TreeInfo> info;
	IntVector distinct_ids;
	evaluateTrees(treeset_file, params, tree, info, distinct_ids);
}



