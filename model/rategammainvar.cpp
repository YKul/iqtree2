/***************************************************************************
 *   Copyright (C) 2009-2015 by                                            *
 *   BUI Quang Minh <minh.bui@univie.ac.at>                                *
 *   Lam-Tung Nguyen <nltung@gmail.com>                                    *
 *                                                                         *
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
#include "rategammainvar.h"

RateGammaInvar::RateGammaInvar(int ncat, double shape, bool median,
                               double p_invar_sites, string optimize_alg,
                               PhyloTree *tree, bool testParamDone) :
        RateGamma(ncat, shape, median, tree), 
        invar(p_invar_sites, tree) {
	name = "+I" + name;
	full_name = "Invar+" + full_name;
    this->optimize_alg = optimize_alg;
    cur_optimize = 0;
    this->testParamDone = testParamDone;
    for (int cat = 0; cat < ncategory; cat++) {
        rates[cat] = 1.0 / (1.0 - invar.getPInvar());
    }
	computeRates();
}

void RateGammaInvar::setPInvar(double pInvar) {
    invar.setPInvar(pInvar);
    for (int cat = 0; cat < ncategory; cat++)
        rates[cat] = 1.0/(1.0-pInvar);
    computeRates();
}

void RateGammaInvar::startCheckpoint() {
    checkpoint->startStruct("RateGammaInvar");
}

void RateGammaInvar::saveCheckpoint() {
    invar.setCheckpoint(checkpoint);
    invar.saveCheckpoint();
    super::saveCheckpoint();
}

void RateGammaInvar::restoreCheckpoint() {
    //should restore p_invar first before gamma,
    //because RateGamma will call computeRates()
    invar.setCheckpoint(checkpoint);
    invar.restoreCheckpoint();
    for (int cat = 0; cat < ncategory; cat++) {
        rates[cat] = 1.0 / (1.0 - invar.getPInvar());
    }
    super::restoreCheckpoint();
}

void RateGammaInvar::setNCategory(int ncat) {
	super::setNCategory(ncat);
    for (int cat = 0; cat < ncategory; cat++) {
        rates[cat] = 1.0 / (1.0 - invar.getPInvar());
    }
	name = "+I" + name;
	full_name = "Invar+" + full_name;
	computeRates();
}

std::string RateGammaInvar::getNameParams() const {
	return invar.getNameParams() + super::getNameParams();
}

double RateGammaInvar::computeFunction(double value) {
	if (cur_optimize == 0)
		gamma_shape = value;
	else {
		setPInvar(value);
    }
	// need to compute rates again if p_inv or Gamma shape changes!
	computeRates();
	phylo_tree->clearAllPartialLH();
	return -phylo_tree->computeLikelihood();
}

void RateGammaInvar::writeInfo(ostream &out) {
	invar.writeInfo(out);
	super::writeInfo(out);
}

void RateGammaInvar::writeParameters(ostream &out) {
	invar.writeParameters(out);
	super::writeParameters(out);
}

void RateGammaInvar::setVariables(double *variables) {
	super::setVariables(variables);
	int gid = super::getNDim();
	invar.setVariables(variables+gid);
}

bool RateGammaInvar::getVariables(double *variables) {
	int  gid     = super::getNDim();
	bool changed = super::getVariables(variables);
	changed     |= invar.getVariables(variables+gid);
    if (changed) {
        // need to compute rates again if p_inv or Gamma shape changes!
        super::computeRates();
    }
    return changed;
}

double RateGammaInvar::targetFunk(double x[]) {
	ASSERT(phylo_tree);
	getVariables(x);
	phylo_tree->clearAllPartialLH();
	return -phylo_tree->computeLikelihood();
}

void RateGammaInvar::setBounds(double *lower_bound, double *upper_bound,
                               bool *bound_check) {
	int gid = super::getNDim();
	super::setBounds(lower_bound, upper_bound, bound_check);
	invar.setBounds     (lower_bound+gid, upper_bound+gid, bound_check+gid);
}

double RateGammaInvar::optimizeParameters(double gradient_epsilon,
                                          PhyloTree* report_to_tree) {

	int ndim = getNDim();

	// return if nothing to be optimized
    if (ndim == 0) {
        return phylo_tree->computeLikelihood();
    }
    TREE_LOG_LINE(*report_to_tree, VerboseMode::VB_MED, 
                  "Optimizing " << name << " model parameters" 
                  " by " << optimize_alg << " algorithm...");

	if (optimize_alg.find("EM_RR") != string::npos) {
        return randomRestartOptimization(gradient_epsilon, report_to_tree);
    } else if (optimize_alg.find("Brent") != string::npos || 
               phylo_tree->aln->frac_const_sites == 0.0 || 
               isFixPInvar() || isFixGammaShape()) {
		double lh = phylo_tree->computeLikelihood();
		cur_optimize = 0;
		double gamma_lh = super::optimizeParameters(gradient_epsilon, report_to_tree);
		ASSERT(gamma_lh >= lh-0.1);
		cur_optimize = 1;
		double invar_lh = -DBL_MAX;
        invar_lh = invar.optimizeParameters(gradient_epsilon, report_to_tree);
		ASSERT(invar_lh >= gamma_lh-0.1);
        cur_optimize = 0;
        return invar_lh;
	} else if (optimize_alg.find("EM") != string::npos) {
        return optimizeWithEM(gradient_epsilon, report_to_tree);
    } else if (optimize_alg.find("BFGS") != string::npos) {
        double* variables = new double[ndim+1];
        double* upper_bound = new double[ndim+1];
        double* lower_bound = new double[ndim+1];
        bool*   bound_check = new bool[ndim+1];
        double score;

        // by BFGS algorithm
        setVariables(variables);
        setBounds(lower_bound, upper_bound, bound_check);

        score = -minimizeMultiDimen(variables, ndim, lower_bound, upper_bound,
                                    bound_check, max(gradient_epsilon, TOL_GAMMA_SHAPE));

        getVariables(variables);

        phylo_tree->clearAllPartialLH();
        score = phylo_tree->computeLikelihood();

        delete [] bound_check;
        delete [] lower_bound;
        delete [] upper_bound;
        delete [] variables;

        return score;
    } else {
        string errMsg = "Unknown optimization algorithm: " + optimize_alg;
        outError(errMsg.c_str());
        return 0.0;
    }
}


int RateGammaInvar::computePatternRates(DoubleVector &pattern_rates,
                                        IntVector &pattern_cat) {
	//cout << "Computing Gamma site rates by empirical Bayes..." << endl;

	phylo_tree->computePatternLhCat(WSL_RATECAT);

	auto npattern = phylo_tree->aln->getNPattern();
	pattern_rates.resize(npattern);
	pattern_cat.resize(npattern);

    double *lh_cat = phylo_tree->tree_buffers._pattern_lh_cat;
	for (int i = 0; i < npattern; i++) {
		double sum_rate = 0.0, sum_lh = phylo_tree->ptn_invar[i];
		int best = 0;
        double best_lh = phylo_tree->ptn_invar[i];
		for (int c = 0; c < ncategory; c++) {
			sum_rate += rates[c] * lh_cat[c];
			sum_lh += lh_cat[c];
			if (lh_cat[c] > best_lh
                || (lh_cat[c] == best_lh && 
                    random_double()<0.5)) { // break tie at random
                best = c+1;
                best_lh = lh_cat[c];
            }
		}
		pattern_rates[i] = sum_rate / sum_lh;
		pattern_cat[i] = best;
		lh_cat += ncategory;
	}
    return ncategory+1;
}

double RateGammaInvar::optimizeWithEM(double gradient_epsilon,
                                      PhyloTree* report_to_tree) {
    double curlh = phylo_tree->computeLikelihood();

    cur_optimize = 0;
    double gamma_lh = super::optimizeParameters(gradient_epsilon, report_to_tree);
    if (!phylo_tree->params->ignore_any_errors) {
        ASSERT(gamma_lh > curlh - 1.0);
    }
    curlh = gamma_lh;

    size_t ncat = getNRate();
    intptr_t nptn = phylo_tree->aln->getNPattern();
    size_t nSites = phylo_tree->aln->getNSite();

    // Compute the pattern likelihood for each category
    // (invariable and variable category)
    phylo_tree->computePatternLhCat(WSL_RATECAT);
    phylo_tree->computePtnInvar();

    double ppInvar = 0;
    for (intptr_t ptn = 0; ptn < nptn; ptn++) {
        double *this_lk_cat = phylo_tree->tree_buffers._pattern_lh_cat + ptn * ncat;
        double lk_ptn = phylo_tree->ptn_invar[ptn];
        for (size_t cat = 0; cat < ncat; cat++) {
            lk_ptn += this_lk_cat[cat];
        }
        ASSERT(lk_ptn != 0.0);
        ppInvar += (phylo_tree->ptn_invar[ptn]) * phylo_tree->ptn_freq[ptn] / lk_ptn;
    }

    double newPInvar = ppInvar / nSites;
    ASSERT(newPInvar < 1.0);
    invar.setPInvar(newPInvar);
    phylo_tree->clearAllPartialLH();
//    phylo_tree->scaleLength((1-newPInvar)/(1-curPInv));
    double pinvLH = phylo_tree->computeLikelihood();
    ASSERT(pinvLH > curlh - 1.0);
    return pinvLH;
}

double RateGammaInvar::randomRestartOptimization(double gradient_epsilon,
                                                 PhyloTree* report_to_tree) {
    if (testParamDone) {
        return optimizeWithEM(gradient_epsilon, report_to_tree);
    }
    double frac_const   = phylo_tree->aln->frac_const_sites;
    double bestLogl     = phylo_tree->getCurScore();
    double bestAlpha    = 0.0;
    double bestPInvar   = 0.0;
    double testInterval = (frac_const - invar.getMinimumProportion()*2) / 10;
    double initPInv     = invar.getMinimumProportion();
    double initAlpha    = getGammaShape();

    while (initPInv <= frac_const) {
        if (verbose_mode >= VerboseMode::VB_MED) {
            cout << endl;
            cout << "Testing with init. pinv = " << initPInv
                 << " / init. alpha = " << initAlpha << endl;
        }
        setPInvar(initPInv);
        setGammaShape(initAlpha);
        phylo_tree->clearAllPartialLH();
        double logl = optimizeWithEM(gradient_epsilon, report_to_tree);
        double estAlpha = getGammaShape();
        double estPInv = getPInvar();

        if (verbose_mode >= VerboseMode::VB_MED) {
            cout << "Est. alpha: " << estAlpha << " / Est. pinv: " << estPInv
            << " / Logl: " << logl << endl;
        }

        initPInv = initPInv + testInterval;

        if (logl > bestLogl) {
            bestLogl = logl;
            bestAlpha = estAlpha;
            bestPInvar = estPInv;
        }
    }

    if (verbose_mode >= VerboseMode::VB_MED) {
        cout << "Best gamma shape: " << bestAlpha
             << " / best p_inv: " << bestPInvar
             << " / logl: " << bestLogl << endl;
    }

    setPInvar(bestPInvar);
    setGammaShape(bestAlpha);
    phylo_tree->clearAllPartialLH();
    testParamDone = true;
    return phylo_tree->computeLikelihood();
}


double RateGammaInvar::meanRates() const {
	double ret = 0.0;
	for (int i = 0; i < ncategory; i++)
		ret += rates[i];
    ret *= (1.0-getPInvar())/ncategory;
	return ret;
}

/**
 * rescale rates s.t. mean rate is equal to 1, useful for FreeRate model
 * @return rescaling factor
 */
double RateGammaInvar::rescaleRates() {
	double norm = meanRates();
	for (int i = 0; i < ncategory; i++)
		rates[i] /= norm;
	return norm;
}




