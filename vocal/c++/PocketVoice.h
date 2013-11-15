#ifndef DEF_POCKET_VOICE
#define DEF_POCKET_VOICE

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <sphinxbase/cont_ad.h>
#include <pocketsphinx.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <exception>
#include <iostream>
#include <string>
#include <map>
#include <cstdlib>

static void sleep_msec(int32 ms);

class PocketVoice {
    public:
        PocketVoice();
        ~PocketVoice();
        int init(std::string configEs, std::string jsgf, std::string hmm, std::string lm, std::string dict);
        void calibrateListener();
        int loadConfigCommands(const std::string configFile);
        std::string recognize();
        int execute(std::string command);
        int synthetize(std::string sentence);

    protected:
        ps_decoder_t *_ps;
        cmd_ln_t *_config;
        ad_rec_t *_ad;
        cont_ad_t *_cont;
        std::map<std::string, std::string> commands;
};

#endif
