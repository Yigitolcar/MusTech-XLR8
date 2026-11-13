#include <stdio.h>
#include <stdint.h>
#include <math.h>


#define SAMPLE_RATE 44100.0
#define PI 3.14159265358979323846


// Available waveforms
typedef enum {
    SAWTOOTH,
    SINE,
    SQUARE
} Waveform;


// Oscillator structure
typedef struct {
    double frequency;
    double phase;
    double amplitude;
    Waveform waveform;
} Oscillator;


// Set oscillator frequency
void set_frequency(Oscillator *osc, double frequency) {
    osc->frequency = frequency;
}


// Set oscillator amplitude
void set_amplitude(Oscillator *osc, double amplitude) {
    osc->amplitude = amplitude;
}


// Change oscillator waveform
void set_waveform(Oscillator *osc, Waveform waveform) {
    osc->waveform = waveform;
}


// Generate one audio sample
double oscillator_next(Oscillator *osc) {


    double sample = 0.0;


    switch (osc->waveform) {


        case SAWTOOTH:
            sample = 2.0 * osc->phase - 1.0;
            break;


        case SINE:
            sample = sin(2.0 * PI * osc->phase);
            break;


        case SQUARE:
            sample = (osc->phase < 0.5) ? 1.0 : -1.0;
            break;
    }


    // Apply individual oscillator amplitude
    sample *= osc->amplitude;


    // Advance phase
    osc->phase += osc->frequency / SAMPLE_RATE;


    // Wrap phase
    if (osc->phase >= 1.0) {
        osc->phase -= 1.0;
    }


    return sample;
}


// Ask user to select a waveform
Waveform select_waveform(int oscillator_number) {


    int choice;


    printf("\nSelect waveform for oscillator %d:\n", oscillator_number);
    printf("1. Sawtooth\n");
    printf("2. Sine\n");
    printf("3. Square\n");
    printf("Choice: ");


    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Using sawtooth.\n");
        return SAWTOOTH;
    }


    switch (choice) {


        case 1:
            return SAWTOOTH;


        case 2:
            return SINE;


        case 3:
            return SQUARE;


        default:
            printf("Invalid choice. Using sawtooth.\n");
            return SAWTOOTH;
    }
}


int main(void) {


    // Initialise oscillator 1
    Oscillator osc1 = {
        .frequency = 440.0,
        .phase = 0.0,
        .amplitude = 0.8,
        .waveform = SAWTOOTH
    };


    // Initialise oscillator 2
    Oscillator osc2 = {
        .frequency = 220.0,
        .phase = 0.0,
        .amplitude = 0.2,
        .waveform = SINE
    };


    printf("\n=== Two Oscillator Synthesiser ===\n");
    
    // User frequency inputs - will use accelerometer values in future
    printf("\nEnter frequency for oscillator 1 (Hz): ");
    if (scanf("%lf", &osc1.frequency) != 1 ||
        osc1.frequency <= 0.0 ||
        osc1.frequency >= SAMPLE_RATE / 2.0) {
        printf("Invalid frequency. Using 440 Hz.\n");
        osc1.frequency = 440.0;
    }


    printf("Enter frequency for oscillator 2 (Hz): ");
    if (scanf("%lf", &osc2.frequency) != 1 ||
        osc2.frequency <= 0.0 ||
        osc2.frequency >= SAMPLE_RATE / 2.0) {
        printf("Invalid frequency. Using 220 Hz.\n");
        osc2.frequency = 220.0;
    }


    // User amplitude inputs - will use accelerometer values in future
    printf("\nEnter amplitude for oscillator 1 (0-1): ");
    if (scanf("%lf", &osc1.amplitude) != 1 ||
        osc1.amplitude < 0.0 ||
        osc1.amplitude > 1.0) {
        printf("Invalid amplitude. Using 0.8.\n");
        osc1.amplitude = 0.8;
    }


    printf("Enter amplitude for oscillator 2 (0-1): ");
    if (scanf("%lf", &osc2.amplitude) != 1 ||
        osc2.amplitude < 0.0 ||
        osc2.amplitude > 1.0) {
        printf("Invalid amplitude. Using 0.2.\n");
        osc2.amplitude = 0.2;
    }


    // Select independent waveforms - will use accelerometer values in future
    set_waveform(&osc1, select_waveform(1));
    set_waveform(&osc2, select_waveform(2));


    // Generate audio samples - will be represented as audio in future
    printf("\nGenerated combined samples:\n");


    for (int i = 0; i < 20; i++) {


        // Generate individual oscillator samples
        double sample1 = oscillator_next(&osc1);
        double sample2 = oscillator_next(&osc2);


        // Combine oscillator outputs
        double output = sample1 + sample2;


        // Prevent output exceeding normal audio range
        if (output > 1.0) output = 1.0;
        if (output < -1.0) output = -1.0;


        printf("%f\n", output);
    }


    return 0;
}
