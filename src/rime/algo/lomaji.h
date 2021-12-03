#ifndef LOMAJI_H_
#define LOMAJI_H_

namespace rime {

class Lomaji {
    public:
        static bool KamAiLianjihu(const std::string& ting_text,
                           const std::string& tsit_text);
        static void SetLianjihu(const std::string lianjihu);
        static std::string GetLianjihu();
    protected:
        static std::string lianjihu_;
    private:
        static bool ThauKamLomaji(const std::string& text);
        static bool BueKamLomaji(const std::string& text);
};

} // end namespace

#endif  // LOMAJI_H_
