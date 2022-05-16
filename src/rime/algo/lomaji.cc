#include <boost/algorithm/string/predicate.hpp>
#include <rime/algo/lomaji.h>
#include <rime/config.h>

namespace rime {

bool Lomaji::BueKamLomaji(const std::string& text) {
  const std::string pi[] = {
    "À", "Á", "Â", "Ă",
    "È", "É", "Ê", "Ĕ",
    "Ì", "Í", "Î", "Ĭ",
    "Ò", "Ó", "Ô", "Ŏ",
    "Ù", "Ú", "Û", "Ŭ",
    "à", "á", "â", "ă",
    "è", "é", "ê", "ĕ",
    "ì", "í", "î", "ĭ",
    "ò", "ó", "ô", "ŏ",
    "ù", "ú", "û", "ŭ",
    "Ā", "ā",
    "Ē", "ē", "Ě", "ě",
    "Ī", "ī", "ı",
    "Ń", "ń", "Ň", "ň",
    "Ō", "ō", "Ő", "ő",
    "Ū", "ū", "Ű", "ű",
    "Ǎ", "ǎ",
    "Ǐ", "ǐ",
    "Ǒ", "ǒ",
    "Ǔ", "ǔ",
    "Ǹ", "ǹ",
    "Ḿ", "ḿ",
    "̀", "̂", "̄", "̋", "̌", "◌̆",
    "̍", "͘",
    "ⁿ",
    "Ṳ", "ṳ",
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


bool Lomaji::ThauKamLomaji(const std::string& text) {
  const std::string pi[] = {

    "À", "Á", "Â", "Ă",
    "È", "É", "Ê", "Ĕ",
    "Ì", "Í", "Î", "Ĭ",
    "Ò", "Ó", "Ô", "Ŏ",
    "Ù", "Ú", "Û", "Ŭ",
    "à", "á", "â", "ă",
    "è", "é", "ê", "ĕ",
    "ì", "í", "î", "ĭ",
    "ò", "ó", "ô", "ŏ",
    "ù", "ú", "û", "ŭ",
    "Ā", "ā",
    "Ē", "ē", "Ě", "ě",
    "Ī", "ī", "ı",
    "Ń", "ń", "Ň", "ň",
    "Ō", "ō", "Ő", "ő",
    "Ū", "ū", "Ű", "ű",
    "Ǎ", "ǎ",
    "Ǐ", "ǐ",
    "Ǒ", "ǒ",
    "Ǔ", "ǔ",
    "Ǹ", "ǹ",
    "Ḿ", "ḿ",
    "̀", "̂", "̄", "̋", "̌", "◌̆",
    "̍", "͘",
    "ⁿ",
    "Ṳ", "ṳ",
    "ˊ", "ˋ", "ˇ", "^", "+",
  };
  size_t len = sizeof(pi)/sizeof(pi[0]);
  for(size_t i=0; i< len; i++) {
    if (boost::algorithm::starts_with(text, pi[i])) {
      return true;
    }
  }
  // Siann-bó
  return isalpha(text.front());
}

std::string Lomaji::lianjihu_ = "-";

// Sentence
bool Lomaji::KamAiLianjihu(const std::string& ting_text,
                    const std::string& tsit_text) {
  if (ting_text.empty() || ting_text == " ") {
    return false;
  }

  bool phuann;
  bool ting_kam_lomaji = BueKamLomaji(ting_text);
  bool tsit_kam_lomaji = ThauKamLomaji(tsit_text);

  phuann = ting_kam_lomaji && tsit_kam_lomaji;

  return phuann;
}

void Lomaji::SetLianjihu(const std::string lianjihu) {
  lianjihu_ = lianjihu;
}

std::string Lomaji::GetLianjihu() {
  return lianjihu_;
}

}
