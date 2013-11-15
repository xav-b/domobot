#ifndef DEF_POCKET_VOICE
#define DEF_POCKET_VOICE

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <sphinxbase/cont_ad.h>
#include <pocketsphinx.h>

#include <iostream>
#include <string>
#include <cstdlib>

static void sleep_msec(int32 ms);

class PocketVoice {
    public:
        PocketVoice();
        ~PocketVoice();
        int init(std::string configEs, std::string jsgf, std::string hmm, std::string lm, std::string dict);
        void calibrateListener();
        void recognize();
        void synthetize(std::string sentence);

    protected:
        ps_decoder_t *_ps;
        cmd_ln_t *_config;
        ad_rec_t *_ad;
        cont_ad_t *_cont;
};

#endif
