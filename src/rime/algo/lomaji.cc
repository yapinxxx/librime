#include <boost/algorithm/string/predicate.hpp>
#include <rime/algo/lomaji.h>
#include <rime/config.h>

namespace rime {

bool BueKamLomaji(const std::string& text) {
  const std::string pi[] = {
    "ˊ", "ˋ", "ˇ", "^", "+",
  };
  size_t len = sizeof(pi)/sizeof(pi[0]);
  for(size_t i=0; i< len; i++) {
    if (boost::algorithm::ends_with(text, pi[i])) {
      return true;
    }
  }
  // p, t, k, h, r, g
  return isalpha(text.back());
}


bool ThauKamLomaji(const std::string& text) {
  // const std::string pi[] = {
  //   "ˊ", "ˋ", "ˇ", "^", "+",
  // };
  // size_t len = sizeof(pi)/sizeof(pi[0]);
  // for(size_t i=0; i< len; i++) {
  //   if (boost::algorithm::starts_with(text, pi[i])) {
  //     return true;
  //   }
  // }
  // Siann-bó
  return isalpha(text.front());
}

std::string Lomaji::lianjihu_ = "-";

// Sentence
bool Lomaji::KamAiLianJiHu(const std::string& ting_text,
                    const std::string& tsit_text) const {
  if (ting_text.empty() || ting_text == " ") {
    return false;
  }

  bool phuann;
  bool ting_kam_lomaji = BueKamLomaji(ting_text);
  bool tsit_kam_lomaji = ThauKamLomaji(tsit_text);

  phuann = ting_kam_lomaji && tsit_kam_lomaji;

  return phuann;
}

void Lomaji::set_lianjihu(const std::string lianjihu) {
  lianjihu_ = lianjihu;
}

std::string Lomaji::get_lianjihu() {
  return lianjihu_;
}

}
