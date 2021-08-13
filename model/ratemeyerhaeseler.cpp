/***************************************************************************
 *   Copyright (C) 2009 by BUI Quang Minh   *
 *   minh.bui@univie.ac.at   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "ratemeyerhaeseler.h"
#include <tree/phylotree.h>
#include <utils/stringfunctions.h> //for convert_double, convert_int

RateMeyerHaeseler::RateMeyerHaeseler()
 : RateHeterogeneity()
{
	name           = "+M";
	full_name      = "Meyer & von Haeseler (2003)";
	dist_mat       = nullptr;
	phylo_tree     = nullptr;
	rate_file      = nullptr;
	rate_mh        = true;
	rate_tolerance = TOL_SITE_RATE;
	rate_maximum   = MAX_SITE_RATE;
	rate_minimum   = MIN_SITE_RATE;
}

RateMeyerHaeseler::RateMeyerHaeseler(char *file_name, PhyloTree *tree, bool rate_type)
 : RateMeyerHaeseler()
{
	//name, full_name, dist_mat, phylo_tree, rate_file, rate_mh,
	//rate_tolerance, rate_minimum are all set by the
	//no-parameter constructor.
	rate_mh   = rate_type;
	setTree(tree);
	if (!rate_mh) {
		name="+CAT";
		full_name = "Stamatakis (2007) experimental";
	}
}

RateMeyerHaeseler::RateMeyerHaeseler(int dummy_rates, PhyloTree* tree, 
                                     PhyloTree* report_to_tree) 
	: RateMeyerHaeseler() {
	setTree(tree);
}

void RateMeyerHaeseler::readRateFile(char *rate_file_to_read) {
	cout << "Reading site-specific rate file " << rate_file_to_read << " ..." << endl;
	try {
		ifstream in;
		in.exceptions(ios::failbit | ios::badbit);
		in.open(rate_file_to_read);
		char   line[256];
		size_t nsites = phylo_tree->aln->getNSite();
		pat_rates.resize(phylo_tree->aln->getNPattern(), -1.0);
		int    saturated_sites = 0;
		int    saturated_ptn = 0;

		in.getline(line, sizeof(line));
		//if (strncmp(line, "Site", 4) != 0) throw "Wrong header line";

		for (size_t i = 0; i < nsites; ++i) {
			in.getline(line, sizeof(line));
			stringstream ss(line);
			string tmp;
			ss >> tmp;
			int site;
			site = convert_int(tmp.c_str());
            if (site <= 0 || site > nsites) {
                throw "Wrong site number (must be between 1 and #sites)";
            }
			site--;
			ss >> tmp;
			double rate;
			rate = convert_double(tmp.c_str());
			if (rate < 0.0) throw "Negative rate not allowed";
			if (rate <= 0.0) rate = rate_minimum;
			int ptn = phylo_tree->aln->getPatternID(site);
			if (rate >= rate_maximum) {
				rate = rate_maximum; 
				saturated_sites += phylo_tree->aln->at(ptn).frequency; 
				saturated_ptn ++;
			}
			pat_rates.at(ptn) = rate;
		}
		in.clear();
		// set the failbit again
		in.exceptions(ios::failbit | ios::badbit);
		in.close();

		for (size_t i = 0; i < pat_rates.size(); ++i) {
			if (pat_rates.at(i) < 0.0) {
				throw "Some site has no rate information";
			}
		}

		if (saturated_sites) {
			stringstream str;
			str << saturated_sites << " sites"
                << " (" << saturated_ptn << " patterns)"
                << " show too high rates (>=" << rate_maximum << ")";
			outWarning(str.str());
		}
	} catch (const char *str) {
		outError(str);
	} catch (string& str) {
		outError(str);
	} catch(ios::failure) {
		outError(ERR_READ_INPUT);
	}
}

RateMeyerHaeseler::~RateMeyerHaeseler()
{
    if (dist_mat) {
		delete [] dist_mat;
		dist_mat = nullptr;
	}
}

int RateMeyerHaeseler::getNDim() const {
    if (phylo_tree) {
        return static_cast<int>(phylo_tree->aln->getNPattern())-1;
    }
    if (pat_rates.empty()) {
        return 0;
    }
    return static_cast<int>(pat_rates.size())-1;
}

double RateMeyerHaeseler::getPtnRate(int ptn) {
	if (ptn < pat_rates.size()) {
		return pat_rates.at(ptn);
	}
	return 1.0;
}

int RateMeyerHaeseler::computePatternRates(DoubleVector &pattern_rates,
                                           IntVector &pattern_cat) {
	pattern_rates.insert(pattern_rates.begin(), pat_rates.begin(), pat_rates.end());
    return static_cast<int>(pat_rates.size());
}

void RateMeyerHaeseler::getRates(DoubleVector &rates) const {
	rates.clear();
	if (pat_rates.empty()) {
		rates.resize(phylo_tree->aln->size(), 1.0);
	} else {
		rates.insert(rates.begin(), pat_rates.begin(), pat_rates.end());
	} 
}

void RateMeyerHaeseler::setRates(const DoubleVector &rates) {
	pat_rates = rates;
}

void RateMeyerHaeseler::initializeRates() {

	int rate_id = 0;
	int nseq    = phylo_tree->leafNum;
	int nstate  = phylo_tree->getModel()->num_states;

	if (nseq < 25) { 
		outWarning("Meyer & von Haeseler model"
                   " is not recommended for < 25 sequences\n");
	}
	pat_rates.resize(phylo_tree->aln->getNPattern(), 1.0);

	for (Alignment::iterator pat = phylo_tree->aln->begin();
         pat != phylo_tree->aln->end(); ++pat, ++rate_id) {
		int diff = 0, total = 0;
        for (size_t i = 0; i+1 < nseq; ++i) {
            int state1 = pat->at(i);
            if (state1 < nstate) {
                for (size_t j = i+1; j < nseq; ++j) {
                    int state2 = pat->at(j);
                    if (state2 < nstate) {
                        //total += dist_mat[state1 * nstate + state2];
                        //if (state1 != state2) diff += dist_mat[state1 * nstate + state2];
                        total++;
                        if (state1 != state2) {
                            diff++;
                        }
                    }
                }
            }
        }
		if (diff == 0) diff = 1;
		if (total == 0) total = 1;
		double obs_diff = double(diff) / total;
		double tolog = 1.0 - obs_diff*nstate/(nstate-1);
		if (tolog > 0.0) {
			pat_rates.at(rate_id) = -log(tolog) * (nstate-1) / nstate;
		} else {
			pat_rates.at(rate_id) = obs_diff;
		}
	}
}

void RateMeyerHaeseler::prepareRateML(IntVector &ptn_id) {
	Alignment *aln       = new Alignment();
	aln->extractPatterns      (phylo_tree->aln, ptn_id);
	ptn_tree             = new PhyloTree(aln);
	stringstream ss;
	phylo_tree->printTree      (ss);
	ptn_tree->readTree         (ss, phylo_tree->rooted);
	ptn_tree->setAlignment     (aln);
	ptn_tree->setModelFactory  (phylo_tree->getModelFactory());
	ptn_tree->setModel         (phylo_tree->getModelFactory()->model);
	ptn_tree->setRate          (new RateHeterogeneity());
	ptn_tree->computeLikelihood();
	//cout << optimizing_pattern << " " << lh << endl;
	cur_scale = 1.0;
}

void RateMeyerHaeseler::completeRateML() {
	ptn_tree->setModelFactory(NULL);
	ptn_tree->setModel(NULL);
	//ptn_tree->setRate(NULL);
	delete ptn_tree->aln;
	delete ptn_tree;
	ptn_tree = NULL;
}

double RateMeyerHaeseler::optimizeRate(int pattern) {
	optimizing_pattern  = pattern;
	double max_rate     = rate_maximum;
	double negative_lh;
	double current_rate = pat_rates.at(pattern);
	double ferror, optx;
	/* constant site alway have ZERO rates */
	if (phylo_tree->aln->at(pattern).isConst()) {
		return (pat_rates.at(pattern) = rate_minimum);
	}
	if (!rate_mh) {	
		IntVector ptn_id;
		ptn_id.push_back(optimizing_pattern);
		prepareRateML(ptn_id);
	}
    if (phylo_tree->optimize_by_newton && rate_mh) // Newton-Raphson method 
	{
		optx = optimizeRateNewton(pattern, current_rate,
                                  max_rate, negative_lh, ferror);
    }
    else {
		optx = minimizeOneDimen(rate_minimum, current_rate, max_rate,
                                rate_tolerance, &negative_lh, &ferror);
		double fnew;
		if ((optx < max_rate) && (fnew = computeFunction(max_rate)) <= negative_lh+rate_tolerance) {
			optx = max_rate;
			negative_lh = fnew;
		}
		if ((optx > rate_minimum) && (fnew = computeFunction(rate_minimum)) <= negative_lh+rate_tolerance) {
			optx = rate_minimum;
			negative_lh = fnew;
		}
	}
	//negative_lh = brent(rate_minimum, current_rate, max_rate, 1e-3, &optx);
	if (optx > max_rate*0.99) optx = rate_maximum;
	if (optx < rate_minimum*2) optx = rate_minimum;
	pat_rates.at(pattern) = optx;

	if (!rate_mh) { 
		completeRateML(); 
		return optx; 
	}

	if (optx == rate_maximum || 
		(optx == rate_minimum && !phylo_tree->aln->at(pattern).isConst())) {
		fixUpOptimizedBoundaryRate(pattern, current_rate, max_rate, 
		                           negative_lh, ferror, optx);
	}

	return optx;
}

double RateMeyerHaeseler::optimizeRateNewton(int pattern, double& current_rate,
                                             double& max_rate, double& negative_lh,
											 double& ferror)  {
	double optx = minimizeNewtonSafeMode(rate_minimum, current_rate, max_rate,
                                      rate_tolerance, negative_lh);
	if (optx > rate_maximum*0.99
		|| (optx < rate_minimum*2 && !phylo_tree->aln->at(pattern).isConst()))
	{
		double optx2, negative_lh2;
		optx2 = minimizeOneDimen(rate_minimum, current_rate, max_rate,
								 rate_tolerance, &negative_lh2, &ferror);
		if (negative_lh2 < negative_lh - 1e-4) {
			cout << "+++NEWTON IS WRONG for pattern " << pattern << ": " << optx2 << " " << 
			negative_lh2 << " (Newton: " << optx << " " << negative_lh <<")" << endl;
		}
		if (negative_lh < negative_lh2 - 1e-4 && 
			verbose_mode >= VerboseMode::VB_MED) {
			cout << "Brent is wrong for pattern " << pattern << ": " << optx2 << " " << 
			negative_lh2 << " (Newton: " << optx << " " << negative_lh <<")" << endl;
		}
	}
	return optx;
}

void RateMeyerHaeseler::fixUpOptimizedBoundaryRate(int pattern, double current_rate,
                                                   double& max_rate, double& negative_lh, 
												   double& ferror,   double& optx) {
	ofstream out;
	if (verbose_mode >= VerboseMode::VB_MED)  {
		cout << "Checking pattern " << pattern
				<< " (" << current_rate << ", " << optx << ")" << endl;
		out.open("x", ios::app);
		out << pattern;
	}
	double minf = INFINITY;
	double minx = 0;
	for (double val=0.1; val <= 100; val += 0.1) {
		double f = computeFunction(val);
		
		if (verbose_mode >= VerboseMode::VB_MED) {
			out << " " << f;
		}
		if (f < minf) { 
			minf = f; 
			minx = val; 
		}
		if (verbose_mode < VerboseMode::VB_MED && minf < negative_lh) {
			break;
		}
	}
	if (verbose_mode >= VerboseMode::VB_MED) {
		out << endl;
		out.close();
	}
	//cout << "minx: " << minx << " " << minf << endl;
	if (negative_lh > minf+1e-3) {
		optx = minimizeOneDimen(rate_minimum, minx, max_rate,
								1e-3, &negative_lh, &ferror);
		pat_rates.at(pattern) = optx;
		if (verbose_mode >= VerboseMode::VB_MED) {
			cout << "FIX rate: " << minx << " , " << optx << endl;
		}
	}
}

void RateMeyerHaeseler::optimizeRates() {
	if (!dist_mat) {
        size_t leaves = (size_t)phylo_tree->leafNum;
		dist_mat = new double[leaves * leaves];
	}
	// compute the distance based on the path lengths between taxa of the tree
	phylo_tree->calcDist(dist_mat);
	IntVector ok_ptn;
	ok_ptn.resize(pat_rates.size(), 0);
	double sum = 0.0;
	int i;
	int ok_sites = 0;
	int saturated_sites = 0, saturated_ptn = 0;
	int invar_sites = 0;
	int ambiguous_sites = 0;
	int nseq = phylo_tree->leafNum;
	int nstates = phylo_tree->aln->num_states;
	for (i = 0; i < pat_rates.size(); i++) {
        int freq = phylo_tree->aln->at(i).frequency;
		if (phylo_tree->aln->at(i).computeAmbiguousChar(nstates) <= nseq-2) {
			optimizeRate(i);
			if (pat_rates.at(i) == rate_minimum) invar_sites += freq;
			if (pat_rates.at(i) == rate_maximum) {
				saturated_sites += freq;
				saturated_ptn ++;
			}
		} else { pat_rates.at(i) = rate_minimum; ambiguous_sites += freq; }
		if (pat_rates.at(i) < rate_maximum)
		{
			if (pat_rates.at(i) > rate_minimum) sum += pat_rates.at(i) * freq;
			ok_ptn[i] = 1;
			ok_sites += freq;
		}
	}

	// now scale such that the mean of rates is 1
	double scale_f = ok_sites / sum;
	for (i = 0; i < pat_rates.size(); i++) {
		if (ok_ptn[i] && pat_rates.at(i) > rate_minimum) {
			pat_rates.at(i) *= scale_f;
		}
	}

    if (ambiguous_sites) {
        stringstream str;
        str << ambiguous_sites << " sites contain too many gaps"
            << " or ambiguous characters";
        outWarning(str.str());
    }
	if (saturated_sites) {
		stringstream str;
		str << saturated_sites << " sites"
            << " (" << saturated_ptn << " patterns)"
            << " show too high rates (>=" << rate_maximum << ")";
		outWarning(str.str());
	}
	//cout << invar_sites << " sites have zero rate" << endl;

}

double RateMeyerHaeseler::optimizeParameters(double epsilon, PhyloTree* report_to_tree) {
	ASSERT(phylo_tree);
	double tree_lh = phylo_tree->computeLikelihood();

	DoubleVector prev_rates;
	getRates(prev_rates);

	if (pat_rates.empty()) {
		if (rate_file) {
			readRateFile(rate_file);
			phylo_tree->clearAllPartialLH();
			return phylo_tree->optimizeAllBranches();
		}
		initializeRates();
	}

	optimizeRates();

	
	phylo_tree->clearAllPartialLH();

	stringstream best_tree_string;
	phylo_tree->printTree(best_tree_string, WT_BR_LEN + WT_TAXON_ID);
	double new_tree_lh = phylo_tree->optimizeAllBranches(1);
	//double new_tree_lh = phylo_tree->computeLikelihood();

    if (new_tree_lh < tree_lh - 1e-5) {
        cout << "Worse likelihood (" << new_tree_lh << ")"
             << ", roll back site rates..." << endl;
        setRates(prev_rates);
        phylo_tree->rollBack(best_tree_string);
        //phylo_tree->clearAllPartialLh();
        new_tree_lh = phylo_tree->computeLikelihood();
        //cout << "Backup log-likelihood: " << new_tree_lh << endl;
        new_tree_lh = tree_lh;
    }
    return new_tree_lh;
}


double RateMeyerHaeseler::computeFunction(double value) {
    if (!rate_mh) {
        if (value != cur_scale) {
            ptn_tree->scaleLength(value/cur_scale);
            cur_scale = value;
            ptn_tree->clearAllPartialLH();
        }
        return -ptn_tree->computeLikelihood();
    }
    int nseq = static_cast<int>(phylo_tree->leafNum);
    int nstate = phylo_tree->getModel()->num_states;
    double lh = 0.0;
    ModelSubst *model = phylo_tree->getModel();
    Pattern *pat = & phylo_tree->aln->at(optimizing_pattern);
    int nseqLess1 = nseq - 1;

    #ifdef _OPENMP
    #pragma omp parallel for reduction(-:lh)
    #endif
    for (int i = 0; i < nseqLess1; ++i) {
        int state1 = pat->at(i);
        const double* distRow = dist_mat + i * nseq;
        if (nstate <= state1 ) {
            continue;
        }
        for (size_t j = i+1; j < nseq; ++j) {
            int state2 = pat->at(j);
            if (nstate <= state2) {
                continue;
            }
            lh -= log(model->computeTrans(distRow[j], state1, state2));
        }
    }
	return lh;
}

void RateMeyerHaeseler::computeFuncDerv(double value, double &df, double &ddf) {
    int nseq = phylo_tree->leafNum;
    int nstate = phylo_tree->getModel()->num_states;
    //	double lh = 0.0;
    double trans, derv1, derv2;
    ModelSubst *model = phylo_tree->getModel();
    Pattern *pat = & phylo_tree->aln->at(optimizing_pattern);
    auto nseqLess1 = nseq - 1;

	double dfLocal  = 0.0; //MSVC's OMP insists that variables in reduction
	double ddfLocal = 0.0; //clauses cannot have reference type.

    #ifdef _OPENMP
    #pragma omp parallel for reduction(-:dfLocal,ddfLocal)
    #endif
    for (int i = 0; i < nseqLess1; ++i) {
        int state1 = pat->at(i);
        if (nstate<=state1) {
            continue;
        }
        double* distRow = dist_mat + i*nseq;
        for (size_t j = i+1; j < nseq; ++j) {
            int state2 = pat->at(j);
            if (nstate<=state2) {
                continue;
            }
            double dist = distRow[j];
            trans = model->computeTrans(value * dist, state1, state2,
                                        derv1, derv2);
            //			lh -= log(trans);
            double t1 = derv1 / trans;
            double t2 = derv2 / trans;
            dfLocal  -= t1 * dist;
            ddfLocal -= dist * dist * (t2 - t1*t1);
        }
    }
	df  = dfLocal;
	ddf = ddfLocal;
}

void RateMeyerHaeseler::runIterativeProc(Params &params, IQTree &tree) {
	if (verbose_mode >= VerboseMode::VB_MED) {
		ofstream out("x");
		out.close();
	}
	setTree(&tree);
	RateHeterogeneity *backup_rate = tree.getRate();
	if (backup_rate->getGammaShape() > 0 ) {
		IntVector pattern_cat;
		backup_rate->computePatternRates(pat_rates, pattern_cat);
		double   sum    = 0.0;
        intptr_t seqLen = static_cast<intptr_t>(pat_rates.size());
        auto     freq   = phylo_tree->getConvertedSequenceFrequencies();
        if (freq!=nullptr && seqLen == phylo_tree->getConvertedSequenceLength()) {
            #ifdef _OPENMP
            #pragma omp parallel for reduction(+:sum)
            #endif
            for (intptr_t i = 0; i < seqLen ; ++i) {
                sum += pat_rates.at(i) * freq[i];
            }
        } else {
            for (intptr_t i = 0; i < seqLen; ++i) {
                sum += pat_rates.at(i) * phylo_tree->aln->at(i).frequency;
            }
        }
		sum /=  phylo_tree->aln->getNSite();
		if (fabs(sum - 1.0) > 0.0001) {
            if (verbose_mode >= VerboseMode::VB_MED) {
                cout << "Normalizing Gamma rates (" << sum << ")" << endl;
            }
            for (size_t i = 0; i < pat_rates.size(); ++i) {
                pat_rates.at(i) /= sum;
            }
        }
	}
	tree.getModelFactory()->site_rate = this;
	tree.setRate(this);

	//if  (empty()) initializeRates();

    //setRates(prev_rates);
    //string rate_file = params.out_prefix;
    //rate_file += ".mhrate";
    double prev_lh   = tree.getCurScore();
    string dist_file = params.out_prefix;
    dist_file       += ".tdist";
    tree.getModelFactory()->stopStoringTransMatrix();

	int i = 2;
	for (; i < 100; ++i) {
		//DoubleVector prev_rates;
		//getRates(prev_rates);
		//writeSiteRates(prev_rates, rate_file.c_str());
		tree.setCurScore(optimizeParameters(0.0, &tree));
		tree.setCurScore(tree.optimizeAllBranches(i));
		cout << "Current Log-likelihood: " << tree.getCurScore() << endl;
		if (tree.getCurScore() <= prev_lh + 1e-4) {
			break;
		}
		prev_lh = tree.getCurScore();
	}
	cout << "Optimization took " << i-1 << " rounds to finish" << endl;
	tree.getModelFactory()->startStoringTransMatrix();
	//tree.getModelFactory()->site_rate = backup_rate;
	//tree.setRate(backup_rate);
}

bool RateMeyerHaeseler::isOptimizingProportions() const     { return false; }
bool RateMeyerHaeseler::isOptimizingRates      () const     { return true;  }
bool RateMeyerHaeseler::isOptimizingShapes     () const     { return false; }

void RateMeyerHaeseler::sortUpdatedRates       ()           { }
void RateMeyerHaeseler::setFixProportions      (bool fix)   { } //means nothing
void RateMeyerHaeseler::setFixRates            (bool fix)   { } //means nothing
void RateMeyerHaeseler::setRateTolerance       (double tol) {
    ASSERT(0<tol);
    rate_tolerance = tol;
}