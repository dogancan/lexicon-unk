// fstphirhocompose.cc

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: Dogan Can (Copyright 2014 - University of Southern California)
// Based on OpenFst fstcompose tool
//
// \file
// Composes two FSTs in tropical semiring.
// Second FST can have phi or rho transitions.

#include "fst/fstlib.h"

DEFINE_int32(phi_label, 1, "Phi label");
DEFINE_int32(rho_label, 2, "Rho label");

namespace fst {
  
  template<typename Arc>
  void PhiRhoCompose(const Fst<Arc> &fst1,
                     const Fst<Arc> &fst2,
                     MutableFst<Arc> *ofst,
                     typename Arc::Label phi_label,
                     typename Arc::Label rho_label) {
    typedef PhiMatcher<SortedMatcher<Fst<Arc> > > PM;
    typedef RhoMatcher<PM> RPM;
  
    CacheOptions copts;
    copts.gc_limit = 0;  // Cache only the last state for fastest copy.
    ComposeFstOptions<Arc, RPM> opts(copts);
    opts.matcher1 = new RPM(fst1, MATCH_NONE, kNoLabel);
    PM * phi_matcher = new PM(fst2, MATCH_INPUT, phi_label,
                              MATCHER_REWRITE_NEVER);
    opts.matcher2 = new RPM(fst2, MATCH_INPUT, rho_label,
                            MATCHER_REWRITE_NEVER, phi_matcher);
    *ofst = ComposeFst<Arc>(fst1, fst2, opts);
    Connect(ofst);
  }

}


int main(int argc, char **argv) {

  string usage = "Composes two FSTs in tropical semiring. ";
  usage += "Second FST can have phi and rho transitions.\n\n  Usage: ";
  usage += argv[0];
  usage += " in1.fst in2.fst [out.fst]\n";

  std::set_new_handler(FailedNewHandler);
  SET_FLAGS(usage.c_str(), &argc, &argv, true);
  if (argc < 3 || argc > 4) {
    ShowUsage();
    return 1;
  }

  string in1_name = strcmp(argv[1], "-") != 0 ? argv[1] : "";
  string in2_name = (argc > 2 && (strcmp(argv[2], "-") != 0)) ? argv[2] : "";
  string out_name = argc > 3 ? argv[3] : "";

  if (in1_name.empty() && in2_name.empty()) {
    LOG(ERROR) << argv[0] << ": Can't take both inputs from standard input.";
    return 1;
  }

  fst::Fst<fst::StdArc> *ifst1 = fst::Fst<fst::StdArc>::Read(in1_name);
  if (!ifst1) return 1;

  fst::Fst<fst::StdArc> *ifst2 = fst::Fst<fst::StdArc>::Read(in2_name);
  if (!ifst2) return 1;

  fst::StdVectorFst ofst;

  fst::PhiRhoCompose(*ifst1, *ifst2, &ofst, FLAGS_phi_label, FLAGS_rho_label);

  ofst.Write(out_name);

  return 0;
}
