#include "PocketVoice.h"

using namespace std;

static void sleep_msec(int32 ms) {
    struct timeval tmo;
    tmo.tv_sec = 0;
    tmo.tv_usec = ms * 1000;
    select(0, NULL, NULL, NULL, &tmo);
}

PocketVoice::PocketVoice() {
}

PocketVoice::~PocketVoice() {
	cont_ad_close(_cont);
    ad_close(_ad);
	ps_free(_ps);
}

int PocketVoice::init(string configEs, string jsgf,
                string hmm = "/usr/local/share/pocketsphinx/model/hmm/en_US/hub4wsj_sc_8k",
                string lm = "/usr/local/share/pocketsphinx/model/lm/en_US/wsj0vp.5000.DMP",
                string dict = "/usr/local/share/pocketsphinx/model/lm/en_US/cmu07a.dic") {
	if ( configEs != "" ) 
		_config = cmd_ln_parse_file_r(NULL, ps_args(), configEs.c_str(), FALSE);
	else {
		_config = cmd_ln_init(NULL, ps_args(), TRUE,
                            "-hmm", hmm.c_str(),
                            "-lm", lm.c_str(),
                            "-dict", dict.c_str(),
                            "-jsgf", jsgf.c_str(),
                            NULL);
	}
   
    if ( _config == NULL )
        return 1;
    
    _ps = ps_init(_config);
    if ( _ps == NULL ) {
      printf("Unable to allocate decoder.\n");
      return 2;
    }
    return 0;
}

void PocketVoice::calibrateListener() {
	if ((_ad = ad_open_dev(cmd_ln_str_r(_config, "-adcdev"),
                          (int)cmd_ln_float32_r(_config, "-samprate"))) == NULL)
        E_FATAL("Failed top open audio device\n");

    /* Initialize continuous listening module */
    if ((_cont = cont_ad_init(_ad, ad_read)) == NULL)
        E_FATAL("Failed to initialize voice activity detection\n");
    if (ad_start_rec(_ad) < 0)
        E_FATAL("Failed to start recording\n");
    if (cont_ad_calib(_cont) < 0)
        E_FATAL("Failed to calibrate voice activity detection\n");
}

void PocketVoice::recognize() {
    int16 adbuf[4096];
    int32 k, ts, rem;
    char const *hyp;
    char const *uttid;
    char word[256];
	int32 score;
	
	/* Indicate listening for next utterance */
        printf("READY....\n");
        fflush(stdout);
        fflush(stderr);

        /* Wait data for next utterance */
        while ((k = cont_ad_read(_cont, adbuf, 4096)) == 0)
            sleep_msec(100);

        if (k < 0)
            E_FATAL("Failed to read audio\n");

        /*
         * Non-zero amount of data received; start recognition of new utterance.
         * NULL argument to uttproc_begin_utt => automatic generation of utterance-id.
         */
        if (ps_start_utt(_ps, NULL) < 0)
            E_FATAL("Failed to start utterance\n");
        ps_process_raw(_ps, adbuf, k, FALSE, FALSE);
        printf("Listening...\n");
        fflush(stdout);

        /* Note timestamp for this first block of data */
        ts = _cont->read_ts;

        /* Decode utterance until end (marked by a "long" silence, >1sec) */
        for (;;) {
            /* Read non-silence audio data, if any, from continuous listening module */
            if ((k = cont_ad_read(_cont, adbuf, 4096)) < 0)
                E_FATAL("Failed to read audio\n");
            if (k == 0) {
                /*
                 * No speech data available; check current timestamp with most recent
                 * speech to see if more than 1 sec elapsed.  If so, end of utterance.
                 */
                if ((_cont->read_ts - ts) > DEFAULT_SAMPLES_PER_SEC)
                    break;
            }
            else {
                /* New speech data received; note current timestamp */
                ts = _cont->read_ts;
            }

            /*
             * Decode whatever data was read above.
             */
            rem = ps_process_raw(_ps, adbuf, k, FALSE, FALSE);

            /* If no work to be done, sleep a bit */
            if ((rem == 0) && (k == 0))
                sleep_msec(20);
        }

        /*
         * Utterance ended; flush any accumulated, unprocessed A/D data and stop
         * listening until current utterance completely decoded
         */
        ad_stop_rec(_ad);
        while (ad_read(_ad, adbuf, 4096) >= 0);
        cont_ad_reset(_cont);

        printf("Stopped listening, please wait...\n");
        fflush(stdout);
        /* Finish decoding, obtain and print result */
        ps_end_utt(_ps);
        hyp = ps_get_hyp(_ps, &score, &uttid);
    	if ( hyp == NULL )
        	return;
    	printf("Recognized: %s (%d %s) with prob %d\n", hyp, score, uttid, ps_get_prob(_ps, NULL));
        fflush(stdout);

        /* Exit if the first word spoken was GOODBYE */
        if (hyp) {
            sscanf(hyp, "%s", word);
            if (strcmp(word, "goodbye") == 0)
                std::cout << "Bye !\n";
        }

        /* Resume A/D recording for next utterance */
        if (ad_start_rec(_ad) < 0)
            E_FATAL("Failed to start recording\n");
}

void PocketVoice::synthetize(string sentence) {}
