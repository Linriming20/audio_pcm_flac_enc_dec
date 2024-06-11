#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "FLAC/ordinals.h"
#include "FLAC/stream_encoder.h"
#include "FLAC/metadata.h"

#define READSIZE 1024

#ifndef PRIu64
#define PRIu64 "lu"
#endif

void progress_callback(const FLAC__StreamEncoder *encoder, FLAC__uint64 bytes_written, FLAC__uint64 samples_written, unsigned frames_written, unsigned total_frames_estimate, void *client_data);


int main(int argc, char *argv[])
{
    /* 输入PCM文件相关信息 */
    char *in_pcm_file_name = NULL;
    FILE *fpPcm = NULL;
    int sample_rate = 0;
    int bits_per_sample = 0;
    int channels = 0;
    int total_samples = 0;
    unsigned char *buffer = NULL; // fixbug: 这里需要用 unsigned char 类型或者 FLAC__uint8 ，否则音频有问题
    /* 输出FLAC文件相关信息 */
    char *out_flac_file_name = NULL;
    FLAC__bool ok = true;
    FLAC__StreamEncoder *encoder = NULL;
	FLAC__StreamEncoderInitStatus init_status;
	FLAC__StreamMetadata *metadata[2];
	FLAC__StreamMetadata_VorbisComment_Entry entry;
    //static FLAC__int32 pcm[READSIZE/*samples*/ * 2/*channels*/];
    int32_t *pcm = NULL; // 这里可以用 uint32_t 或者 FLAC__int32

    /* 检查参数 */
    if(argc != 6)
    {
        printf("Usage: \n"
                "\t %s <in-pcm-file> <sample-rate> <bits per sample> <channels> <out-flac-file>\n"
                "Examples: \n"
                "\t %s ./audio/test_16000_16_1.pcm 16000 16 1 out1.flac\n"
                "\t %s ./audio/test_44100_16_2.pcm 44100 16 2 out2.flac\n"
                , argv[0], argv[0], argv[0]);
        return -1;
    }
    in_pcm_file_name = argv[1];
    sample_rate = atoi(argv[2]);
    bits_per_sample = atoi(argv[3]);
    channels = atoi(argv[4]);
    out_flac_file_name = argv[5];
    /* calc audio sample cnt */
    int filesize = 0;
    struct stat statbuff;
    int ret = stat((const char *)in_pcm_file_name, &statbuff);
    if(ret == 0)
    {
        filesize = statbuff.st_size;
    }
    else
    {
        fprintf(stderr, "stat file error(%s).\n", in_pcm_file_name);
        return -1;
    }
    total_samples = filesize/(bits_per_sample/8)/channels; // 同一时间多个通道算一个采样点
    printf("It will encode a PCM file as [file size: %d] [total samples: %d] [sample rate: %d] - [bits per sample: %d] - [channels: %d] !\n", 
            filesize, total_samples, sample_rate, bits_per_sample, channels);
    buffer = (unsigned char *)calloc(READSIZE*(bits_per_sample/8)*channels, 1);
    pcm = (int32_t *)calloc(READSIZE*channels*sizeof(int32_t), 1);

    /* 打开pcm文件 */
    fpPcm = fopen(in_pcm_file_name, "rb");
    if(fpPcm == NULL)
    {
        perror("open pcm file failed!");
        return -1;
    }

	/* allocate the encoder */
    if((encoder = FLAC__stream_encoder_new()) == NULL) {
        fprintf(stderr, "ERROR: allocating encoder\n");
        fclose(fpPcm);
        return 1;
    }

    ok &= FLAC__stream_encoder_set_verify(encoder, true);
    ok &= FLAC__stream_encoder_set_compression_level(encoder, 5);
    ok &= FLAC__stream_encoder_set_channels(encoder, channels);
    ok &= FLAC__stream_encoder_set_bits_per_sample(encoder, bits_per_sample);
    ok &= FLAC__stream_encoder_set_sample_rate(encoder, sample_rate);
    ok &= FLAC__stream_encoder_set_total_samples_estimate(encoder, total_samples);

    /* now add some metadata; we'll add some tags and a padding block */
	if(ok) {
        if((metadata[0] = FLAC__metadata_object_new(FLAC__METADATA_TYPE_VORBIS_COMMENT)) == NULL ||
           (metadata[1] = FLAC__metadata_object_new(FLAC__METADATA_TYPE_PADDING)) == NULL ||
            /* there are many tag (vorbiscomment) functions but these are convenient for this particular use: */
            !FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&entry, "ARTIST", "Some Artist") ||
            !FLAC__metadata_object_vorbiscomment_append_comment(metadata[0], entry, /*copy=*/false) || /* copy=false: let metadata object take control of entry's allocated string */
            !FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair(&entry, "YEAR", "2024") ||
            !FLAC__metadata_object_vorbiscomment_append_comment(metadata[0], entry, /*copy=*/false))
        {
            fprintf(stderr, "ERROR: out of memory or tag error\n");
            ok = false;
		}
        else
        {
			metadata[1]->length = 1234; /* set the padding length */
			ok = FLAC__stream_encoder_set_metadata(encoder, metadata, 2);
        }
	}

    /* initialize encoder */
    if(ok) {
        init_status = FLAC__stream_encoder_init_file(encoder, out_flac_file_name, progress_callback, /*client_data=*/NULL);
        if(init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK) {
            fprintf(stderr, "ERROR: initializing encoder: %s\n", FLAC__StreamEncoderInitStatusString[init_status]);
            ok = false;
        }
    }

    if(ok) {
        size_t left = (size_t)total_samples;
        while(ok && left) {
            size_t need = (left>READSIZE? (size_t)READSIZE : (size_t)left);
            if(fread(buffer, channels*(bits_per_sample/8), need, fpPcm) != need) {
                fprintf(stderr, "ERROR: reading from PCM file\n");
                ok = false;
            }
            else {
                /* convert the packed little-endian 16-bit PCM samples into an interleaved FLAC__int32 buffer for libFLAC */
                size_t i;
                for(i = 0; i < need*channels; i++) {
                    /* inefficient but simple and works on big- or little-endian machines */
                    pcm[i] = (FLAC__int32)(((FLAC__int16)(FLAC__int8)buffer[2*i+1] << 8) | (FLAC__int16)buffer[2*i]);
                }
                /* feed samples to encoder */
                ok = FLAC__stream_encoder_process_interleaved(encoder, pcm, need);
            }
            left -= need;
        }
    }

    fprintf(stderr, "   state: %s\n", FLAC__StreamEncoderStateString[FLAC__stream_encoder_get_state(encoder)]);
    ok &= FLAC__stream_encoder_finish(encoder);

    fprintf(stderr, "encoding: %s\n", ok? "succeeded" : "FAILED");
    fprintf(stderr, "   state: %s\n", FLAC__StreamEncoderStateString[FLAC__stream_encoder_get_state(encoder)]);

    /* now that encoding is finished, the metadata can be freed */
    FLAC__metadata_object_delete(metadata[0]);
    FLAC__metadata_object_delete(metadata[1]);
    FLAC__stream_encoder_delete(encoder);

    /* 关闭文件 */
    fclose(fpPcm);
    if(buffer)
        free(buffer);
    if(pcm)
        free(pcm);

    printf("%s -> %s: Success!\n", in_pcm_file_name, out_flac_file_name);

    return 0;
}


void progress_callback(const FLAC__StreamEncoder *encoder, FLAC__uint64 bytes_written, FLAC__uint64 samples_written, unsigned frames_written, unsigned total_frames_estimate, void *client_data)
{
	(void)encoder, (void)client_data;

//	fprintf(stderr, "wrote %" PRIu64 " bytes, %" PRIu64 "/%u samples, %u/%u frames\n", bytes_written, samples_written, total_samples, frames_written, total_frames_estimate);
	fprintf(stderr, "wrote %" PRIu64 " bytes, %" PRIu64 "/%u samples, %u/%u frames\n", bytes_written, samples_written, 111111, frames_written, total_frames_estimate);
}

