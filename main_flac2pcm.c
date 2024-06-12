#include <stdio.h>
#include <stdlib.h>

#include "FLAC/ordinals.h"
#include "FLAC/stream_decoder.h"
#include "FLAC/metadata.h"

#ifndef PRIu64
#define PRIu64 "lu"
#endif

static FLAC__uint64 total_samples = 0;
static unsigned sample_rate = 0;
static unsigned channels = 0;
static unsigned bps = 0;

void metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data);
FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data);
void error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data);


int main(int argc, char *argv[])
{
    /* 输入FLAC文件相关信息 */
    char *in_flac_file_name = NULL;
    FLAC__bool ok = true;
    FLAC__StreamDecoder *decoder = NULL;
    FLAC__StreamDecoderInitStatus init_status = 0;
    /* 输出PCM文件相关信息 */
    char *out_pcm_file_name = NULL;
    FILE *fp_out_pcm = NULL;

    /* 检查参数 */
    if(argc != 3)
    {
        printf("Usage: \n"
                "\t %s <in-flac-file> <out-pcm-file>\n"
                "Examples: \n"
                "\t %s ./audio/out1.flac out_16000_16_1.pcm\n"
                "\t %s ./audio/out2.flac out_44100_16_2.pcm\n"
                , argv[0], argv[0], argv[0]);
        return -1;
    }
    in_flac_file_name = argv[1];
    out_pcm_file_name = argv[2];

    if((fp_out_pcm = fopen(out_pcm_file_name, "wb")) == NULL) {
        fprintf(stderr, "ERROR: opening %s for output\n", out_pcm_file_name);
        return 1;
    }

    if((decoder = FLAC__stream_decoder_new()) == NULL) {
        fprintf(stderr, "ERROR: allocating decoder\n");
        fclose(fp_out_pcm);
        return 1;
    }

    (void)FLAC__stream_decoder_set_md5_checking(decoder, true);

    init_status = FLAC__stream_decoder_init_file(decoder, in_flac_file_name, write_callback, metadata_callback, error_callback, /*client_data=*/fp_out_pcm);
    if(init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK) {
        fprintf(stderr, "ERROR: initializing decoder: %s\n", FLAC__StreamDecoderInitStatusString[init_status]);
        ok = false;
    }

    if(ok) {
        ok = FLAC__stream_decoder_process_until_end_of_stream(decoder);
        fprintf(stderr, "decoding: %s\n", ok? "succeeded" : "FAILED");
        fprintf(stderr, "   state: %s\n", FLAC__StreamDecoderStateString[FLAC__stream_decoder_get_state(decoder)]);
    }

    FLAC__stream_decoder_delete(decoder);
    fclose(fp_out_pcm);

    return 0;
}


static FLAC__bool write_little_endian_uint16(FILE *f, FLAC__uint16 x)
{
    return
        fputc(x, f) != EOF &&
        fputc(x >> 8, f) != EOF
    ;
}
static FLAC__bool write_little_endian_int16(FILE *f, FLAC__int16 x)
{
    return write_little_endian_uint16(f, (FLAC__uint16)x);
}



void metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
    (void)decoder, (void)client_data;

    /* print some stats */
    if(metadata->type == FLAC__METADATA_TYPE_STREAMINFO) {
        /* save for later */
        total_samples = metadata->data.stream_info.total_samples;
        sample_rate = metadata->data.stream_info.sample_rate;
        channels = metadata->data.stream_info.channels;
        bps = metadata->data.stream_info.bits_per_sample;

        fprintf(stderr, "sample rate    : %u Hz\n", sample_rate);
        fprintf(stderr, "channels       : %u\n", channels);
        fprintf(stderr, "bits per sample: %u\n", bps);
        fprintf(stderr, "total samples  : %" PRIu64 "\n", total_samples);
    }
}

FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
    (void)decoder;

    FILE *fp_out_pcm = (FILE*)client_data;

    if(total_samples == 0) {
        fprintf(stderr, "ERROR: this example only works for FLAC files that have a total_samples count in STREAMINFO\n");
        return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
    }
    if(bps != 16) {
        fprintf(stderr, "ERROR: this example only supports 16bit streams\n");
        return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
    }
    if(frame->header.channels == 1 && buffer [0] == NULL) {
        fprintf(stderr, "ERROR: buffer [0] is NULL\n");
        return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
    }
    if(frame->header.channels == 2 && buffer [1] == NULL) {
        fprintf(stderr, "ERROR: buffer [1] is NULL\n");
        return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
    }

    /* write decoded PCM samples */
    for(int i = 0; i < frame->header.blocksize; i++)
    {
        for(int channel_index = 0; channel_index < frame->header.channels; channel_index++)
        {
            if(!write_little_endian_int16(fp_out_pcm, (FLAC__int16)buffer[channel_index][i]))
            {
                fprintf(stderr, "ERROR: write channel(%d) error\n", channel_index);
                return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
            }
        }
    }

    return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

void error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
    (void)decoder, (void)client_data;

    fprintf(stderr, "Got error callback: %s\n", FLAC__StreamDecoderErrorStatusString[status]);
}
