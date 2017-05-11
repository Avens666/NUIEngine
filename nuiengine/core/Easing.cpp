
/*
Disclaimer for Robert Penner's Easing Equations license:

TERMS OF USE - EASING EQUATIONS

Open source under the BSD License.

Copyright 2001 Robert Penner
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of the author nor the names of contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <cmath>
#include "NE_type.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 (M_PI / 2)
#endif

using namespace std;

/**
 * Easing equation function for a simple linear tweening, with no easing.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeNone(kn_double progress)
{
    return progress;
}

/**
 * Easing equation function for a quadratic (t^2) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeInQuad(kn_double t)
{
    return t*t;
}

/**
* Easing equation function for a quadratic (t^2) easing out: decelerating to zero velocity.
*
* @param t		Current time (in frames or seconds).
* @return		The correct value.
*/
static kn_double easeOutQuad(kn_double t)
{
    return -t*(t-2);
}

/**
 * Easing equation function for a quadratic (t^2) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeInOutQuad(kn_double t)
{
    t*=2.0;
    if (t < 1) {
        return t*t/kn_double(2);
    } else {
        --t;
        return -0.5 * (t*(t-2) - 1);
    }
}

/**
 * Easing equation function for a quadratic (t^2) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeOutInQuad(kn_double t)
{
    if (t < 0.5) return easeOutQuad (t*2)/2;
    return easeInQuad((2*t)-1)/2 + 0.5;
}

/**
 * Easing equation function for a cubic (t^3) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeInCubic(kn_double t)
{
    return t*t*t;
}

/**
 * Easing equation function for a cubic (t^3) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeOutCubic(kn_double t)
{
    t-=1.0;
    return t*t*t + 1;
}

/**
 * Easing equation function for a cubic (t^3) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeInOutCubic(kn_double t)
{
    t*=2.0;
    if(t < 1) {
        return 0.5*t*t*t;
    } else {
        t -= kn_double(2.0);
        return 0.5*(t*t*t + 2);
    }
}

/**
 * Easing equation function for a cubic (t^3) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeOutInCubic(kn_double t)
{
    if (t < 0.5) return easeOutCubic (2*t)/2;
    return easeInCubic(2*t - 1)/2 + 0.5;
}

/**
 * Easing equation function for a quartic (t^4) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeInQuart(kn_double t)
{
    return t*t*t*t;
}

/**
 * Easing equation function for a quartic (t^4) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeOutQuart(kn_double t)
{
    t-= kn_double(1.0);
    return - (t*t*t*t- 1);
}

/**
 * Easing equation function for a quartic (t^4) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeInOutQuart(kn_double t)
{
    t*=2;
    if (t < 1) return 0.5*t*t*t*t;
    else {
        t -= 2.0f;
        return -0.5 * (t*t*t*t- 2);
    }
}

/**
 * Easing equation function for a quartic (t^4) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeOutInQuart(kn_double t)
{
    if (t < 0.5) return easeOutQuart (2*t)/2;
    return easeInQuart(2*t-1)/2 + 0.5;
}

/**
 * Easing equation function for a quintic (t^5) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeInQuint(kn_double t)
{
    return t*t*t*t*t;
}

/**
 * Easing equation function for a quintic (t^5) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeOutQuint(kn_double t)
{
    t-=1.0;
    return t*t*t*t*t + 1;
}

/**
 * Easing equation function for a quintic (t^5) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeInOutQuint(kn_double t)
{
    t*=2.0;
    if (t < 1) return 0.5*t*t*t*t*t;
    else {
        t -= 2.0;
        return 0.5*(t*t*t*t*t + 2);
    }
}

/**
 * Easing equation function for a quintic (t^5) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeOutInQuint(kn_double t)
{
    if (t < 0.5) return easeOutQuint (2*t)/2;
    return easeInQuint(2*t - 1)/2 + 0.5;
}

/**
 * Easing equation function for a sinusoidal (sin(t)) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeInSine(kn_double t)
{
    return (t == 1.0) ? 1.0 : -::cos(t * M_PI_2) + 1.0;
}

/**
 * Easing equation function for a sinusoidal (sin(t)) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeOutSine(kn_double t)
{
    return ::sin(t* M_PI_2);
}

/**
 * Easing equation function for a sinusoidal (sin(t)) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeInOutSine(kn_double t)
{
    return -0.5 * (cos(M_PI*t) - 1);
}

/**
 * Easing equation function for a sinusoidal (sin(t)) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeOutInSine(kn_double t)
{
    if (t < 0.5) return easeOutSine (2*t)/2;
    return easeInSine(2*t - 1)/2 + 0.5;
}

/**
 * Easing equation function for an exponential (2^t) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeInExpo(kn_double t)
{
    return (t==0 || t == 1.0) ? t : pow(2.0, 10 * (t - 1)) - kn_double(0.001);
}

/**
 * Easing equation function for an exponential (2^t) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeOutExpo(kn_double t)
{
    return (t==1.0) ? 1.0 : 1.001 * (-pow(kn_double(2.0f), -10 * t) + 1);
}

/**
 * Easing equation function for an exponential (2^t) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeInOutExpo(kn_double t)
{
    if (t==0.0) return kn_double(0.0);
    if (t==1.0) return kn_double(1.0);
    t*=2.0;
    if (t < 1) return 0.5 * pow(kn_double(2.0), 10 * (t - 1)) - 0.0005;
    return 0.5 * 1.0005 * (-pow(kn_double(2.0), -10 * (t - 1)) + 2);
}

/**
 * Easing equation function for an exponential (2^t) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeOutInExpo(kn_double t)
{
    if (t < 0.5) return easeOutExpo (2*t)/2;
    return easeInExpo(2*t - 1)/2 + 0.5;
}

/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeInCirc(kn_double t)
{
    return -(sqrt(1 - t*t) - 1);
}

/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeOutCirc(kn_double t)
{
    t-= kn_double(1.0);
    return sqrt(1 - t* t);
}

/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeInOutCirc(kn_double t)
{
    t*=kn_double(2.0);
    if (t < 1) {
        return -0.5 * (sqrt(1 - t*t) - 1);
    } else {
        t -= kn_double(2.0);
        return 0.5 * (sqrt(1 - t*t) + 1);
    }
}

/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static kn_double easeOutInCirc(kn_double t)
{
    if (t < 0.5) return easeOutCirc (2*t)/2;
    return easeInCirc(2*t - 1)/2 + 0.5;
}

static kn_double easeInElastic_helper(kn_double t, kn_double b, kn_double c, kn_double d, kn_double a, kn_double p)
{
    if (t==0) return b;
    kn_double t_adj = (kn_double)t / (kn_double)d;
    if (t_adj==1) return b+c;

    kn_double s;
    if(a < fabs(c)) {
        a = c;
        s = p / 4.0f;
    } else {
        s = p / (2 * M_PI) * asin(c / a);
    }

    t_adj -= 1.0f;
    return -(a*pow(kn_double(2.0f),10*t_adj) * sin( (t_adj*d-s)*(2*M_PI)/p )) + b;
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static kn_double easeInElastic(kn_double t, kn_double a, kn_double p)
{
    return easeInElastic_helper(t, 0, 1, 1, a, p);
}

static kn_double easeOutElastic_helper(kn_double t, kn_double /*b*/, kn_double c, kn_double /*d*/, kn_double a, kn_double p)
{
    if (t==0) return 0;
    if (t==1) return c;

    kn_double s;
    if(a < c) {
        a = c;
        s = p / 4.0f;
    } else {
        s = p / (2 * M_PI) * asin(c / a);
    }

    return (a*pow(kn_double(2.0f),-10*t) * sin( (t-s)*(2*M_PI)/p ) + c);
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static kn_double easeOutElastic(kn_double t, kn_double a, kn_double p)
{
    return easeOutElastic_helper(t, 0, 1, 1, a, p);
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static kn_double easeInOutElastic(kn_double t, kn_double a, kn_double p)
{
    if (t==0) return 0.0;
    t*=2.0;
    if (t==2) return 1.0;

    kn_double s;
    if(a < 1.0) {
        a = 1.0;
        s = p / 4.0f;
    } else {
        s = p / (2 * M_PI) * asin(1.0 / a);
    }

    if (t < 1) return -.5*(a*pow(kn_double(2.0f),10*(t-1)) * sin( (t-1-s)*(2*M_PI)/p ));
    return a*pow(kn_double(2.0f),-10*(t-1)) * sin( (t-1-s)*(2*M_PI)/p )*.5 + 1.0;
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static kn_double easeOutInElastic(kn_double t, kn_double a, kn_double p)
{
    if (t < 0.5) return easeOutElastic_helper(t*2, 0, 0.5, 1.0, a, p);
    return easeInElastic_helper(2*t - 1.0, 0.5, 0.5, 1.0, a, p);
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static kn_double easeInBack(kn_double t, kn_double s)
{
    return t*t*((s+1)*t - s);
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static kn_double easeOutBack(kn_double t, kn_double s)
{
    t-= kn_double(1.0);
    return t*t*((s+1)*t+ s) + 1;
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static kn_double easeInOutBack(kn_double t, kn_double s)
{
    t *= 2.0;
    if (t < 1) {
        s *= 1.525f;
        return 0.5*(t*t*((s+1)*t - s));
    } else {
        t -= 2;
        s *= 1.525f;
        return 0.5*(t*t*((s+1)*t+ s) + 2);
    }
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static kn_double easeOutInBack(kn_double t, kn_double s)
{
    if (t < 0.5) return easeOutBack (2*t, s)/2;
    return easeInBack(2*t - 1, s)/2 + 0.5;
}

static kn_double easeOutBounce_helper(kn_double t, kn_double c, kn_double a)
{
    if (t == 1.0) return c;
    if (t < (4/11.0)) {
        return c*(7.5625*t*t);
    } else if (t < (8/11.0)) {
        t -= (6/11.0);
        return -a * (1. - (7.5625*t*t + .75)) + c;
    } else if (t < (10/11.0)) {
        t -= (9/11.0);
        return -a * (1. - (7.5625*t*t + .9375)) + c;
    } else {
        t -= (21/22.0);
        return -a * (1. - (7.5625*t*t + .984375)) + c;
    }
}

/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static kn_double easeOutBounce(kn_double t, kn_double a)
{
    return easeOutBounce_helper(t, 1, a);
}

/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static kn_double easeInBounce(kn_double t, kn_double a)
{
    return 1.0 - easeOutBounce_helper(1.0-t, 1.0, a);
}


/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static kn_double easeInOutBounce(kn_double t, kn_double a)
{
    if (t < 0.5) return easeInBounce (2*t, a)/2;
    else return (t == 1.0) ? 1.0 : easeOutBounce (2*t - 1, a)/2 + 0.5;
}

/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static kn_double easeOutInBounce(kn_double t, kn_double a)
{
    if (t < 0.5) return easeOutBounce_helper(t*2, 0.5, a);
    return 1.0 - easeOutBounce_helper (2.0-2*t, 0.5, a);
}

static inline kn_double sinProgress(kn_double value)
{
    return sin((value * M_PI) - M_PI_2) / 2 + kn_double(0.5);
}

static inline kn_double smoothBeginEndMixFactor(kn_double value)
{
    return min(max(1 - value * 2 + kn_double(0.3), kn_double(0.0)), kn_double(1.0));
}

// SmoothBegin blends Smooth and Linear Interpolation.
// Progress 0 - 0.3      : Smooth only
// Progress 0.3 - ~ 0.5  : Mix of Smooth and Linear
// Progress ~ 0.5  - 1   : Linear only

/**
 * Easing function that starts growing slowly, then increases in speed. At the end of the curve the speed will be constant.
 */
static kn_double easeInCurve(kn_double t)
{
    const kn_double progress = sinProgress(t);
    const kn_double mix = smoothBeginEndMixFactor(t);
    return progress * mix + t * (1 - mix);
}

/**
 * Easing function that starts growing steadily, then ends slowly. The speed will be constant at the beginning of the curve.
 */
static kn_double easeOutCurve(kn_double t)
{
    const kn_double progress = sinProgress(t);
    const kn_double mix = smoothBeginEndMixFactor(1 - t);
    return progress * mix + t * (1 - mix);
}

/**
 * Easing function where the value grows sinusoidally. Note that the calculated  end value will be 0 rather than 1.
 */
static kn_double easeSineCurve(kn_double t)
{
    return (sin(((t * M_PI * 2)) - M_PI_2) + 1) / 2;
}

/**
 * Easing function where the value grows cosinusoidally. Note that the calculated start value will be 0.5 and the end value will be 0.5
 * contrary to the usual 0 to 1 easing curve.
 */
static kn_double easeCosineCurve(kn_double t)
{
    return (cos(((t * M_PI * 2)) - M_PI_2) + 1) / 2;
}

