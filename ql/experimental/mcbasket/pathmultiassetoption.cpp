/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2008 Andrea Odetti

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/experimental/mcbasket/pathmultiassetoption.hpp>
#include <ql/processes/stochasticprocessarray.hpp>
#include <ql/termstructures/volatility/equityfx/blackconstantvol.hpp>
#include <ql/math/solvers1d/brent.hpp>

namespace QuantLib {

    PathMultiAssetOption::PathMultiAssetOption(
                          const boost::shared_ptr<StochasticProcess>& process,
                          const boost::shared_ptr<PricingEngine>&     engine)
        : stochasticProcess_(process) {
        if (engine)
            setPricingEngine(engine);

        registerWith(stochasticProcess_);
    }

    bool PathMultiAssetOption::isExpired() const {
        return fixingDates().back() < Settings::instance().evaluationDate();
    }

    void PathMultiAssetOption::setupExpired() const {
        NPV_ = 0.0;
    }

    void PathMultiAssetOption::setupArguments(PricingEngine::arguments* args)
                                                                       const {
        PathMultiAssetOption::arguments* arguments =
            dynamic_cast<PathMultiAssetOption::arguments*>(args);

        QL_REQUIRE(arguments != 0, "wrong argument type");

        QL_REQUIRE(stochasticProcess_->size() == numberOfAssets(),
                   "inconsistent sizes");

        arguments->stochasticProcess = stochasticProcess_;
        arguments->payoff            = pathPayoff();
        arguments->fixingDates       = fixingDates();
    }

    void PathMultiAssetOption::arguments::validate() const {
        QL_REQUIRE(payoff,                 "no payoff given");
        QL_REQUIRE(fixingDates.size() > 0, "no dates given");
        QL_REQUIRE(stochasticProcess,      "no process given");
    }

}
