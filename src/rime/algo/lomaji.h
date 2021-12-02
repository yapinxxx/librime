#ifndef LOMAJI_H_
#define LOMAJI_H_

namespace rime {

class Lomaji {
    protected:
        Lomaji();
        bool KamAiLianJiHu(const std::string& ting_text,
                           const std::string& tsit_text) const;

        std::string lianjihu;
};

} // end namespace

#endif  // LOMAJI_H_
