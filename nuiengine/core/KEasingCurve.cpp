//  **************************************
//  File:        KEasingCurve.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description: This code is part of NUI Engine (NUI Graphics Lib)
//  Comments:
//  Rev:         2
//  Created:     2017/4/11
//  Last edit:   2017/4/28
//  Author:      Chen Zhi
//  E-mail:      cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
//  ***************************************
#include "KEasingCurve.h"
#include <numeric>
#include <limits>
#include <math.h>
using namespace std;

static bool isConfigFunction(KEasingCurve::Type type)
{
	return type >= KEasingCurve::InElastic
		&& type <= KEasingCurve::OutInBounce;
}

class KEasingCurveFunction
{
public:
	enum Type { In, Out, InOut, OutIn };

	KEasingCurveFunction(KEasingCurveFunction::Type type = In, kn_double period = 0.3, kn_double amplitude = 1.0,
		kn_double overshoot = 1.70158)
		: _t(type), _p(period), _a(amplitude), _o(overshoot)
	{ }
	virtual ~KEasingCurveFunction() {}
	virtual kn_double value(kn_double t);
	virtual KEasingCurveFunction *copy() const;
	bool operator==(const KEasingCurveFunction& other);

	Type _t;
	kn_double _p;
	kn_double _a;
	kn_double _o;
};

kn_double KEasingCurveFunction::value(kn_double t)
{
	return t;
}

KEasingCurveFunction *KEasingCurveFunction::copy() const
{
	return new KEasingCurveFunction(_t, _p, _a, _o);
}

bool KEasingCurveFunction::operator==(const KEasingCurveFunction& other)
{
	return _t == other._t &&
		fabs(_p - other._p)  <= numeric_limits<kn_double>::epsilon() &&
		fabs(_a - other._a) <= numeric_limits<kn_double>::epsilon()&&
		fabs(_o - other._o) <= numeric_limits<kn_double>::epsilon();
}

#include "Easing.cpp"

struct ElasticEase : public KEasingCurveFunction
{
	ElasticEase(Type type)
		: KEasingCurveFunction(type, kn_double(0.3), kn_double(1.0))
	{ }

	KEasingCurveFunction *copy() const
	{
		ElasticEase *rv = new ElasticEase(_t);
		rv->_p = _p;
		rv->_a = _a;
		return rv;
	}

	kn_double value(kn_double t)
	{
		kn_double p = (_p < 0) ? kn_double(0.3) : _p;
		kn_double a = (_a < 0) ? kn_double(1.0) : _a;
		switch(_t) {
		case In:
			return easeInElastic(t, a, p);
		case Out:
			return easeOutElastic(t, a, p);
		case InOut:
			return easeInOutElastic(t, a, p);
		case OutIn:
			return easeOutInElastic(t, a, p);
		default:
			return t;
		}
	}
};

struct BounceEase : public KEasingCurveFunction
{
	BounceEase(Type type)
		: KEasingCurveFunction(type, kn_double(0.3), kn_double(1.0))
	{ }

	KEasingCurveFunction *copy() const
	{
		BounceEase *rv = new BounceEase(_t);
		rv->_a = _a;
		return rv;
	}

	kn_double value(kn_double t)
	{
		kn_double a = (_a < 0) ? kn_double(1.0) : _a;
		switch(_t) {
		case In:
			return easeInBounce(t, a);
		case Out:
			return easeOutBounce(t, a);
		case InOut:
			return easeInOutBounce(t, a);
		case OutIn:
			return easeOutInBounce(t, a);
		default:
			return t;
		}
	}
};

struct BackEase : public KEasingCurveFunction
{
	BackEase(Type type)
		: KEasingCurveFunction(type, kn_double(0.3), kn_double(1.0), kn_double(1.70158))
	{ }

	KEasingCurveFunction *copy() const
	{
		BackEase *rv = new BackEase(_t);
		rv->_o = _o;
		return rv;
	}

	kn_double value(kn_double t)
	{
		kn_double o = (_o < 0) ? kn_double(1.70158) : _o;
		switch(_t) {
		case In:
			return easeInBack(t, o);
		case Out:
			return easeOutBack(t, o);
		case InOut:
			return easeInOutBack(t, o);
		case OutIn:
			return easeOutInBack(t, o);
		default:
			return t;
		}
	}
};

static KEasingCurve::EasingFunction curveToFunc(KEasingCurve::Type curve)
{
	switch(curve) {
	case KEasingCurve::Linear:
		return &easeNone;
	case KEasingCurve::InQuad:
		return &easeInQuad;
	case KEasingCurve::OutQuad:
		return &easeOutQuad;
	case KEasingCurve::InOutQuad:
		return &easeInOutQuad;
	case KEasingCurve::OutInQuad:
		return &easeOutInQuad;
	case KEasingCurve::InCubic:
		return &easeInCubic;
	case KEasingCurve::OutCubic:
		return &easeOutCubic;
	case KEasingCurve::InOutCubic:
		return &easeInOutCubic;
	case KEasingCurve::OutInCubic:
		return &easeOutInCubic;
	case KEasingCurve::InQuart:
		return &easeInQuart;
	case KEasingCurve::OutQuart:
		return &easeOutQuart;
	case KEasingCurve::InOutQuart:
		return &easeInOutQuart;
	case KEasingCurve::OutInQuart:
		return &easeOutInQuart;
	case KEasingCurve::InQuint:
		return &easeInQuint;
	case KEasingCurve::OutQuint:
		return &easeOutQuint;
	case KEasingCurve::InOutQuint:
		return &easeInOutQuint;
	case KEasingCurve::OutInQuint:
		return &easeOutInQuint;
	case KEasingCurve::InSine:
		return &easeInSine;
	case KEasingCurve::OutSine:
		return &easeOutSine;
	case KEasingCurve::InOutSine:
		return &easeInOutSine;
	case KEasingCurve::OutInSine:
		return &easeOutInSine;
	case KEasingCurve::InExpo:
		return &easeInExpo;
	case KEasingCurve::OutExpo:
		return &easeOutExpo;
	case KEasingCurve::InOutExpo:
		return &easeInOutExpo;
	case KEasingCurve::OutInExpo:
		return &easeOutInExpo;
	case KEasingCurve::InCirc:
		return &easeInCirc;
	case KEasingCurve::OutCirc:
		return &easeOutCirc;
	case KEasingCurve::InOutCirc:
		return &easeInOutCirc;
	case KEasingCurve::OutInCirc:
		return &easeOutInCirc;
	case KEasingCurve::InCurve:
		return &easeInCurve;
	case KEasingCurve::OutCurve:
		return &easeOutCurve;
	case KEasingCurve::SineCurve:
		return &easeSineCurve;
	case KEasingCurve::CosineCurve:
		return &easeCosineCurve;
	default:
		return 0;
	};
}

static KEasingCurveFunction *curveToFunctionObject(KEasingCurve::Type type)
{
	KEasingCurveFunction *curveFunc = 0;
	switch(type) {
	case KEasingCurve::InElastic:
		curveFunc = new ElasticEase(ElasticEase::In);
		break;
	case KEasingCurve::OutElastic:
		curveFunc = new ElasticEase(ElasticEase::Out);
		break;
	case KEasingCurve::InOutElastic:
		curveFunc = new ElasticEase(ElasticEase::InOut);
		break;
	case KEasingCurve::OutInElastic:
		curveFunc = new ElasticEase(ElasticEase::OutIn);
		break;
	case KEasingCurve::OutBounce:
		curveFunc = new BounceEase(BounceEase::Out);
		break;
	case KEasingCurve::InBounce:
		curveFunc = new BounceEase(BounceEase::In);
		break;
	case KEasingCurve::OutInBounce:
		curveFunc = new BounceEase(BounceEase::OutIn);
		break;
	case KEasingCurve::InOutBounce:
		curveFunc = new BounceEase(BounceEase::InOut);
		break;
	case KEasingCurve::InBack:
		curveFunc = new BackEase(BackEase::In);
		break;
	case KEasingCurve::OutBack:
		curveFunc = new BackEase(BackEase::Out);
		break;
	case KEasingCurve::InOutBack:
		curveFunc = new BackEase(BackEase::InOut);
		break;
	case KEasingCurve::OutInBack:
		curveFunc = new BackEase(BackEase::OutIn);
		break;
	default:
		curveFunc = new KEasingCurveFunction(KEasingCurveFunction::In, kn_double(0.3), kn_double(1.0), kn_double(1.70158));
	}

	return curveFunc;
}
/*!
    Constructs an easing curve of the given \a type.
 */
KEasingCurve::KEasingCurve(Type type)
:m_type(KEasingCurve::Linear),
	m_pConfig(0),
	m_func(&easeNone)
{
    SetType(type);
}

KEasingCurve::KEasingCurve(const KEasingCurve &other)
{
	*this = other;
    if (other.m_pConfig)
        m_pConfig = other.m_pConfig->copy();
}

KEasingCurve::~KEasingCurve()
{
    delete m_pConfig;
}

KEasingCurve &KEasingCurve::operator=(const KEasingCurve &other)
{
    if (m_pConfig) {
        delete m_pConfig;
        m_pConfig = 0;
    }

    m_func = other.m_func;
	m_type = other.m_type;
    if (other.m_pConfig)
        m_pConfig = other.m_pConfig->copy();

    return *this;
}

bool KEasingCurve::operator==(const KEasingCurve &other) const
{
    bool res = m_func == other.m_func
            && m_type == other.m_type;
    if (res) {
        if (m_pConfig && other.m_pConfig) {
        // catch the config content
            res = (*m_pConfig == *(other.m_pConfig));

        } else if (m_pConfig || other.m_pConfig) {
        // one one has a config object, which could contain default values
            res = fabs(Amplitude() - other.Amplitude()) <= numeric_limits<kn_double>::epsilon() &&
                  fabs(Period() - other.Period()) <= numeric_limits<kn_double>::epsilon() &&
                  fabs(Overshoot() - other.Overshoot()) <= numeric_limits<kn_double>::epsilon(); 
        }
    }
    return res;
}

/*!
    Returns the amplitude. This is not applicable for all curve types.
    It is only applicable for bounce and elastic curves (curves of type()
    QEasingCurve::InBounce, QEasingCurve::OutBounce, QEasingCurve::InOutBounce,
    QEasingCurve::OutInBounce, QEasingCurve::InElastic, QEasingCurve::OutElastic,
    QEasingCurve::InOutElastic or QEasingCurve::OutInElastic).
 */
kn_double KEasingCurve::Amplitude() const
{
    return m_pConfig ? m_pConfig->_a : kn_double(1.0);
}

/*!
    Sets the amplitude to \a amplitude.

    This will set the amplitude of the bounce or the amplitude of the
    elastic "spring" effect. The higher the number, the higher the amplitude.
    \sa amplitude()
*/
void KEasingCurve::SetAmplitude(kn_double amplitude)
{
    if (!m_pConfig)
        m_pConfig = curveToFunctionObject(m_type);
    m_pConfig->_a = amplitude;
}

/*!
    Returns the period. This is not applicable for all curve types.
    It is only applicable if type() is KEasingCurve::InElastic, KEasingCurve::OutElastic,
    KEasingCurve::InOutElastic or KEasingCurve::OutInElastic.
 */
kn_double KEasingCurve::Period() const
{
    return m_pConfig ? m_pConfig->_p : kn_double(0.3);
}

/*!
    Sets the period to \a period.
    Setting a small period value will give a high frequency of the curve. A
    large period will give it a small frequency.

    \sa period()
*/
void KEasingCurve::SetPeriod(kn_double period)
{
    if (!m_pConfig)
        m_pConfig = curveToFunctionObject(m_type);
    m_pConfig->_p = period;
}

/*!
    Returns the overshoot. This is not applicable for all curve types.
    It is only applicable if type() is KEasingCurve::InBack, KEasingCurve::OutBack,
    KEasingCurve::InOutBack or KEasingCurve::OutInBack.
 */
kn_double KEasingCurve::Overshoot() const
{
    return m_pConfig ? m_pConfig->_o : kn_double(1.70158) ;
}

/*!
    Sets the overshoot to \a overshoot.

    0 produces no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent.

    \sa overshoot()
*/
void KEasingCurve::SetOvershoot(kn_double overshoot)
{
    if (!m_pConfig)
        m_pConfig = curveToFunctionObject(m_type);
    m_pConfig->_o = overshoot;
}

/*!
    Returns the type of the easing curve.
*/
KEasingCurve::Type KEasingCurve::GetType() const
{
    return m_type;
}

void KEasingCurve::SetTypeHelper(KEasingCurve::Type newType)
{
    kn_double amp = -1.0;
    kn_double period = -1.0;
    kn_double overshoot = -1.0;

    if (m_pConfig) {
        amp = m_pConfig->_a;
        period = m_pConfig->_p;
        overshoot = m_pConfig->_o;
        delete m_pConfig;
        m_pConfig = 0;
    }

    if (isConfigFunction(newType) || (amp != -1.0) || (period != -1.0) || (overshoot != -1.0)) {
        m_pConfig = curveToFunctionObject(newType);
        if (amp != -1.0)
            m_pConfig->_a = amp;
        if (period != -1.0)
            m_pConfig->_p = period;
        if (overshoot != -1.0)
            m_pConfig->_o = overshoot;
        m_func = 0;
    } else if (newType != KEasingCurve::Custom) {
        m_func = curveToFunc(newType);
    }
    m_type = newType;
}

/*!
    Sets the type of the easing curve to \a type.
*/
void KEasingCurve::SetType(Type type)
{
    if (m_type == type)
        return;
    if (type < Linear || type >= NCurveTypes - 1) {
        return;
    }

    SetTypeHelper(type);
}

/*!
    Sets a custom easing curve that is defined by the user in the function \a func.
    The signature of the function is kn_double myEasingFunction(kn_double progress),
    where \e progress and the return value is considered to be normalized between 0 and 1.
    (In some cases the return value can be outside that range)
    After calling this function type() will return KEasingCurve::Custom.
    \a func cannot be zero.

    \sa customType()
    \sa valueForProgress()
*/
void KEasingCurve::SetCustomType(EasingFunction func)
{
    if (!func) {
        return;
    }
    m_func = func;
    SetTypeHelper(Custom);
}

/*!
    Returns the function pointer to the custom easing curve.
    If type() does not return KEasingCurve::Custom, this function
    will return 0.
*/
KEasingCurve::EasingFunction KEasingCurve::CustomType() const
{
    return m_type == Custom ? m_func : 0;
}

/*!
    Return the effective progress for the easing curve at \a progress.
    While  \a progress must be between 0 and 1, the returned effective progress
    can be outside those bounds. For instance, KEasingCurve::InBack will
    return negative values in the beginning of the function.
 */
kn_double KEasingCurve::valueForProgress(kn_double progress) const
{
    progress = max(0.0, min(progress, 1.0));
    if (m_func)
        return m_func(progress);
    else if (m_pConfig)
        return m_pConfig->value(progress);
    else
        return progress;
}

kn_double KEasingCurve::GetInterValue(kn_double iStart, kn_double iEnd, kn_double index, kn_double iSize)
{
	if(index < 0)
	{
		index = 0;
	}

	if(index > iSize)
	{
		index = iSize;
	}

	return iStart + (iEnd - iStart) * valueForProgress((double)index / iSize);
}

kn_double KEasingCurve::GetInterValue(kn_double iStart, kn_double iEnd, kn_double v)
{
	if(v < 0)
	{
		v = 0;
	}

	if(v > 1)
	{
		v = 1;
	}

	return iStart + (iEnd - iStart) * valueForProgress(v);
}
