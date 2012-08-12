#include <pocketsphinx.h>

int main(int argc, char* argv[])
{
    ps_decoder_t *ps;
    cmd_ln_t *config;
    FILE *fh;
    int rv;
    char const *hyp, *uttid;
    int32 score;
    int16 buf[512];

    config = cmd_ln_init(NULL, ps_args(), TRUE,
                        "-hmm", MODELDIR "/hmm/en_US/hub4wsj_sc_8k",
                        "-lm", MODELDIR "/lm/en/turtle.DMP",
                        "-dict", MODELDIR "/lm/en/turtle.dic",
                        NULL);
    if ( config == NULL )
        return 1;
    
    ps = ps_init(config);
    if ( ps == NULL )
        return 1;

    // Audio file for test
    fh = fopen("goforward.raw", "rb");
    if ( fh == NULL ) {
        perror("Failed to open file");
        return 1;
    }
    
    // Decoding
    rv = ps_decode_raw(ps, fh, "goforward", -1);
    if ( rv < 0 )
        return 1;
    // Hypothesis
    hyp = ps_get_hyp(ps, &score, &uttid);
    if ( hyp == NULL )
        return 1;
    printf("Recognized: %s (%d %s)\n", hyp, score, uttid);

    /* Do it again, from block memory
    fseek(fh, 0, SEEK_SET);
    while ( !feof(fh) ) {
        size_t nsamp;
        nsamp = fread(buf, 2, 512, fh);
        rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
    }
    // End of the utterance
    rv = ps_end_utt(ps);
    if ( rv < 0 )
        return 1;
    // Retrieving hypothesis
    hyp = ps_get_hyp(ps, &score, &uttid);
    if ( hyp == NULL )
        return 1;
    printf("Recognized: %s (%d %s)\n", hyp, score, uttid);*/

    // Cleaning up
    fclose(fh);
    ps_free(ps);
    return 0;
}
