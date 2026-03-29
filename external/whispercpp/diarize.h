/*
 * diarize.h — Speaker diarization API for Praat integration
 *
 * Provides a C API following the same pattern as whisper.h:
 *   - Context init/free (loads segmentation + embedding models)
 *   - diarize_full() runs the entire pipeline on audio samples
 *   - Accessor functions to read results (segments with speaker labels)
 *
 * The output is a list of time-stamped segments, each with a speaker ID.
 * Praat maps these to a TextGrid with one interval tier per speaker.
 *
 * Models:
 *   - ggml-segmentation.bin  (pyannote/segmentation-3.0, converted to GGML)
 *   - ggml-embedding.bin     (pyannote/wespeaker-voxceleb-resnet34-LM, converted to GGML)
 *
 * Usage:
 *   struct diarize_context * ctx = diarize_init("seg.bin", "emb.bin");
 *   diarize_full(ctx, samples, n_samples);
 *   int n = diarize_n_segments(ctx);
 *   for (int i = 0; i < n; i++) {
 *       float t0 = diarize_segment_t0(ctx, i);
 *       float t1 = diarize_segment_t1(ctx, i);
 *       int   sp = diarize_segment_speaker(ctx, i);
 *   }
 *   diarize_free(ctx);
 */

#ifndef DIARIZE_H
#define DIARIZE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Opaque context — holds loaded models and last result
struct diarize_context;

// Parameters for diarization
struct diarize_params {
    // Segmentation
    float   seg_duration;       // chunk duration in seconds (default: 10.0)
    float   seg_step_ratio;     // step as fraction of duration (default: 0.1 = 90% overlap)

    // Clustering
    float   cluster_threshold;  // agglomerative clustering threshold (default: 0.7046)
    int     cluster_min_size;   // minimum cluster size (default: 12)

    // Embedding filter
    float   min_active_ratio;   // minimum non-overlapping activity to include (default: 0.2)

    // Output
    int     max_speakers;       // upper bound on number of speakers (default: 20)
};

// Get default parameters
struct diarize_params diarize_default_params(void);


// Model loader — abstraction for reading from file or memory
struct diarize_model_loader {
	void * context;

	size_t (*read) (void * ctx, void * output, size_t read_size);
	bool   (*eof)  (void * ctx);
	void   (*close)(void * ctx);
};

// Various functions for loading both models. Return NULL on failure
struct diarize_context * diarize_init_from_file(const char * seg_model_path, const char * emb_model_path);
struct diarize_context * diarize_init_from_memory(const void * seg_data, size_t seg_size, const void * emb_data, size_t emb_size);
struct diarize_context * diarize_init(struct diarize_model_loader * seg_loader, struct diarize_model_loader * emb_loader);

// Free context and all associated memory
void diarize_free(struct diarize_context * ctx);

// Run the full diarization pipeline on audio samples
// samples: float array, 16kHz mono, normalized to [-1, 1]
// n_samples: number of samples
// Returns 0 on success, non-zero on failure
int diarize_full(
    struct diarize_context * ctx,
    struct diarize_params    params,
    const float            * samples,
    int                      n_samples);

// --- Result accessors ---

// Number of segments in the last result
int diarize_n_segments(struct diarize_context * ctx);

// Number of speakers detected in the last result
int diarize_n_speakers(struct diarize_context * ctx);

// Get start time (seconds) of segment i
float diarize_segment_t0(struct diarize_context * ctx, int i_segment);

// Get end time (seconds) of segment i
float diarize_segment_t1(struct diarize_context * ctx, int i_segment);

// Get speaker ID (0-indexed) of segment i
int diarize_segment_speaker(struct diarize_context * ctx, int i_segment);

#ifdef __cplusplus
}
#endif

#endif // DIARIZE_H
