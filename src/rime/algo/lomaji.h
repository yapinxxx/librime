#ifndef LOMAJI_H_
#define LOMAJI_H_

namespace rime {

class Lomaji {
    protected:
        bool KamAiLianJiHu(const std::string& ting_text,
                           const std::string& tsit_text) const;

        static std::string lianjihu_;
    public:
        static void set_lianjihu(const std::string lianjihu);
        static std::string get_lianjihu();
};

} // end namespace

#endif  // LOMAJI_H_
